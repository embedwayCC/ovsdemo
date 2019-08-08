#ifndef __RPC_DEF_H__
#define __RPC_DEF_H__

#define SLOT_NUMBER_PER_SYSTEM    14

#define MAX_PORT_NUM_SF3600 24
//#define MAX_PORT_NUM_TRAP (SLOT_NUMBER_PER_SYSTEM * MAX_PORT_NUMBER )
#define MAX_PORT_NUM_TRAP (SLOT_NUMBER_PER_SYSTEM * MAX_PORT_INDEX )
#define MAX_BOARD_TEMPERATURE 	2
#define MAX_FCS_PORT_NUM			MAX_PORT_NUM_TRAP
#define MAX_MEMORY_NUM			1
#define MAX_AIR_FAN_NUM			1
#define MAX_POWER_NUM				1 
#define MAX_CPU_NUM				1

#define PORT_BIT_MASK_SF3600     0xFFFFFF

#define SLOT_BIT_MASK             0x3FFF
#define MODULE_BIT_MASK           0x70
#define PORT_BIT_MASK             0xFFFFFFFFFFFFFFLL
#define AC1200_BCM_PORT_BIT_MASK          0xfffffff
#define ET5100_BCM_PORT_BIT_MASK		  0x1fffffffffffeLL
#define SF3600_BCM_PORT_BIT_MASK          0x2222a22223ffffeaLL
#define ST3600_BCM_PORT_BIT_MASK          0x1fffffffffffeLL

#define SF3600_BCM_NOM_PBMP 0x1ffffe0
#define SF3600_F16_BCM_NOM_PBMP 0x1fffe00
#define AC1200_BCM_NOM_PBMP 0xffffff

#define AC_BOARDS_BIT_MASK             0x3FFC

#define MODULE_REAR 4
#define MODULE_FRONT 5
#define MODULE_BACK 6
#define MAX_MODULE_INDEX 7

#define REAR_MAX_PORT_NUM 24
#define FRONT_MAX_PORT_NUM 24

#define MAX_PORTS_WIDTH           4
#define UD_NUM                    8
#define MODULE_NUMBER_PER_BOARD   3
#define MAX_PORT_NUMBER           128
//#define MAX_PORT_INDEX            112
#define MAX_FRONT_PORT_INDEX      128
#define MAX_BACK_PORT_INDEX       52
#define MAX_REAR_PORT_INDEX       36
#define MAX_PORT_INDEX            (MAX_FRONT_PORT_INDEX+MAX_BACK_PORT_INDEX+MAX_REAR_PORT_INDEX)
#define MAX_SINGLE_PORT_NUM       MAX_PORT_INDEX*SLOT_NUMBER_PER_SYSTEM
#define MAX_STRING_LEN            32
#define MAX_VENDOR_NAME_LEN       16
#define DEFAULT_RULES_NUMBER      1024
#define DEFAULT_ACLS_NUMBER       2048
#define DEFAULT_ENTRY_NUMBER      128
#define PORTS_GROUP_NUMBER        1000
#define MAX_PRIO_VALUE 4096
#define MAX_ENTRY_INDEX 128
#define MAX_VLAN_VALUE 4094

#define MAX_PB_STR_LEN 128

#define PORT_TYPE_FIBER 1
#define PORT_TYPE_ELEC 2

#define LINK_PROTO_PPP 1
#define LINK_PROTO_HDLC 2
#define LINK_PROTO_ETHERNET 3

#define PORT_RUNNING_STATUS_UP 1
#define PORT_RUNNING_STATUS_DOWN 2
#define PORT_RUNNING_STATUS_SHUT 3

#define MAX_MAINTENANCE_ACCOUNT_NUM 10
#define MAX_BUSINESS_ACCOUNT_NUM 40

#define MAX_USER_NAME_LEN	16
#define MAX_USER_AUTH_PASSWD_LEN = 22

#define SPECIAL_SUPPER_UID 99
#define MAX_VLAN_S_NUM 4096
#define AC1200_TRUNK_MEMBER_NUM 8
#define MAX_HEAD_OFFSET_NUM 122

