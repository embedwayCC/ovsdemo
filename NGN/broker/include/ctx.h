#ifndef _CTX_H_
#define _CTX_H_
#include "global.h"
#include "nngn.h"
#include "db.h"

extern zhash_t *mutil_msg_list;
extern zhash_t *workerid_obj;


typedef struct _worker_t
{
       int      slot;
       char     worker_id[NNGX_ID_MAX_LENGTH];
       wk_st_t  worker_state;
       int      msgcode_opcode[MAX_OPCODE_SIZE];
       uint64_t heart_time;
}worker_t;

typedef struct _mutil_msg_t
{
       char      client_id[NNGX_ID_MAX_LENGTH];
       zlist_t   *worker_id;
       zmsg_t    *msg;
       uint64_t	 start_time;								//消息开始时间
       uint32_t   timeout;								//客户端定义超时时间，单位为秒
}mutil_msg_t;

enum
{
    REV_HEART = 0,
    TIMEOUT_HEART,
};

extern zsock_t *publisher;

#define HEART_MSG_TIMEOU      3000
#define DEFALUT_WORKE_FILE "/usr/local/workinfo.conf"
#define WORKERKEY            "worker-list"

int ctx_init();
void ctx_exit();

//for worker
int register_worker(char *worker_id,int slot_id);
void delete_worker(void *worker);
void send_msg_worker(zmsg_t *msg,char *workerid,void *socket);


//for state
void heart_msg_handle(char *worker_id,int optype);
void pub_worker_state_msg(char *workerid,int slot,int current_state,int old_state);
int pub_state_msg(struct _wk_st *wks);


//for mutil msg
int register_mutil_msg(zframe_t *client_id,zlist_t *worker_id,int timeout);
mutil_msg_t *get_mutil_msg(char *client_id);
void destroy_mutil_msg(char *client_id);
int check_mutilmsg_success(char *client_id);
void delete_mutilmsg_workerid(char *client_id,char *worker_id);
void delete_msg(void *worker);
void send_res_msg(zframe_t *client_id,void *socket);


//for opcode
int register_opcode(int opcode,char *worker_id);
int unregister_opcode(int opcode,char *worker_id);
int get_msginfo(worker_t *pWorker,int opcode,uint8_t *sendflag,uint8_t *recvflag);

//for saveinfo
void save_workerinfo(char *worker_id);
void reload_workerinfo();
void save_msginfo();
int reload_msginfo();
void workerinfo_2_buf(worker_t *pWorker,char **buf,size_t *size);
void buf_2_workerinfo(char *buf,int size,worker_t *pWorker);
void workerlist_2_buf(zlist_t *list);
void buf_2_workerlist(zlist_t *list);
void save_workername_list();


worker_t *get_worker_by_id(const char *worker_id);
wk_st_t   get_workerstate_by_id(const char *worker_id);

#endif

