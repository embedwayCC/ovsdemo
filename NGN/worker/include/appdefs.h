#ifndef __APPDEFS_H__
#define __APPDEFS_H__
#include <string.h>

#include "os.h"
#include "def.h"
#include "exp_def.h"

#define CONFIG_FILE_PATH         "/appfs/config/"_CURRENT_CONFIG_PATH_
#define CONFIG_FILE_NAME         CONFIG_FILE_PATH"/save.conf.gz"
#define LOG_CONFIG_FILE_NAME     CONFIG_FILE_PATH"/log.config.gz"
#define STARTUP_CONFIG_FILE_NAME CONFIG_FILE_PATH"/startup_save.conf"
#define CONFIG_SYNCONFIG_FILE_NAME "/appfs/warm.conf"
#define LKPT_CONFIG_FILE_NAME    "/appfs/config/lkpt.conf"
#define TEMP_CONFIG_FILE_NAME    "/usr/local/bin/save.conf"
#define TEMP_LOG_CONFIG_FILE_NAME    "/usr/local/bin/log.conf"
#define TEMP_LKPT_CONFIG_FILE_NAME "/usr/local/bin/lkpt.conf"
#define BCM_CONFIG_FILE_NAME "/appfs/oe_bcm_config.soc"
#define LOAD_CONFIG_RESULT "/usr/local/bin/load.result"
#define BIN_VERSION_PATH "/usr/local/bin/version.info"
#define BIN_CLI "/usr/local/bin/cli"

#define TIME_ZONE_FILE  "/sysfs/timezone"

#define SYS_INFO_FILE "/var/system_info"
#define CHASSIS_ID_FILE "/appfs/chassis_id.conf"

#define PLATFORM_INFO_FILE  "/var/platform_info"

#define APPADMIN_TMP_DIR "/usr/local/appadmin/tmp"

#define SHOW_CTR_CHANGED        0x01
#define SHOW_CTR_SAME           0x02
#define SHOW_CTR_Z              0x04
#define SHOW_CTR_NZ             0x08
#define SHOW_CTR_HEX            0x10
#define SHOW_CTR_RAW            0x20
#define SHOW_CTR_ED             0x40

#define APPFS_TFTPROOT_DIR "/appfs/tftproot"
#define APPFS_TFTPROOT_APPCONF_DIR APPFS_TFTPROOT_DIR"/appconf"
#define APPFS_TFTPROOT_CONFIG "/appfs/tftproot/config/"_CURRENT_CONFIG_PATH_
#define APPFS_TFTPROOT_CONFIG_SAVE APPFS_TFTPROOT_CONFIG"/save.conf.gz"
#define APPFS_TFTPROOT_CONFIG_STARTUP  APPFS_TFTPROOT_CONFIG"/startup_save.conf"
#define APPFS_DIR "/appfs"
#define BCM_DIR "/usr/local/bcm"

#define APPFS_TFTPROOT_VERSION_FILE APPFS_TFTPROOT_DIR"/version/"

#define BCM_UNIT0 (0)
#define BCM_UNIT1 (1)

#define INVALID_TABLE_NUMBER -1

#define SNMP_STATUS_FILE "/var/log/snmp_status"


#define SNMP_CONF "/appfs/extend_service.conf"

#define RUNNING_CFG "/appfs/running.conf"

#define MODULE_TYPE_MAX 5
#define HASH_MODE_NUM 3

#define ENABLE_SWITCHOVER "/sysfs/ms_enable"
#define FAKEBYPASS "/appfs/fakebypass"
#define USERDEF_FILE_NAME "/appfs/board_info.conf"
#define BOARD_CONF_FILE "/appfs/board.info"

#define MIPS_PPC_ETH_NAME    "eth0"
#define X86_ETH_NAME    "p3p1"

#define TRUNK_LOAD_BALANCE_SRCMAC       1          /* Source MAC address. */
#define TRUNK_LOAD_BALANCE_DSTMAC       2          /* Destination MAC address. */
#define TRUNK_LOAD_BALANCE_SRCDSTMAC    3          /* Source+dest MAC address. */
#define TRUNK_LOAD_BALANCE_SRCIP        4          /* Source IP address. */
#define TRUNK_LOAD_BALANCE_DSTIP        5          /* Destination IP address. */
#define TRUNK_LOAD_BALANCE_SRCDSTIP     6          /* Source+dest IP address. */
#define TRUNK_LOAD_BALANCE_REDUNDANT    7          /* Redundant (xgs_fabric). */
#define TRUNK_LOAD_BALANCE_PORTINDEX    8          /* Port index. */
#define TRUNK_LOAD_BALANCE_PORTFLOW     9          /* Enhanced hashing. */