#define MAX_RANGE_NUM 32

typedef struct _exp_handle_rpc{
    void *handle;
	 int slot;
	 int platform;
} exp_handle_rpc;

typedef enum {
	 RPC_EXP_PLATFORM_S320,
	 RPC_EXP_PLATFORM_S40,
	 RPC_EXP_PLATFORM_MAX
}rpc_platform_t;

enum {
	STATUS_NORMAL = 1,
	STATUS_ABNORMAL = 2,
	STATUS_OFFLINE = 3
};

enum {
    RPC_EXP_KEY_MASK_PROTOCOL = 0x0002,
    RPC_EXP_KEY_MASK_SIP      = 0x0004,
    RPC_EXP_KEY_MASK_DIP      = 0x0008,
    RPC_EXP_KEY_MASK_SPORT    = 0x0010,
    RPC_EXP_KEY_MASK_DPORT    = 0x0020,
    RPC_EXP_KEY_MASK_FIN = 0x0040,
    RPC_EXP_KEY_MASK_SYN = 0x0080,
    RPC_EXP_KEY_MASK_RST = 0x0100,
    RPC_EXP_KEY_MASK_PSH = 0x0200,
    RPC_EXP_KEY_MASK_ACK = 0x0400,
    RPC_EXP_KEY_MASK_URG = 0x0800,
    RPC_EXP_KEY_MASK_TCP_FLAG = 0x0fc0,
    RPC_EXP_KEY_MASK_SMAC = 0x800000,
    RPC_EXP_KEY_MASK_DMAC = 0x1000000
};

typedef struct bp_map_s
{
    int          board_type;   
    int		     rio_type;
    int          module;     // 4:r 5:f
    int          unit;
    unsigned int port_number; 
    unsigned long long port_mask;
    int          port_map[MAX_PORT_NUMBER];
    int          port_xe[MAX_PORT_NUMBER];
}bp_map_t;

typedef struct used_info_s
{
    int slot;
    int n_count;
    bp_map_t *bpmap;
}used_info_t;

typedef struct rpc_used_info_s
{
    int n_info;
    used_info_t *info;
}rpc_used_info_t;

#if 0
struct smp_t {
    int slots;
    int modules;
    unsigned long long ports;
};
#endif

typedef struct smp_new_s {
    int slots;
    int modules;
    unsigned long long ports[4];
} smp_new_t;

typedef struct {
	smp_new_t smp;
	int trunk_id;
	int vlan_id;
}input_t;

typedef struct {
    int slot;
    int unit;
    int fp_full;
    int trunk_full;
    int ruleid;
}oe_rule_install_res_t;

typedef enum{
    RPC_BD_STATUS_BOARD_NOT_EXSIT =0,
    RPC_BD_STATUS_CONFIG_SUCCEED =1,
    RPC_BD_STATUS_CONFIG_AND_CONFIG_FAILED =2,
    RPC_BD_STATUS_MAX
} rpc_board_status_t;

/* be careful if you want to add board type */
typedef enum {
    RPC_BOARD_TYPE_UNKNOWN,
    RPC_BOARD_TYPE_SF2300,
    RPC_BOARD_TYPE_AC1200,
    RPC_BOARD_TYPE_AC2240,
    RPC_BOARD_TYPE_MAX
} rpc_board_type_t;

/*fru type*/
enum {
	RPC_CPU,
	RPC_FB,
	RPC_RTM,
	RPC_FRU_MAX
};


enum {
	 PROTOCOL_ICMP = 1,
	 PROTOCOL_TCP = 6,
	 PROTOCOL_UDP = 17
};

enum {
    UD_TYPE_HEAD = 0,
    UD_TYPE_L3 = 1,
    UD_TYPE_L4 = 2
};

enum {
    RPC_FW_MODE_DROP = 0,
    RPC_FW_MODE_HASH = 1,
    RPC_FW_MODE_MC = 2
};

