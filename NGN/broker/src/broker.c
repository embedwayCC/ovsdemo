//#include <czmq.h>
#include "broker.h"
#include "ctx.h"

zsock_t *frontend = NULL;
zsock_t *backend  = NULL;
zsock_t *publisher = NULL;
zsock_t *rep = NULL;
zloop_t *loop = NULL;

static int s_set_id (void *socket,char *str)
{
    int ret = 0;
    char identity [32] = {0};
    sprintf (identity, "%s",str);
    zsock_set_identity(socket,identity);
    return ret;
}


//recv msg from worker
static int
s_back_event (zloop_t *loop, zsock_t *backend, void *frontend)
{
    worker_register_t register_t = NNGN_REGISTER__INIT;
    int rc = -1;
    char work_addr[NNGX_BACK_ID_MAX_LENGTH] = {0};
    char *client_addr = NULL;
    char *worker_name = NULL;
    zframe_t *last_name = NULL;
    mutil_msg_t *pMsg = NULL;
    zframe_t *first_frame = NULL;	
    zmsg_t *msg = zmsg_recv (backend);

    zframe_t *address = zmsg_unwrap (msg);
    memcpy(work_addr,zframe_data(address),zframe_size(address));

    zframe_t *frame = zmsg_first (msg);
    rc = zframe_2_register(frame, &register_t);
    if(rc != 0)
    {
          zframe_destroy(&address);
          zmsg_destroy (&msg);
          zsys_error("zframe_2_register error");
          return 0;
    }

    //register msg
    switch(register_t.op)
    {
            case worker_init_type:
                register_worker(work_addr,register_t.op_code);
                break;
            case broker_heart_type:
             	  last_name = zmsg_last(msg);
		  worker_name = zframe_strdup(last_name);
		  assert(worker_name);
                heart_msg_handle(worker_name,REV_HEART);
		  free(worker_name);
                break;
            case register_opcode_type:
                register_opcode(register_t.op_code,work_addr);
                break;
            case unregister_opcode_type:
                unregister_opcode(register_t.op_code,work_addr);
                break;
            case register_opcode_end:
                save_workerinfo(work_addr);
                break;
	     case normal_msg:
		  first_frame = zmsg_next(msg);
        	  assert(first_frame);
        	  client_addr = zframe_strhex(first_frame);
        	  assert(client_addr != NULL);
        	  zframe_t *last_frame = zmsg_last(msg);
        	  assert(last_frame);
        	  //this frame is data frame
        	 mutil_msg_t *pMsg = get_mutil_msg(client_addr);
        	 if(pMsg)
        	 {
			if(ngn_debug_enable()){
					zsys_debug("delete_mutilmsg_workerid clientaddr is %s worker addr is %s",client_addr,work_addr);
			}
            		delete_mutilmsg_workerid(client_addr,work_addr);
            		zframe_t *frame_new = zframe_dup(last_frame);
            		zmsg_append(pMsg->msg,&frame_new);
            		rc = check_mutilmsg_success(client_addr);
            		if(rc == 0)
            		{
                		rc = zmsg_send(&pMsg->msg,frontend);
                		if(rc != 0)
                		{
                    			zsys_warning("zmsg_send mutilmsg frontend error");
                    			zmsg_destroy(&pMsg->msg);
                		}
                		destroy_mutil_msg(client_addr);
            		}
            		zmsg_destroy(&msg);
        	  }
        	 else
        	{
            		zmsg_print(msg);
            		zsys_warning("zmsg_send singlemsg error client-id error");
            		zmsg_destroy(&msg);
        	}
	       break;
            default:
                zsys_error("unkown op is %d",register_t.op);
                break;
    }
    zmsg_destroy (&msg);
   
    free(client_addr);
    zframe_destroy(&address);
    return 0;
}

