#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "appdefs.h"
#include "module.h"
#include "app.h"
#include "imatrix/im-types.h"
#include "imatrix/im-port.h"
#include "imatrix/im-statistic.h"
#include "imatrix/im-l2.h"
#include "imatrix/im-vlan.h"
#include "imatrix/im-rpc.h"
#include "ow_api.h"

#define DEFAULT_SPEED 10000

int my_slot_id = 0;
int my_board_type = 0;
oe_port_status_t g_port_status[SLOT_NUMBER_PER_SYSTEM][MODULE_NUMBER_PER_BOARD][MAX_PORT_NUMBER];

typedef struct
{
	char reg_name[40];
	char cntr_name[40];
	int show_on;
}cntr_map_t;

cntr_map_t ge_cntr_map[]=
{

        {"RIPD4" , "Rx IPv4 L3 Discards",               0},
        {"RIPC4" , "Rx IPv4 L3 Good",                   0},
        {"RIPHE4", "Rx IPv4 Header Error",              0},
        {"IMRP4" , "Rx IPv4 Multicast",          0},
        {"RIPD6" , "Rx IPv6 L3 Discards",               0},
        {"RIPC6" , "Rx IPv6 L3 Good",                   0},
        {"RIPHE6", "Rx IPv6 Header Error",              0},
        {"IMRP6" , "Rx IPv6 Multicast",          0},
        {"RDISC" , "Rx Discard(IBP&CBP)",          0},
        {"RUC"   , "Rx Good Unicast",          0},
        {"RPORTD", "Rx Dropped For Link",     0},
        {"RDBGC1", "Rx Debug 1",                        0},
        {"RDBGC2", "Rx Debug 2",                        0},
        {"RDBGC3", "Rx Debug 3",                        0},
        {"RDBGC4", "Rx Debug 4",                        0},
        {"RDBGC5", "Rx Debug 5",                        0},
        {"RDBGC6", "Rx Debug 6",                        0},
        {"RDBGC7", "Rx Debug 7",                        0},
        {"RDBGC8", "Rx Debug 8",                        0},
        {"HOLD"  , "Dropped Pkts",                      3},

        {"TDBGC0", "Tx Debug 0",                        0},
        {"TDBGC1", "Tx Debug 1",                        0},
        {"TDBGC2", "Tx Debug 2",                        0},
        {"TDBGC3", "Tx Debug 3",                        0},
        {"TDBGC4", "Tx Debug 4",                        0},
        {"TDBGC5", "Tx Debug 5",                        0},
        {"TDBGC6", "Tx Debug 6",                        0},
        {"TDBGC7", "Tx Debug 7",                        0},
        {"TDBGC8", "Tx Debug 8",                        0},
        {"TDBGC9", "Tx Debug 9",                        0},
        {"TDBGC10","Tx Debug 10",                       0},
        {"TDBGC11","Tx Debug 11",                       0},
        {"TPCE"  , "Tx Purge and Cell Error",           0},

        {"GR64"  , "Rx 64bytes",            4},
        {"GR127" , "Rx 65-127bytes",        4},
        {"GR255" , "Rx 128-255bytes",       4},
        {"GR511" , "Rx 256-511bytes",       4},
        {"GR1023", "Rx 512-1023bytes",      4},
        {"GR1518", "Rx 1024-1518bytes",     4},
        {"GRMGV" , "Rx Good Vlan Frame", 0},
        {"GR2047", "Rx 1519-2047bytes",     4},
        {"GR4095", "Rx 2048-4095bytes",     4},
        {"GR9216", "Rx 4096-9216bytes",     4},
        {"GRPKT" , "Rx Pkts",                           1},
        {"GRBYT" , "Rx Bytes",                          2},
        {"GRMCA" , "Rx Multicast",                      4},
        {"GRBCA" , "Rx Broadcast",                      4},
        {"GRFCS" , "Rx FCS Error",                      3},
        {"GRXCF" , "Rx Control Frame",                  0},
        {"GRXPF" , "Rx Pause Frame",                    0},
        {"GRXUO" , "Rx Unsupported Opcode",             0},
        {"GRALN" , "Rx Alignment Error",                0},
        {"GRFLR" , "Rx Length Out of Range",            0},
        {"GRCDE" , "Rx Code Error",                     0},
        {"GRFCR" , "Rx False Carrier",                  0},
        {"GROVR" , "Rx Oversized Packet",                1},
        {"GRJBR" , "Rx Jabber Frame",                   0},
        {"GRMTUE", "Rx MTU Check Error",                0},
        {"RRPKT" , "Rx RUNT Frame",                     0},
        {"GRUND" , "Rx Undersize Packet",                1},
        {"GRFRG" , "Rx Fragment",                       0},
        {"RRBYT" , "Rx Runt Byte",                      0},

        {"GT64"  , "Tx 64bytes"       ,     4},
        {"GT127" , "Tx 65-127bytes"   ,     4},
        {"GT255" , "Tx 128-255bytes"  ,     4},
        {"GT511" , "Tx 256-511bytes"  ,     4},
        {"GT1023", "Tx 512-1023bytes" ,     4},
        {"GT1518", "Tx 1024-1518bytes",     4},
        {"GTMGV" , "Tx Good Vlan Frame", 0},
        {"GT2047", "Tx 1519-2047bytes",     4},
        {"GT4095", "Tx 2048-4095bytes",     4},
        {"GT9216", "Tx 4096-9216bytes",     4},
        {"GTPKT" , "Tx Pkts",                           1},
        {"GTMCA" , "Tx Multicast"    ,                  4},
        {"GTBCA" , "Tx Broadcast"    ,                  4},
        {"GTXPF" , "Tx Pause Frame"    ,                0},
        {"GTJBR" , "Tx Jabber Frame",                   0},
        {"GTXCF" , "Tx Control Frame",                  0},
        {"GTOVR" , "Tx Oversized Frame",                4},
        {"GTDFR" , "Tx Single Deferral",                0},
        {"GTEDF" , "Tx Multiple Deferral",              0},
        {"GTSCL" , "Tx Single Collision",               0},
        {"GTMCL" , "Tx Multiple Collision",             0},
        {"GTLCL" , "Tx Late Collision",                 0},
        {"GTXCL" , "Tx Excessive Collision",            0},
        {"GTFRG" , "Tx Fragment",                       0},
        {"GTNCL" , "Tx Total Collision",                0},
        {"GTBYT" , "Tx Bytes",                          2},
        {"GTFCS" , "Tx FCS Error",                      3},

	// For AC1200 xe
        {"ITPKT"                ,"XE Tx Pkts",                             1},
        {"ITXPF"                ,"XE Tx Pause Frame",                    4},
        {"ITMCA"                ,"XE Tx Multicast",                      4},
        {"ITBCA"                ,"XE Tx Broadcast",                      4},
        {"ITFRG"                ,"XE Tx Fragment Packet",                0},
        {"ITOVR"                ,"XE Tx Oversized Frame",                4},
        {"IT64"                 ,"XE Tx 64bytes"       ,       4},
        {"IT127"                ,"XE Tx 65-127bytes"   ,       4},
        {"IT255"                ,"XE Tx 128-255bytes"  ,       4},
        {"IT511"                ,"XE Tx 256-511bytes"  ,       4},
        {"IT1023"               ,"XE Tx 512-1023bytes" ,       4},
        {"IT1518"               ,"XE Tx 1024-1518bytes",       4},
        {"IT2047"               ,"XE Tx 1519-2047bytes",     4},
        {"IT4095"               ,"XE Tx 2048-4095bytes",       4},
        {"IT9216"               ,"XE Tx 4096-9216bytes",       4},
        {"IT16383"              ,"XE Tx 9217-16383bytes",      4},
        {"ITMAX"                ,"XE Tx Max Size Packet",                0},
        {"ITUFL"                ,"XE Tx Underflow Packet",               0},
        {"ITERR"                ,"XE Tx Error Packet",                   0},
        {"ITBYT"                ,"XE Tx Bytes",                          2},
        {"ITFCS"                ,"XE Tx FCS Error",                      3},
        
        {"IR64"                 ,"XE Rx 64bytes",              4},
        {"IR127"                ,"XE Rx 65-127bytes",  4},
        {"IR255"                ,"XE Rx 128-255bytes", 4},
        {"IR511"                ,"XE Rx 256-511bytes", 4},
        {"IR1023"               ,"XE Rx 512-1023bytes",        4},
        {"IR1518"               ,"XE Rx 1024-1518bytes",       4},
        {"IR2047"               ,"XE Rx 1519-2047bytes",       4},
        {"IR4095"               ,"XE Rx 2048-4095bytes",       4},
        {"IR9216"               ,"XE Rx 4096-9216bytes",       4},
        {"IR16383"              ,"XE Rx 9217-16383bytes",    4},
        {"IRMAX"                ,"XE Rx Max Size Packet",                  0},
        {"IRPKT"                ,"XE Rx Pkts",                           1},
        {"IRMCA"                ,"XE Rx Multicast",                      4},
        {"IRBCA"                ,"XE Rx Broadcast",                      4},
        {"IRXCF"                ,"XE Rx Control Frame",                  0},
        {"IRXPF"                ,"XE Rx Pause Frame",                    4},
        {"IRXUO"                ,"XE Rx Unsupported Opcode",             0},
        {"IRJBR"                ,"XE Rx Jabber Frame",                   0},
        {"IROVR"                ,"XE Rx Oversized Packet",                1},
        {"IRFLR"                ,"XE Rx Length OOR Frame",      0},
        {"IRMEG"                ,"XE Rx MTU Error Good",                 0},
        {"IRMEB"                ,"XE Rx MTU Error Bad",                  0},
        {"IRBYT"                ,"XE Rx Bytes",                          2},
        {"IRUND"                ,"XE Rx Undersize Packet",               1},
        {"IRFRG"                ,"XE Rx Fragment Packet",                0},
        {"IRERBYT"              ,"XE Rx Error Byte",                     0},
        {"IRERPKT"              ,"XE Rx Error Pkt",                      0},
        {"IRJUNK"               ,"XE Rx Junk Pkt",                       0},
        {"IRFCS"                ,"XE Rx FCS Error",                      3},


	// For xe
	{"TPKT",   "Tx Pkts",					      1},
	{"TXCF",   "Tx Control Frame",				  4},
	{"TXPF",   "Tx Pause Frame",			      4},
	{"TJBR",   "Tx Jabber Frame",				  4},
	{"TMCA",   "Tx Multicast",				      4},
	{"TBCA",   "Tx Broadcast",				      4},
	{"TFRG",   "Tx Fragment Packet",		      0},
	{"TOVR",   "Tx Oversized Frame",		      0},
	{"T64",    "Tx 64bytes"	   ,			      4},
	{"T127",   "Tx 65-127bytes"   ,			      4},
	{"T255",   "Tx 128-255bytes"  ,			      4},
	{"T511",   "Tx 256-511bytes"  ,			      4},
	{"T1023",  "Tx 512-1023bytes" ,			      4},
	{"T1518",  "Tx 1024-1518bytes",			      4},
	{"T2047",  "Tx 1519-2047bytes",			      4},
	{"T4095",  "Tx 2048-4095bytes",			      4},
	{"T9216",  "Tx 4096-9216bytes",			      4},
	{"T16383", "Tx 9217-16383bytes",		      4},
	{"TMAX"	,  "Tx Max Size Packet",		      0},
	{"TUFL"	,  "Tx Underflow Packet",		      0},
	{"TERR"	,  "Tx Error Packet",			      0},
	{"TBYT"	,  "Tx Bytes", 					      2},
	{"TFCS"	,  "Tx FCS Error", 				      3},

	{"RDBGC0", "Rx Drop",					      3},
	{"DROP_PKT_IN(0)","Rx Congestion Drop",	  3},
	{"R64",    "Rx 64bytes",				      4},
	{"R127",   "Rx 65-127bytes",			      4},
	{"R255",   "Rx 128-255bytes",			      4},
	{"R511",   "Rx 256-511bytes",			      4},
	{"R1023",  "Rx 512-1023bytes",			      4},
	{"R1518",  "Rx 1024-1518bytes",			      4},
	{"R2047",  "Rx 1519-2047bytes",			      4},
	{"R4095",  "Rx 2048-4095bytes",			      4},
	{"R9216",  "Rx 4096-9216bytes",			      4},
	{"R16383", "Rx 9217-16383bytes",		      4},
	{"RMAX",   "Rx Max Size Packet", 		      0},
	{"RPKT",   "Rx Pkts",					      1},
	{"RMCA",   "Rx Multicast",				      4},
	{"RBCA",   "Rx Broadcast",				      4},
	{"RXCF",   "Rx Control Frame",			      4},
	{"RXPF",   "Rx Pause Frame", 			      4},
	{"RXUO",   "Rx Unsupported Opcode",		      0},
	{"RJBR",   "Rx Jabber Frame",			      4},
	{"ROVR",   "Rx Oversized Packet",		      0},
	{"RFLR",   "Rx Length OOR Frame",		      0},
	{"RMEG",   "Rx MTU Error Good",			      0},
	{"RMEB",   "Rx MTU Error Bad",			      4},
	{"RBYT",   "Rx Bytes",					      2},
	{"RUND",   "Rx Undersize Packet",		      0},
	{"RFRG",   "Rx Fragment Packet", 		      0},
	{"RERBYT", "Rx Error Byte",				      0},
	{"RERPKT", "Rx Error Pkt",				      0},
	{"RJUNK",  "Rx Junk Pkt",				      0},
	{"RFCS",   "Rx FCS Error",				      3},
};




