//help broker manger data struct
#include <czmq.h>
#include "ctx.h"

zhash_t *mutil_msg_list = NULL;
zhash_t *workerid_obj = NULL;
zlist_t *pList = NULL;

int ctx_init()
{
    int rc = -1;
    mutil_msg_list = zhash_new();
    assert(mutil_msg_list);
    workerid_obj = zhash_new();
    assert(workerid_obj);
    pList = zlist_new();
    assert(pList);
    zlist_autofree(pList);
    zlist_comparefn(pList,(zlist_compare_fn *) strcmp);

    //check_db();	
    reload_workerinfo();

    rc = reload_msginfo();
    if(rc == -1)
    {
        zsys_warning("reload_workerinfo read conf error");
    }
    return NNGN_ERR_NONE;
}

int register_worker(char *worker_id,int slot_id)
{
    int rc = -1;
    bool pub_state = true;
    zsys_debug("register worker worker_id is %s",worker_id);
    worker_t *pWorker = get_worker_by_id(worker_id);
    if(pWorker)
    {
        if(pWorker->worker_state == WK_ONLINE)
        {
            pub_state = true;
        }
        zhash_delete(workerid_obj,worker_id);
    }

    pWorker = calloc(sizeof(worker_t),1);
    assert(pWorker);
    memset(pWorker->msgcode_opcode,0,sizeof(int)*MAX_OPCODE_SIZE);
    pWorker->heart_time = zclock_time();
    pWorker->worker_state = WK_ONLINE;
    pWorker->slot = slot_id;
    memcpy(pWorker->worker_id,worker_id,strlen(worker_id));

    rc = zhash_insert(workerid_obj, worker_id, pWorker);
    assert(rc == 0);

    zhash_freefn(workerid_obj,worker_id,delete_worker);

    if(pub_state)
    {
        pub_worker_state_msg(worker_id,slot_id,WK_ONLINE, WK_OFFLINE);
    }
    save_workername_list();
    return NNGN_ERR_NONE;
}

void delete_worker(void *worker)
{
    free(worker);
    worker = NULL;
}

void heart_msg_handle(char *worker_id,int optype)
{
    worker_t *pWorker = get_worker_by_id(worker_id);
    
    if(pWorker == NULL)
    {
       zsys_warning("recv heart worker is NULL");
    	return;
    }
    switch(optype)
    {
        case TIMEOUT_HEART:
            zsys_warning("worker heart timeout id is %s",worker_id);
            if(pWorker->worker_state == WK_ONLINE)
            {
                pWorker->worker_state = WK_OFFLINE;
                //pub offline msg
                pub_worker_state_msg(worker_id, pWorker->slot,WK_OFFLINE, WK_ONLINE);
            }
            break;
        case REV_HEART:
            pWorker->heart_time = zclock_time();
            if(pWorker->worker_state == WK_OFFLINE)
            {
                pWorker->worker_state = WK_ONLINE;
                //pub online msg
                pub_worker_state_msg(worker_id, pWorker->slot,WK_ONLINE, WK_OFFLINE);
            }
            break;
        default:
            break;
    }
    return;
}


//for mutil msg
int register_mutil_msg(zframe_t *addr,zlist_t *worker_id,int timeout)
{
    int rc = -1;
    mutil_msg_t *msg = calloc(sizeof(mutil_msg_t),1);
    assert(msg);

    msg->worker_id = worker_id;
    msg->start_time = zclock_time();
    msg->msg = zmsg_new();
    msg->timeout = timeout;

    rc = zmsg_pushstr(msg->msg, "");
    assert(rc == 0);

    rc = zmsg_pushmem(msg->msg, zframe_data(addr),zframe_size(addr));
    assert(rc == 0);

    assert(msg->msg);
    char *client_id = zframe_strhex(addr);
    assert(client_id != NULL);
    memcpy(msg->client_id,client_id,strlen(client_id));
    rc = zhash_insert(mutil_msg_list, client_id, msg);
    assert(rc == 0);
    zhash_freefn(mutil_msg_list,client_id,delete_msg);
    free(client_id);
    return NNGN_ERR_NONE;
}

mutil_msg_t *get_mutil_msg(char *client_id)
{
    assert(client_id);
    return zhash_lookup(mutil_msg_list, client_id);
}

