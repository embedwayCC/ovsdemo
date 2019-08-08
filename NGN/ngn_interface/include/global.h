#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "nngn_struct.h"
#include "mpack.h"


/*
* nngn inner functions struct
*/
extern bool g_systeminit;

#define GET_OPCODE_SEND_BIT(op_code)     (op_code&0x00020000)>>17
#define GET_OPCODE_RECV_BIT(op_code)     (op_code&0x00010000)>>16
#define GET_OPCODE_MSG_INFO(op_code)     (op_code&0x0000ffff)

#define BROKER_FRONT_ID                  "broker-front"
#define BROKER_BACK_ID                   "broker-back"
#define BROKER_PUBLISH_ID                "broker-publish"
#define BROKER_PUBLISH_STR               "state-pub"
#define BROKER_DBPUT_STR                 "put"
#define BROKER_DBGET_STR                 "get"
#define DB_RET_SUCCESS                   "success"
#define DB_RET_FAIL                      "fail"

#define OM_PIPE_ID                         "om-pipe"


#define TIMER_SET                         1000
#define USLEEP_TIME                       2*1000*100
#define MSG_TIMEOUT_SET                   5*60*1000

#define NNGN_REGISTER__INIT	{ -1, 0}

#define WORKER_ROUTER_MSG_SIZE   2
#define WORKER_REGISTER_MSG_SIZE 1
#define MAX_OPCODE_SIZE 32768
#define MAX_PIPEMSG_NUM 10

#define NGN_TIMEOUT_NONE -1

typedef enum nngn_register_e{
    worker_init_type = 0,
    broker_heart_type,
    register_opcode_type,
    unregister_opcode_type,
    register_opcode_end,
    normal_msg,
    pipeline_msg,
    worker_topo_msg,
} op_t;

//for heart and opcode
typedef struct _worker_register_t{
    u_int8_t  op;
    u_int32_t op_code;
}worker_register_t;

int req_2_zframe(const req_t *req,zframe_t *frame);
int zframe_2_req(zframe_t *frame,req_t *req);
int res_2_zframe(res_t *res,zframe_t *frame);
int zframe_2_res(zframe_t *frame,res_t *res);
int zframe_2_register(zframe_t *frame,worker_register_t *reg);
int register_2_zframe(worker_register_t *reg,zframe_t *frame);
void wkst_2_buf(wk_st *wkst,char **buf,int *size);
void buf_2_wkst(char *buf,int size,wk_st *wkst);
void list_2_buf(zlist_t *pList,char **buf,int *size);
void buf_2_list(char *buf,int size,zlist_t *pList);
int pipenode_2_zframe(const pipe_node *pipeNode,zframe_t *frame);
int zframe_2_pipenode(zframe_t *frame,pipe_node *pipeNode);
int pipemsg_2_zframe(const pipe_msg*pipeMsg,zframe_t *frame);
int zframe_2_pipemsg(zframe_t *frame,pipe_msg *pipeMsg);
uint32_t ngn_debug_enable(void);


#endif