//recv msg from worker
static int
s_db_event (zloop_t *loop, zsock_t *rep, void *output)
{
    int ret = 0;
    zmsg_t *msg_send = zmsg_new();
    zmsg_t *msg_org = zmsg_recv (rep);
    assert(msg_org);
    //zmsg_print(msg_org);
    char *op_str = zmsg_popstr(msg_org);
    if(strcmp(op_str,BROKER_DBPUT_STR) == 0)
    {
        zframe_t *key_frame = zmsg_first(msg_org);
        assert(key_frame);
        zframe_t *value_frame = zmsg_next(msg_org);
        assert(value_frame);
        ret = put_data_local((char *)zframe_data(key_frame), zframe_size(key_frame), (char *)zframe_data(value_frame), zframe_size(value_frame),DB_FLAG_REMOTE);
        if(ret != 0)
        {
            zmsg_pushstr(msg_send,DB_RET_FAIL);
        }
        else
        {
            zmsg_pushstr(msg_send,DB_RET_SUCCESS);
        }
        ret = zmsg_send(&msg_send,rep);
        if(ret != 0)
        {
            zmsg_destroy(&msg_send);
        }
    }
    else if(strcmp(op_str,BROKER_DBGET_STR) == 0)
    {
        char *value = NULL;
        int size = 0;
        zframe_t *key_frame = zmsg_first(msg_org);
        assert(key_frame);
        ret = get_data_local((char *)zframe_data(key_frame), zframe_size(key_frame), &value, &size,DB_FLAG_REMOTE);
        if(ret != 0)
        {
            zmsg_pushstr(msg_send,DB_RET_FAIL);
        }
        else
        {
            zmsg_pushstr(msg_send,DB_RET_SUCCESS);
            zframe_t *value_frame=zframe_new(value,size);
            assert(value_frame);
            zmsg_append(msg_send,&value_frame);
            free(value);
        }
        ret = zmsg_send(&msg_send,rep);
        if(ret != 0)
        {
            zmsg_destroy(&msg_send);
        }
    }
    else
    {
        zsys_warning("unknow option str is %s",op_str);
    }
    zmsg_destroy(&msg_org);
    zstr_free(&op_str);
    return 0;
}


//recv msg from client
static int
s_front_event (zloop_t *loop, zsock_t *frontend, void *backend)
{
    int rc = -1;
    uint32_t slot_code = 0;
    worker_t *pWorker = NULL;
    req_t request = NNGN_REQUEST__INIT;
    zlist_t *pList = NULL;
    uint8_t sendflag = 0;
    uint8_t recvflag = 0;
    zmsg_t *msg_org = zmsg_recv (frontend);
    assert(msg_org);
    zmsg_print(msg_org); 	

    //this frame is client addr
    zframe_t *first_frame = zmsg_first (msg_org);
    assert(first_frame);

    //add by youxs
    char *client_addr = NULL;
    client_addr = zframe_strhex(first_frame);
    assert(client_addr != NULL);	
   
   mutil_msg_t *pMsg = get_mutil_msg(client_addr);
   if(pMsg)
   {
   	 zsys_warning("get_mutil_msg  clientaddr=%s is exist",client_addr);
   	free(client_addr);
	client_addr = NULL;
	zmsg_destroy(&msg_org);
   	return 0;
   }
   
	
    //this frame is req data
    zframe_t *last_frame = zmsg_last (msg_org);
    assert(last_frame);
    rc = zframe_2_req(last_frame, &request);
    if(rc != 0)
    {
        if(request.content)
        {
            free(request.content);
            request.content = NULL;
        }
        free(client_addr);
	 client_addr = NULL;
        zsys_error("zframe_2_req  error");
        return 0;
    }

    if(ngn_debug_enable()){
        zsys_debug("request opcode is %d slot is 0x%08x",request.op_code,request.slot);
    }
    pList = zlist_new();
    assert(pList);
    zlist_autofree(pList);
    zlist_comparefn(pList,(zlist_compare_fn *) strcmp);
    for (pWorker = zhash_first (workerid_obj); pWorker != NULL; pWorker = zhash_next(workerid_obj))
    {
        if(pWorker->worker_state != WK_ONLINE)
        {
            continue;
        }
        rc = get_msginfo(pWorker,request.op_code,&sendflag,&recvflag);

        if(rc != NNGN_ERR_NONE)
        {
            continue;
        }
        if(request.slot == 0)
        {
            //judge by opcode
            send_msg_worker(msg_org,pWorker->worker_id,backend);
            if(recvflag && request.timeout != 0)
            {
                assert(zlist_push(pList, pWorker->worker_id) == 0);
            }
        }
        else
        {
            //judge by opcode and slot
            slot_code = (uint32_t)(1) << (pWorker->slot-1);
	     if(ngn_debug_enable()){
                zsys_debug("slot code is 0x%08x pWorkerslot is %d sendflag is %d request.slot is 0x%08x workerid is %s",slot_code,pWorker->slot,sendflag,request.slot,pWorker->worker_id);
	     }
            if((request.slot & slot_code) || sendflag)
            {
                send_msg_worker(msg_org,pWorker->worker_id,backend);
                if(recvflag && request.timeout != 0)
                {
                    assert(zlist_push(pList, pWorker->worker_id) == 0);
                }
            }
        }
    }

    if(zlist_size(pList) != 0)
    {
        register_mutil_msg(first_frame, pList,request.timeout);
    }
    else
    {
        //send res msg
        zlist_destroy(&pList);
        send_res_msg(first_frame,frontend);
    }

    if(request.content)
    {
        free(request.content);
        request.content = NULL;
    }

    free(client_addr);
    client_addr = NULL;
    zmsg_destroy(&msg_org);
    return 0;
}

