#include "nngn.h"
#include "worker.h"


zactor_t *actor_worker;
zactor_t *actor_process;
int32_t g_opcode[MAX_OPCODE_SIZE] = {0};
op_callback_t msg_process[MAX_OPCODE_SIZE] = {NULL};
op_pipe_callback_t msg_pipe_process[MAX_OPCODE_SIZE] = {NULL};
wkst_change_handler changestate_handle;
msg_timeout_handler msg_timeout_handle;
zframe_t *heart_frame = NULL;

static int
s_worker_event (zloop_t *loop, zsock_t *worker, void *actor)
{
    int rc = -1;
    zmsg_t *msg = zmsg_recv (worker);
    if (!msg)
       return NNGN_ERR_NONE;
 
    req_t requset = NNGN_REQUEST__INIT;
    res_t result = NNGN_RESULT__INIT;
 
    //last frame is req
    if(ngn_debug_enable()){
        zmsg_print(msg);
    }
    zframe_t *frame_req = zmsg_last(msg);
    assert(frame_req);
    rc = zframe_2_req(frame_req, &requset);
    if(rc != 0)
    {
        //req data is error
        zsys_error("zframe_2_req error");
        goto END;
    }
 
    //add by youxs
    zframe_t *frame_broker=zmsg_first(msg);
    assert(frame_broker);
    zframe_t *frame_client=zmsg_next(msg);
    assert(frame_client);    
    if(get_om_sock() != NULL)
    {
        set_client_frame(frame_client,frame_req);
    }
    //end
 
    if (msg_process[requset.op_code])
    {
      msg_process[requset.op_code](&requset, &result);
    }
    else
    {
        zsys_error("call user fuc error opcode is 0x%04x",requset.op_code);
        goto END;
    }
 
    //add by youxs for pipe msg
    if(result.rv == NNGN_PIPEMSG_RET)
    {
           goto END;
    }
 
    //no res data
    if(g_opcode[requset.op_code] != 1)
    {
        goto END;
    }
 
    //end
 
    zframe_t *frame_res = zframe_new_empty();
    rc = res_2_zframe(&result, frame_res);
    if(rc != 0)
    {
        zsys_error("res_2_zframe error");
        goto END;
    }
 
    zframe_reset (frame_req, zframe_data(frame_res), zframe_size(frame_res));
    zframe_destroy(&frame_res);
 
    zframe_t *frame_type = zframe_new_empty();
    assert(frame_type);
 
    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = normal_msg;
 
    rc = register_2_zframe(&register_s,frame_type);
    assert(rc == 0);
 
    zframe_t *addr = zmsg_pop(msg);
    rc = zmsg_push(msg,frame_type);
    assert(rc == 0);
    rc = zmsg_push(msg,addr);
    assert(rc == 0);
   
    if(ngn_debug_enable() == 1)
    {
       zsys_debug("worker send res msg:");
       zmsg_print(msg);
    }
    rc = zmsg_send (&msg, worker);
    if(rc != 0)
    {
        zsys_error("send res fail errno is %d",rc);
        goto END;
    }    
END:
    zmsg_destroy(&msg);
    if(requset.content)
    {
        free(requset.content);
        requset.content = NULL;
    }
    if(result.content)
    {
        free(result.content);
        result.content = NULL;
    }
    return NNGN_ERR_NONE;
}


static int
s_pipe_event (zloop_t *loop, zsock_t *pipe, void *worker)
{
    int rc = -1;
    zmsg_t *msg = zmsg_recv (pipe);
    assert(msg);

    char *command = zmsg_popstr (msg);
    if (streq (command, "$TERM"))
    {
        zsys_info("s_pipe_event recv term msg");
        zstr_free (&command);
        zmsg_destroy (&msg);
        return -1;
    }
    zstr_free (&command);
    if(ngn_debug_enable() != 0)
    {
       zsys_debug("worker actor recv msg:");
    	zmsg_print(msg);	
    }
    
    rc = zmsg_send(&msg,worker);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        zsys_error("send opcode fail errno is %d",rc);
    }
    return NNGN_ERR_NONE;
}


