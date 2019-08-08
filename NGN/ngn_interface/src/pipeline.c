#include "nngn.h"
#include "global.h"
#include "pipeline.h"

//om worker
zsock_t *om_pipe = NULL;
//pipe worker 
zsock_t *worker_pipe_client = NULL;
zsock_t *worker_pipe_server = NULL;
//om worker topo
zhash_t *worker_topo_list = NULL;
//pipe worker connect
zlist_t  *pipe_worker_list = NULL;
//pipe worker topo
zhash_t *worker_rev_list = NULL;

zmsg_t    *topo_msg = NULL;
int topo_num = 0;
zactor_t *actor_pipe = NULL;

zframe_t *client_frame = NULL;
zframe_t *org_req_frame = NULL;
int isPipeMsg = 0;
char g_worker[NNGX_ID_MAX_LENGTH] = {0};


extern op_callback_t msg_process[MAX_OPCODE_SIZE];

uint32_t create_om_pipe_line()
{
    int rc = -1;
    om_pipe = zsock_new(ZMQ_ROUTER);
    assert(om_pipe);
   
    zsock_set_identity(om_pipe,OM_PIPE_ID);	 

    rc = zsock_bind(om_pipe, "tipc://{%d,%d,%d}",DEFAULT_OM_PIPE_PORT,DEFAULT_OM_PIPE_PORT,DEFAULT_OM_PIPE_PORT);
    if(rc == -1)
    {
       zsys_error("create_pipe_line om pipe port is %d",DEFAULT_OM_PIPE_PORT);
       return rc;
    }

    topo_msg = zmsg_new();
    assert(topo_msg);

    client_frame = zframe_new_empty();
    assert(client_frame);

    org_req_frame= zframe_new_empty();
    assert(org_req_frame);
	
    return NNGN_ERR_NONE;
}

uint32_t close_om_pipe_line()
{
    zsock_destroy(&om_pipe);
    zhash_destroy(&worker_topo_list);	
    return NNGN_ERR_NONE;
}

uint32_t create_worker_pipe_line(int slot,char *worker)
{
    char worker_id[NNGX_WKID_MAX_LENGTH] = {0};
    int rc = -1;

    sprintf(g_worker,"%d-%s",slot,worker);
    sprintf(worker_id,"%d-%s-pipe-client",slot,worker); 
	
    worker_pipe_client = zsock_new(ZMQ_ROUTER);
    assert(worker_pipe_client);
    zsock_set_identity(worker_pipe_client,worker_id);

    sprintf(worker_id,"%d-%s-pipe",slot,worker);
    worker_pipe_server= zsock_new(ZMQ_ROUTER);
    assert(worker_pipe_server);
    zsock_set_identity(worker_pipe_server,worker_id);

    worker_rev_list=zhash_new();
    assert(worker_rev_list);

    pipe_worker_list = zlist_new();
    assert(pipe_worker_list);
    zlist_autofree(pipe_worker_list);
    zlist_comparefn(pipe_worker_list,(zlist_compare_fn *) strcmp);	
    
    rc = zsock_connect(worker_pipe_client, "tipc://{%d,%d}",DEFAULT_OM_PIPE_PORT,DEFAULT_OM_PIPE_PORT);
    assert(rc == 0);

    usleep(USLEEP_TIME);
    //read topo msg from db 
	
    actor_pipe = zactor_new (pipe_actor, NULL);
    assert (actor_pipe);
    return NNGN_ERR_NONE;
}

uint32_t close_worker_pipe_line()
{
    zactor_destroy(&actor_pipe);
    zsock_destroy(&worker_pipe_client);
    zsock_destroy(&worker_pipe_server);	
    zlist_destroy(&pipe_worker_list);
    return NNGN_ERR_NONE;
}


