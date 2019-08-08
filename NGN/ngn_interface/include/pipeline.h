#ifndef _PIPE_LINE_H_
#define _PIPE_LINE_H_

#include "nngn_struct.h"
#include "global.h"
#include "common.h"
/*
* nngn inner functions struct
*/

uint32_t process_state_change_msg(wk_st *pState);
uint32_t process_pipemsg(zmsg_t *msg,worker_register_t *register_t,zsock_t *client );
extern zsock_t *get_om_sock();
void set_client_frame(zframe_t *client_frame,zframe_t *org_req);
void check_nexthop(char *nexthop);
int checkout_nexthop_state(char *nexthop);
static uint32_t send_pipeline_topo(char *addr);
static void delete_msg(void *msg);
static void pipe_actor (zsock_t *pipe, void *args);
static int s_pipe_client_event (zloop_t *loop, zsock_t *client, void *argv);
static int s_pipe_server_event (zloop_t *loop, zsock_t *server, void *argv);
static int s_actor_event (zloop_t *loop, zsock_t *pipe, void *worker);
static int add_om_frame(zmsg_t *msg);
static void send_pipe_msg(zmsg_t *msg_send,char *pipe_worker,int nNum,char *pipe_id);
static void gen_random_string(char *str);
extern uint32_t porcess_state_change_msg(wk_st *pState, zlist_t *pList);


#endif
