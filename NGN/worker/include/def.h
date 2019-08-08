#ifndef __DEF_H__
#define __DEF_H__

#include "exp_def.h"
#include "rpc_def.h"


#define UNIT_NUMBER_PER_BOARD 2

#define SF2300_MAX_PORT 11
#define AC1200_MAX_PORT 48
#define AC2240_MAX_PORT MAX_PORT_NUMBER

#define AC1200_TO_S1_S2     26 //xe2
#define S1_S2_TO_AC1200     27 //xe3
#define SF3600_TO_S1        33

#define L2_NUMBER_PER_UNIT 1024
#define TRUNK_NUMBER_PER_UNIT   1024
#define FP_NUMBER_PER_UNIT   4096
#define FP_NUMBER_LOOKUP_STAGE 1024
#define FP_NUMBER_INGRESS_STAGE 2048
#define FP_NUMBER_EGRESS_STAGE 1024
#define FABRIC_TRUNK_OFFSET 1024
#define INNER_TRUNK_OFFSET  1010

#define BACK_PORT_SPACING   4

#define MAX_VID 1024

#define MAX_HASH_OUT_PORTS_NUM 8

#define MAX_SNMP_TRAP_IP_NUM 5

#define MAX_BCM_MODULE_NUM 32

#define CMD_LEN_MAX 4096
#define MAX_PRINT_BUF 4000 
#define DEFAULT_TIMEOUT 5000

/* Add for port bitmap */
#define PORT_WORD_WIDTH 32
#define PORT_WORD_MAX   8
typedef struct opt_pbmp {
     unsigned int pbits[PORT_WORD_MAX];
} opt_pbmp_t;



#define PORT_BMP_ADD(pbmp, port) \
    do { \
        pbmp.pbits[port / PORT_WORD_WIDTH] |= (1u << (port % PORT_WORD_WIDTH)); \
    } while(0)
#define PORT_BMP_DEL(pbmp, port) \
    do { \
        pbmp.pbits[port / PORT_WORD_WIDTH] &= ~(1u << (port % PORT_WORD_WIDTH)); \
    } while(0)
#define PORT_BMP_SET(pbmp, port) \
    do { \
        pbmp.pbits[port / PORT_WORD_WIDTH] = (1u << (port % PORT_WORD_WIDTH)); \
    } while(0)
#define IS_PORT_IN_BMP(pbmp, port) \
     ((pbmp.pbits[port / PORT_WORD_WIDTH] & (1u << (port % PORT_WORD_WIDTH))) ? 1 : 0)
#define CLEAR_BMP(pbmp) memset(&pbmp, 0, sizeof(pbmp))
#define SET_BMP(pbmp_a, pbmp_b) memcpy(&pbmp_a, &pbmp_b, sizeof(pbmp_a))
#define MERGE_BMP(pbmp_a, pbmp_b) \
    do { \
        int _index; \
        for (_index = 0; _index < PORT_WORD_MAX; _index++) { \
            pbmp_a.pbits[_index] |= pbmp_b.pbits[_index]; \
        } \
    } while(0)
#define EMASK_BMP(pbmp) \
    do { \
        int _index; \
        for (_index = 0; _index < PORT_WORD_MAX; _index++) { \
            pbmp.pbits[_index] = ~pbmp.pbits[_index]; \
        } \
    } while(0)
#define IS_EQUAL_BMP(pbmp_a, pbmp_b) \
    ((memcmp(&pbmp_a, &pbmp_b, sizeof(pbmp_a)) == 0) ? 1 : 0)
#define IS_EMPTY_BMP(pbmp) \
    ((pbmp.pbits[0] == 0 && pbmp.pbits[1] == 0 && \
        pbmp.pbits[2] == 0 && pbmp.pbits[3] == 0 && \
        pbmp.pbits[4] == 0 && pbmp.pbits[5] == 0 && \
        pbmp.pbits[6] == 0 && pbmp.pbits[7] == 0)? 1 : 0)


