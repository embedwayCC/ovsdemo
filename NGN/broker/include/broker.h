#ifndef _BROKER_H_
#define _BROKER_H_

#include "nngn_struct.h"
#include "global.h"
/*
* nngn inner functions struct
*/
#define MULTI_MSG_CHECK      1000

static int s_back_event (zloop_t *loop, zsock_t *backend, void *frontend);
static int s_front_event (zloop_t *loop, zsock_t *frontend, void *backend);
static int s_db_event (zloop_t *loop, zsock_t *frontend, void *output);
static int s_msgtimer_event (zloop_t *loop, int timer_id, void *output);
static int log_init();
static int broker_init();


#endif