int module_set_port_autoneg(struct smp_t *smp, int onoff)
{
    int  module, port;
    bcm_port_map_t *bpm;
    int unit = 0, bcm_port = 0;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if ((smp->modules  & (1 << module)) == 0)
            continue;
     
        bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
        if(bpm == NULL)
            continue;
     
        for (port = 0; port < MAX_PORT_NUMBER; port++)  {
            if (port >= bpm->port_number)
                continue;
            if (smp->ports & (1ull << port)) {
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                    if(im_port_autoneg_set(unit, bcm_port, onoff) != 0) {
                        return OE_RST_SET_CAM_ERR;
                    }
                    if (BOARD_TYPE_AC1200 == my_board_type 
                        && module == MODULE_BACK) {
                        if(im_port_autoneg_set(unit, S1_S2_TO_AC1200, onoff) != 0) {
                            return OE_RST_SET_CAM_ERR;
                        }
                    }
                }
            }
        }
    }

    return OE_RST_OK;
}



static int _check_smp_valid(int board_type, int m, int p)
{
    bcm_port_map_t *bpm = NULL;
    bpm = OeAp_Get_Bcm_Portmap_By_Module(m);
    if (bpm != NULL) {
        if (bpm->port_map[p] < 0) {
            return OE_RST_INVLD_PRM;
        }
    }

    return OE_RST_OK;
}