enum {
    RPC_HASH_MODE_S =4,
    RPC_HASH_MODE_D =5,
    RPC_HASH_MODE_M =6
};

enum {
	OE_ENTRY_TYPE_FIVE_TUPLE = 1,
	OE_ENTRY_TYPE_MAC = 2,
	OE_ENTRY_TYPE_IPV6 = 3,
};

enum {
	ADD_ENTRY = 1,
	DEL_ENTRY = 2,
};

enum {
	VLAN_TAG_REPLACE,
	VLAN_TAG_RESERVE,
};

enum {
	USER_LEVEL_MAINTENANCE = 1,
	USER_LEVEL_BUSINESS = 2,
};


enum {
    L4_SRC_PORT = 0x1,
    L4_DST_PORT = 0x2,
    OUTVLAN = 0x40,
    L3_PACKET_LEN = 0x100,
};

enum {
    VLAN_TYPE_TAG = 0,
    VLAN_TYPE_UNTAG = 1,
    VLAN_TYPE_MAX_NUM = 2,
};

typedef struct {
	int vlan_id;
	int *trunk_id;
	int *untag_trunk_id;
	int n_trunk;
	struct smp_t *smp;
	struct smp_t *untag_smp;
	int n_smp;
}vlan_content_t;

typedef struct {
	vlan_content_t *vlan_content;
	int n_vlan_content;
}oe_vlan_t;

typedef struct {
    int data;
    int mask;
} oe_ud_t;

typedef struct {
    int used;
    int range_type;
    int min;
    int max;
}oe_range_t;

struct class_entry_t {
    unsigned int entry_index;

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
    unsigned int outer_vlan;
    unsigned int outer_vlan_mask;
    unsigned int inner_vlan;
    unsigned int inner_vlan_mask;
    
    oe_ud_t uds[UD_NUM];
    
    oe_range_t range[MAX_RANGE_NUM];
    unsigned int range_invert_flag[MAX_RANGE_NUM];
    unsigned int range_num;

    int stat_id;
};

typedef struct {
	int acl_id;
	int acl_count;
	int acl_type;
	int acl_prio;
    int n_class_entry;
    struct class_entry_t *class_entry_p;
	input_t input;
	input_t output;
	int fw_mode;
	int slot_mask;
}acl_content_t;

typedef struct {
	int n_acl_content;
	acl_content_t *acl_content;
}oe_acl_t;

struct board_info_t{
    int type;
    char version[MAX_STRING_LEN];
    char serial_num[MAX_STRING_LEN];
    int slot_id;
    int status;
    int om_switchover;
    int temprature;
    int up_time;
};

struct board_status_t{
    int status;
    int types;
};

struct oe_syshard_t{
    int n_board_info;
    struct board_info_t **board_info;
    
    int n_board_status;
    struct board_status_t **board_status;
};

struct uptime_t
{
    int up_day;
    int up_hour;
    int up_minute;
    int up_sec;
};

struct cpu_utilization_t
{
    int cpu_id;
    int cpu_util_5_30s;
    int cpu_util_1_5m;
    int cpu_status;
};

struct memory_utilization_t
{
    int memory_id;
    unsigned int max_memory;
    int memory_utilization;
    int memory_status;
};

struct power_t
{
    int power_id;
    int power_status;
};

struct air_fan_t
{
    int airfan_id;
    int airfan_status;
};

struct oe_system_t
{
    char device_name[MAX_PB_STR_LEN];
    char device_type[MAX_PB_STR_LEN];
    char software_version[MAX_PB_STR_LEN];
    char sys_time[MAX_PB_STR_LEN];
    struct uptime_t sys_uptime;
    int n_cpu;
    struct cpu_utilization_t **cpu_utilization;
    int n_memory;
    struct memory_utilization_t **memory_utilization;
    int n_power;
    struct power_t **power;
    int n_air_fan;
    struct air_fan_t **air_fan;
    char build_date[MAX_PB_STR_LEN];
    char build_time[MAX_PB_STR_LEN];
    char system_ip[MAX_PB_STR_LEN];
    char system_gateway[MAX_PB_STR_LEN];
    int platform;
    char systemname[MAX_PB_STR_LEN];
    int temps_count;
    unsigned char temps[10];
    unsigned char fan1_def_level;
    unsigned char fan1_cur_level;
    unsigned char fan2_def_level;
    unsigned char fan2_cur_level;
};