#define ITERATOR_BMP_START(pbmp,port_iterator) \
{ \
    for(port_iterator=0; port_iterator<256; port_iterator++) \
    { \
        if(IS_PORT_IN_BMP(pbmp, port_iterator)) \
        { 

#define ITERATOR_BMP_END \
        } \
    } \
}

#define OW_PTR_VALID_CHECK(ptr) \
    if ((ptr) == NULL) \
    { \
        return OE_RST_INVLD_PRM; \
    }


typedef struct {
	char port_descr[MAX_PB_STR_LEN];
	int autoneg;
	int pause;
	int shutdown;
	int single_fiber_transmit;
	int single_fiber_receive;
	int duplex;
	int speed;
	int mac_address_learning;
	int mode_route;
	int mac_ignore;
	int default_vlan;
	int rx_vlantag;
	int eid;
    int station_id;
	int used_by_trunk;
    int mode;
    int connect_to_fpga;
    int connect_to_ac1200;
} oe_port_status_t;

typedef struct {
    int used;
    int offset_type;
    int offset_value;
} oe_ud_conf_t;

typedef struct {
    unsigned long long pre_stat_bytes;
    unsigned long long pre_stat_packets;
    unsigned long long cur_stat_bytes;
    unsigned long long cur_stat_packets;
    unsigned long long stat_bytes_rate;
    unsigned long long stat_packets_rate;
}oe_stat_counters;

typedef struct oe_class_entry{
    unsigned int used;
    unsigned int entry_index;  /*begin with 1; init: 0*/

    unsigned int key_mask;
    unsigned int ud_mask;

    unsigned int protocol;
    unsigned int ethertype;

    unsigned int sip;
    unsigned int sip_mask;

    unsigned int dip;
    unsigned int dip_mask;

    unsigned short sport;
    unsigned short sport_mask;

    unsigned short dport;
    unsigned short dport_mask;

    unsigned char smac[6];
    unsigned char smac_mask[6];

    unsigned char dmac[6];
    unsigned char dmac_mask[6];
    unsigned int tcp_flag;

    unsigned short sipv6[8];
    unsigned short sipv6_mask[8];

    unsigned short dipv6[8];
    unsigned short dipv6_mask[8];
	unsigned int inner_vlan;
	unsigned int inner_vlan_mask;
    oe_ud_t uds[UD_NUM];
    
    oe_range_t range[MAX_RANGE_NUM];
    unsigned int range_invert_flag[MAX_RANGE_NUM];
    unsigned int range_num;

    unsigned int entry_id; //fp entry id
    int stat_id;  // statistics id
    oe_stat_counters fp_stat;
    
    struct oe_class_entry *prev;
    struct oe_class_entry *next;
    struct oe_class_entry *last;
} oe_class_entry_t;

typedef struct
{
    int used;         // Work on this slot
    //int drop;         // 1 means drop
    //int type;         // RULE_CROSS_XXX
    //int sub_type;     // if has local ports, RULE_CROSS_LOCAL_XXX
    opt_pbmp_t back;     // back ports on this slot
    opt_pbmp_t front;    // front ports on this slot
    opt_pbmp_t rear;     // rear ports on this slot
    unsigned int outports[8];   // if HASH, 8 outports most
    unsigned int num_outports;
    opt_pbmp_t cross_back;  // cross to which slots , cross_back[0] means connecting to slot-1
} single_slot_record_t; // devide a complete rule into serveral tiny rules on seprate slot

typedef struct
{
    opt_pbmp_t back;     //back ports in this vlan
    opt_pbmp_t front;    // front ports in this vlan
    opt_pbmp_t rear;     // rear ports in this vlan
}slot_vlan_record_t;