void destroy_mutil_msg(char *client_id)
{
    zhash_delete(mutil_msg_list, client_id);
    return;
}

void delete_msg(void *msg)
{
    zlist_destroy(&((mutil_msg_t *)msg)->worker_id);
    free(msg);
    msg = NULL;
    return;
}

void delete_mutilmsg_workerid(char *client_id,char *worker_id)
{
    mutil_msg_t *msg = get_mutil_msg(client_id);
    assert(msg);
    zlist_t *list = msg->worker_id;
    assert(list);
    //safe call this fuc

    zlist_remove(list,worker_id);
    return;
}


int check_mutilmsg_success(char *client_id)
{
    mutil_msg_t *msg = get_mutil_msg(client_id);
    assert(msg);
    if(zlist_size(msg->worker_id) == 0)
    {
        //success
        return NNGN_ERR_NONE;
    }
    return NNGN_ERR_SERVER_TYPE;
}

//for opcode
int register_opcode(int opcode,char *worker_id)
{
    zsys_debug("regisetr opcode is 0x%08x",opcode);
    int msg_code = 0;
    worker_t *pWorker = get_worker_by_id(worker_id);
    assert(pWorker);
    msg_code = GET_OPCODE_MSG_INFO(opcode);
    pWorker->msgcode_opcode[msg_code] = opcode;
    return NNGN_ERR_NONE;
}

int unregister_opcode(int opcode,char *worker_id)
{
    int msg_code = 0;
    worker_t *pWorker = get_worker_by_id(worker_id);
    assert(pWorker);
    msg_code = GET_OPCODE_MSG_INFO(opcode);
    pWorker->msgcode_opcode[msg_code] = 0;
    return NNGN_ERR_NONE;
}

int get_msginfo(worker_t *pWorker,int opcode,uint8_t *sendflag,uint8_t *recvflag)
{
    int op_code = 0;
    assert(pWorker);
    op_code = pWorker->msgcode_opcode[opcode];
    if(op_code== 0)
    {
        return NNGN_ERR_SERVER_TYPE;
    }
    *sendflag = GET_OPCODE_SEND_BIT(op_code);
    *recvflag = GET_OPCODE_RECV_BIT(op_code);
    return NNGN_ERR_NONE;
}

void workerinfo_2_buf(worker_t *pWorker,char **buf,size_t *size)
{
     assert(pWorker);
     int index = 0;
     char *buf_inner = NULL;
     mpack_writer_t writer;
     mpack_writer_init_growable(&writer, &buf_inner, (size_t *)size);
     mpack_start_array(&writer,3);	 
     mpack_write_i32(&writer,pWorker->slot);
     mpack_write_cstr(&writer,pWorker->worker_id);
     //mpack_finish_array(&writer);
     mpack_start_array(&writer,MAX_OPCODE_SIZE);
     for(index = 0;index<MAX_OPCODE_SIZE;index++)
     {
        mpack_write_i32(&writer,pWorker->msgcode_opcode[index]);
     }

     mpack_finish_array(&writer);
      mpack_finish_array(&writer);
     if (mpack_writer_destroy(&writer) != mpack_ok) {
         free(buf_inner);
         return;
     }

     assert(buf_inner);
     *buf = calloc(*size,sizeof(char));
     memcpy(*buf,buf_inner,*size);
     free(buf_inner);
     return;
}

void buf_2_workerinfo(char *buf,int size,worker_t *pWorker)
{
     assert(pWorker);
     assert(buf);
     int index = 0;

     mpack_reader_t reader;
     mpack_reader_init(&reader,buf,sizeof(buf),size);
     mpack_expect_array_match(&reader,3);
     pWorker->slot = mpack_expect_i32(&reader);
     mpack_expect_cstr(&reader,pWorker->worker_id,NNGX_ID_MAX_LENGTH);
     mpack_expect_array_match(&reader,MAX_OPCODE_SIZE);
     for(index = 0;index<MAX_OPCODE_SIZE;index++)
     {
         pWorker->msgcode_opcode[index] = mpack_expect_i32(&reader);
     }
     pWorker->worker_state = WK_OFFLINE;
     mpack_done_array(&reader);
     mpack_done_array(&reader); 	 
     if (mpack_reader_destroy(&reader) != mpack_ok)
     {
        fprintf(stderr, "An error occurred decoding the data!\n");
     }
}

