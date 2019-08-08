#include "nngn.h"
#include "global.h"

bool g_systeminit = false;
extern char broker_addr[NNGX_ID_MAX_LENGTH];
extern wkst_change_handler changestate_handle;
extern msg_timeout_handler msg_timeout_handle;

//system init
uint32_t common_init(char *log)
{
    FILE *fp;
    //fp=fopen("/usr/local/NGN/ngn_log","w+x");
    char log_name[32] = {0};

    sprintf(log_name,"/usr/local/ngn/%s.log",log);
    zsys_dir_create("/usr/local/ngn");
    fp=fopen(log_name,"a+");
    //zsys_set_logstream(fp);
    zsys_set_logsystem(true);
    zsys_set_logident(log);
    g_systeminit = true;
    return NNGN_ERR_NONE;
}

uint32_t ngn_reg_cb(ncb_spec_t *cbspec)
{
    zsys_info("ngn_reg_cb fun type is %d",cbspec->type);
    assert(cbspec);
    switch(cbspec->type)
    {
        case CALLBACK_STATGE:
            changestate_handle = (int (*)(wk_st*,zlist_t*))(cbspec->func);
            break;
        case CALLBACK_MSG_TIMEOUT:
            msg_timeout_handle = (int (*)())(cbspec->func);
            break;
        default:
            return NNGN_ERR_PARAM;
            break;
    }

    return NNGN_ERR_NONE;
}

//获取broker当前所有worker及其状态
uint32_t ngn_get_wks(wk_st **worker_status,uint32_t *nNum)
{
    return NNGN_ERR_NONE;
}

int req_2_zframe(const req_t *req,zframe_t *frame)
{
    assert(req);
    assert(frame);
    size_t size;
    char *buf = NULL;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,4);
    mpack_write_i32(&writer,req->size);
    mpack_write_i32(&writer,req->slot);
    mpack_write_i32(&writer,req->op_code);
    mpack_write_i32(&writer,req->timeout);	
    mpack_write_bin(&writer,(const char*)req->content,req->size);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return NNGN_ERR_PARAM;
    }
    zframe_reset(frame,buf,size);
    free(buf);

    return NNGN_ERR_NONE;
}

int zframe_2_req(zframe_t *frame,req_t *req)
{
    assert(frame);
    assert(req);
    mpack_reader_t reader;
    mpack_reader_init(&reader, (char *)zframe_data(frame), zframe_size(frame), zframe_size(frame));
    //num = mpack_expect_array(&reader);
    mpack_expect_array_match(&reader, 4);
    req->size = mpack_expect_i32(&reader);
    req->slot = mpack_expect_i32(&reader);
    req->op_code= mpack_expect_i32(&reader);
    req->timeout= mpack_expect_i32(&reader);
    size_t len;
    size_t max = req->size;
      // must free by user
    req->content = (u_int8_t *)mpack_expect_bin_alloc(&reader,max,&len);

    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        free(req->content);
        req->content = NULL;
        return NNGN_ERR_PARAM;
    }
    return NNGN_ERR_NONE;
}

int pipemsg_2_zframe(const pipe_msg*pipeMsg,zframe_t *frame)
{
    assert(pipeMsg);
    assert(frame);
    size_t size;
    char *buf = NULL;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,6);
    mpack_write_str(&writer,pipeMsg->worker,strlen(pipeMsg->worker));	
    mpack_write_str(&writer,pipeMsg->next_worker,strlen(pipeMsg->next_worker));		
    mpack_write_i32(&writer,pipeMsg->req.size);
    mpack_write_i32(&writer,pipeMsg->req.slot);
    mpack_write_i32(&writer,pipeMsg->req.op_code);
    mpack_write_bin(&writer,(const char*)(pipeMsg->req.content),pipeMsg->req.size);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return NNGN_ERR_PARAM;
    }
    zframe_reset(frame,buf,size);
    free(buf);

    return NNGN_ERR_NONE;
}