static int
s_statechange_event (zloop_t *loop, zsock_t *sub, void *argv)
{
    wk_st status = {0,{0},0,0};
    zmsg_t *msg_recv = zmsg_recv(sub);
    assert(msg_recv);
    zmsg_print(msg_recv);
    zframe_t *frame = zmsg_last(msg_recv);
    zlist_t *pList = NULL;
    pList = zlist_new();
    assert(pList);
    zlist_autofree(pList);
    buf_2_list((char *)zframe_data(frame),zframe_size(frame),pList);
    zmsg_remove (msg_recv, frame);
    zframe_t *stateframe = zmsg_last(msg_recv);
    buf_2_wkst((char *)zframe_data(stateframe),zframe_size(stateframe),&status);
    porcess_state_change_msg(&status, pList);
    if(changestate_handle != NULL)
    {
        changestate_handle(&status, pList);
    }
    zlist_destroy (&pList);
    return NNGN_ERR_NONE;
}


//rev msg from worker pipeline 
//error return,success return
static int 
s_pipeline_event (zloop_t *loop, zsock_t *om_pipe, void *worker)
{
    int rc = -1;
    req_t requset = NNGN_REQUEST__INIT;
    res_t result_in = NNGN_RESULT__INIT;
    res_t result_out = NNGN_RESULT__INIT;
	
    zmsg_t *msg = zmsg_recv (om_pipe);
    assert(msg);
    if(ngn_debug_enable() == 1)
    {
    	zsys_debug("om pipeline recv msg:");
    	zmsg_print(msg);
    }
    zframe_t *address = zmsg_unwrap (msg);
    
    zframe_t *frame_client = zmsg_pop (msg);
    assert(frame_client);

    zframe_t *frame_id = zmsg_pop (msg);
    assert(frame_id);

    char *pipe_id = zframe_strdup(frame_id);

    zframe_t *org_req = zmsg_pop (msg);
    assert(org_req);
    
    rc = zframe_2_req(org_req, &requset);
    if(rc != 0)
    {
        //req data is error
        zsys_error("zframe_2_req error");
	 goto END;
    }

    zframe_t *res_in = zmsg_pop (msg);
    assert(res_in);
    rc = zframe_2_res(res_in, &result_in);
    if(rc != 0)
    {
        //req data is error
        zsys_error("zframe_2_res error");
	 goto END;
    }	

    if (msg_pipe_process[requset.op_code])
    {
	   msg_pipe_process[requset.op_code](&requset, &result_in,&result_out,pipe_id);
    }
    else
    {
        zsys_error("call user fuc error opcode is 0x%04x",requset.op_code);
        goto END;
    }

    //add by youxs for pipe msg
    if(result_out.rv == NNGN_PIPEMSG_RET)
    {
        zframe_destroy(&frame_client);
        goto END;
    }

    zframe_t *res_frame = zframe_new_empty();
    assert(res_frame);
    rc = res_2_zframe(&result_out, res_frame);
    if(rc != 0)
    {
        //req data is error
        zsys_error("res_2_zframe error");
	 goto END;
    }		
    //add res
    zmsg_push(msg, res_frame);

    //add empty
    zmsg_pushmem (msg, "", 0);
    //add client
    zmsg_push(msg,frame_client);
	
    zframe_t *frame_type = zframe_new_empty();
    assert(frame_type);

    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = normal_msg;

    rc = register_2_zframe(&register_s,frame_type);
    assert(rc == 0);
    zmsg_push(msg,frame_type);	
	
    add_brokerid_frame(msg);
    zmsg_send(&msg,worker);
END:
    zmsg_destroy(&msg);
    if(requset.content)
    {
        free(requset.content);
        requset.content = NULL;
    }
    if(result_out.content)
    {
        free(result_out.content);
        result_out.content = NULL;
    }
    zframe_destroy(&address);
    zframe_destroy(&frame_id);
    zframe_destroy(&org_req);
    zframe_destroy(&res_in);
    free(pipe_id);
    return NNGN_ERR_NONE;
}