//for save worker info
void save_workerinfo(char *worker_id)
{
    char *buf = NULL;
    size_t size = 0;

    worker_t *pWorker = get_worker_by_id(worker_id);

    workerinfo_2_buf(pWorker,&buf,&size);

    zsys_debug("save_workerinfo worker id is %s size is %d",worker_id,size);
    int ret = put_data_local(worker_id, strlen(worker_id), buf, size,DB_FLAG_LOCAL);
    if(ret != 0)
    {
    	zsys_warning("put_data_local error ret=%d",ret);
    }
    free(buf);
    buf=NULL;
}

void buf_2_workerlist(zlist_t *list)
{
    void *item = zlist_first(list);
    char *buf = NULL;
    int rc = 0;
    int size = 0;
    while(item)
    {
        rc = get_data_local(item,strlen(item), &buf, &size,DB_FLAG_LOCAL);
        if(rc != 0)
        {
            zsys_error("worker id is %s buf is NULL",item);
            item = zlist_next(list);
            continue;
        }

        assert(buf);
        worker_t *pWorker = NULL;
        pWorker = calloc(sizeof(worker_t),1);
        assert(pWorker);
        memset(pWorker->msgcode_opcode,0,sizeof(int)*MAX_OPCODE_SIZE);
        memcpy(pWorker->worker_id,item,strlen(item));
        buf_2_workerinfo(buf,size,pWorker);
        rc = zhash_insert(workerid_obj, item, pWorker);
        assert(rc == 0);

        zhash_freefn(workerid_obj,item,delete_worker);

        zsys_debug("insert workerid is %s slot is %d ",pWorker->worker_id,pWorker->slot);
        item = zlist_next(list);
        free(buf);
    }
}

void workerlist_2_buf(zlist_t *list)
{
    void *item = zlist_first(list);
    int size = zlist_size(list);
    size_t buf_size = 0;
    mpack_writer_t writer;
    char *buf_inner = NULL;

    mpack_writer_init_growable(&writer, &buf_inner, &buf_size);
    mpack_start_array(&writer,size);
    while(item)
    {
        mpack_write_cstr(&writer,item);
        item = zlist_next(list);
    }
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok)
    {
         zsys_debug("mpack_writer_destroy size is %d",size);
         free(buf_inner);
         return;
    }
    assert(buf_inner);
    put_data_local(WORKERKEY, strlen(WORKERKEY), buf_inner, buf_size,DB_FLAG_LOCAL);
    free(buf_inner);
    return;
}



void reload_workerinfo()
{
    char *buf = NULL;
    int size = 0;
    int index = 0;
    char worker_id[NNGX_ID_MAX_LENGTH] = {0};

    zlist_t *list = zlist_new();
    zlist_autofree(list);

    int ret = get_data_local(WORKERKEY, strlen(WORKERKEY), &buf, &size,DB_FLAG_LOCAL);

    if(ret != 0)
    {
        return;
    }
    assert(buf);
    #if 0
    mpack_tree_t tree;
    mpack_tree_init(&tree,buf,size);
    mpack_node_print_file(mpack_tree_root(&tree), stdout);
    mpack_tree_destroy(&tree);
    #endif

    mpack_reader_t reader;
    mpack_reader_init(&reader,buf,size,size);
    int num = mpack_expect_array_max(&reader,32);
    //mpack_expect_array_match(&reader, num);
    for(index=0; index<num; index++)
    {
        mpack_expect_cstr(&reader,worker_id,NNGX_ID_MAX_LENGTH);
        zsys_debug("reload_workerinfo workerid is %s",worker_id);
        zlist_push(list,worker_id);
    }
    buf_2_workerlist(list);
    zlist_destroy(&list);
    free(buf);
}

//for save multil msg info
void save_msginfo()
{
    return;
}

int reload_msginfo()
{
    return NNGN_ERR_NONE;
}


worker_t *get_worker_by_id(const char *worker_id)
{
    assert(worker_id);
    return zhash_lookup(workerid_obj, worker_id);
}