int OeMdl_Set_Interface_Autoneg(int modules, unsigned long long ports, int onoff)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					break;

				if (ports & (1ull << k))
					g_port_status[my_slot_id][j][k].autoneg = onoff;
			}
		}
	}

	return OE_RST_OK;
}

int module_set_port_pause(struct smp_t *smp, int onoff)
{
    int module, port;
    bcm_port_map_t *bpm;
    int unit = 0, bcm_port;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if ((smp->modules & (1 << module)) == 0)
            continue;
        bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
        if(bpm == NULL)
            continue;

        for (port = 0; port < MAX_PORT_NUMBER; port++) {
            if (port >= bpm->port_number)
                continue;
            if (smp->ports & (1ull << port)) {
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                    if(im_port_pause_set(unit, bcm_port, onoff, onoff) != 0)
                        return OE_RST_SET_CAM_ERR;
                    if (BOARD_TYPE_AC1200 == my_board_type 
                        && module == MODULE_BACK) {
                        if(im_port_pause_set(unit, S1_S2_TO_AC1200, onoff, onoff) != 0)
                            return OE_RST_SET_CAM_ERR;
                    }
                }
            }
        }
    }

    return OE_RST_OK;
}

int OeMdl_Set_Interface_Pause(int modules, unsigned long long ports, int onoff)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					break;

				if (ports & (1ull << k))
					g_port_status[my_slot_id][j][k].pause = onoff;
			}
		}
	}

	return OE_RST_OK;
}

int module_set_port_shutdown(struct smp_t *smp, int shutdown)
{
   int module, port;
   bcm_port_map_t *bpm;
   int unit = 0, bcm_port;

   for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
       if ((smp->modules & (1 << module)) == 0)
           continue;
       bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
       if(bpm == NULL)
           continue;

       for (port = 0; port < MAX_PORT_NUMBER; port++) {
            if (port >= bpm->port_number)
                continue;
           if (smp->ports & (1ull << port)) {
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                    if(im_port_enable_set(unit, bcm_port, !shutdown) != 0)
                        return OE_RST_SET_CAM_ERR;
                    if (BOARD_TYPE_AC1200 == my_board_type 
                        && module == MODULE_BACK) {
                        if(im_port_enable_set(unit, S1_S2_TO_AC1200, !shutdown) != 0)
                            return OE_RST_SET_CAM_ERR;
                    }
               }
           }
       }
   }

   return OE_RST_OK;   
}

int OeMdl_Set_Interface_Shutdown(int modules, unsigned long long ports, int shutdown)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					continue;

				if (ports & (1ull << k))
					g_port_status[my_slot_id][j][k].shutdown = shutdown;
			}
		}
	}

	return OE_RST_OK;
}

int module_set_port_duplex(struct smp_t *smp, int duplex)
{
    int module, port;
    bcm_port_map_t *bpm;
    int unit = 0, bcm_port;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if ((smp->modules & (1 << module)) == 0)
            continue;
        bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
        if(bpm == NULL)
            continue;

        for (port = 0; port < MAX_PORT_NUMBER; port++) {
            if (port >= bpm->port_number)
                continue;
            if (smp->ports & (1ull << port)) {
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                    int speed = 0;
                    im_port_speed_get(unit, bcm_port, &speed);
                    if ((speed == 1000 || speed == 10000) && duplex == 1) {
                        return OE_RST_NOT_SUPPORT;
                    }
                    if(im_port_duplex_set(unit, bcm_port, (duplex == 0)?IM_PORT_DUPLEX_FULL:IM_PORT_DUPLEX_HALF) != 0)
                        return OE_RST_SET_CAM_ERR;
                    if (BOARD_TYPE_AC1200 == my_board_type 
                        && module == MODULE_BACK) {
                        im_port_speed_get(unit, S1_S2_TO_AC1200, &speed);
                        if ((speed == 1000 || speed == 10000) && duplex == 1) {
                            return OE_RST_NOT_SUPPORT;
                        }
                        if(im_port_duplex_set(unit, S1_S2_TO_AC1200, (duplex == 0)?IM_PORT_DUPLEX_FULL:IM_PORT_DUPLEX_HALF) != 0)
                            return OE_RST_SET_CAM_ERR;
                    }
                }
            }
        }
    }

    return OE_RST_OK;
}