typedef struct {
    oe_class_entry_t*  class_entry_p;
    unsigned int entry_used_num;
    int rule_key_mask;
    int rule_ud_mask;
    int used;
    int inports_id; 	/*begin with 0; default: -1*/
    struct outports_id_list_t hash_list;
    struct outports_id_list_t mirror_list;
    struct outports_id_list_t out_list;
    int forward_action;    /*FW_MODE_DROP;  FW_MODE_HASH;  FW_MODE_MC; FW_MODE_HASH_MC*/
	unsigned int outer_vlan; //outervlan by user
    unsigned int outer_vlan_work; //outervlan actually works
    unsigned int vlan_min;   // vlan range min
    unsigned int vlan_max;   // vlan range max
    int entry_record[SLOT_NUMBER_PER_SYSTEM];       //Slots have been set acls by This rule
    slot_vlan_record_t vlan_record[SLOT_NUMBER_PER_SYSTEM][VLAN_TYPE_MAX_NUM];  //Ports have been assigned vlan by This rule
    single_slot_record_t slot_record[SLOT_NUMBER_PER_SYSTEM];
    int range_index;       //vlan range index , if used
} oe_rule_t;

#define MAX_VLAN_RANGE_NUM 32
typedef struct{
    int used;
    int min;
    int max;
    int worked;  // if works by HARDWARE
}oe_vlan_range_t;

typedef struct {
    int vlan_id;
    smp_new_t pbmp;
    smp_new_t ubmp;
} oe_vlan_info_t;

typedef struct {
    int sm;  /*bit0~bit1: module(r,f,b)  bit2~bit5: slot(0~13)*/
    unsigned long long port; /*portmask*/
} smp_unit_t;

typedef struct {
	int port_num;
	int ports[MODULE_NUMBER_PER_BOARD*MAX_PORT_NUMBER];
}inports_t;

struct ip_addr_t {
	int ip;
	unsigned char ipv6[16];
};

enum {
    IO_PORTS_SLOT1,
    IO_PORTS_SLOT2,
    IO_PORTS_AC1200_UNIT0,
    IO_PORTS_AC1200_UNIT1,
    IO_PORTS_SF3600,
};

typedef struct{
    int flag;
    int min;
    int max;
}bcm_range_t;

typedef struct {
   bcm_range_t range_collection[MAX_RANGE_NUM];
   int range_num;
}oe_bcm_range_t;

typedef struct {
    int used;
    unsigned int key_mask;
    unsigned int ud_mask;

    unsigned int protocol;
    unsigned int ethertype;

    unsigned int sip;
    unsigned int sip_mask;

    unsigned int dip;
    unsigned int dip_mask;

    unsigned short sport;
    unsigned short sport_mask;

    unsigned short dport;
    unsigned short dport_mask;

    unsigned char smac[6];
    unsigned char smac_mask[6];

    unsigned char dmac[6];
    unsigned char dmac_mask[6];

    unsigned int tcp_flag;	
	
    unsigned short sipv6[8];
    unsigned short sipv6_mask[8];

    unsigned short dipv6[8];
    unsigned short dipv6_mask[8];

    oe_ud_t uds[UD_NUM];

    unsigned char range_flag[MAX_RANGE_NUM];
    unsigned char range_invert_flag[MAX_RANGE_NUM];

    int outer_vlan;
    int inner_vlan;
    int inner_vlan_mask;
    int forward_action;
    opt_pbmp_t out_pbmap;
    int ruleid;
	int eid;
}oe_bcm_fp_t;

typedef struct {
    int forward_action;
    int rtag;
    unsigned long long out_pbmap;
} oe_bcm_fp_action_t;

typedef struct {
    unsigned int modid;
    opt_pbmp_t pbmp;
} bcm_trunk_t;

typedef struct {
    unsigned int member_num;
    bcm_trunk_t trunk_member[MAX_BCM_MODULE_NUM];
    unsigned int rtag;
    unsigned int bcm_trunk_id;//this element only makes sense in BOARD_TYPE_AC1200
} oe_bcm_trunk_t;