struct sm_common_t
{
	int slot_id;
	int module_id;
};

struct smp_common_t
{
	int port_id;
	int port_valid_flag;
	int port_insert_flag;
};

struct port_status_t
{
    struct smp_common_t smp_common;
    int link;
    int autoneg;
    int pause;
    int duplex;
    int speed;
    int mac_address_learning;
    int mtu;
    char port_name[MAX_PB_STR_LEN];
};

struct list_status_t
{
	int n_port_status;
	struct sm_common_t sm_common;
	struct port_status_t port_status[MAX_PORT_NUMBER];
};

struct oe_status_t
{
	int n_list_status;
	struct list_status_t list_status[MODULE_NUMBER_PER_BOARD];
};

struct counter_t
{
	int type;
	char name[32];
	unsigned long long total;
	unsigned long long rate;
};
struct port_counter_t
{
	struct smp_common_t smp_common;
	int n_counter;
	struct counter_t *counter;
};

struct board_counter_t
{
	int n_port_counter;
	struct sm_common_t sm_common;
	struct port_counter_t port_counter[MAX_PORT_NUMBER];
};

struct oe_counter_t
{
	int n_board_counter;
	struct board_counter_t board_counter[MODULE_NUMBER_PER_BOARD];
};

typedef struct
{
	int tid;
	int is_valid;
	int n_counter;
	struct counter_t *counter;
}trunk_counter_t;

typedef struct

{
	int n_trunk_counter;
	trunk_counter_t *trunk_counter;
}oe_trunk_counter_t;

struct port_sfp_t
{
	struct smp_common_t smp_common;
	char port_name[MAX_PB_STR_LEN];
	char port_descr[MAX_PB_STR_LEN];
	int port_type;
	int link_proto;
	int running_status;
    int present;

	double tx_power;
	int tx_power_flag; /*1: N/A*/
	double tx_dbm;
    double tx_dbm_1;
    double tx_dbm_2;
    double tx_dbm_3;
	double rx_power;
	int rx_power_flag; /*1: N/A*/
	double rx_dbm;
	double rx_dbm_1;  /*1-3 for 100G optical module*/
	double rx_dbm_2;
	double rx_dbm_3;

    int valid_window_flag; /*indicate if window exists*/
    double rx_min;
    double rx_max;
    double tx_min;
    double tx_max;

    char vendor_name[64];
    char vendor_pn[64];
    int wavelength;
    int module_type;

	int speed;

	int temperature;
	int temperature_flag; /*1: N/A*/
};

struct board_sfp_t
{
	int n_port_sfp;
	struct sm_common_t sm_common;
	struct port_sfp_t port_sfp[MAX_PORT_NUMBER];
};

struct oe_info_t
{
	int n_board_sfp;
	struct board_sfp_t board_sfp[MODULE_NUMBER_PER_BOARD];
};

struct user_info_t
{
	char login[MAX_USER_NAME_LEN];
	int level;
	int active;
};

struct oe_users_info_t
{
	int n_user_sfp;
	struct user_info_t **user_sfp;
};

typedef struct port_list{
    smp_new_t smp;
    struct port_list *p_next;
    int weight;
}port_list_t;

struct oe_inports_t {
    int used;
    int id; 
    int smp_num;
    int pvlan_able;
    int rx_vlantag_id;
    port_list_t *p_list;
};

struct oe_inports_all_t{
    int n_inports;
    struct oe_inports_t **inports;
};

