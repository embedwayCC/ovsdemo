//send.c
//包含zmq的头文件
#include "nngn.h"
#include "client.h"
#include "common.h"

zsock_t *client = NULL;
zsock_t *asyn_client = NULL;


uint32_t nclt_new()
{
    char client_id[NNGX_ID_MAX_LENGTH] = {0};
    char asyn_id[NNGX_ID_MAX_LENGTH] = {0};
    zsys_info("nclt_new now doesn't need broker_addr!");
    uint32_t ret = 0;
    gen_random_string(client_id);
    gen_asyn_random_string(asyn_id);
    if(!g_systeminit)
    {
       ret=common_init("cli");
       if(ret != NNGN_ERR_NONE)
       {
           return ret;
       }
    }

    client = zsock_new(ZMQ_REQ);
    assert(client);
    asyn_client = zsock_new(ZMQ_ROUTER);
    assert(asyn_client);

    zsock_set_identity(client,client_id);
    zsock_set_identity(asyn_client,asyn_id);
    //reconnect until connected
    do
    {
       ret = zsock_connect(client, "tipc://{%d,%d}",DEFAULT_BROKER_FRONT_PORT,DEFAULT_BROKER_FRONT_PORT);
       ret = zsock_connect(asyn_client, "tipc://{%d,%d}",DEFAULT_BROKER_FRONT_PORT,DEFAULT_BROKER_FRONT_PORT);
    }
    while(ret != 0);
    sleep(2);
    zsys_info("client connect broker success");
    return NNGN_ERR_NONE;
}

uint32_t rebuild_sock()
{
    assert (client);
    char client_id[NNGX_ID_MAX_LENGTH] = {0};
    uint32_t ret = 0;
    zsock_destroy(&client);

    client = zsock_new(ZMQ_REQ);
    assert(client);
    gen_random_string(client_id);
    zsock_set_identity(client,client_id);

    //reconnect until connected
    do
    {
      ret = zsock_connect(client, "tipc://{%d,%d}",DEFAULT_BROKER_FRONT_PORT,DEFAULT_BROKER_FRONT_PORT);
    }
    while(ret != 0);
    zsys_info("client rebuild broker success");
    return NNGN_ERR_NONE;
}


uint32_t nclt_sr(req_t *req, res_t *res)
{
    int rc = -1;
    int size = 0;

    if(req == NULL || res == NULL)
    {
        return NNGN_ERR_PARAM;
    }

    if(client == NULL)
    {
        return NNGN_ERR_BAD_REQUEST;
    }

    zmsg_t *msg_send = zmsg_new();
    assert(msg_send);
    req->timeout = NGN_TIMEOUT_NONE;
    zframe_t *frame_req = zframe_new_empty();
    rc = req_2_zframe(req,frame_req);
    if(rc != 0)
    {
        zsys_error("req_2_zframe error");
        zmsg_destroy(&msg_send);
        return NNGN_ERR_PARAM;
    }

    rc = zmsg_push(msg_send,frame_req);
    assert(rc == 0);
    rc = zmsg_send(&msg_send,client);
    if(rc != 0)
    {
        return NNGN_ERR_CONNECT;
    }

    zmsg_t *msg_recv = zmsg_recv(client);
    assert(msg_recv);
    //zmsg_print(msg_recv);
    size = zmsg_size(msg_recv);
    if(size > 1)
    {
        zsys_warning("nclt_sr recv more size is %d",size);
    }
    zframe_t *frame_res = zmsg_last(msg_recv);
    assert(frame_res);
    rc = zframe_2_res(frame_res, res);
    if(rc != 0)
    {
        zsys_debug("zframe size is %d",zframe_size(frame_res));
        zframe_print(frame_res,"frame_2_res");
        zsys_error("zframe_2_res error");
        zmsg_destroy(&msg_recv);
        return NNGN_ERR_PARAM;
    }
    zmsg_destroy(&msg_recv);
    return NNGN_ERR_NONE;
}

uint32_t nclt_sr_plus(req_t *req, res_t **res,uint32_t *nNum,  uint32_t timeout)
{
     int rc = -1;
     int nIndex = 0;

     if(req == NULL || *res == NULL)
     {
         return NNGN_ERR_PARAM;
     }

     if(client == NULL)
     {
        return NNGN_ERR_BAD_REQUEST;
     }

     req->timeout = timeout;
     zmsg_t *msg_send = zmsg_new();
     assert(msg_send);
     zframe_t *frame_req = zframe_new_empty();

     rc = req_2_zframe(req,frame_req);
     if(rc != 0)
     {
         zsys_error("req_2_zframe error");
         zmsg_destroy(&msg_send);
         return NNGN_ERR_PARAM;
     }

     rc = zmsg_push(msg_send,frame_req);
     assert(rc == 0);

     if(timeout == 0)
     {
       add_brokerid_frame(msg_send);
       rc = zmsg_send(&msg_send,asyn_client);
     	if(rc != 0)
     	{
       	zmsg_destroy(&msg_send);
         	zsys_error("zmsg_send error");
         	return NNGN_ERR_CONNECT;
     	}
       return NNGN_ERR_NONE;
     }
     else
     {
       zmsg_print(msg_send); 
     	rc = zmsg_send(&msg_send,client);
     	if(rc != 0)
     	{
       	 zmsg_destroy(&msg_send);
         	zsys_error("zmsg_send error");
         	return NNGN_ERR_CONNECT;
     	}
     }

     zpoller_t *poller = zpoller_new (client, NULL);
     assert(poller);
     zsock_t *client = (zsock_t *) zpoller_wait (poller, timeout);
     if(client == NULL)
     {
        //timeout
        *nNum = 0;
        rebuild_sock();
        zpoller_destroy (&poller);
        if(timeout == 0)
        {
            rc = NNGN_ERR_NONE;
        }
        else
        {
            rc = NNGN_ERR_TIMEOUT;
        }
        return rc;
     }
     zpoller_destroy (&poller);

     zmsg_t *msg_recv = zmsg_recv(client);
     assert(msg_recv);
     //zmsg_print(msg_recv);
     zframe_t *frame = zmsg_first (msg_recv);
     while (frame)
     {
          rc = zframe_2_res(frame, res[nIndex]);
          if(rc != 0)
          {
             frame = zmsg_next (msg_recv);
             zsys_error("zframe_2_res error");
             continue;
          }
          frame = zmsg_next (msg_recv);
          nIndex++;
     }
     *nNum = nIndex;
     zmsg_destroy(&msg_recv);
     zpoller_destroy (&poller);
     return NNGN_ERR_NONE;

}

uint32_t nclt_destroy()
{
    zsock_destroy(&client);
    zsock_destroy(&asyn_client);
    return NNGN_ERR_NONE;
}

uint32_t nclt_req_fill(req_t *req, u_int32_t size, uint32_t op_code, uint32_t slot, void *content)
{
    req->size = size;
    req->op_code = op_code;
    req->slot = slot;
    req->content = content;
    return NNGN_ERR_NONE;
}

static void gen_random_string(char *str)
{
    sprintf(str,"client-%ld",zclock_time());
    return ;
}

static void gen_asyn_random_string(char *str)
{
    sprintf(str,"test-%ld",zclock_time());
    return ;
}

static int add_brokerid_frame(zmsg_t *msg)
{
    zframe_t *frame = zframe_new (BROKER_FRONT_ID, strlen(BROKER_FRONT_ID));
    //zmsg_wrap(msg, frame);
    zmsg_pushstr(msg,"");
    //zmsg_pushstr(msg,"timeout-client");	
    zmsg_push(msg,frame);	
    return   NNGN_ERR_NONE;
}