#define L2_MAC_AGING_TIME_MIN   60   // (s)
#define L2_MAC_AGING_TIME_MAX   3000   // (s)

#define MAX_INPORTS_ID PORTS_GROUP_NUMBER
#define MAX_OUTPORTS_ID PORTS_GROUP_NUMBER

#define MAX_SMPORT_NUM (14 << 8 | 3 << 6 | 63)

#define GET_SLOT(s) (((s) >> 10)&0xf)
#define GET_MODULE(s) (((s) >> 8)&0x3)
#define GET_PORT(s) ((s)&0x7f)

#define GET_SMP(s,m,p) ((((s)&0xf) <<10) | (((m)&0x3) <<8) | ((p)&0x7f))

#define PVLAN_OFFSET 2

#define SNMP_TRAP_FCS_THRESHOLD		100
#define SNMP_TRAP_DROP_THRESHOLD		100
#define SNMP_TRAP_CPU_THRESHOLD		90
#define SNMP_TRAP_MEMORY_THRESHOLD	80
#define SNMP_TRAP_TEMP_THRESHOLD	       80

enum {
    E_OP_GET,
    E_OP_SET,
};

enum {
    TFTP_GET = 1,
    TFTP_PUT
};

typedef enum{
    BD_STATUS_BOARD_NOT_EXSIT							=0,
    BD_STATUS_CONFIG_SUCCEED							=1,
    BD_STATUS_CONFIG_AND_CONFIG_FAILED					=2,
    BD_STATUS_MAX
} board_status_t;

// be careful if you want to add board type
typedef enum {
    BOARD_TYPE_UNKNOWN,
    BOARD_TYPE_AC1200,
    BOARD_TYPE_SF3600,
    BOARD_TYPE_SF3600_SW,
    BOARD_TYPE_SF3600_F16,
    BOARD_TYPE_ST3600,
    BOARD_TYPE_ET5100,
    BOARD_TYPE_EP7000,
    BOARD_TYPE_EP7024,
    BOARD_TYPE_AC3800,
    BOARD_TYPE_EP7032,
    BOARD_TYPE_EP7048,
    BOARD_TYPE_EP7056,
    BOARD_TYPE_SF4800,
    BOARD_TYPE_AC2820,
    BOARD_TYPE_AC4300,
    BOARD_TYPE_EP8010,
    BOARD_TYPE_EP8020,
    BOARD_TYPE_MAX
} board_type_t;

typedef enum {
    RIO_TYPE_UNKNOWN = -1,
	RIO_TYPE_RT3016 = 3016,
} rio_type_t;

typedef enum {
    FR_20XE,
    FR_16XE,
} opmode_t;

typedef enum {
    PLATFORM_S320,
    PLATFORM_S40,
    PLATFORM_MAX
}platform_t;

enum {
    FW_MODE_DROP = 1,
    FW_MODE_HASH,
    FW_MODE_MC,
    FW_MODE_HASH_MC,
};

enum {
    HASH_MODE_S   =4,
    HASH_MODE_D   =5,
    HASH_MODE_M   =6
};

enum {
    SNMP_START = 1,
    SNMP_RESTART = 2,
    SNMP_STOP = 3,
    SNMP_RESTORE = 4
};

typedef enum {
    SNMP_SER = 0,
    SNMP_SYSNAME = 1,
    SNMP_SYSCONTACT = 2,
    SNMP_SYSLOCATION = 3,
    SNMP_ROCOMMUNITY = 4,
    SNMP_RWCOMMUNITY = 5,
    SNMP_TRAP_VERSION = 6,
    SNMP_USERNAME = 7,
    SNMP_AUTH_TYPE = 8,
    SNMP_AUTH_PASSWD = 9,
    SNMP_PRIV_TYPE = 10,
    SNMP_PRIV_PASSWD = 11,
    SNMP_ACCESS = 12,
    SNMP_TRAP_IP1 = 13,
    SNMP_TRAP_IP2 = 14,
    SNMP_TRAP_IP3 = 15,
    SNMP_TRAP_IP4 = 16,
    SNMP_TRAP_IP5 = 17,

    SNMP_NUM
}snmp_param_e;