static int
s_msgtimer_event (zloop_t *loop, int timer_id, void *output)
{
    int rc = -1;
    mutil_msg_t *pMsg = NULL;
    for (pMsg = zhash_first (mutil_msg_list); pMsg != NULL; pMsg = zhash_next(mutil_msg_list))
    {
    	 if(pMsg->timeout == NGN_TIMEOUT_NONE)
    	 {
    	 	continue;
    	 }
  
        if((zclock_time() - pMsg->start_time) > pMsg->timeout)
        {
            zsys_warning("multimsg timeout client id is %s remain num is %d timeout is %d",pMsg->client_id,zlist_size(pMsg->worker_id),pMsg->timeout);
            if(zmsg_size(pMsg->msg) > 2)
            {
                 rc = zmsg_send(&pMsg->msg,frontend);
                 if(rc != 0)
                 {
                    zmsg_destroy(&pMsg->msg);
                 }
            }
            else
            {
                zmsg_destroy(&pMsg->msg);
            }

            zhash_delete(mutil_msg_list, pMsg->client_id);
        }
    }
    return NNGN_ERR_NONE;
}

static int
s_hearttimer_event (zloop_t *loop, int timer_id, void *output)
{
    worker_t *pWorker = NULL;
    for (pWorker = zhash_first (workerid_obj); pWorker != NULL; pWorker = zhash_next(workerid_obj))
    {
        if(pWorker->worker_state == WK_OFFLINE)
        {
            continue;
        }
        if((zclock_time() - pWorker->heart_time) > HEART_MSG_TIMEOU)
        {
            heart_msg_handle(pWorker->worker_id,TIMEOUT_HEART);
        }
    }
    return NNGN_ERR_NONE;
}

static int log_init()
{
    FILE *fp;
    zsys_dir_create("/usr/local/ngn/");
    fp=fopen("/usr/local/ngn/broker_log","a+");
    if(fp == NULL)
    {
        return errno;
    }
    zsys_set_logstream(fp);
    //zsys_set_logsystem(true);
    zsys_set_logident("broker:");
    return NNGN_ERR_NONE;
}