int OeMdl_Set_Interface_Duplex(int modules, unsigned long long ports, int duplex)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					break;

				if (ports & (1ull << k))
					g_port_status[my_slot_id][j][k].duplex = duplex;
			}
		}
	}

	return OE_RST_OK;
}

int module_set_port_speed(struct smp_t *smp, int speed)
{
    int module, port = 0; 
    bcm_port_map_t *bpm;
    int unit = 0, bcm_port;

    if (speed == 10000 && my_board_type == BOARD_TYPE_AC1200)
        return OE_RST_NOT_SUPPORT;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++)  {
        if ((smp->modules & (1 << module)) == 0)
            continue;
        bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
        if(bpm == NULL)
            continue;

        for (port = 0; port < MAX_PORT_NUMBER; port++) {
            if (port >= bpm->port_number)
                continue;
            if (smp->ports & (1ull << port))  {
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                    if(im_port_speed_set(unit, bcm_port, speed) != 0)
                        return OE_RST_SET_CAM_ERR;
                    if (im_port_frame_max_set(unit, bcm_port, 16356) != 0)
                        return OE_RST_SET_CAM_ERR;
                    if (BOARD_TYPE_AC1200 == my_board_type 
                        && module == MODULE_BACK) {
                        if(im_port_speed_set(unit, S1_S2_TO_AC1200, speed) != 0)
                            return OE_RST_SET_CAM_ERR;
                        if (im_port_frame_max_set(unit, S1_S2_TO_AC1200, 16356) != 0)
                            return OE_RST_SET_CAM_ERR;
                    }
                }    
            }    
        }    
    }    

    return OE_RST_OK;   
}

int OeMdl_Set_Interface_Speed(int modules, unsigned long long ports, int speed)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					break;

				if (ports & (1ull << k))
					g_port_status[my_slot_id][j][k].speed = speed;
			}
		}
	}

	return OE_RST_OK;
}

int _check_if_used_by_trunk(int module, int port)
{
	if (g_port_status[my_slot_id][module - MODULE_REAR][port].used_by_trunk != 0)
		return OE_RST_IF_USED_BY_TRUNK;
	else 
		return 0;
}

int module_set_port_mac_address_learning(struct smp_t *smp, int onoff)
{
   int module, port = 0, rv = 0;
   bcm_port_map_t *bpm;
   int unit = 0, bcm_port;
   int flags = (onoff == 0)?(IM_PORT_LEARN_FWD):(IM_PORT_LEARN_ARL|IM_PORT_LEARN_FWD);

   for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++)  {
       if ((smp->modules & (1 << module)) == 0)
           continue;
       bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
       if(bpm == NULL)
           continue;

       for (port = 0; port < MAX_PORT_NUMBER; port++) {
            if (port >= bpm->port_number)
                continue;
           if (smp->ports & (1ull << port))  {
               rv = _check_if_used_by_trunk(module, port);
               if (rv != 0) 
                   return rv;
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                   if (im_port_learn_set(unit, bcm_port, flags) != 0) {
                       return OE_RST_SET_CAM_ERR;
                   }
               }
           }
       }
   }

   return OE_RST_OK;   
}

int OeMdl_Set_Interface_Mac_Address_Learning( int modules, unsigned long long ports, int onoff)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					break;

				if (ports & (1ull << k))
					g_port_status[my_slot_id][j][k].mac_address_learning = onoff;
			}
		}
	}

	return OE_RST_OK;
}

int module_get_bcm_port_status(int module, int port, struct port_status_t *port_status_p)
{
    int rv = OE_RST_OK;
    bcm_port_map_t *bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
    char module_str[MODULE_NUMBER_PER_BOARD] = {'r', 'f', 'b'};
    int unit = 0, bcm_port = 0;
    int tmp = 0;

    port_status_p->smp_common.port_id = port;

    if (bpm == NULL || OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) != 0) {
        port_status_p->smp_common.port_valid_flag = 0; /*not valid*/
        return OE_RST_OK;
    }

    port_status_p->smp_common.port_valid_flag = 1;

    if((rv = im_port_link_status_get(unit, bcm_port, &(port_status_p->link))) != 0)
	{
        return OE_RST_GET_CAM_ERR;
	}
	

    if((rv = im_port_autoneg_get(unit, bcm_port, &(port_status_p->autoneg))) != 0)
        return OE_RST_GET_CAM_ERR;

    if((rv = im_port_pause_get(unit, bcm_port, &(port_status_p->pause), &tmp)) != 0)
        return OE_RST_GET_CAM_ERR;

    if((rv = im_port_duplex_get(unit, bcm_port, &(port_status_p->duplex))) != 0)
        return OE_RST_GET_CAM_ERR;
    if (port_status_p->duplex == IM_PORT_DUPLEX_FULL)
        port_status_p->duplex = 0;
    else
        port_status_p->duplex = 1;

    if((rv = im_port_speed_get(unit, bcm_port, &(port_status_p->speed))) != 0)
        return OE_RST_GET_CAM_ERR;

    if((rv = im_port_learn_get(unit, bcm_port, (uint_32_t *)&(port_status_p->mac_address_learning))) != 0)
        return OE_RST_GET_CAM_ERR;

    if (port_status_p->mac_address_learning & IM_PORT_LEARN_ARL)
        port_status_p->mac_address_learning = 1;
    else
        port_status_p->mac_address_learning = 0;

    if((rv = im_port_frame_max_get(unit, bcm_port, &(port_status_p->mtu))) != 0)
        return OE_RST_GET_CAM_ERR;
	
    memset(port_status_p->port_name, 0, sizeof(port_status_p->port_name));
    sprintf(port_status_p->port_name, "%d/%c/%d", my_slot_id + 1, module_str[module - MODULE_REAR], port + 1);

    return rv;
}


