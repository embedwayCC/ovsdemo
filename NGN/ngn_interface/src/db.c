#include "db.h"
char db_addr[NNGX_ID_MAX_LENGTH];
zsock_t *req = NULL;

pthread_mutex_t mutext =PTHREAD_MUTEX_INITIALIZER;

uint32_t database_init(const char *addr)
{
    int ret = 0;
    if(req != NULL)
    {
    	return NNGN_ERR_NONE;
    }
    req = zsock_new(ZMQ_REQ);
    assert(req);
    //reconnect until connected
    memcpy(db_addr,addr,strlen(addr));
    do
    {
       ret = zsock_connect(req, "tipc://{%d,%d}",DEFAULT_BROKER_REP_PORT,DEFAULT_BROKER_REP_PORT);
    }
    while(ret != 0);
    zsys_info("database init success broker_addr is %s",db_addr);
    return NNGN_ERR_NONE;
}

void database_exit()
{
    zsock_destroy(&req);
    pthread_mutex_destroy(&mutext);
}

uint32_t rebuild()
{
    int ret = 0;
    zsock_destroy(&req);
    req = zsock_new(ZMQ_REQ);
    assert(req);
    //reconnect until connected
    do
    {
       ret = zsock_connect(req, "tipc://{%d,%d}",DEFAULT_BROKER_REP_PORT,DEFAULT_BROKER_REP_PORT);
    }
    while(ret != 0);
    zsys_info("rebuild req success");
    return NNGN_ERR_NONE;
}

uint32_t put_data(char *key,int key_size,char *value,int value_size)
{
    int ret = 0;
    zmsg_t *msg = zmsg_new();
    assert(msg);
    pthread_mutex_lock(&mutext);
    //push option str
    zmsg_pushstr(msg, BROKER_DBPUT_STR);
    zframe_t *frame_key = zframe_new(key,key_size);
    assert(frame_key);
    zmsg_append(msg,&frame_key);

    zframe_t *frame_value = zframe_new(value,value_size);
    assert(frame_value);
    zmsg_append(msg,&frame_value);

    ret = zmsg_send(&msg,req);
    if(ret != 0)
    {
        pthread_mutex_unlock(&mutext);
        zmsg_destroy(&msg);
    }
    zpoller_t *poller = zpoller_new (req, NULL);
    assert(poller);
    zsock_t *req = (zsock_t *) zpoller_wait (poller, DB_TIMEOUT);
    if(req == NULL)
    {
        zpoller_destroy (&poller);
        rebuild();
        pthread_mutex_unlock(&mutext);
        return NNGN_ERR_TIMEOUT;
    }
    zpoller_destroy (&poller);

    zmsg_t *msg_recv = zmsg_recv(req);
    assert(msg_recv);
    //zmsg_print(msg_recv);
    char *str_ret = zmsg_popstr(msg_recv);
    assert(str_ret);
    if(strcmp(str_ret,DB_RET_SUCCESS) == 0)
    {
        ret = NNGN_ERR_NONE;
    }
    else
    {
        zsys_error("put_data error");
        ret = NNGN_ERR_PARAM;
    }
    zstr_free(&str_ret);

    zmsg_destroy(&msg_recv);
    pthread_mutex_unlock(&mutext);
    return ret;
}

uint32_t get_data(char *key,int key_size,char **value,int *value_size)
{
    int ret = 0;
    zmsg_t *msg = zmsg_new();
    assert(msg);
    pthread_mutex_lock(&mutext);
    //push option str
    zmsg_pushstr(msg, BROKER_DBGET_STR);
    zframe_t *frame_key = zframe_new(key,key_size);
    assert(frame_key);
    zmsg_append(msg,&frame_key);

    ret = zmsg_send(&msg,req);
    if(ret != 0)
    {
        pthread_mutex_unlock(&mutext);
        zmsg_destroy(&msg);
    }
    zpoller_t *poller = zpoller_new (req, NULL);
    assert(poller);
    zsock_t *req = (zsock_t *) zpoller_wait (poller, DB_TIMEOUT);
    if(req == NULL)
    {
        zpoller_destroy (&poller);
        rebuild();
        pthread_mutex_unlock(&mutext);
        return NNGN_ERR_TIMEOUT;
    }
    zpoller_destroy (&poller);

    zmsg_t *msg_recv = zmsg_recv(req);
    assert(msg_recv);
    //zmsg_print(msg_recv);
    char *str_ret = zmsg_popstr(msg_recv);
    assert(str_ret);
    if(strcmp(str_ret,DB_RET_SUCCESS) == 0)
    {
        //get data
        zframe_t *frame_data = zmsg_first(msg_recv);
        assert(frame_data);
        *value_size = zframe_size(frame_data);
        *value = calloc(*value_size,1);
        memcpy(*value,zframe_data(frame_data),*value_size);
        ret = NNGN_ERR_NONE;
    }
    else
    {
        zsys_error("get_data error");
        ret = NNGN_ERR_PARAM;
    }
    zstr_free(&str_ret);

    zmsg_destroy(&msg_recv);
    pthread_mutex_unlock(&mutext);
    return ret;
}