uint32_t add_pipeline_topo(pipe_node *pNode,int add_flag)
{
    int rc = -1;
    assert(pNode);

    zframe_t *frame_node = zframe_new_empty();
    assert(frame_node);
    rc = pipenode_2_zframe(pNode,frame_node);
    if(rc != 0)
    {
    	zsys_error("pipenode_2_zframe error");
	return NNGN_ERR_PARAM;
    }

    topo_num++;
    zmsg_append(topo_msg, &frame_node);

    if(add_flag == worker_top_end)
    {
	zframe_t *frame_type = zframe_new_empty();
    	assert(frame_type);
    	worker_register_t register_s= NNGN_REGISTER__INIT;
    	register_s.op = worker_topo_msg;
	register_s.op_code = topo_num;

	rc = register_2_zframe(&register_s,frame_type);
    	assert(rc == 0);
    	zmsg_push(topo_msg,frame_type);
    }

    return NNGN_ERR_NONE;
}


static uint32_t send_pipeline_topo(char *addr)
{
    char worker_id[NNGX_WKID_MAX_LENGTH] = {0};
    int rc = -1;
    
    sprintf(worker_id,"%s-pipe-client",addr);
	
    zmsg_t *msg  = zmsg_dup(topo_msg);
    assert(msg);

    if(topo_msg == NULL)
    {
    	   zsys_error("send pipe line topo_msg is NULL");
 	   return   NNGN_ERR_PARAM;	
    }

    zframe_t *frame_worker = zframe_new (worker_id, strlen(worker_id));
    zmsg_push(msg,frame_worker);
    //for debug
    zsys_debug("send pipeline addr is %s ",addr);
    zmsg_print(msg);

    rc = zmsg_send(&msg,om_pipe);
    if(rc != 0)
    {
        zsys_warning("send_pipeline_topo fail");
        zmsg_destroy(&msg);
    }

    return NNGN_ERR_NONE;
}

uint32_t create_pipe_msg(pipe_msg *msg,int nNum,int broadcast,char *pipe_id)
{	
	int msgNum= 0;
	int rc = -1;
	zmsg_t *msg_send = NULL;
	char pipe_worker[NNGX_WKID_MAX_LENGTH] = {0};
	assert(msg);
	assert(pipe_id);

	gen_random_string(pipe_id);

	if(broadcast == NNGN_BROAD_CAST_DISABLE)
	{
		sprintf(pipe_worker,"%s-pipe-client",msg->worker);
		msg_send = zmsg_new();
		assert(msg_send);
	}
	
	for(msgNum= 0;msgNum<nNum;msgNum++)
	{
		zframe_t *frame_req = zframe_new_empty();
		assert(frame_req);
		rc = pipemsg_2_zframe(msg, frame_req);
		if(rc != NNGN_ERR_NONE)
		{
			zsys_error("create_pipe_msg pipemsg_2_zframe error");
			return NNGN_ERR_PARAM;
		}
		if(broadcast == NNGN_BROAD_CAST_ENABLE)
		{
			sprintf(pipe_worker,"%s-pipe-client",msg->worker);
			zmsg_t *msg_broad = zmsg_new();
			assert(msg_broad);
			zmsg_append(msg_broad, &frame_req);
			send_pipe_msg(msg_broad,pipe_worker,1,pipe_id);
		}
		else
		{
			zmsg_append(msg_send, &frame_req);
		}
		msg++;
	}
	if(broadcast == NNGN_BROAD_CAST_DISABLE)
	{
		send_pipe_msg(msg_send,pipe_worker,nNum,pipe_id);
	}
	return NNGN_ERR_NONE;
}