int module_get_interface_status(struct smp_t *smp, struct oe_status_t *if_status_attr)
{
    int rv = OE_RST_OK;
    int module, port, m_count = 0, p_count = 0;
    struct list_status_t *board_status_p = NULL;

    assert(if_status_attr != NULL);

    for (module = MODULE_REAR; module <= MODULE_BACK; module++) {
        if (smp->modules & (1 << module)) {
            board_status_p = &if_status_attr->list_status[m_count];
            board_status_p->sm_common.slot_id = my_slot_id + 1;
            board_status_p->sm_common.module_id = module;
            for (port = 0; port < MAX_PORT_NUMBER; port++) {
                if (smp->ports & (1ull << port)) {
                    rv = module_get_bcm_port_status(module, port, &board_status_p->port_status[p_count]);
                    if (rv != 0) {
                        board_status_p->n_port_status = p_count;
                        if_status_attr->n_list_status = m_count;
                        return rv;
                    }
                    if (board_status_p->port_status[p_count].smp_common.port_valid_flag == 0) {
                        continue;
                    }
                    p_count++;
                }
            }
            board_status_p->n_port_status = p_count;
            m_count++;
            p_count = 0;
        }
    }
    if_status_attr->n_list_status = m_count;

    return rv;  
}

static int _get_good_name(char * reg_name, char * cntr_name, int * show_on)
{
	int i;

	*show_on = 0;

    for(i=0;i<sizeof(ge_cntr_map)/sizeof(ge_cntr_map[0]);i++)
	{
		if(!strcmp(ge_cntr_map[i].reg_name, reg_name))
		{
			strcpy(cntr_name,ge_cntr_map[i].cntr_name);
			*show_on = ge_cntr_map[i].show_on;
			return 0;
		}
	}
	
	return -1;
}

static int _get_port_counter(im_counter_t *cntr, int verbose, struct counter_t *counter)
{
	int show_on = 1;
	char cname[80];

	memset(cname,0,sizeof(cname));
	if (_get_good_name(cntr->name,cname,&show_on) < 0) {
		return -1;
	}

	memset(counter->name, 0, strlen(cname)+1);
	counter->type = show_on;
	sprintf(counter->name,"%s",cname);
	if (show_on == 1) {
		counter->total = cntr->total;		
		counter->rate = cntr->rate;
	} else if (show_on == 2) {
		counter->total = cntr->total;		
		counter->rate = cntr->rate * 8 / 1000;
	} else if (show_on==3) {
		counter->type = 1;
		counter->total = cntr->total;		
		counter->rate = cntr->rate;
	} else if (show_on == 4 && verbose) {
		counter->type = 1;
		counter->total = cntr->total;		
		counter->rate = cntr->rate;
	}else {
		return -1;
	}
	
	return 0;
}

static void _merge_port_counter(
        struct counter_t *counter_new,
        struct counter_t *counter_old)
{
    assert(counter_new != NULL);
    assert(counter_old != NULL);
    
    if (strcmp(counter_new->name, counter_old->name) == 0) {
        if (counter_new->total != 0 && counter_old->total == 0) {
            counter_old->type = counter_new->type;
            counter_old->total = counter_new->total;
            counter_old->rate = counter_new->rate;
        }
    }
}


static int _get_ac1200_back_bcm_counter(
        int unit,
        im_counter_t *counter,
        int verbose,
        struct port_counter_t *p_counter)
{
    int i, index = 0, cntr_num = 0;
    im_counter_t *p_im_counter = NULL;
    struct counter_t *p_tmp_cntr = NULL;

    /* First, get xe2 pkt counter */
    if (im_counters_get(unit, AC1200_TO_S1_S2, &cntr_num, counter, 
        SHOW_CTR_CHANGED | SHOW_CTR_SAME | SHOW_CTR_Z | SHOW_CTR_NZ) != 0) {
        return OE_RST_GET_CAM_ERR;
    }
    p_counter->counter = (struct counter_t *)calloc(cntr_num, sizeof(struct counter_t));
    if (p_counter->counter == NULL)  {
        return OE_RST_NO_MEM;
    }
    for(i = 0; i < cntr_num; i++) {
        if (_get_port_counter(&counter[i], verbose, &p_counter->counter[index]) < 0) {
            continue;
        }
        index++;
    }    
    p_counter->n_counter = index;  

    /* Second, get xe3 pkt counter */
    p_im_counter = (im_counter_t *)calloc(30*101, sizeof(im_counter_t));
    if (p_im_counter == NULL) {
        return OE_RST_NO_MEM;
    }
    if (im_counters_get(unit, S1_S2_TO_AC1200, &cntr_num, p_im_counter, 
        SHOW_CTR_CHANGED | SHOW_CTR_SAME | SHOW_CTR_Z | SHOW_CTR_NZ) != 0) {
        free(p_im_counter);
        return OE_RST_GET_CAM_ERR;
    }
    p_tmp_cntr = (struct counter_t *)calloc(cntr_num, sizeof(struct counter_t));
    if (p_tmp_cntr == NULL)  {
        free(p_im_counter);
        return OE_RST_NO_MEM;
    }
    for(i = 0, index = 0; i < cntr_num; i++) {
        if (_get_port_counter(&p_im_counter[i], verbose, &p_tmp_cntr[index]) < 0) {
            continue;
        }
        _merge_port_counter(&p_tmp_cntr[index], &p_counter->counter[index]); 
        index++;
    }

    free(p_im_counter);
    free(p_tmp_cntr);
    return OE_RST_OK;
}


static int _get_bcm_port_counter(int slot, int module, int port, int verbose, struct port_counter_t *port_counter_p)
{
    int i, cntr_num;
    int index = 0; 
    im_counter_t cntr[30 * 101];
    bcm_port_map_t *bpm = OeAp_Get_Bcm_Portmap_By_Sm(slot, module);
    int unit = 0, bcm_port = 0;

    port_counter_p->smp_common.port_id = port;

    if (bpm == NULL || OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) != 0) {
        port_counter_p->smp_common.port_valid_flag = 0; /*not valid*/
        return OE_RST_OK;
    }    

    port_counter_p->smp_common.port_valid_flag = 1; 
    memset(cntr, 0, sizeof(cntr));
    if (BOARD_TYPE_AC1200 == my_board_type 
        && module == MODULE_BACK) {
        return _get_ac1200_back_bcm_counter(unit, cntr, verbose, port_counter_p);
    }
    
    if(im_counters_get(unit, bcm_port, &cntr_num, cntr, SHOW_CTR_CHANGED | SHOW_CTR_SAME | SHOW_CTR_Z | SHOW_CTR_NZ) != 0) {
        return OE_RST_GET_CAM_ERR;
    }    

    port_counter_p->counter = (struct counter_t *)calloc(cntr_num, sizeof(struct counter_t));
    if (port_counter_p->counter == NULL)  {
        return OE_RST_NO_MEM;
    }    
    for(i = 0; i < cntr_num; i++) {
        if (_get_port_counter(&(cntr[i]), verbose, &port_counter_p->counter[index]) < 0) {
            continue;
        }    
        index++;
    }    
    port_counter_p->n_counter = index;     
   
    return OE_RST_OK;
}