typedef struct {
    int valid;
    char mac[8];
    int vlan_id;
    int pmodid;
} oe_bcm_l2_t;

typedef struct {
    opt_pbmp_t pbmp;
    opt_pbmp_t ubmp;
} oe_bcm_vlan_t;

typedef struct {
	int acl_id;
	input_t input;
	input_t output;
} oe_statistic_t;

typedef struct {
	int eid;
	int stat_id;
} stat_t;

typedef struct {
	stat_t stat[DEFAULT_ENTRY_NUMBER];
	int n_stat;
	int stage;
} oe_bcm_statistic_t;

typedef struct {
	int num;
	int type;
} oe_bcm_group_info;

#if 0
/*fru type*/
enum {
CPU,
FB,
RTM,
/*AMC1,
AMC2,
AMC3,
AMC4*/
FRU_MAX
};
#endif

struct port_snmp_t
{
    struct smp_common_t smp_common;
    unsigned long long ifInKbits;
    unsigned long long ifOutKbits;
    unsigned long long ifInOctets;
    unsigned long long ifInUcastPkts;
    unsigned long long ifInNUcastPkts;
    unsigned long long ifInDiscards;
    unsigned long long ifInErrors;
    unsigned long long ifInUnknownProtos;
    unsigned long long ifOutOctets;
    unsigned long long ifOutUcastPkts;
    unsigned long long ifOutNUcastPkts;
    unsigned long long ifOutDiscards;
    unsigned long long ifOutErrors;
    unsigned long long ifOutQLen;
    unsigned long long ifHCInOctets;
    unsigned long long ifHCInUcastPkts;
    unsigned long long ifHCInMulticastPkts;
    unsigned long long ifHCInBroadcastPkts;
    unsigned long long ifHCOutOctets;
    unsigned long long ifHCOutUcastPkts;
    unsigned long long ifHCOutMulticastPkts;
    unsigned long long ifHCOutBroadcastPkts;
    unsigned long long ifInFcserrorPkts;
    unsigned long long ifOutFcserrorPkts;
};

struct module_snmp_t
{
    int n_port_snmp;
    struct sm_common_t sm_common;
    struct port_snmp_t *port_snmp;
};

struct oe_snmp_t
{
    int n_module_snmp;
    struct module_snmp_t *module_snmp;
};

struct oe_snmp_conf_t
{
    char *service;
    char *sys_name;
    char *sys_contact;
    char *sys_location;
    char *rocommunity;
    char *rwcommunity;
    char *trap_ip[MAX_SNMP_TRAP_IP_NUM];
    char *trap_version;
    char *username;
    char *auth_type;
    char *auth_password;
    char *priv_type;
    char *priv_password;
    char *access;
};

struct oe_login_info_t
{  
    char username[MAX_USER_NAME_LEN];
    char logintool[16];
    unsigned int ip;
    unsigned short port;
};


#define EV_HEADER_LEN (sizeof(struct ev_header_t))

struct ev_header_t {
    int rv;
    int len;
    int op;
};

struct ev_buffer_t {
    int total_len;
    char *buffer;
};

typedef struct {
    char slot;
    char module;
    char port;
    char link_status;
} ow_port_status_sync_t;

typedef struct {
    int used;
    int index;
    smp_new_t smp;
    int vlan_old;
    int vlan_old_mask;
    int vlan_new;
} oe_e_fp;

typedef struct {
    int rule_id;
    int entry_num;
    int fw_type;
    int inport;  // 0:hash 1:mc 2:drop
    int outport_num; // outports_num <=0
    int outports[PORTS_GROUP_NUMBER];
    int outervlan;
}oe_rule_info_t;

typedef struct
{
    int n_rule_info;
    oe_rule_info_t *rule_info;
}oe_rule_info_set_t;

#endif