static int broker_init()
{
    int rc = -1;

    frontend = zsock_new(ZMQ_ROUTER);
    assert(frontend);
    backend  = zsock_new(ZMQ_ROUTER);
    assert(backend);
    publisher = zsock_new(ZMQ_PUB);
    assert(publisher);
    rep = zsock_new(ZMQ_REP);
    assert(rep);

    rc = s_set_id(frontend,BROKER_FRONT_ID);
    assert(rc == 0);

    rc = s_set_id(backend,BROKER_BACK_ID);
    assert(rc == 0);

    rc = zsock_bind(frontend, "tipc://{%d,%d,%d}",DEFAULT_BROKER_FRONT_PORT,DEFAULT_BROKER_FRONT_PORT,DEFAULT_BROKER_FRONT_PORT);
    if(rc == -1)
    {
       zsys_error("zmq_bind broker frontend router port is %d",DEFAULT_BROKER_FRONT_PORT);
       return rc;
    }

    zsock_set_router_handover(backend,1);
    rc = zsock_bind(backend, "tipc://{%d,%d,%d}",DEFAULT_BROKER_BACK_PORT,DEFAULT_BROKER_BACK_PORT,DEFAULT_BROKER_BACK_PORT);
    if(rc == -1)
    {
       zsys_error("zmq_bind broker back router port is %d",DEFAULT_BROKER_BACK_PORT);
       return rc;
    }

    rc = zsock_bind(publisher, "tipc://{%d,%d,%d}", DEFAULT_BROKER_PUB_PORT,DEFAULT_BROKER_PUB_PORT,DEFAULT_BROKER_PUB_PORT);
    if(rc == -1)
    {
       zsys_error("zmq_bind broker back router port is %d",DEFAULT_BROKER_PUB_PORT);
       return rc;
    }

    rc = zsock_bind(rep, "tipc://{%d,%d,%d}", DEFAULT_BROKER_REP_PORT,DEFAULT_BROKER_REP_PORT,DEFAULT_BROKER_REP_PORT);
    if(rc == -1)
    {
       zsys_error("zmq_bind broker rep port is %d",DEFAULT_BROKER_REP_PORT);
       return rc;
    }

    return NNGN_ERR_NONE;
}

int main(int argc,char *argv[])
{
    int rc = -1;
    int isdaemon = 1;

    if(isdaemon)
    {
        rc =daemon(1, 0);
    }

    rc = log_init();
    if(rc != 0)
    {
        printf("broker:log init error rc is %d\n",rc);
        return NNGN_ERR_NONE;
    }

    rc = broker_init();
    if(rc != 0)
    {
        zsys_error("broker_init error");
        return NNGN_ERR_RESOURCE;
    }

    rc = ctx_init();
    if(rc != 0)
    {
        zsys_error("ctx_init error");
        return NNGN_ERR_RESOURCE;
    }

    loop = zloop_new ();
    assert (loop);
    //zloop_set_verbose (loop, 1);
    rc = zloop_reader (loop, backend, s_back_event, frontend);
    assert(rc == 0);
    rc = zloop_reader (loop, frontend, s_front_event, backend);
    assert (rc == 0);
    rc = zloop_reader (loop, rep, s_db_event, NULL);
    assert (rc == 0);

    zloop_reader_set_tolerant (loop, frontend);
    zloop_reader_set_tolerant (loop, backend);

    rc = zloop_timer (loop, TIMER_SET, 0, s_hearttimer_event, NULL);
    assert (rc != -1);
    rc = zloop_timer (loop, MULTI_MSG_CHECK, 0, s_msgtimer_event, NULL);
    assert (rc != -1);
    zloop_start (loop);
    zloop_destroy (&loop);
    zsock_destroy(&backend);
    zsock_destroy(&frontend);
    zsock_destroy(&publisher);
    zsock_destroy(&rep);
    ctx_exit();
    return 0;
}