#define SNMP_ADD_TRAP_IP    100
#define SNMP_DEL_TRAP_IP    101
enum {
    OE_KEY_MASK_PROTOCOL = 0x0002,
    OE_KEY_MASK_SIP      = 0x0004,
    OE_KEY_MASK_DIP      = 0x0008,
    OE_KEY_MASK_SPORT    = 0x0010,
    OE_KEY_MASK_DPORT    = 0x0020,
    OE_KEY_MASK_FIN = 0x0040,
    OE_KEY_MASK_SYN = 0x0080,
    OE_KEY_MASK_RST = 0x0100,
    OE_KEY_MASK_PSH = 0x0200,
    OE_KEY_MASK_ACK = 0x0400,
    OE_KEY_MASK_URG = 0x0800,
    OE_KEY_MASK_TCP_FLAG    = 0x0fc0,
	OE_KEY_MASK_OUTERVLAN = 0x10000,
	OE_KEY_MASK_INNERVLAN = 0x20000,
	OE_KEY_MASK_SIPV6  = 0x100000,
	OE_KEY_MASK_DIPV6  = 0x200000,
    OE_KEY_MASK_SMAC = 0x800000,
    OE_KEY_MASK_DMAC = 0x1000000,
    OE_KEY_MASK_ETHERTYPE = 0x400000, 
};

enum {
	E_RULE_TYPE_FIVE_TUPLE = 1,
	E_RULE_TYPE_MAC = 4,
	E_RULE_TYPE_IPV6 = 3,
};

enum {
	LOGIN_MODE_SSH = 1,
	LOGIN_MODE_TELNET = 2,
	LOGIN_MODE_CONSOLE = 3,
	LOGIN_MODE_FTP = 4,
	LOGIN_MODE_TFTP = 5,
	LOGIN_MODE_RCP = 6,
	LOGIN_MODE_SNMP = 7,
	LOGIN_MODE_SYSLOG = 8,
	LOGIN_MODE_OTHERS = 9
};

enum {
	E_LOOKUP_STAGE = 0,
	E_INGRESS_STAGE = 1,
	E_EGRESS_STAGE = 2,
	MAX_STAGE_NUM = 3,
};

enum {
    OM_STATE_BOARD_NOT_EXIST = 0,
    OM_STATE_BOARD_NOT_ACTIVE = 1,
    OM_STATE_BOARD_ACTIVE = 2,
    OM_STATE_MAX
};

enum {
    E_MODE_MIN_RX_RATE,
    E_MODE_MIN_RX_FCS_RATE,
    E_MODE_MIN_TX_FCS_RATE,
    E_MODE_MIN_RX_TX_PERCENT,
    E_MODE_SRV_MIN_TX_RATE,
    E_MODE_SRV_MIN_RX_FCS_RATE,
    E_MODE_SRV_MIN_TX_FCS_RATE,
    E_MODE_SRV_MIN_RX_TX_PERCENT,
};

enum                    
{                       
    E_LINK_PROTECT_OPTPOWER_CHECK_MASK  = 0x1,
    E_LINK_PROTECT_RX_CNT_CHECK_MASK    = 0x2,
    E_LINK_PROTECT_TX_CNT_CHECK_MASK    = 0x4,
    E_LINK_PROTECT_DROP_CNT_CHECK_MASK  = 0x8, 
    E_LINK_PROTECT_RX_FCS_CNT_CHECK_MASK= 0x10,
    E_LINK_PROTECT_TX_FCS_CNT_CHECK_MASK= 0x20,
    E_LINK_PROTECT_ZERO_RATE_CHECK_MASK = 0x40,
    E_LINK_PROTECT_LINK_STAT_CHECK_MASK = 0x80,
    E_LINK_PROTECT_CHECK_ALL_MASK       = 0xff,
};

enum {
    E_OUTERVLAN_RESERVE = 1,
    E_OUTERVLAN_REPLACE = 2,
};

enum
{
	TRAP_TYPE_LINK = 0,
	TRAP_TYPE_TEMP,
	TRAP_TYPE_FCS,
	TRAP_TYPE_MEM,
	TRAP_TYPE_AIRFAN,
	TRAP_TYPE_POWER,
	TRAP_TYPE_CPU,
	TRAP_TYPE_DROP,
	TRAP_TYPE_OLP,
};

typedef struct snmp_settings_s
{
    snmp_param_e index;
    char *  name;
}snmp_settings_t;

typedef struct {
    int passive; // 1:passive; 0:active
    int my_slot_id;
    board_type_t my_board_type;
    char my_board_version[MAX_STRING_LEN];
    char my_board_serial_num[MAX_STRING_LEN];
    rio_type_t my_rio_type;
    platform_t platform;	
    int opmode;
    int chassis_id;
}sys_sw_init_t;

typedef struct {
    int wk_id;
    char ip[128];

    board_status_t board_status;
    board_type_t board_type;
    rio_type_t rio_type;
    opmode_t opmode;

    pthread_rwlock_t rwlock; 
    pthread_rwlockattr_t attr;
	unsigned int timeout_count;
}slot_info_t;