wk_st_t  get_workerstate_by_id(const char *worker_id)
{
    assert(worker_id);
    void *worker_obj = zhash_lookup(workerid_obj, worker_id);
    if(worker_obj == NULL)
    {
        return WK_ERROR;
    }
    return ((worker_t *)worker_obj)->worker_state;
}

void save_workername_list()
{
    zlist_t *list = zhash_keys(workerid_obj);
    workerlist_2_buf(list);
    zlist_destroy(&list);
}

void ctx_exit()
{
    //first save worker list
    zhash_destroy(&mutil_msg_list);
    zhash_destroy(&workerid_obj);
    zlist_destroy(&pList);
    return;
}

void send_msg_worker(zmsg_t *msg,char *workerid,void *socket)
{
    assert(msg);
    int rc = -1;
    zmsg_t *msg_send = zmsg_dup(msg);
    assert(msg_send);

    assert(zmsg_pushstr (msg_send, workerid) == 0);

    rc = zmsg_send(&msg_send,socket);
    if(rc != 0)
    {
        zmsg_destroy(&msg_send);
    }
    return;
}

void pub_worker_state_msg(char *workerid,int slot,int current_state,int old_state)
{
    zsys_debug("pub_worker_state_msg workerid is %s currentstate is %d",workerid,current_state);
    wk_st wks = {0,{0},0,0};
    //char *buf = NULL;
    //int size = 0;

    wks.current_status = current_state;
    wks.old_status = old_state;
    wks.slot = (uint32_t)(1) << (slot-1);
    memcpy(wks.uname,workerid,strlen(workerid));
    /*wkst_2_buf(&wks, &buf,&size);
    assert(buf);
    pub_state_msg(buf,size);
    if(buf != NULL)
    {
        free(buf);
    }*/
    pub_state_msg(&wks);
}

int pub_state_msg(struct _wk_st *wks)
{
    zsys_debug("pub_state_msg workerid is %s currentstate is %d oldstatus is %d slot is %d",wks->uname,wks->current_status,wks->old_status,wks->slot);
    if(wks->current_status == WK_ONLINE)
    {
        zsys_debug("add worker:%s",wks->uname);
		assert(zlist_append(pList, wks->uname) == 0);
    }
	else
	{
	    zsys_debug("remove worker:%s",wks->uname);
		assert(zlist_exists(pList, wks->uname));
		zlist_remove(pList, wks->uname);
	}
    char *buf = NULL;
    char *msgbuf = NULL;
    int size = 0;
    int msgsize = 0;
    int rc = -1;

    zmsg_t *msg = zmsg_new();
    assert(msg);

    list_2_buf(pList, &msgbuf, &msgsize);
    assert(msgbuf);
    zframe_t *msgframe = zframe_new(msgbuf,msgsize);
    assert(msgframe);
    rc = zmsg_push(msg,msgframe);
    assert(rc == 0);

    wkst_2_buf(wks, &buf, &size);
    assert(buf);
    zframe_t *frame = zframe_new(buf,size);
    assert(frame);
    rc = zmsg_push(msg,frame);
    assert(rc == 0);

    rc = zmsg_pushstr(msg,BROKER_PUBLISH_STR);
    assert(rc == 0);

    zmsg_print(msg);
    rc = zmsg_send(&msg,publisher);
    if(rc != 0)
    {
        zmsg_destroy (&msg);
        zsys_warning("send new_pub_state_msg fail broker disconnected");
    }
    return NNGN_ERR_NONE;
}

//add by youxs
void send_res_msg(zframe_t *client_id,void *socket)
{
    struct _res_t res = {0,0,0,NULL,0};
    int rc = 0;
    zmsg_t *msg = zmsg_new();

    rc = zmsg_pushstr(msg, "");
    assert(rc == 0);
    rc = zmsg_pushmem(msg, zframe_data(client_id),zframe_size(client_id));
    assert(rc == 0);

    zframe_t *frame_res = zframe_new_empty();

    res.rv = MSG_NO_ANSWER_ERROR_CODE;
    res.size = 0;

    rc = res_2_zframe(&res, frame_res);
    assert(rc == 0);

    rc = zmsg_append(msg,&frame_res);
    assert(rc == 0);

    rc = zmsg_send(&msg,socket);
    if(rc != 0)
    {
        zmsg_destroy(&msg);
    }
}


