#ifndef __APP_H__
#define __APP_H__

#include "appdefs.h"
#include "version.h"

int OeAp_Get_My_Slot_Id(void);
int OeAp_Get_Platform(void);
board_type_t OeAp_Get_My_Board_Type(void);
int OeAp_Get_Bcm_Unit_Port(bcm_port_map_t *bpm, int module, int port, int *bcm_unit, int *bcm_port);
bcm_port_map_t *OeAp_Get_Bcm_Portmap_By_Module(int module);
bcm_port_map_t *OeAp_Get_Bcm_Portmap_By_Sm(int slot, int module);
int port_init();
int setup_fault_trap(void);
#endif