int module_get_interface_counter(struct smp_t *smp, int verbose, struct oe_counter_t *if_counter_attr)
{
    int rv = OE_RST_OK;
    int module, port, m_count = 0, p_count = 0;
    struct sm_common_t m_common;
    struct board_counter_t *board_counter_p = NULL;

    assert(if_counter_attr != NULL);

    for (module = MODULE_REAR; module <= MODULE_BACK; module++) {
        if (smp->modules & (1 << module)) {
            memset(&m_common, 0, sizeof(m_common));
            m_common.slot_id = my_slot_id;
            m_common.module_id = module;
            board_counter_p = &if_counter_attr->board_counter[m_count];
            memcpy(&board_counter_p->sm_common, &m_common, sizeof(struct sm_common_t));
            for (port = 0; port < MAX_PORT_NUMBER; port++) {
                if (smp->ports & (1ull << port)) {
                    rv = _get_bcm_port_counter(my_slot_id, module, port, verbose, &board_counter_p->port_counter[p_count]);
                    if (rv != 0) {
                        board_counter_p->n_port_counter = p_count;
                        if_counter_attr->n_board_counter = m_count;
                        return rv;
                    }
                    if (board_counter_p->port_counter[p_count].smp_common.port_valid_flag == 0) {
                        continue;
                    }
                    p_count++;
                }
            }
            board_counter_p->n_port_counter = p_count;
            m_count++;
            p_count = 0;
        }
    }
    if_counter_attr->n_board_counter = m_count;

    return rv;
}

static int _clear_bcm_port_counter(int module, int port)
{
    bcm_port_map_t *bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
    int unit = 0, bcm_port = 0;

    if(bpm == NULL || OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) != 0)
        return OE_RST_OK;
    if(im_counters_clear(unit, bcm_port) != 0)
        return OE_RST_SET_CAM_ERR;
    if (BOARD_TYPE_AC1200 == my_board_type 
        && module == MODULE_BACK) {
        if(im_counters_clear(unit, S1_S2_TO_AC1200) != 0)
            return OE_RST_SET_CAM_ERR;
    }

    return OE_RST_OK;
}

int module_clear_interface_counter(int module, unsigned long long ports)
{
    int i;
	int rv = 0;

    for(i = 0; i < MAX_PORT_NUMBER; i++) {
        if(ports & 1ull << i) {
            rv = _clear_bcm_port_counter(module, i);
        }
    }

    return rv;
}

static int _check_b_class_port_40g(int port) /*smp port NOT bcm_port*/
{
   FILE* fp=NULL;
   int slot=0;
   int flag=1;
   char buf_1[256];
   char buf_2[256];

   port = port +1 ;
   if((0!=my_slot_id) && (1!= my_slot_id)) /*node card :40G only*/
   {
       /*for node , b/2-4 b/6-8 should be 10G ports*/
       if((port!=1) && (port!=5))
           return 0;

       if(port==1)
           sprintf(buf_1, "slot_1=");
       else
           sprintf(buf_1, "slot_2=");

       fp = fopen("/appfs/port_switch/userdef", "r");
       if(fp==NULL)
       {
           return 1;/*file not exist , indicate 40g by default*/
       }

       flag=1;
       while (fgets(buf_2,sizeof(buf_2),fp) != NULL) {
           if(0==strncmp(buf_1, buf_2, strlen(buf_1)))
           {
               flag = atoi(strchr((void *)buf_2, '=') + 1);
               break;
           }
       }    
       fclose(fp);

       return flag;
   }

   if(0!=(port-5)%4) /*apparently, b/6,7,8 ports like these are 10G ports*/
   {
       return 0;
   }

   slot = (port-5)/4+3;
   sprintf(buf_1, "slot_%d=", slot);

   fp = fopen("/appfs/port_switch/userdef", "r");
   if(fp==NULL)
   {
        return 1;/*file not exist , indicate 40g by default*/
   }
    
   while (fgets(buf_2,sizeof(buf_2),fp) != NULL) {
       if(0==strncmp(buf_1, buf_2, strlen(buf_1)))
       {
            flag = atoi(strchr((void *)buf_2, '=') + 1);
            break;
       }
   }    
   fclose(fp);

   return !flag;
}

static int _check_b_class_2_10G(int port)
{
    FILE* fp=NULL;
    int slot=0;
    int flag=1;
    char buf_1[256];
    char buf_2[256];

    port = port +1 ;
    if((0!=my_slot_id) && (1!= my_slot_id))
    {
        if((port==3) || (port==4) || (port==7) || (port==8))
            return 0;

        if(port<=4)
            sprintf(buf_1, "slot_1=");
        else
            sprintf(buf_1, "slot_2=");

        fp = fopen("/appfs/port_switch/userdef", "r");
        if(fp==NULL)
        {
            return 0;/*file not exist , indicate 4*10G by default*/
        }

        flag=1;
        while (fgets(buf_2,sizeof(buf_2),fp) != NULL) {
            if(0==strncmp(buf_1, buf_2, strlen(buf_1)))
            {
                flag = atoi(strchr((void *)buf_2, '=') + 1);
                break;
            }
        }    
        fclose(fp);

        if(flag==1)
            return 0;
        else           //flag=3;
            return 1;
    }

    slot = (port-5)/4+3;
    sprintf(buf_1, "slot_%d=", slot);

    fp = fopen("/appfs/port_switch/userdef", "r");
    if(fp==NULL)
    {
        return 1;/*file not exist , indicate 40g by default*/
    }

    while (fgets(buf_2,sizeof(buf_2),fp) != NULL) {
        if(0==strncmp(buf_1, buf_2, strlen(buf_1)))
        {
            flag = atoi(strchr((void *)buf_2, '=') + 1);
            break;
        }
    }    
    fclose(fp);

    if(flag==1)
        return 0;
    else           //flag=3;
        return 1;
}