void send_pipe_msg(zmsg_t *msg_send,char *pipe_worker,int nNum,char *pipe_id)
{
	int rc = -1;
	assert(pipe_worker);
	assert(pipe_id);

	zframe_t *org_frame = zframe_dup(org_req_frame);
	zmsg_push(msg_send, org_frame);

	zframe_t *pipe_id_frame = zframe_from(pipe_id);
	zmsg_push(msg_send, pipe_id_frame);

	
	zframe_t *current_frame = zframe_dup(client_frame);
	zmsg_push(msg_send, current_frame);

 	zframe_t *frame_type = zframe_new_empty();
    	assert(frame_type);
    	worker_register_t register_s= NNGN_REGISTER__INIT;
    	register_s.op = pipeline_msg;
       register_s.op_code = nNum;

    	rc = register_2_zframe(&register_s,frame_type);
    	assert(rc == 0);
	zmsg_push(msg_send, frame_type);	

	zframe_t * worker_frame = zframe_from(pipe_worker);
	zmsg_push(msg_send, worker_frame);

	//for debug
	if(ngn_debug_enable() == 1)
	{
		zsys_debug("create_pipe_msg send msg");
		zmsg_print(msg_send);
	}

	rc =zmsg_send(&msg_send,om_pipe);
	if(rc != 0)
       {
                zsys_warning("zmsg_send create_pipe_msg om_pipe error");
                zmsg_destroy(&msg_send);
		  return ;
       }
}

static void delete_msg(void *msg)
{
    free(msg);
    msg = NULL;
    return;
}

zsock_t *get_om_sock()
{
	return om_pipe;
}

void set_client_frame(zframe_t *frame,zframe_t *org_req)
{
	zframe_reset (client_frame, zframe_data(frame), zframe_size(frame));
	zframe_reset (org_req_frame, zframe_data(org_req), zframe_size(org_req));	
}

static void pipe_actor (zsock_t *pipe, void *args)
{
    int rc = -1;

    zsock_signal (pipe, 0);

    zloop_t *loop = zloop_new ();
    assert (loop);
	
    //zloop_set_verbose (loop, 1);
    zloop_reader_set_tolerant (loop, worker_pipe_client);
    zloop_reader_set_tolerant (loop, worker_pipe_server);	
	
    rc = zloop_reader (loop, worker_pipe_client, s_pipe_client_event, NULL);
    assert(rc == 0);

    rc = zloop_reader (loop, worker_pipe_server, s_pipe_server_event, worker_pipe_client);
    assert(rc == 0);

    rc = zloop_reader (loop, pipe, s_actor_event, NULL);
    assert(rc == 0);
	
    zloop_start (loop);
    zloop_destroy (&loop);
}

static int
s_pipe_client_event (zloop_t *loop, zsock_t *client, void *argv)
{
    	int rc = -1;
	worker_register_t register_t = NNGN_REGISTER__INIT;
    	zmsg_t *msg = zmsg_recv (client);
    	if (!msg)
    	{	
		return NNGN_ERR_NONE;
    	}

	zframe_t *frame_addr = zmsg_unwrap(msg);
	assert(frame_addr);
	
	zframe_t *frame_topo = NULL;
	pipe_node *pPipeNode = NULL;
	zframe_t *frame_register_old = zmsg_first(msg);
	assert(frame_register_old);
	
	rc = zframe_2_register(frame_register_old, &register_t);
	switch(register_t.op)
	{
		case worker_topo_msg:
			//for debug
			//zsys_debug("s_pipe_line recv topo msg");
			//zmsg_print(msg);
			
			frame_topo = zmsg_next(msg);
			//zframe_print(frame_topo, "topo frame:");
			assert(frame_topo);
			while (frame_topo) {
				pPipeNode = calloc(sizeof(pipe_node),1);
				assert(pPipeNode);

				rc = zframe_2_pipenode(frame_topo, pPipeNode);
				assert(rc == 0);

				zhash_insert(worker_rev_list,pPipeNode->worker_id,pPipeNode);
				zhash_freefn(worker_rev_list,pPipeNode->worker_id,delete_msg);
				
				if(strcmp(pPipeNode->worker_id,g_worker) == 0)
				{
					rc = zsock_bind(worker_pipe_server, "tipc://{%d,%d,%d}",pPipeNode->port,pPipeNode->port,pPipeNode->port);
					zsys_debug("bind port is %d,using tipc doesn't need ip!!",pPipeNode->port);
				}
				frame_topo = zmsg_next (msg);
    			}
			zmsg_destroy(&msg);
			break;
		case pipeline_msg:
			process_pipemsg(msg,&register_t,client);
			break;
		default:
			zsys_error("rev unknow msg type is %d",register_t.op);
			break;
	}
       return NNGN_ERR_NONE;	
}