int zframe_2_pipemsg(zframe_t *frame,pipe_msg *pipeMsg)
{
    assert(frame);
    assert(pipeMsg);
    mpack_reader_t reader;
    mpack_reader_init(&reader, (char *)zframe_data(frame), zframe_size(frame), zframe_size(frame));
    //num = mpack_expect_array(&reader);
    mpack_expect_array_match(&reader, 6);
    mpack_expect_cstr(&reader,pipeMsg->worker,NNGX_WKID_MAX_LENGTH);	
    mpack_expect_cstr(&reader,pipeMsg->next_worker,NNGX_WKID_MAX_LENGTH);		
    pipeMsg->req.size = mpack_expect_i32(&reader);
    pipeMsg->req.slot = mpack_expect_i32(&reader);
    pipeMsg->req.op_code= mpack_expect_i32(&reader);

    size_t len;
    size_t max = pipeMsg->req.size;
      // must free by user
    pipeMsg->req.content = (u_int8_t *)mpack_expect_bin_alloc(&reader,max,&len);

    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        free(pipeMsg->req.content);
        pipeMsg->req.content = NULL;
        return NNGN_ERR_PARAM;
    }
    return NNGN_ERR_NONE;
}

int pipenode_2_zframe(const pipe_node *pipeNode,zframe_t *frame)
{
    assert(pipeNode);
    assert(frame);
    size_t size;
    char *buf = NULL;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,2);
    mpack_write_str(&writer,pipeNode->worker_id,strlen(pipeNode->worker_id));		
    mpack_write_i32(&writer,pipeNode->port);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return NNGN_ERR_PARAM;
    }
    zframe_reset(frame,buf,size);
    free(buf);

    return NNGN_ERR_NONE;
}

int zframe_2_pipenode(zframe_t *frame,pipe_node *pipeNode)
{
    assert(frame);
    assert(pipeNode);
    mpack_reader_t reader;
    mpack_reader_init(&reader, (char *)zframe_data(frame), zframe_size(frame), zframe_size(frame));
    //num = mpack_expect_array(&reader);
    mpack_expect_array_match(&reader, 2);
    mpack_expect_cstr(&reader,pipeNode->worker_id,NNGX_WKID_MAX_LENGTH);		
    pipeNode->port= mpack_expect_i32(&reader);

    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        return NNGN_ERR_PARAM;
    }
    return NNGN_ERR_NONE;
}


int res_2_zframe(res_t *res,zframe_t *frame)
{
    assert(res);
    assert(frame);
    size_t size;
    char *buf = NULL;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,5);
    mpack_write_i16(&writer,res->rv);
    mpack_write_i32(&writer,res->slot);
    mpack_write_i32(&writer,res->size);
    if(res->size != 0)
    {
        mpack_write_bin(&writer,(const char *)res->content,res->size);
    }
    mpack_write_u8(&writer,res->seria_op);

    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return NNGN_ERR_PARAM;
    }
    zframe_reset(frame,buf,size);
    free(buf);
    return NNGN_ERR_NONE;
}

int zframe_2_res(zframe_t *frame,res_t *res)
{
    assert(frame);
    assert(res);
    mpack_reader_t reader;

    mpack_reader_init(&reader, (char *)zframe_data(frame), zframe_size(frame), zframe_size(frame));
    mpack_expect_array_match(&reader, 5);
    res->rv = mpack_expect_i16(&reader);
    res->slot = mpack_expect_i32(&reader);
    res->size= mpack_expect_i32(&reader);
    size_t len = 0;
    size_t max = res->size;
    if(res->size != 0)
    {
        res->content = (u_int8_t *)mpack_expect_bin_alloc(&reader, max, &len);
    }
    else
    {
        res->content = NULL;
    }
    res->seria_op= mpack_expect_i8(&reader);
    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        return NNGN_ERR_PARAM;
    }
    return NNGN_ERR_NONE;
}