int module_set_port_mode(struct smp_t *smp, int able)
{
    int rv = 0;
    int module, port;
    bcm_port_map_t *bpm;
    int unit = 0, bcm_port = 0;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if (smp->modules & (1 << module)) {
            bpm = OeAp_Get_Bcm_Portmap_By_Sm(my_slot_id, module);
            if(bpm == NULL)
                continue;

            for (port = 0; port < MAX_PORT_NUMBER; port++) {
                if (port >= bpm->port_number)
                    continue;
                if (smp->ports & (1ull << port)) {
                    if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) {
                        if(module==MODULE_BACK)
                        {
                            /*40G b-class port should set autoneg-off and speed-40000 before loopback*/
                            char file_name[64]={0};
                            char buf[256];
                            FILE* fp=NULL;
                            unsigned long long port_mask = (uint64_t)1 << bcm_port;

                            if(_check_b_class_port_40g(port))
                            {
                                printf("port:%d is 40G port \r\n ", port);
                                if(able)
                                {
                                    sprintf(file_name, "/usr/local/bcm/tmp.soc");
                                    if((fp = fopen(file_name, "w")))
                                    {
                                        fprintf(fp, "port 0x%llx autoneg=off speed=40000", port_mask);
                                    }
                                    fclose(fp);
                                    sprintf(buf, "echo -e \"%d:\nrcload /usr/local/bcm/tmp.soc\nexit\" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1", unit);
                                    rv = system("killall -9 bcm.user.proxy 1>/dev/null 2>&1");
                                    if (system(buf) != 0)
                                        return OE_RST_INT_ERR;
                                }
                                else
                                {
                                    sprintf(file_name, "/usr/local/bcm/tmp.soc");
                                    if((fp = fopen(file_name, "w")))
                                    {
                                        fprintf(fp, "port 0x%llx autoneg=on", port_mask);
                                    }
                                    fclose(fp);
                                    sprintf(buf, "echo -e \"%d:\nrcload /usr/local/bcm/tmp.soc\nexit\" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1", unit);
                                    rv = system("killall -9 bcm.user.proxy 1>/dev/null 2>&1");
                                    if (system(buf) != 0)
                                        return OE_RST_INT_ERR;

                                }
                                rv = im_port_loopback_set(unit, bcm_port, able==1?2:0);
                                if (rv != 0)
                                    rv = OE_RST_SET_CAM_ERR;

                            }
                            else  /*back ports, 10G ports*/
                            {
                                if(_check_b_class_2_10G(port))
                                {
                                    rv = im_port_loopback_set(unit, bcm_port, able==1?2:0);
                                    if (rv != 0)
                                        rv = OE_RST_SET_CAM_ERR;

                                    printf("Port:%d is 2*10G port \r\n", port);

                                    return 0;
                                }
                                //im_port_autoneg_get(unit, bcm_port, &autoneg_tmp);
                                if(able)
                                    im_port_autoneg_set(unit, bcm_port,0);
                                else
                                    im_port_autoneg_set(unit, bcm_port,1);
                                rv = im_port_loopback_set(unit, bcm_port, able==1?2:0);
                                if (rv != 0)
                                    rv = OE_RST_SET_CAM_ERR;
                                im_port_speed_set(unit, bcm_port, 10000);

                                if(!able)
                                    im_port_autoneg_set(unit, bcm_port,1);

                                printf("port:%d is not 40G port or 2*10G , able=%d \r\n ", port, able);
                                return rv;
                            }
                        }
                        else
                        {
                           /*Front Ports, 10G ports*/
                            rv = im_port_loopback_set(unit, bcm_port, able==1?2:0);
                        }
                    }
                }
            }
        }
    }

    return rv;
}

int OeMd_Set_Interface_Mode(int modules, unsigned long long ports, int mode)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					break;

				if (ports & (1ull << k)) {
					g_port_status[my_slot_id][j][k].mode = mode;
                }
			}
		}
	}

	return OE_RST_OK;
}

int OeMdl_Set_Interface_Descript(int modules, unsigned long long ports, char* descript)
{
       int j, k;

       for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
               if (modules & (1 << (j + MODULE_REAR))) {
                       for (k = 0; k < MAX_PORT_NUMBER; k++) {
                               /*check smp.*/
                               if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
                                       continue;
                               if (ports & (1ull << k))
                               {
                                       memcpy(g_port_status[my_slot_id][j][k].port_descr,descript,MAX_PB_STR_LEN);
                               }
                       }
               }
       }

       return OE_RST_OK;
}

int module_get_interface_info(struct smp_t *smp, struct oe_info_t *if_sfp_attr)
{
    int rv = 0;
    int module, port, m_count = 0, p_count = 0;
    struct board_sfp_t *board_sfp_p = NULL;

    assert(if_sfp_attr != NULL);

    for (module = MODULE_REAR; module <= MODULE_BACK; module++) {
        if (smp->modules & (1 << module)) {
            board_sfp_p = &if_sfp_attr->board_sfp[m_count];
            board_sfp_p->sm_common.slot_id = my_slot_id;
            board_sfp_p->sm_common.module_id = module;
            for (port = 0; port < MAX_PORT_NUMBER; port++) {
                if (smp->ports & (1ull << port)) {
                    rv = ow_get_port_info(module, port, &board_sfp_p->port_sfp[p_count]);
                    if (rv != 0) {
                        board_sfp_p->n_port_sfp = p_count;
                        if_sfp_attr->n_board_sfp = m_count;
                    }
                    if (board_sfp_p->port_sfp[p_count].smp_common.port_valid_flag == 0) {
                        continue;
                    }
                    p_count++;
                }
            }
            board_sfp_p->n_port_sfp = p_count;
            m_count++;
            p_count = 0;
        }
    }
    if_sfp_attr->n_board_sfp = m_count;

    return rv;
}