static int
s_pipe_server_event (zloop_t *loop, zsock_t *server, void *client)
{
	int rc = -1;
	worker_register_t register_t = NNGN_REGISTER__INIT;
    	zmsg_t *msg = zmsg_recv (server);
    	if (!msg)
    	{	
		return NNGN_ERR_NONE;
    	}

	if(ngn_debug_enable() == 1)
	{
		zsys_debug("pipe server rev msg");
		zmsg_print(msg);
	}

	zframe_t *frame_addr = zmsg_unwrap(msg);
	assert(frame_addr);
	zframe_destroy(&frame_addr);
		
	zframe_t *frame_register = zmsg_first(msg);
	assert(frame_register);
	rc = zframe_2_register(frame_register, &register_t);
	switch(register_t.op)
	{
		case worker_topo_msg:
			zsys_warning("server rev topo msg type is %d",register_t.op);
			break;
		case pipeline_msg:
			process_pipemsg(msg,&register_t,client);
			break;
		default:
			zsys_error("rev unknow msg type is %d",register_t.op);
			break;
	}
	return NNGN_ERR_NONE;
}

static int
s_actor_event (zloop_t *loop, zsock_t *pipe, void *worker)
{
    int rc = -1;
    zmsg_t *msg = zmsg_recv (pipe);
    assert(msg);

    char *command = zmsg_popstr (msg);
    if (streq (command, "$TERM"))
    {
        zsys_info("s_actor_event recv term msg");
        zstr_free (&command);
        zmsg_destroy (&msg);
        return -1;
    }
    return NNGN_ERR_NONE;
}


uint32_t porcess_state_change_msg(wk_st *pState, zlist_t *pList)
{
	void *item = NULL;
	static int firsttime = 1;
	if(om_pipe != NULL)
	{
		if(firsttime)
		{
			for(item = zlist_first(pList); item != NULL; item = zlist_next(pList))
			{
				send_pipeline_topo((char *)item);
			}
			firsttime = 0;
		}
		else
		{
			if(pState->current_status == WK_ONLINE)
			{
				send_pipeline_topo(pState->uname);
			}
			else
			{
				zsys_debug("worker:%s is offline,don't need to send_pipeline_topo",pState->uname);
			}
		}
	}
	return NNGN_ERR_NONE;
}

void check_nexthop(char *nexthop)
{
	bool exists = false;
	pipe_node *node = NULL;
	int rc = -1;
	exists = zlist_exists(pipe_worker_list, nexthop);
	if(exists == false)
	{
		node = zhash_lookup(worker_rev_list, nexthop);
		if(node != NULL)
		{
			//connect next hop
			 rc = zsock_connect(worker_pipe_client, "tipc://{%d,%d}",node->port,node->port);
			 if(rc != 0)
			 {
				zsys_warning("checkout nexthop port is error",node->port); 
				return;
			 }
			 zsys_info("connect nexthop port is %d nexthop is %s",node->port,nexthop); 
			 rc = zlist_push(pipe_worker_list,nexthop);
			 assert(rc == 0);
			 usleep(USLEEP_TIME);
		}
		else
		{
			if(ngn_debug_enable() == 1)
			{
				zsys_warning("checkout nexthop unknow node ip is %s",nexthop);
			}
		}
	}
	return;
}

int checkout_nexthop_state(char *nexthop)
{
	return NNGN_ERR_NONE;
}

