#ifndef _NNGX_H_
#define _NNGX_H_

#include "nngn_struct.h"
/*
* nngn common functions
*/
extern uint32_t ngn_reg_cb(ncb_spec_t *cbspec);
extern uint32_t ngn_get_wks(wk_st **worker_status,uint32_t *nNum);
extern uint32_t database_init(const char *addr);
extern uint32_t put_data(char *key,int key_size,char *value,int value_size);
extern uint32_t get_data(char *key,int key_size,char **value,int *value_size);
extern void database_exit();


/*
* server related functions
*/
extern uint32_t nsrv_new(sinit_data_t *data);
extern uint32_t nsrv_register_opcode(u_int8_t force_send,u_int8_t recv_flag,uint16_t msg_type, void *handle_data,u_int8_t reg_flag);
extern uint32_t nsrv_unregister_opcode(uint32_t op_code);
extern uint32_t nsrv_destroy();

/*
* client related functions
*/
extern uint32_t nclt_new();
extern uint32_t nclt_sr(req_t *req, res_t *res);
extern uint32_t nclt_sr_plus(req_t *req, res_t **res,uint32_t *nNum, uint32_t timeout);
extern uint32_t nclt_destroy();
extern uint32_t nclt_req_fill(req_t *req, u_int32_t size, uint32_t op_code, uint32_t slot, void *content);

/*
* pipe related functions
*/
//for om worker
extern uint32_t nsrv_register_pipeline_opcode(u_int8_t force_send,u_int8_t recv_flag,uint16_t msg_type, void *handle_data,u_int8_t reg_flag);
extern uint32_t create_om_pipe_line();
extern uint32_t close_om_pipe_line();
extern uint32_t add_pipeline_topo(pipe_node *pNode,int add_flag);
extern uint32_t create_pipe_msg(pipe_msg *msg,int nNum,int broadcast,char *pipe_id);

//for pipe worker
extern uint32_t create_worker_pipe_line(int slot,char *worker);
extern uint32_t close_worker_pipe_line();



#endif