int register_2_zframe(worker_register_t *reg,zframe_t *frame)
{
    assert(reg);
    assert(frame);
    size_t size;
    char *buf = NULL;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,2);
    mpack_write_i8(&writer,reg->op);
    mpack_write_i32(&writer,reg->op_code);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return NNGN_ERR_PARAM;
    }

    zframe_reset(frame,buf,size);
    free(buf);

    return NNGN_ERR_NONE;
}

int zframe_2_register(zframe_t *frame,worker_register_t *reg)
{
    assert(frame);
    assert(reg);
    //int num = 0;
    mpack_reader_t reader;
    mpack_reader_init(&reader, (char *)zframe_data(frame), zframe_size(frame), zframe_size(frame));
    //num = mpack_expect_array(&reader);
    mpack_expect_array_match(&reader,2);

    reg->op = mpack_expect_i8(&reader);
    reg->op_code= mpack_expect_i32(&reader);

    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        return NNGN_ERR_PARAM;
    }
    return NNGN_ERR_NONE;
}

void wkst_2_buf(wk_st *wkst,char **buf,int *size)
{
    assert(wkst);
    assert(buf);

    char *buf_inner = NULL;
    size_t len = 0;
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf_inner, &len);
    mpack_start_array(&writer,4);
    mpack_write_i32(&writer,wkst->slot);
    mpack_write_str(&writer,wkst->uname,strlen(wkst->uname));
    mpack_write_i32(&writer,wkst->current_status);
    mpack_write_i32(&writer,wkst->old_status);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf_inner);
        return;
    }
    assert(buf_inner);
    *buf = calloc(len,sizeof(char));
    memcpy(*buf,buf_inner,len);
    *size = len;
    free(buf_inner);
}

void buf_2_wkst(char *buf,int size,wk_st *wkst)
{
    assert(buf);
    assert(wkst);

    mpack_reader_t reader;
    mpack_reader_init(&reader, buf, size, size);
    mpack_expect_array_match(&reader,4);

    wkst->slot = mpack_expect_i32(&reader);
    mpack_expect_cstr(&reader,wkst->uname,NNGX_ID_MAX_LENGTH);
    wkst->current_status = mpack_expect_i32(&reader);
    wkst->old_status = mpack_expect_i32(&reader);
    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        return ;
    }
}

void list_2_buf(zlist_t *pList,char **buf,int *size)
{
    assert(pList);
    assert(buf);

    void *item = zlist_first(pList);
    char *buf_inner = NULL;
    size_t len = 0;
    size_t count = zlist_size(pList);
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &buf_inner, &len);
    mpack_start_array(&writer,2);
    mpack_write_i32(&writer,count);
    mpack_start_array(&writer,count);
    while(item)
    {
        mpack_write_cstr(&writer,item);
        item = zlist_next(pList);
    }
    mpack_finish_array(&writer);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf_inner);
        return;
    }
    assert(buf_inner);
    *buf = calloc(len,sizeof(char));
    memcpy(*buf,buf_inner,len);
    *size = len;
    free(buf_inner);
}

void buf_2_list(char *buf,int size,zlist_t *pList)
{
    assert(buf);
    assert(pList);

    size_t count;
    int i;
    void *item = NULL;

    mpack_reader_t reader;
    mpack_reader_init(&reader, buf, size, size);
    mpack_expect_array_match(&reader,2);

    count = mpack_expect_i32(&reader);
    mpack_expect_array_match(&reader,count);
    for(i = 0; i < count; i++)
    {
        item = calloc(NNGX_ID_MAX_LENGTH,sizeof(char));
        mpack_expect_cstr(&reader,item,NNGX_ID_MAX_LENGTH);
        assert(zlist_append(pList, item) == 0);
        free(item);
    }
    mpack_done_array(&reader);
    mpack_done_array(&reader);
    if (mpack_reader_destroy(&reader) != mpack_ok)
    {
        return;
    }
}

uint32_t ngn_debug_enable(void)
{
   char *env_name = "DEBUG";
   char *env_value = NULL;
   int enable = 0;
 
   env_value = getenv(env_name);
   if(env_value){
       enable = 1;
   }
 
   return enable;
}




