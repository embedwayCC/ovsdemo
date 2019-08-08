#ifndef __STRUCT_H_INCLUDED__
#define __STRUCT_H_INCLUDED__

#include <czmq.h>

#define DEFAULT_BROKER_FRONT_PORT 4007
#define DEFAULT_BROKER_BACK_PORT  4003
#define DEFAULT_BROKER_PUB_PORT   4004
#define DEFAULT_BROKER_REP_PORT   4005

#define DEFAULT_OM_PIPE_PORT		4006


#define NNGX_ID_MAX_LENGTH 64
#define NNGX_BACK_ID_MAX_LENGTH 128
#define NNGX_WKID_MAX_LENGTH 128

#define MSG_NO_ANSWER_ERROR_CODE 200
#define NNGN_REQUEST__INIT	{ 0, -1, 0,0,NULL}
#define NNGN_PIPEMSG__INIT	{ {0},{0}, -1, 0,NULL}
#define NNGN_RESULT__INIT	{ 0, 0, 0,NULL,0}
#define NNGN_REGISTER__INIT	{ -1, 0}
#define NNGN_TOPO_NODE__INIT	{{0},{0},0}

#define NNGN_MAX_PIPE_MSG_NUM 5
#define NNGN_TYPE_STRING_LEN     8

#define NNGN_PIPEMSG_RET             1000
#define NNGN_BROAD_CAST_ENABLE   1
#define NNGN_BROAD_CAST_DISABLE   0


enum nngn_ret_{
    NNGN_ERR_NONE           =  0,
    NNGN_ERR_SERVER_TYPE    = -1,
    NNGN_ERR_DISTRI         = -2,
    NNGN_ERR_INTERNAL       = -3,
    NNGN_ERR_BAD_REQUEST    = -4,
    NNGN_ERR_PARAM          = -5,
    NNGN_ERR_CONNECT        = -6,
    NNGN_ERR_AUTH           = -7,
    NNGN_ERR_MSG_SUCK       = -8,
    NNGN_ERR_MEMORY         = -9,
    NNGN_ERR_TIMEOUT        = -10,
    NNGN_ERR_RESOURCE       = -11,
} ;

typedef enum nngn_wk_st_t{
    WK_ERROR = 1,
    WK_ONLINE,
    WK_OFFLINE,
}wk_st_t;

/* nngx_func_ */
enum {
    NNGX_FUNC_WKSCHANGE_HANDLER = 1,
};

typedef struct _pipe_node_t{
    char    worker_id[NNGX_ID_MAX_LENGTH];					
    u_int32_t port;
}pipe_node;

typedef struct _req_t{
    u_int32_t size;
	u_int32_t slot;												/* 槽位掩码 */
    u_int32_t op_code;
    u_int32_t timeout;
    u_int8_t *content;											/* request content */
}req_t;

typedef struct _res_t {
    u_int16_t rv;	    									    /* result status */
	u_int32_t slot;												/* 槽位掩码 */
    u_int32_t size;                                             /* size of the result content */
    u_int8_t *content;                                          /* result content */
	u_int8_t seria_op;											/* 序列化类型（无序列化，msgpack,protobuf） */
}res_t;

typedef struct _pipe_msg_t{
	char worker[NNGX_WKID_MAX_LENGTH];
	char next_worker[NNGX_WKID_MAX_LENGTH];
	req_t req;
}pipe_msg;

typedef struct _wk_st {
	u_int32_t slot;												/* 槽位 */
    char      uname[NNGX_WKID_MAX_LENGTH];                      /*worker id*/
    wk_st_t   current_status;
    wk_st_t   old_status;
}wk_st;

enum {
     opcode_reg_more = 0,
     opcode_reg_end = 1,
};

enum {
     worker_top_more = 0,
     worker_top_end = 1,
};

//回调函数的类型
enum {
     CALLBACK_STATGE,
     CALLBACK_MSG_TIMEOUT,
};

//目前提供worker状态的回调
typedef int (*wkst_change_handler)(wk_st *worker_status,zlist_t *List);
typedef struct _nngx_callback_spec_s {
    uint32_t type;
    void *func;
}ncb_spec_t;

//注册opcode回调函数
typedef int (*op_callback_t)(req_t*, res_t*);
typedef int (*op_pipe_callback_t)(req_t*, res_t*,res_t *,char *);


//注册客户端提供的配置信息
/*typedef struct _cinit_data_t {
    char            broker_addr[NNGX_ID_MAX_LENGTH];
}cinit_data_t;*/

//注册服务端提供的配置信息
typedef struct _sinit_data_t {
    u_int32_t       slot;												                        /* 槽位掩码 */
    char            worker_id[NNGX_ID_MAX_LENGTH];												/* worker id */
}sinit_data_t;

//消息超时的回调
typedef int (*msg_timeout_handler)();

#endif