static int
s_timer_event (zloop_t *loop, int timer_id, void *worker)
{
    send_heart(worker);
    return NNGN_ERR_NONE;
}

static int
s_msgtimeout_event (zloop_t *loop, int timer_id, void *output)
{
    zsys_debug("s_msgtimeout_event is called");
    if(msg_timeout_handle!= NULL)
    {
        msg_timeout_handle();
    }

    return NNGN_ERR_NONE;
}

static void
process_actor (zsock_t *pipe, void *args)
{
    int rc = -1;
    char worker_id[NNGX_WKID_MAX_LENGTH] = {0};
    char org_id[NNGX_WKID_MAX_LENGTH] = {0};

    sinit_data_t *data=(sinit_data_t *)args;
    sprintf(worker_id,"%d-%s-heart",data->slot,data->worker_id);	
    sprintf(org_id,"%d-%s",data->slot,data->worker_id);		
    heart_frame = zframe_from(org_id);
    assert(heart_frame);
  
    zsock_signal (pipe, 0);	

    zsock_t *worker = zsock_new(ZMQ_ROUTER);	
    zsock_set_identity(worker,worker_id);
	
    zloop_t *loop = zloop_new ();
    assert (loop);
    //zloop_set_verbose (loop, 1);
    do
    {
       rc = zsock_connect(worker, "tipc://{%d,%d}",DEFAULT_BROKER_BACK_PORT,DEFAULT_BROKER_BACK_PORT);
    }
    while(rc != 0);
	
    zloop_reader_set_tolerant (loop, pipe);
    rc = zloop_timer (loop, TIMER_SET, 0, s_timer_event, worker);
    assert(rc != -1);

    rc = zloop_reader (loop, pipe, s_process_event, NULL);
    assert(rc == 0);

    zloop_start (loop);
    zloop_destroy (&loop);
}


static void
worker_actor (zsock_t *pipe, void *args)
{
    char worker_id[NNGX_WKID_MAX_LENGTH] = {0};
    int rc = -1;
    zsock_t *om_pipe = NULL;

    sinit_data_t *data=(sinit_data_t *)args;
    sprintf(worker_id,"%d-%s",data->slot,data->worker_id);

    zsock_signal (pipe, 0);
    zsock_t *worker = zsock_new(ZMQ_ROUTER);
	
    zsock_set_identity(worker,worker_id);

    int value = 1;
    zsock_set_router_mandatory (worker, value);

    //reconnect until connected
    do
    {
       rc = zsock_connect(worker, "tipc://{%d,%d}",DEFAULT_BROKER_BACK_PORT,DEFAULT_BROKER_BACK_PORT);
    }
    while(rc != 0);
    //wait for broker zloop

    zsys_info("worker_actor connect broker success");

    //for worker state callback
    zsock_t *sub = NULL;
	
    sub = zsock_new(ZMQ_SUB);
    zsock_set_subscribe(sub, BROKER_PUBLISH_STR);
    assert(sub);
    do
    {
        rc = zsock_connect(sub, "tipc://{%d,%d}",DEFAULT_BROKER_PUB_PORT,DEFAULT_BROKER_PUB_PORT);
    }
    while(rc != 0);

    rc = -1;
    while(rc != 0)
    {
        rc = send_init_msg(worker,data->slot);
    }
    zsys_info("worker_actor send initmsg success rc is %d",rc);

    zloop_t *loop = zloop_new ();
    assert (loop);
    //zloop_set_verbose (loop, 1);
    zloop_reader_set_tolerant (loop, worker);

    rc = zloop_reader (loop, worker, s_worker_event, NULL);
    assert(rc == 0);

    rc = zloop_reader (loop, sub, s_statechange_event, NULL);
    assert(rc == 0);
	
    rc = zloop_reader (loop, pipe, s_pipe_event, worker);
    assert(rc == 0);

    //add by youxs for pipeline
    om_pipe = get_om_sock();
    if(om_pipe != NULL)
    {
        zsys_debug("register om pipe");
        rc = zloop_reader (loop, om_pipe, s_pipeline_event, worker);
        rc = zloop_timer (loop, MSG_TIMEOUT_SET, 0, s_msgtimeout_event, NULL);
        assert(rc != -1);
    }	

    zloop_start (loop);
	
    zloop_destroy (&loop);
    zsock_destroy (&worker);
    zsock_destroy(&sub);
}