uint32_t process_pipemsg(zmsg_t *msg,worker_register_t *register_t,zsock_t *client )
{
	pipe_msg requset ;
	int rc = -1;
       res_t result = NNGN_RESULT__INIT;
	char nexthop[NNGX_WKID_MAX_LENGTH] = {0};   

	//for debug 
	if(ngn_debug_enable() == 1)
	{
		zsys_debug("process_pipemsg revc msg");
		zmsg_print(msg);
	}

	
	zframe_t *frame_register_old = zmsg_first(msg);
	assert(frame_register_old);

	worker_register_t register_new;
	zframe_t *frame_register_new = zframe_new_empty();
	zframe_t *frame_client = NULL;
	zframe_t *client_addr = NULL;
	zframe_t *pipe_msg = NULL;
	zframe_t *org_req = NULL;
	zframe_t *pipe_id = NULL;
	
	client_addr = zmsg_next(msg);
	assert(client_addr);

	pipe_id = zmsg_next(msg);
	assert(pipe_id);

	org_req = zmsg_next(msg);
	assert(org_req);

	pipe_msg = zmsg_next(msg);
	assert(pipe_msg);
	
	zframe_2_pipemsg(pipe_msg,&requset);

	sprintf(nexthop,"%s-pipe",requset.next_worker);
	check_nexthop(requset.next_worker);

	if (msg_process[requset.req.op_code])
    	{
		 msg_process[requset.req.op_code](&requset.req, &result);
    	}
    	else
   	{
        	zsys_error("call user fuc error opcode is 0x%04x",requset.req.op_code);
        	goto END;
    	}

	if(result.rv != 0 || register_t->op_code == 1)
	{
		//send msg to om worker
		zframe_t *result_frame = zframe_new_empty();
		assert(result_frame);
		
		zmsg_t *end_msg = zmsg_new();
		assert(end_msg);	
		//add res frame
		res_2_zframe(&result,result_frame);
		zmsg_push(end_msg,result_frame);
		//add org req
		zframe_t *org_frame = zframe_dup(org_req);
		assert(org_frame);
		zmsg_push(end_msg,org_frame);

		zframe_t *id_frame = zframe_dup(pipe_id);
		assert(id_frame);
		zmsg_push(end_msg,id_frame);

		//add client frame
		zframe_t *end_client = zframe_dup(client_addr);
		assert(end_client);
		zmsg_push(end_msg,end_client);

		//add om pipe
		add_om_frame(end_msg);

		if(ngn_debug_enable() == 1)
		{
			zsys_debug("send om msg:");
			zmsg_print(end_msg);
		}
		rc = zmsg_send(&end_msg,client);
		if(rc != 0)
		{
			zmsg_destroy(&end_msg);
			zsys_error("pipe_msg process send end_msg error");
		}
	}
	else
	{
		//send msg nexthop
		register_new.op = register_t->op;
		register_new.op_code = register_t->op_code-1;

		register_2_zframe(&register_new, frame_register_new);
		zframe_reset(frame_register_old,zframe_data(frame_register_new),zframe_size(frame_register_new));
		zmsg_remove(msg, pipe_msg);
				

		frame_client = zframe_from(nexthop);
		assert(frame_client);
		zmsg_push(msg,frame_client);

		if(ngn_debug_enable() == 1)
		{
			zsys_debug("send nexthop msg:");
			zmsg_print(msg);
		}
		rc = zmsg_send(&msg,client);
		if(rc != 0)
		{
			zsys_error("zmsg_send nexthop error");
		}
	}
END:
      zmsg_destroy(&msg);
      if(requset.req.content)
     {
        free(requset.req.content);
        requset.req.content = NULL;
     }
     if(result.content)
     {
        free(result.content);
        result.content = NULL;
     }
    return NNGN_ERR_NONE;
}


static int add_om_frame(zmsg_t *msg)
{
    zframe_t *frame = zframe_new (OM_PIPE_ID, strlen(OM_PIPE_ID));
    //zmsg_wrap(msg, frame);
    zmsg_push(msg,frame);	
    return   NNGN_ERR_NONE;
}

static void gen_random_string(char *str)
{
    zuuid_t *uuid = zuuid_new();
    assert(uuid);
    sprintf(str,"om-%s",zuuid_str(uuid));
    zuuid_destroy(&uuid);
    return ;
}