#ifdef SW_BCM_56960
int __oe_set_smp_single_fiber_transmit_56960( 
        struct smp_t *smp,  
        bcm_port_map_t *pbmp,  
        int module,  
        int onoff) 
{ 
    FILE *fp = NULL; 
    char file_name[64]={0}; 
    char buf[256] = {0}; 
    int speed = 0, slot_type = 0, slot = 0; 
    int port, unit = 0, bcm_port = 0; 
    opt_pbmp_t port_mask; 

    sprintf(file_name, "/usr/local/bcm/tmp.soc"); 

    slot = OeAp_Get_My_Slot_Id(); 
    slot_type = OeAp_Get_My_Board_Type(); 

    for (port = 0; port < MAX_PORT_NUMBER; port++) { 
        if (port >= pbmp->port_number) 
            break; 
        if (smp->ports & (1ull << port)) { 
            if (OeAp_Get_Bcm_Unit_Port(pbmp, module, port, &unit, &bcm_port) == 0) { 
                if (slot_type == BOARD_TYPE_SF4800) { 
                    if (onoff == 1) { 
                        if (im_port_single_transmit_set(unit, bcm_port, onoff) != 0) { 
                            return OE_RST_SET_CAM_ERR; 
                        } 
                        sleep(1); 
                        if (im_port_linkscan_set(unit, bcm_port, 0) != 0) { 
                            return OE_RST_SET_CAM_ERR; 
                        } 
                    } else { 
                        if (im_port_linkscan_set(unit, bcm_port, 1) != 0) { 
                            return OE_RST_SET_CAM_ERR; 
                        } 
                        sleep(1); 
                        if (im_port_single_transmit_set(unit, bcm_port, onoff) != 0) { 
                            return OE_RST_SET_CAM_ERR; 
                        } 
                    } 
                    continue; 
                } 
                if (NULL == (fp = fopen(file_name, "w"))) { 
                    return OE_RST_FILE_ERR; 
                } 
                CLEAR_BMP(port_mask); 
                PORT_BMP_ADD(port_mask, bcm_port); 
                im_port_speed_get(unit, bcm_port, &speed); 
                if (onoff == 1) { 
                    if(slot_type != BOARD_TYPE_SF4800) 
                    { 
                        fprintf(fp, "port 0x%08x%08x%08x%08x%08x%08x%08x%08x linkscan=off\n", 
                                port_mask.pbits[7], port_mask.pbits[6], port_mask.pbits[5], port_mask.pbits[4], 
                                port_mask.pbits[3], port_mask.pbits[2], port_mask.pbits[1], port_mask.pbits[0]); 
                        fprintf(fp, "sleep 0 100000 \r\n"); 
                    } 
                    if (speed == 10000) 
                    { 
                        fprintf(fp, "modreg xmac_ctrl.0x%08x%08x%08x%08x%08x%08x%08x%08x rx_en=0\n", 
                                port_mask.pbits[7], port_mask.pbits[6], port_mask.pbits[5], port_mask.pbits[4], 
                                port_mask.pbits[3], port_mask.pbits[2], port_mask.pbits[1], port_mask.pbits[0]); 
                        fprintf(fp, "sleep 0 100000 \r\n"); 
                    } 
                    else 
                    { 
                        fprintf(fp, "modreg command_config.0x%08x%08x%08x%08x%08x%08x%08x%08x rx_ena=0\n", 
                                port_mask.pbits[7], port_mask.pbits[6], port_mask.pbits[5], port_mask.pbits[4], 
                                port_mask.pbits[3], port_mask.pbits[2], port_mask.pbits[1], port_mask.pbits[0]); 
                        fprintf(fp, "sleep 0 100000 \r\n"); 
                    } 
                } else { 
                    if(slot_type != BOARD_TYPE_SF4800) 
                    { 
                        fprintf(fp, "port 0x%08x%08x%08x%08x%08x%08x%08x%08x linkscan=on\n", 
                                port_mask.pbits[7], port_mask.pbits[6], port_mask.pbits[5], port_mask.pbits[4], 
                                port_mask.pbits[3], port_mask.pbits[2], port_mask.pbits[1], port_mask.pbits[0]); 
                        fprintf(fp, "sleep 0 100000 \r\n"); 
                    } 
                    if (speed == 10000) 
                    { 
                        fprintf(fp, "modreg xmac_ctrl.0x%08x%08x%08x%08x%08x%08x%08x%08x rx_en=1\n", 
                                port_mask.pbits[7], port_mask.pbits[6], port_mask.pbits[5], port_mask.pbits[4],
                                port_mask.pbits[3], port_mask.pbits[2], port_mask.pbits[1], port_mask.pbits[0]);
                        fprintf(fp, "sleep 0 100000 \r\n");
                    }    
                    else 
                    {    
                        fprintf(fp, "modreg command_config.0x%08x%08x%08x%08x%08x%08x%08x%08x rx_ena=1\n",
                                port_mask.pbits[7], port_mask.pbits[6], port_mask.pbits[5], port_mask.pbits[4],
                                port_mask.pbits[3], port_mask.pbits[2], port_mask.pbits[1], port_mask.pbits[0]);
                        fprintf(fp, "sleep 0 100000 \r\n");
                    }    
                }    
            }    
        }    
    }    
    if (slot_type == BOARD_TYPE_SF4800) {
        return OE_RST_OK;
    }    
    fclose(fp);
    sprintf(buf, "echo -e \"%d:\nrcload /usr/local/bcm/tmp.soc\nexit\" | /usr/local/bcm-user/bcmcli 1>/dev/null 2>&1", unit);
    if (system(buf) != 0) { 
        return OE_RST_INT_ERR;
    }    

    return OE_RST_OK;
}
#endif


int OeMdl_Set_Interface_Single_Fiber_Transmit(int modules, unsigned long long ports, int onoff)
{
	int j, k;

	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		if (modules & (1 << (j + MODULE_REAR))) {
			for (k = 0; k < MAX_PORT_NUMBER; k++) {
				/*check smp.*/
				if (_check_smp_valid(my_board_type, j + MODULE_REAR, k) != OE_RST_OK)
					continue;
				if (ports & (1ull << k))
                {
					g_port_status[my_slot_id][j][k].single_fiber_transmit = onoff;
                }
			}
		}
	}

	return OE_RST_OK;
}

int OeMdl_Initialize(void)
{
	int j, k;

	memset(g_port_status, 0, sizeof(g_port_status));
	for (j = 0; j < MODULE_NUMBER_PER_BOARD; j++) {
		for (k = 0; k < MAX_PORT_NUMBER; k++) {
			g_port_status[my_slot_id][j][k].speed = DEFAULT_SPEED;
			g_port_status[my_slot_id][j][k].mac_address_learning = 0;
			g_port_status[my_slot_id][j][k].default_vlan = 1;
			//sprintf(g_port_status[i][j][k].port_descr, "%d/%c/%d", i + 1, module_str[j], k + 1);
			g_port_status[my_slot_id][j][k].port_descr[0] = '\0';
		}
	}

	return OE_RST_OK;
}


int module_init()
{
	int rv  = 0;

    my_slot_id = OeAp_Get_My_Slot_Id();
	my_board_type = OeAp_Get_My_Board_Type();

	/*install bcm api*/
	if (ow_install_api() != 0) {
		rv = -1;
		return rv;
	}

	if (ow_dev_open() < 0){
		rv = -1;
		return rv;
	}

	port_init();

	OeMdl_Initialize();
	ow_init();

	return rv;
}