uint32_t nsrv_new(sinit_data_t *data)
{
    assert (data);
    uint32_t ret = 0;

    if(!g_systeminit)
    {
       ret=common_init(data->worker_id);
       if(ret != NNGN_ERR_NONE)
       {
           return ret;
       }
    }

    zsys_info("nsrv_new slot is %d worker_id is %s",data->slot,data->worker_id);
    actor_worker = zactor_new (worker_actor, data);
    assert (actor_worker);

    actor_process = zactor_new (process_actor, data);
    assert(actor_process);

    return NNGN_ERR_NONE;
}

extern uint32_t nsrv_register_opcode(u_int8_t force_send,u_int8_t recv_flag,uint16_t msg_type, void *handle_data,u_int8_t reg_flag)
{
    //send opcode to broker
    int rc = -1;
    zmsg_t *msg  = zmsg_new();
    assert(msg);
    
    zframe_t *frame = zframe_new_empty();
    assert(frame);

    int op_code = nsrv_generate_opcode(force_send,recv_flag,msg_type);
    zsys_debug("register opcode is 0x%08x",op_code);

    if(handle_data == NULL)
    {
        zsys_error("nsrv_register_opcode op_handle data is NULL");
        return NNGN_ERR_RESOURCE;
    }

    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = register_opcode_type;
    register_s.op_code = op_code;

    rc = register_2_zframe(&register_s,frame);
    assert(rc == 0);
    rc = zmsg_push(msg,frame);
    assert(rc == 0);

    //send op code to broker
    rc = add_brokerid_frame(msg);	
    zmsg_pushstr(msg,"empty");
    rc = zmsg_send (&msg, actor_worker);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        return NNGN_ERR_RESOURCE;
    }
    msg_process[msg_type] = (int (*)(req_t*, res_t*))(handle_data);

    if(reg_flag == opcode_reg_end)
    {
        send_opcode_end_msg(actor_worker);
    }
    return NNGN_ERR_NONE;
}

extern uint32_t nsrv_register_pipeline_opcode(u_int8_t force_send,u_int8_t recv_flag,uint16_t msg_type, void *handle_data,u_int8_t reg_flag)
{
    //send opcode to broker
    int rc = -1;
    zmsg_t *msg  = zmsg_new();
    assert(msg);
    
    zframe_t *frame = zframe_new_empty();
    assert(frame);

    int op_code = nsrv_generate_opcode(force_send,recv_flag,msg_type);
    zsys_debug("register opcode is 0x%08x",op_code);

    if(handle_data == NULL)
    {
        zsys_error("nsrv_register_opcode op_handle data is NULL");
        return NNGN_ERR_RESOURCE;
    }

    msg_pipe_process[msg_type] = (int (*)(req_t*, res_t*,res_t *,char *))(handle_data);

    return NNGN_ERR_NONE;
}


uint32_t nsrv_unregister_opcode(uint32_t op_code)
{
    //send opcode to broker
    int rc = -1;
    zmsg_t *msg  = zmsg_new();
    assert(msg);
    	
    zframe_t *frame = zframe_new_empty();
    

    uint32_t opcode = GET_OPCODE_MSG_INFO(op_code);
    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = unregister_opcode_type;
    register_s.op_code = op_code;

    rc = register_2_zframe(&register_s,frame);
    assert(rc == 0);
    rc = zmsg_push(msg,frame);
    assert(rc == 0);
    rc = zmsg_send (&msg, actor_worker);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        return NNGN_ERR_RESOURCE;
    }
    msg_process[opcode] = NULL;
    msg_pipe_process[opcode] = NULL;	
    return NNGN_ERR_NONE;
}