typedef struct {
    slot_info_t slot[SLOT_NUMBER_PER_SYSTEM];
    //codevice_con_t *ctl;
}slots_t;

typedef struct {
    int          board_type;   
    int		     rio_type;
    int          module;     // 4:r 5:f
    int          unit;
    unsigned int port_number; 
    unsigned long long port_mask;
    int          port_map[MAX_PORT_NUMBER];
    int          port_xe[MAX_PORT_NUMBER];
} bcm_port_map_t;

typedef struct {
    bcm_port_map_t port_maps[3];
} slot_port_map_t;

typedef struct {
    int No;
    char *str;
} num_str_t;

struct file_md5_t
{
    char file_name[1024];
    char md5_sum[1024];
};

// trunk defines
#define TRUNK_MAX_NUM               1000
#define TRUNK_NAME_MAX_LEN          31
#define TRUNK_INTERFACE_MAX_NUM     256

typedef struct interface_list{
    smp_new_t smp;
    smp_new_t untag;
    unsigned long long pbmp;
	unsigned long long untag_pbmp;
	int pmodid;
    struct interface_list *p_next;
}interface_list_t;

typedef struct {
    unsigned char is_valid;
    unsigned char load_balance;
    unsigned short id;
    char name[TRUNK_NAME_MAX_LEN+1];
    unsigned short default_vlan_id;
	unsigned short rx_vlan_tag;
    unsigned short mac_learn;
    unsigned int interface_num;
    unsigned int refer;
    interface_list_t *p_list;
    int used_by_acl;
    int used_by_l2;
}trunk_t;

typedef struct {
	trunk_t *trunk;
	int n_trunk;
}oe_trunk_t;

// VLAN defines
#define VLAN_MAX_NUM      4095

typedef struct trunk_list{
	int trunk_id;
	int untag;
	struct trunk_list *p_next;
}trunk_list_t;

typedef struct {
	unsigned char is_valid;
	unsigned int vlan_id;
	unsigned int interface_num;
	interface_list_t *p_list;
	trunk_list_t *p_trunk;
	unsigned int trunk_num;
}vlan_t;

#define DEV_NAME_MAX_LEN  63
typedef struct {
    unsigned char is_valid;
    unsigned char res;
    unsigned short vlan_id;
    unsigned int intf_id;
    unsigned int ip_addr;
    unsigned int mask;
    unsigned char mac[8];
    char dev_name[DEV_NAME_MAX_LEN+1];
}vlanif_t;

typedef struct {
    int enable; /*0: disable; 1: enable   default:enable*/
    int state[2]; /*0: not exist;  1: not active;  2:active*/
    pthread_rwlock_t rwlock[2]; 
    pthread_rwlockattr_t attr[2];
} master_slave_t;

typedef struct {
    int dev_id;
    int link_id;
} olp_id_t;

typedef struct {
    char name[128];
    int device;
    int ip;
    int port;
} olp_property_t;

struct if_counter_t
{
    uint64_t rx_rate;
    uint64_t tx_rate;
    uint64_t rx_fcs_rate;
    uint64_t tx_fcs_rate;
    uint64_t rx_drop_rate;
    uint64_t tx_drop_rate;
};

typedef struct bcm_pbmp_s
{
    bcm_port_map_t bpmap;
    int valid;
}bcm_pbmp_t;

static inline char *GetString(
    num_str_t * table,
    int No,
    int size)
{
    int i = 0;

    while ((i < size) && (table[i].str != NULL)) {
        if (table[i].No == No)
            return table[i].str;
        i++;
    }
    return "INVALID PARAMETER";

}

static inline int GetNo(
    num_str_t * table,
    char *str,
    int size)
{
    int i = 0;

    while (i < size) {
        if (!strcmp(table[i].str, str))
            return table[i].No;
        i++;
    }
    return INVALID_TABLE_NUMBER;

}

enum {
    INNER_VLAN_REPLACE,
    OUTER_VLAN_REPLACE,
};

struct Logo_B {
    char logo[MAX_STRING_LEN];
    char inc_name[MAX_STRING_LEN];
    char soft_name[MAX_STRING_LEN];
    char copyright_year[MAX_STRING_LEN];
    char cli_prompt[MAX_STRING_LEN];
};

struct Board_name {
    char board_userdef_name[BOARD_TYPE_MAX+1][MAX_STRING_LEN];
};

struct snmp_trap_threshold_t {
    int cpu_value;
    int mem_value;
    int fcs_value;
    int drop_value;
};

typedef struct
{
    unsigned long long ports[4];
}ports_mask;
#endif