#define OUTPORT_HASH 1
#define OUTPORT_MC   2
struct oe_outports_t {
    int id; 
    int smp_num;
    int load_balance;
    int untag;
    int type; // OUTPORT_HASH or OUTPORT_MC
    int used; // 1 means this outport has been used already
    int index; // trunk_id or linkagg_id
    port_list_t *p_list;
};

struct oe_outports_all_t{
    int n_outports;
    struct oe_outports_t **outports;
};

struct oe_l2_static_entry_t{
    unsigned char mac[8];
    int inports_id;
    int outports_id;
    int vlan_id;
};

struct oe_l2_static_t {
    int n_mac_res;
    struct oe_l2_static_entry_t **mac_res;
};

struct oe_l2_dynamic_entry_t{
    unsigned char mac[8];
    int vlan_id;
    smp_new_t smp;
};

struct oe_l2_dynamic_t {
    int n_mac_res;
    struct oe_l2_dynamic_entry_t **mac_res;
};

struct oe_rule_config_t {
    int inport_id;
    struct outports_id_list_t * hash_outports_list;
    int vlan_min;
    int vlan_max;
};

struct temperature_t
{
    int temp_id;
    int temp_status;
};

struct port_fcs_t
{
    char port_name[MAX_PB_STR_LEN];
    int fcs_status;
    unsigned long rate;
};

struct port_drop_t
{
    char port_name[MAX_PB_STR_LEN];
    int drop_status;
    unsigned long rate;
};

struct oe_snmp_trap_info_t {
    struct cpu_utilization_t cpu_utilization;
    struct memory_utilization_t memory_utilization;
    struct power_t power;
    struct air_fan_t air_fan;
    struct temperature_t temperature;
    struct port_fcs_t fcs;
    struct port_drop_t drop;;
};

enum {
    RPC_EXP_RST_OK,                        /* ok */

    RPC_EXP_RST_NO_MEM = 501,              /* malloc failed */
    RPC_EXP_RST_RS_FULL = 502,             /* resource full */
    RPC_EXP_RST_ERR_NETWORK_FALSE = 503,   /* network false */
    RPC_EXP_RST_ERR_CONNECT = 504,         /* connect false */

    RPC_EXP_RST_INVLD_PRM = 601,           /* invalid parameter */
    RPC_EXP_RST_FILE_ERR = 602,            /* file operate error */

    RPC_EXP_RST_CE_NOT_FOUND = 701,        /* class entry not found*/
    RPC_EXP_RST_CE_EXIST = 702,            /* class entry already exist*/
    RPC_EXP_RST_RULE_INSTALL = 703,        /* need rule uninstall*/
    RPC_EXP_RST_INPORT_REPEAT = 704,       /* inport repeat */
    RPC_EXP_RST_OUTPORTS_NUM = 705,        /* hash outports number must be smaller than 8 */
    RPC_EXP_RST_HASH_ERR = 706,            /* hash outports isn't in the same module */
    RPC_EXP_RST_SET_CAM_ERR = 707,         /* set cam error */
    RPC_EXP_RST_GET_CAM_ERR = 708,         /* get cam error */
    RPC_EXP_RST_DEAD_LOOP = 710,           /* dead loop*/
    RPC_EXP_RST_INIT = 711,                /* init config */
    RPC_EXP_RST_UD_ERR_CE = 712,           /* UD , MAC and the five-tuple shouldn't be in the same class entry */
    RPC_EXP_RST_UD_ERR_RULE = 713,         /* UD , MAC and the five-tuple shouldn't be in the same rule */
    RPC_EXP_RST_UD_ERR_UD = 714,           /* ud0-ud3 and ud4-ud7 can't be in the same class entry */
    RPC_EXP_RST_NO_PORT_IN = 715,          /* no port in the inports */
    RPC_EXP_RST_NO_PORT_OUT = 716,         /* no port in the outports */
    RPC_EXP_RST_UD_ERR = 717,              /* ud can't be used in the situation */

    RPC_EXP_RST_INTERNAL_ERR = 999         /* internal err */
};

struct outports_id_list_t {
    int num;
    int list[PORTS_GROUP_NUMBER];
};

#endif