uint32_t nsrv_destroy()
{
    zsys_info("nsrv_destroy");
    g_systeminit = false;
    zactor_destroy (&actor_worker);
    zactor_destroy (&actor_process);
    return NNGN_ERR_NONE;
}

uint32_t nsrv_generate_opcode(u_int8_t force_send,u_int8_t recv_flag,uint16_t msg_type)
{
    uint32_t opcode = (force_send<<17)+(recv_flag<<16)+msg_type;
    //zsys_debug("msg type is %d opcode is 0x%08x",msg_type,opcode);
    g_opcode[msg_type] = recv_flag;
    return opcode;
}

static int send_heart(void *worker)
{
    int rc = -1;
    zmsg_t *msg  = zmsg_new();
    assert(msg);
	
    zframe_t *frame = zframe_new_empty();
    assert(frame);

    zframe_t *frame_name = zframe_dup(heart_frame);
    assert(frame_name);

    rc = zmsg_push(msg,frame_name);
    assert(rc == 0);
    
    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = broker_heart_type;

    rc = register_2_zframe(&register_s,frame);
    assert(rc == 0);
    rc = zmsg_push(msg,frame);
    assert(rc == 0);

    add_brokerid_frame(msg);
    rc = zmsg_send(&msg,worker);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        zsys_warning("send heart fail broker disconnected");
    }
    return NNGN_ERR_NONE;
}

static int send_init_msg(void *worker,int slot)
{
    int rc = -1;
    zmsg_t *msg  = zmsg_new();
    assert(msg);
	
    zframe_t *frame = zframe_new_empty();
    assert(frame);

    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = worker_init_type;
    register_s.op_code = slot;

    rc = register_2_zframe(&register_s,frame);
    assert(rc == 0);
    rc = zmsg_push(msg,frame);
    assert(rc == 0);
    add_brokerid_frame(msg);

    rc = zmsg_send(&msg,worker);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        zsys_warning("send init msg fail broker disconnected");
        return NNGN_ERR_INTERNAL;
    }
    return NNGN_ERR_NONE;
}


static int send_opcode_end_msg(void *worker)
{
    int rc = -1;
    zmsg_t *msg  = zmsg_new();
    assert(msg);
	
    zframe_t *frame = zframe_new_empty();
    assert(frame);

    zsys_debug("send opcode end");
    worker_register_t register_s= NNGN_REGISTER__INIT;
    register_s.op = register_opcode_end;

    rc = register_2_zframe(&register_s,frame);
    assert(rc == 0);
    rc = zmsg_push(msg,frame);
    assert(rc == 0);

    //send op code to broker
    rc = add_brokerid_frame(msg);
    zmsg_pushstr(msg,"");

    rc = zmsg_send(&msg,worker);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        zsys_warning("send init msg fail broker disconnected");
    }
    return NNGN_ERR_NONE;
}



static int add_brokerid_frame(zmsg_t *msg)
{
    zframe_t *frame = zframe_new (BROKER_BACK_ID, strlen(BROKER_BACK_ID));
    //zmsg_wrap(msg, frame);
    zmsg_push(msg,frame);	
    return   NNGN_ERR_NONE;
}

//process actor recv req send res
static int
s_process_event (zloop_t *loop, zsock_t *pipe, void *argv)
{
    int rc = -1;
    zmsg_t *msg = zmsg_recv (pipe);
    assert(msg);

    char *command = zmsg_popstr (msg);
    if (streq (command, "$TERM"))
    {
        zsys_info("s_pipe_event recv term msg");
        zstr_free (&command);
        zmsg_destroy (&msg);
        return -1;
    }
    zstr_free (&command);

    return NNGN_ERR_NONE;
}

