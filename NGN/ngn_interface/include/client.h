#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "nngn_struct.h"
#include "global.h"
/*
* nngn inner functions struct
*/
static void gen_random_string(char *str);
static void gen_asyn_random_string(char *str);
static int add_brokerid_frame(zmsg_t *msg);


#endif
