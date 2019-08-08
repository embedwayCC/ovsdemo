#ifndef _WORKER_H_
#define _WORKER_H_

#include "nngn_struct.h"
#include "global.h"
#include "common.h"
#include "pipeline.h"
/*
* nngn inner functions struct
*/

static int s_worker_event (zloop_t *loop, zsock_t *worker, void *argv);
static int s_pipe_event (zloop_t *loop, zsock_t *pipe, void *argv);
static int s_timer_event (zloop_t *loop, int timer_id, void *worker);
static int send_init_msg(void *worker,int slot);
static int send_opcode_end_msg(void *worker);
static uint32_t nsrv_generate_opcode(u_int8_t force_send,u_int8_t recv_flag,uint16_t msg_type);
static int send_heart();
static int add_brokerid_frame(zmsg_t *msg);

static void process_actor (zsock_t *pipe, void *args);
static void worker_actor (zsock_t *pipe, void *args);
static int s_process_event (zloop_t *loop, zsock_t *pipe, void *argv);

#endif
