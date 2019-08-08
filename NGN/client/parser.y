%{

#define YYERROR_VERBOSE
#include <stdio.h>
#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

//#include "utils/log.h"

#include "my_parser.h"
#include "my_lexer.h"
#include "appdefs.h"
//#include "def.h"
//#include "rpc_def.h"
//#include "log_def.h"
#include "app.h"
#include "string_buf.h"
#include "error_code.h"
#include "look_up.h"
#include "cli_trans.h"
#include "exp_def.h"

#define printPrompt()   do {                        \
    if (load_config_end && !hy_is_interactive()) {  \
        printf("\004%3d\n", EN);                    \
    }                                               \
    fflush(stdout);                                 \
} while (0)

#define printEnq(str)   do {                        \
    if (hy_is_interactive()) {                      \
        printf(str);                                \
    } else {                                         \
        printf("\005%s\n", str);                    \
    }                                               \
    fflush(stdout);                                 \
} while (0)

#define CHECK_SMP_VALID(slots, modules, ports) do {     \
    if (slots == 0 || modules == 0 || ports == 0 || ports >= (1ull << MAX_PORT_NUMBER)) {     \
        EN = OE_RST_INVLD_PRM;                          \
        YYERROR;                                        \
    }                                                   \
} while (0)

#define PROMPT(str)  "   " str

#define help_print(str) __printf_buf("    %s",str)

static int EN = 0;

int end_of_line = 0;
int load_config_end = 0;
int g_my_slot;

#if 0
static int g_log_level_mask_init = 0;
extern struct log_api g_log;
extern int g_load_file_flag;
struct ip_addr_t g_ip_addr;
struct outports_id_list_t g_hash_list;
struct outports_id_list_t g_mirror_list;
olp_property_t g_olp_property;
#endif
char g_description[128];
char g_name[128];

string_buf g_sb;
int in_list = 0;
int in_map = 0;
int in_pair = 0;
int int_key = 0;
unsigned long long slot_mask  = 0x3fff;
int opcode = 0;

#if 0
extern struct oe_login_info_t g_login_info;
#endif

int yyerror(char const *s)
{
    EN = OE_RST_SYNTAX_ERR;

//TODO by caicai
#if 0
    if (is_master_om_board() != 0)  
        return 0;

    if(NULL==s)
    {
        __printf_buf("\n");
        __printf_rv(EN);
        return 0;    
    }

    __printf_buf("%s\n", s);
    __printf_rv(EN);
#else
	fprintf(stderr, "%s\n", s);
#endif
    return 0;
}

static void cli_reset_command(void);


%}

%token A9 AB ABNORMAL AC1200 ACCESS ACK ACL ADD AES AGING_TIME ALL APPLICATION AUTH AUTO AUTONEG AVAILABLE
%token B BACKPLANE BOARD BY
%token CHANGE_TTL CHASSIS CLEAR CONCATENATE CONFIGURATION CONNECT COUNTER CPU CREATE CROSS CURRENT
%token D DATETIME DEFAULT DEFAULT_PVLAN DELETE DES DESCRIPT DESTROY DEVICE DIP DISABLE DISCONNECT DMAC DPORT DROP DST_IP DST_MAC DUAL_DUAL_STAR DUAL_STAR DUPLEX DYNAMIC
%token ENABLE ENTRY EOS ERROR ETHER ETHERTYPE EXIT EXITSHELL EXPORT
%token F FAILOVER FCS FCS_RATE FIN FIVE_TUPLE FORCE FPGA FR_16XE_FB_11QXE_2DXE FR_20XE_FB_9QXE_4DXE FR_24XE_FB_8QXE_4DXE FROM FTP FULL FW
%token GATEWAY GLOBAL GRULE
%token HALF HASH HEAD HEARTBEAT HELP HOST HOSTIP
%token ICMP ID IMPORT INFO INFOSHOW INLINE INNERVLAN INPORTS INSTALL INT INTERFACE INTERVAL INVERT IP IP_ROUTE_STATIC IPCONFIG IPV6 IPV6_ROUTE_STATIC
%token L2 L2CMDS L3 L3CMDS L3PACKETLEN L4 LEARNING LINE LINK LINK_PROTECT LIST LOAD_BALANCE LOCAL LOG LOOP LOOPBACK
%token M MAC MAC_ADDRESS MAC_IGNORE MASK MAX MC MD5 MEMORY MIN MIRROR MODE MODE_ROUTE MODULE MONITOR
%token NAME NO NO_SHUTDOWN NO_VLAN NOAUTH NONE NORMAL
%token OFF OFFSET OLP OM ON OPMODE OUTERVLAN OUTPORTS OUTPUT_INTERFACE OVERLOAD
%token PASSWORD PAUSE PERCENT PLATFORM PORT PORTMAP PRIV PROTOCOL PSH PVLAN
%token QUIET
%token R RATE RCLOAD RECEIVE REPLACE RESERVE RESET RESTART RESTORE ROCOMMUNITY ROUTE_STATIC ROUTING_TABLE RST RULE RULESET RWCOMMUNITY RX RX_FCS RX_FCS_RATE RX_RATE RX_TX_PERCENT RX_VLANTAG RXTX
%token S S320 S40 SAVE SCAN_FREQUENCY SELECT SEQUENCE SERVER SET SHA SHOW SHUTDOWN SINGLE_FIBER SIP SIZE SLOT SMAC SMP SNMP SOC SOC SPEED SPORT SRC_DST_IP SRC_DST_MAC SRC_IP SRC_MAC ST START STATIC STATUS STOP STRING SYN SYNCONFIG SYSCONTACT SYSLOCATION SYSNAME SYSTEM
%token TCP TCPFLAG THRESHOLD TIMEOUT TIMEZONE TO TRACE TRAFFIC TRANSCEIVING TRAP TRUNK TX TX_FCS TX_FCS_RATE TX_RATE TX_RX_PERCENT TYPE
%token UD0 UD1 UD2 UD3 UD4 UD5 UD6 UD7 UDP UNDO UNINSTALL UNTAG UP URG URL USERNAME
%token VALUE VERBOSE VERSION VLAN VLAN_FILTER VLAN_MAPPING VLAN_S VLANMASK
%token WARM WARN WEIGHT WEIGHT

%union {
    char *sptr;
    void *vptr;
    unsigned int ipaddr;
    unsigned short ipv6addr[8];
    unsigned long long ival;
    unsigned char mac[6];
    char time[12];
    struct smps_s{
        unsigned long long s;
        unsigned long long m;
        unsigned long long p[4];
    } smps;
    struct mps_s{
        unsigned long long m;
        unsigned long long p[4];
    } mps;
} 

%token <ival> _INT_
%token <sptr> _NAME_ _STRING_ _URL_ _PWD_STRING_
%token <ipaddr> _IPADDR_
%token <ipv6addr> _IPV6ADDR_
%token <mac> _MAC_
%token <time> _TIME_FORMAT_
%token <smps> _SMPS_
%token <mps> _MPS_

%type <ival> command infocmds ifcmds trunkcmds l2cmds rulecmds syscmds helpcmds snmpcmds logcmds vermgrcmds olpcmds

%type <ival> able agingtime 
%type <ival> ce ce_dip ce_dmac ce_dport ce_ethertype ce_protocol ce_sip ce_smac ce_sport ce_tcpflag class_entry ce_innervlan ce_userdata ce_range_outervlan ce_range_l3packetlen
%type <ival> duplex
%type <ival> entry_id
%type <ival> fpga_type
%type <ival> hash_outports_id_attr hash_outports_id_list
%type <ival> ip inports_id if_type
%type <ival> log_enable log_config log_show log_enable_level log_level log_lines link_protect_type link_protect_types
%type <ival> onoff olp_propertys olp_property olp_device op_mode outports_id outports_type
%type <ival> platform psc pwd_str
%type <ival> rule_id rxtx_mode
%type <ival> shutdown slot_id snmp_op snmp_param snmp_auth_type snmp_priv_type snmp_access slot_info description sys_name
%type <ival> tcpflag tcpflags trap_type
%type <ival> ud_id ud_mode ud_offset
%type <ival> verbose vlan_id vlan_action vlan_mask
%type <ival> ip_addr mask 
%type <ival> quiet 

%type <vptr> mac_addr smps olpid 

%left ':'
%left '-'
%left ','
%left '/'

%%
input:
    |  input line
    {
        if (end_of_line) {
            printPrompt();
        }
    }
    ;

line:
    EOS
    {
        cli_reset_command();
    }
    |  command
    {
        cli_reset_command();
    }
    | error EOS
    {
        cli_reset_command();
        yyerrok;
        EN = -1;
    }
    ;

command:
    infocmds
    |   ifcmds
    |   trunkcmds
    |   l2cmds
    |   rulecmds
    |   syscmds
    |   helpcmds
    |   snmpcmds
    |   logcmds
    |   vermgrcmds
    |   olpcmds
    |   EXIT EOS
    {
        YYACCEPT;
    }
    ;


infocmds:
    SHOW CONFIGURATION EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_CONFIG);
    }
    |   SHOW SYSTEM EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_SYSTEM);
    }
    |   SHOW BOARD INFO EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_BOARDINFO);
    }
    |   SHOW BOARD _INT_ INFO EOS
    {
		convert_val_to_json($3);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_BOARDINFO);
    }
    |   SHOW BOARD STATUS EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_BOARDSTATUS);
    }
    /*|   BOARD TYPE EOS
    {
        EN = oe_get_my_board_type();
    }
    |   SLOT _INT_ TYPE _INT_ EOS
    {
        EN = oe_set_slot_board_type($2, $4);
    }
    |   SLOT _INT_ STATUS _INT_ EOS
    {
        EN = oe_set_slot_board_stat($2, $4);
    }
    |   SHOW SLOT EOS
    {
    	EN = oe_show_slot();
    }
    | SLOT _INT_ MODULE _INT_ PORTMAP EOS
    {
    	EN = oe_get_slot_port_map($2, $4);
    }
    |SET SYSTEM NAME sys_name
    {
       EN = oe_set_system_name(g_name);
    }*/    
  ;
    
ifcmds:
    SET INTERFACE smps AUTONEG onoff EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_AUTONEG);
    }
    |   SET INTERFACE smps PAUSE onoff EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_PAUSE);
    }
    |   SET INTERFACE smps shutdown EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_SHUTDOWN);
    }
    |   SET INTERFACE smps FORCE UP able EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_SINGLEFIBER_TRANSMIT);
    }
    |   SET INTERFACE smps DUPLEX duplex EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_DUPLEX);
    }
    |   SET INTERFACE smps SPEED _INT_ EOS
    {
        if ($5 != 1000 && $5 != 10000) {
            //__printf_buf("the speed should be 1000|10000 Mbps.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_val_to_json($5);
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_SPEED);
        }
    }
    /*|   SET INTERFACE smps MAC_ADDRESS LEARNING onoff EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, );
    }
    |   SET INTERFACE smps MODE_ROUTE onoff EOS
    {
        EN = 0;
        free($3);
    }
    |   SET INTERFACE smps MAC_IGNORE onoff EOS
    {
        EN = 0;
        free($3);
    }*/
    |   SHOW INTERFACE smps INFO EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_GET_INFO);
    }
    |   SHOW INTERFACE smps STATUS EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_GET_STATUS);
    }
    |   SHOW INTERFACE smps COUNTER verbose EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_SHOW_COUNTER);
    }
    |   CLEAR INTERFACE smps COUNTER EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_CLEAR_COUNTER);
    }
    |   SET INTERFACE smps MODE if_type EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_SET_PORTMODE);
    }

trunkcmds:
    ADD INPORTS inports_id smps EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_INPORTS);
    }
    |   DELETE INPORTS inports_id smps EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_INPORTS);
    }
    |   DELETE INPORTS inports_id ALL EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_INPORTS_ALL);
    }
    |   ADD OUTPORTS outports_id outports_type smps EOS
    {
		convert_val_to_json(1);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_OUTPORTS);
    }
    |   ADD OUTPORTS outports_id outports_type smps WEIGHT _INT_ EOS
    {
        if(($7<2)||($7>128))
        {
            EN=OE_RST_INVLD_PRM;
            YYERROR;
        }
		convert_val_to_json($7);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_OUTPORTS);
    }

    |   DELETE OUTPORTS outports_id smps EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_OUTPORTS);
    }
    |   DELETE OUTPORTS outports_id ALL EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_OUTPORTS_ALL);
    }
    |   SET OUTPORTS outports_id LOAD_BALANCE psc EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_PSC);
    }
    |   SET OUTPORTS GLOBAL LOAD_BALANCE psc EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_GLOBLE_LOAD_BALANCE);
    }
	|	SET FAILOVER able EOS
	{
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_FAILOVER);
	}
    |   SET FAILOVER INTERVAL _INT_ EOS
    {
		convert_val_to_json($4);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_FAILOVER_INTERVAL);
    }
    |   SHOW OUTPORTS outports_id EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_OUTPORTS);
    }
    |   SHOW OUTPORTS ALL EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_OUTPORTS_ALL);
    }
    |   SHOW INPORTS outports_id EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_INPORTS);
    }
    |   SHOW INPORTS ALL EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_INPORTS_ALL);
    }
    |   SET INPORTS inports_id PVLAN able EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_INPORTS_PVLAN_ENABLE);
    }
    |   SET INPORTS inports_id PVLAN ID vlan_id EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_INPORTS_PVID);
    }
    /*|   SET INPORTS inports_id PVLAN ID AUTO EOS
    {
        EN = oe_set_inports_rx_vlantag_id($3, 0);
    }
    |   SET INPORTS RX_VLANTAG vlan_action EOS
    {
        EN = oe_set_inports_rx_vlantag_action(0, $4);
    }
	|   SET SLOT slot_id INPORTS RX_VLANTAG vlan_action EOS
    {
        EN = oe_set_inports_rx_vlantag_action($3, $6);
    }*/
    |   SET OUTPORTS outports_id VLAN UNTAG able EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_OUTPORTS_UNTAG_ENABLE);
    }
    /*|   SET LOAD_BALANCE PORT able EOS
    {
        EN = oe_set_load_balance_port_able(0, $4);
    }
	|   SET SLOT slot_id LOAD_BALANCE PORT able EOS
    {
        EN = oe_set_load_balance_port_able($3, $6);
    }*/
;

vlan_id:
    _INT_
    {
        if ($1 < 2 || $1 >= 4096) {
            //__printf_buf("Vlan id should be from 2 to 4095\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        $$ = $1;
		convert_val_to_json($$);
    }

vlan_mask:
    _INT_
    {
         if ($1 < 0 || $1 >= 4096) {
            //__printf_buf("Vlan mask should be from 0 to 4095\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        $$ = $1;
    }

vlan_action:
    RESERVE
    {
        $$ = E_OUTERVLAN_RESERVE;
    }
    |   REPLACE
    {
        $$ = E_OUTERVLAN_REPLACE;
    }

inports_id:
    _INT_
    {
        if($1 < 1 || $1 > MAX_INPORTS_ID )
        {
            //__printf_buf("Inport id should be from 1 to %d\n", MAX_INPORTS_ID);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        else{
            $$ = $1;
		}
		convert_val_to_json($$);
    }
    ;
    
outports_type:
    HASH
    {
        $$=OUTPORT_HASH;
		convert_val_to_json($$);
    }
    | MC
    {
        $$=OUTPORT_MC;
		convert_val_to_json($$);
    }

outports_id:
    _INT_
    {
        if($1 < 1 || $1 > MAX_OUTPORTS_ID)
        {
            //__printf_buf("Outport id should be from 1 to %d\n", MAX_OUTPORTS_ID);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        else{
		
            $$ = $1;
		}
		convert_val_to_json($$);
    }
    ;
 
l2cmds:
    /*ADD MAC STATIC mac_addr INPORTS inports_id OUTPORTS outports_id EOS
    {
        EN = oe_l2_mac_add($4, $6, $8);
        free($4);
    }
    |   DELETE MAC STATIC mac_addr INPORTS inports_id EOS
    {
        EN = oe_l2_mac_del($4, $6);
        free($4);
    }
    |   DELETE MAC STATIC ALL EOS
    {
        EN = oe_l2_mac_del_all();
    }
    |   SHOW MAC STATIC INPORTS inports_id verbose EOS
    {
        EN = oe_l2_mac_get(NULL, $5, 0, $6, 0x2);
    }
    |   SHOW MAC STATIC OUTPORTS outports_id verbose EOS
    {
        EN = oe_l2_mac_get(NULL, 0, $5, $6, 0x4);
    }
    |   SHOW MAC STATIC mac_addr verbose EOS
    {
        EN = oe_l2_mac_get($4, 0, 0, $5, 0x1);
        free($4);
    }
    |   SHOW MAC STATIC verbose EOS
    {
        EN = oe_l2_mac_get(NULL, 0, 0, $4, 0);
    }
    |   SHOW MAC DYNAMIC EOS
    {
        EN = oe_l2_mac_dy_get(NULL, 0, 0, 0);
    }
    |   SHOW MAC DYNAMIC mac_addr EOS
    {
        EN = oe_l2_mac_dy_get($4, 0, 0, 0x1);
        free($4);
    }
    |   SHOW MAC DYNAMIC INPORTS inports_id EOS
    {
        EN = oe_l2_mac_dy_get(NULL, $5, 0, 0x2);
    }
    |   SHOW MAC DYNAMIC OUTPORTS outports_id EOS
    {
        EN = oe_l2_mac_dy_get(NULL, 0, $5, 0x4);
    }
    |   DELETE MAC DYNAMIC mac_addr EOS
    {
        EN = oe_l2_mac_dy_del($4);
        free($4);
    }
    |   DELETE MAC DYNAMIC ALL EOS
    {
        EN = oe_l2_mac_dy_del_all();
    }
    |   MAC AGING_TIME agingtime EOS
    {
        EN = oe_l2_mac_aging_time($3);
    }
    |   SHOW MAC AGING_TIME EOS
    {
        EN = oe_l2_mac_aging_time_show();
    }*/
;

rulecmds:
    /*SET ud_id MODE ud_mode OFFSET ud_offset EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_RULE_UD);
    }
    |   ADD RULE rule_id ALL EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_RULE_ALL);
    }

    |   ADD RULE rule_id class_entry EOS
    {
        EN = oe_add_rule_class_entry($3, &g_class_entry);
    }
    |   DELETE RULE rule_id ENTRY entry_id EOS
    {
        EN = oe_del_rule_entry_id($3, $5, $5);
    }
    |   DELETE RULE rule_id ENTRY entry_id '-' entry_id EOS
    {
        EN = oe_del_rule_entry_id($3, $5, $7);
    }
    |   DELETE RULE rule_id ENTRY ALL EOS
    {
        EN = oe_del_rule_entry_id_all($3);
    }
    |   DELETE RULE _INT_ EOS
    {
        EN = oe_del_rule_id($3);
    }
    |   DELETE RULE _INT_ '-' _INT_ EOS
    {
        int i=0;
        if(($3<1)||($3>$5)||($5>2048))
        {
            EN=OE_RST_INVLD_PRM;
            YYERROR;
        }
        for(i=$3; i<=$5; i++)
        {
            EN=oe_del_rule_id(i);
        }
      
    }
    |   SHOW RULE rule_id EOS
    {
        EN = oe_show_rule($3);
    }
    |   SET RULE rule_id INPORTS inports_id FW DROP EOS
    {
        EN = oe_set_rule($3, $5 ,NULL,0,0); 
    }
    |   SET RULE rule_id INPORTS inports_id FW DROP OUTERVLAN vlan_id EOS
    {
        EN = oe_set_rule($3, $5 ,NULL,$9,0); 
    }
    |   SET RULE rule_id INPORTS inports_id FW DROP OUTERVLAN vlan_id vlan_id EOS
    {
        EN = oe_set_rule($3, $5 ,NULL,$9,$10); 
    }    
    |   SET RULE rule_id INPORTS inports_id FW OUTPORTS hash_outports_id_list EOS
    {
        EN = oe_set_rule($3, $5 ,&g_hash_list,0,0); 
    }
    |   SET RULE rule_id INPORTS inports_id FW OUTPORTS hash_outports_id_list OUTERVLAN vlan_id EOS
    {
        EN = oe_set_rule($3, $5 ,&g_hash_list,$10,0); 
    }
    |   SET RULE rule_id INPORTS inports_id FW OUTPORTS hash_outports_id_list OUTERVLAN vlan_id vlan_id EOS
    {
        EN = oe_set_rule($3, $5 ,&g_hash_list,$10,$11); 
    }
    |   RULE CLEAR EOS
    {
        EN = oe_rule_clear();
    }
    |   SET INTERFACE smps MIRROR TO NONE EOS
    {
        EN = oe_set_interface_mirror($3, NULL);
        free($3);
    }
    |   SET INTERFACE smps MIRROR TO smps EOS
    {
        EN = oe_set_interface_mirror($3, $6);
        free($3);
        free($6);
    }
    |   SET INTERFACE smps VLAN_S vlan_id VLANMASK vlan_mask REPLACE BY vlan_id EOS
    {
        EN = oe_set_interface_vlan_change($3, $5, $7, $10, INNER_VLAN_REPLACE);
        free($3);
    }
    |   SET INTERFACE smps OUTERVLAN vlan_id VLANMASK vlan_mask REPLACE BY vlan_id EOS
    {
        EN = oe_set_interface_vlan_change($3, $5, $7,$10,OUTER_VLAN_REPLACE);
        free($3);
    }
    | SHOW RULE rule_id COUNTER EOS
    {
    	EN = oe_show_rule_counter($3, -1);
    }
    | SHOW RULE rule_id ENTRY _INT_ COUNTER EOS
    {
    	EN = oe_show_rule_counter($3, $5);
    }
    | CLEAR RULE rule_id COUNTER EOS
    {
    	EN = oe_clear_rule_counter($3, -1);
    }
    | CLEAR RULE rule_id ENTRY _INT_ COUNTER EOS
    {
    	EN = oe_clear_rule_counter($3, $5);
    }
    | SOC INSTALL EOS
    {
        EN = oe_soc_install();
    }
    | SOC UNINSTALL EOS
    {
        EN = oe_soc_uninstall();
    }*/
;

syscmds:
    RESET SYSTEM quiet EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_RESET_SYSTEM);
    }
    |   RESTORE CONFIGURATION DEFAULT quiet EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_RESTORE_CONFIG);
    }
    |   SAVE CONFIGURATION EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_SAVE_CONFIG);
        if (EN == 0) {
            printf("save configuration success.\n");
        } else {
            printf("save configuration error.\n");
        }
    }
	|   CLEAR CONFIGURATION EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_CLEAR_CONFIG);
    }
    |   IMPORT CONFIGURATION FTP ip _STRING_ _STRING_ _STRING_ EOS
    {
        if (strlen($5) == 0 || strlen($6) == 0 || strlen($7) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json($5);
			convert_spec_str_to_json($6);
			convert_spec_str_to_json($7);
			convert_spec_str_to_json(".");
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_IMPORT_CONFIG);
        }
    }
    |   IMPORT CONFIGURATION FTP ip _STRING_ _STRING_ _STRING_ _STRING_ EOS
    {
        if (strlen($5) == 0 || strlen($6) == 0 || strlen($7) == 0 || strlen($8) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json($5);
			convert_spec_str_to_json($6);
			convert_spec_str_to_json($7);
			convert_spec_str_to_json($8);
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_IMPORT_CONFIG);
        }
    }
    |   EXPORT CONFIGURATION FTP ip _STRING_ _STRING_ _STRING_ EOS
    {
        if (strlen($5) == 0 || strlen($6) == 0 || strlen($7) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json($5);
			convert_spec_str_to_json($6);
			convert_spec_str_to_json($7);
			convert_spec_str_to_json(".");
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_EXPORT_CONFIG);
        }
    }
    |   EXPORT CONFIGURATION FTP ip _STRING_ _STRING_ _STRING_ _STRING_ EOS
    {
        if (strlen($5) == 0 || strlen($6) == 0 || strlen($7) == 0 || strlen($8) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json($5);
			convert_spec_str_to_json($6);
			convert_spec_str_to_json($7);
			convert_spec_str_to_json($8);
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_EXPORT_CONFIG);
        }
    }
    |   SYSNAME _STRING_ EOS
    {
		convert_spec_str_to_json($2);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_SYSNAME);
    }
    |    DATETIME EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_DATETIME);
    }
    |    DATETIME slot_id _STRING_ EOS
    {
		convert_spec_str_to_json($3);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_DATEIIME);
    }
	|    TIMEZONE slot_id _STRING_ EOS
    {
		convert_spec_str_to_json($3);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_TIMEZONE);
    }
    |   RESET BOARD slot_id EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_RESET_BOARD);
    }
    |   EXITSHELL EOS
    {
        EN = 0;
    }
    |   IPCONFIG ip_addr mask EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_IPCONFIG);
    }
    |   GATEWAY ip_addr EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GATEWAY);
    }
    /*|   SET SLOT slot_id OPMODE op_mode EOS
    {
        EN = oe_opmode_set($3, $5);
    }
    |   RCLOAD slot_id _STRING_ EOS
    {
        EN = oe_rcload_bcm_config($2, $3);
    }
    |   SET CHASSIS ID _INT_ EOS
    {
        EN = oe_set_chassis_id($4);
    }
	|	SET SLOT slot_id AC1200 BACKPLANE RXTX MODE rxtx_mode EOS
	{
		EN = oe_set_ac1200_backplane_rxtx_mode($3, $8);
	}
    |   WARM SYNCONFIG able EOS
    {
        EN = oe_set_warm_sys_enable($3);
    }
    |   DEBUG ACL RULE _INT_ EOS
    {
        EN = oe_debug_acl_g_rule($4);
    }
    |   SET CHASSIS MODE DUAL_STAR EOS
    {
        EN = oe_set_chassis_mode(0);
    }
    |  SET CHASSIS MODE DUAL_DUAL_STAR EOS
    {
        EN = oe_set_chassis_mode(1);
    }*/
    | PASSWORD pwd_str EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_PASSWORD);
    }
    ;

helpcmds:
/*
     HELP EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("To get more specific help use one of the following\n");
            help_print("help commands below:\n\n");
            help_print("HELP INFOSHOW\n");
            help_print("HELP INTERFACE\n");
            help_print("HELP TRUNK\n");
            help_print("HELP L2CMDS\n");
            help_print("HELP RULE\n");

            help_print("HELP SYSTEM\n");
            help_print("HELP SNMP\n");
            help_print("HELP LOG\n");
            help_print("HELP VERMGR\n");
            help_print("HELP LINK_PROTECT\n");
            help_print("\n");
        }
    }
    |    HELP INFOSHOW EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("SHOW CONFIGURATION \n");
            help_print("SHOW SYSTEM \n");
            help_print("SHOW BOARD INFO \n");
            help_print("SHOW BOARD <slot_id> INFO \n");
            help_print("SHOW BOARD STATUS \n");
        }
    }
  
    |    HELP INTERFACE EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("INTERFACE related commands:\n\n");
            help_print("HELP SET INTERFACE ...\n");
            help_print("HELP SHOW INTERFACE ...\n");
            help_print("HELP CLEAR INTERFACE ...\n");
            help_print("To get more specific help type 'HELP <command>',\n");
            help_print("with one of the commands above.\n");
            help_print("\n");
        }
    }

    |    HELP SET INTERFACE EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Set the auto-negotiation of port:\n");
            help_print("SET INTERFACE <interfacelist> AUTONEG {ON|OFF}\n\n");

            help_print("Set pause of port:\n");
            help_print("SET INTERFACE <interfacelist> PAUSE {ON|OFF}\n\n");

            help_print("Set shutdown of port:\n");
            help_print("SET INTERFACE <interfacelist> {SHUTDOWN|NO_SHUTDOWN}\n\n");

            help_print("Set duplex of port:\n");
            help_print("SET INTERFACE <interfacelist> DUPLEX {FULL|HALF}\n\n");

            help_print("Set speed of port:\n");
            help_print("SET INTERFACE <interfacelist> SPEED {100|1000|10000}\n\n");

            help_print("Set mac_address learning of port:\n");
            help_print("SET INTERFACE <interfacelist> MAC_ADDRESS LEARNING {ON|OFF}\n\n");

            help_print("Set type of port:\n");
            help_print("SET INTERFACE <interfacelist> MODE {LOOPBACK|NORMAL}\n\n");
        }
    }
    |    HELP SHOW INTERFACE EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Show the status of port.\n");
            help_print("SHOW INTERFACE <interfacelist> STATUS\n\n");

            help_print("Show the counters of port.\n");
            help_print("SHOW INTERFACE <interfacelist> COUNTER [VERBOSE]\n\n");

            help_print("Show the information of port\n");
            help_print("SHOW INTREFACE <interfacelist> INFO\n\n");
        }
    }
    |    HELP CLEAR INTERFACE EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Clear interface counters:\n");
            help_print("CLEAR INTERFACE <interfacelist> COUNTER\n\n");
        }
    }

    |    HELP TRUNK EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("TRUNK related commands:\n\n");
            help_print("HELP INPORTS...\n");
            help_print("HELP OUTPORTS...\n");
            help_print("To get more specific help type 'HELP <command>',\n");
            help_print("with one of the commands above.\n");
            help_print("\n");
        }
    }

    |   HELP INPORTS EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Add inports:\n");
            help_print("ADD INPORTS <inports_id> <interface>\n\n");

            help_print("Delete inports:\n");
            help_print("DELETE INPORTS <inports_id> <interface>\n\n");

            help_print("Delete inports all:\n");
            help_print("DELETE INPORTS <inports_id> ALL\n\n");

            help_print("Show inports:\n");
            help_print("SHOW INPORTS <inports_id>\n\n");

            help_print("Show inports all:\n");
            help_print("SHOW INPORTS ALL\n\n");
        }
    }

    |   HELP OUTPORTS EOS

    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Add outports:\n");
            help_print("ADD OUTPORTS <inports_id> <interface>\n\n");

            help_print("Delete outports:\n");
            help_print("DELETE OUTPORTS <inports_id> <interface>\n\n");

            help_print("Delete outports all:\n");
            help_print("DELETE OUTPORTS <inports_id> ALL\n\n");

            help_print("Set LoadBalance:\n");
            help_print("SET OUTPORTS <outports_id> LOAD_BALANCE <psc>\n\n");

            help_print("Set Global LoadBalance:\n");
            help_print("SET OUTPORTS GLOBAL LOAD_BALANCE <psc>\n\n");

            help_print("Set Failover:\n");
            help_print("SET FAILOVER <able>\n\n");

            help_print("Show outports:\n");
            help_print("SHOW OUTPORTS <inports_id>\n\n");

            help_print("Show outports all:\n");
            help_print("SHOW OUTPORTS ALL\n\n");
        }
    }

    |    HELP L2CMDS EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("L2CMDS related commands:\n\n");
            help_print("HELP SET L2CMDS...\n");
            help_print("HELP SHOW L2CMDS...\n");
            help_print("To get more specific help type 'HELP <command>',\n");
            help_print("with one of the commands above.\n");
            help_print("\n");
        }
    }

    |   HELP SET L2CMDS EOS 
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Set :\n");
            help_print("ADD MAC STATIC <mac_addr> INPORTS <inports_id> OUTPORTS <outports_id>\n\n");

            help_print("Set :\n");
            help_print("DELETE MAC STATIC <mac_addr> INPORTS <inports_id>\n\n");

            help_print("Set :\n");
            help_print("DELETE MAC STATIC ALL\n\n");
       
            help_print("Set :\n");
            help_print("DELETE MAC DYNAMIC <mac_addr>\n\n");

            help_print("Set :\n");
            help_print("DELETE MAC DYNAMIC ALL\n\n");

            help_print("Set :\n");
            help_print("MAC AGING_TIME <agingtime>\n\n");
        }

    }

    |   HELP SHOW L2CMDS EOS
    {

        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Show :\n");
            help_print("SHOW MAC STATIC <verbose>\n\n");

            help_print("Show :\n");
            help_print("SHOW MAC STATIC INPORTS <inports_id> <verbose>\n\n");

            help_print("Show :\n");
            help_print("SHOW MAC STATIC OUTPORTS <outports_id> <verbose>\n\n");

            help_print("Show :\n");
            help_print("SHOW MAC STATIC <mac_addr> <verbose>\n\n");

            help_print("Show :\n");
            help_print("SHOW MAC DYNAMIC\n\n");

            help_print("Show :\n");
            help_print("SHOW MAC DYNAMIC <mac_addr>\n\n");
            
            help_print("Show :\n");
            help_print("SHOW MAC DYNAMIC INPORTS <inports_id>\n\n");
            
            help_print("Show :\n");
            help_print("SHOW MAC DYNAMIC OUTPORTS <outports_id>\n\n");
            
            help_print("Show :\n");
            help_print("SHOW MAC AGING_TIME\n\n");
        }
    }

    |    HELP RULE EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("RULE related commands:\n\n");
            help_print("HELP SET RULE...\n");
            help_print("HELP SHOW RULE...\n");
            help_print("To get more specific help type 'HELP <command>',\n");
            help_print("with one of the commands above.\n");
            help_print("\n");
        }
    }

    |    HELP SET RULE EOS
    {

        if (is_master_om_board() == 0)
        {
            help_print("\n");

            help_print(" :\n");
            help_print("SET <ud_id> MODE <ud_mode> OFFSET <ud_offset>\n\n"); 

            help_print(" :\n");
            help_print("ADD RULE <rule_id> <class_entry>\n\n"); 

            help_print(" :\n");
            help_print("DELETE RULE <rule_id> <class_entry>\n\n"); 

            help_print(" :\n");
            help_print("DELETE RULE <rule_id> ENTRY <entry_id>\n\n"); 

            help_print(" :\n");
            help_print("DELETE RULE <rule_id> ENTRY <entry_id>-<entry_id>\n\n"); 

            help_print(" :\n");
            help_print("SET RULE <rule_id> INPORTS <inports_id>\n\n"); 

            help_print(" :\n");
            help_print("SET RULE <rule_id> INPORTS NONE\n\n"); 

            help_print(" :\n");
            help_print("SET RULE <rule_id> FW DROP\n\n"); 

            help_print(" :\n");
            help_print("SET RULE <rule_id> FW HASH OUTPORTS <outports_id_list>\n\n"); 

            help_print(" :\n");
            help_print("SET RULE <rule_id> FW MC OUTPORTS <outports_id_list>\n\n"); 

            help_print(" :\n");
            help_print("SET RULE <rule_id> FW HASH OUTPORTS <outports_id_list> MC OUTPORTS <outports_id_list>\n\n"); 

            help_print(" :\n");
            help_print("RULE CLEAR\n\n"); 
        }
    }
    |    HELP SHOW RULE EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");

            help_print(" :\n");
            help_print("SHOW RULE <rule_id>\n\n");
        }
    }
    |    HELP SYSTEM EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Show information about the system:\n");
            help_print("SHOW SYSTEM\n\n");

            help_print("Modify the device name:\n");
            help_print("SYSNAME <sysname>\n\n");

            help_print("Show or set the current system time:\n");
            help_print("DATETIME <slot_id> [datetime]\n\n");

			help_print("Set the current timezone:\n");
            help_print("TIMEZONE <slot_id> [timezone]\n\n");

            help_print("Show information about the board:\n");
            help_print("SHOW BOARD [slot_id] INFO\n\n");

            help_print("Show board status in current chassis:\n");
            help_print("SHOW BOARD STATUS\n\n");

            help_print("Restart the board:\n");
            help_print("RESET BOARD <slot_id>\n\n");

            help_print("Restart the system:\n");
            help_print("RESET SYSTEM [QUIET]\n\n");

            help_print("Restore configuration:\n");
            help_print("RESTORE CONFIGURATION DEFAULT [QUIET]\n\n");

            help_print("Set platform:\n");
            help_print("SET PLATFORM <slot-id> {S40/ST | S320/OM}\n\n");

            help_print("Set Ip:\n");
            help_print("IPCONFIG <ip-addr> <mask>\n\n");

            help_print("Set Gateway:\n");
            help_print("GATEWAY <ip-addr>\n\n");

            help_print("Set SLOT OPMODE:\n");
            help_print("Set Slot <slot-id> Opmode <opmode>\n\n");

            help_print("LOAD RCFILE:\n");
            help_print("Rcload <slot_id> <file>\n\n");

            help_print("IMPORT CONFIGURATION:\n");
            help_print("Import Configuration Ftp <ip-addr> <username> <passwd> <file> [file-path]\n\n");

            help_print("EXPORT CONFIGURATION:\n");
            help_print("Export Configuration Ftp <ip-addr> <username> <passwd> <file> [file-path]\n\n");

            help_print("SAVE CONFIGURATION:\n");
            help_print("Save Configuration\n\n");

            help_print("Exit the cli:\n");
            help_print("EXIT\n\n");
        }
    }
    |   HELP SNMP EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("SNMP {START|STOP|RESTART}\n");
            help_print("SNMP RESTORE CONFIGURATION\n");
            help_print("SNMP STATUS\n");
            help_print("SNMP SHOW CONFIGURATION\n");
            help_print("SNMP SET...\n");
            help_print("SNMP TRAP {INTERFACE FCS|INTERFACE DROP|CPU OVERLOAD|MEMORY OVERLOAD} THRESHOLD <value>\n");
            help_print("SNMP SHOW TRAP THRESHOLD VALUE\n");
            help_print("To get more specific help type 'HELP <command>',\n");
            help_print("with one of the commands above.\n\n");
        }
    }
    |   HELP SNMP START EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Start the snmp service.\n");
            help_print("SNMP START\n\n");
        }
    }
    |   HELP SNMP RESTART EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Reload the snmp configuration.\n");
            help_print("SNMP RESTART\n\n");
        }
    }
    |   HELP SNMP STOP EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Stop the snmp service.\n");
            help_print("SNMP STOP\n\n");
        }
    }
    |   HELP SNMP STATUS EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Show the real time status of the snmp service.\n");
            help_print("SNMP STATUS\n\n");
        }
    }
    |   HELP SNMP SHOW CONFIGURATION EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Show the configuration of the snmp service.\n");
            help_print("SNMP SHOW CONFIGURATION\n\n");
        }
    }
    |   HELP SNMP SET EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Set snmp configuration. password string's length>=8.\n");
            help_print("SNMP SET SYSNAME \"<string>\"\n");
            help_print("SNMP SET SYSCONTACT \"<string>\"\n");
            help_print("SNMP SET SYSLOCATION \"<string>\"\n");
            help_print("SNMP SET ROCOMMUNITY \"<string>\"\n");
            help_print("SNMP SET RWCOMMUNITY \"<string>\"\n");
            help_print("SNMP ADD TRAP IP <*.*.*.*>\n");
            help_print("SNMP DELETE TRAP IP <*.*.*.*>\n");
            help_print("SNMP SET TRAP VERSION {\"2c\"|\"3\"}\n");
            help_print("SNMP SET USERNAME \"<string>\"\n");
            help_print("SNMP SET AUTH TYPE {MD5|SHA}\n");
            help_print("SNMP SET AUTH PASSWORD \"<string>\"\n");
            help_print("SNMP SET PRIV TYPE {DES|AES}\n");
            help_print("SNMP SET PRIV PASSWORD \"<string>\"\n");
            help_print("SNMP SET ACCESS {NOAUTH|AUTH|PRIV}\n");
        }
    }
    |   HELP SNMP TRAP EOS
    {
    	if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Set snmp trap threshold value.\n");
            help_print("SNMP TRAP INTERFACE FCS THRESHOLD <value>\n");
            help_print("SNMP TRAP INTERFACE DROP THRESHOLD <value>\n");
            help_print("SNMP TRAP CPU OVERLOAD THRESHOLD <value>\n");
            help_print("SNMP TRAP MEMORY OVERLOAD THRESHOLD <value>\n");
        }
    }
    |   HELP LOG EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("LOG related commands:\n\n");
            help_print("LOG SET...\n");
            help_print("LOG SHOW...\n");
            help_print("To get more specific help type 'HELP <command>',\n");
            help_print("with one of the commands above.\n\n");
        }
    }
    |   HELP LOG SET EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Set type of log\n");
            help_print("LOG {ENABLE|DISABLE} {INFO|RULE|ERROR|ALL}\n\n");

            help_print("Set configuration of log\n");
            help_print("LOG HOST LOCAL\n");
            help_print("LOG HOST <ip_address> [port]\n\n");
            help_print("LOG NO HOST <ip_address> [port]\n\n");

            help_print("Clear log record\n");
            help_print("RESET LOG\n");
        }
    }
    |   HELP LOG SHOW EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("\n");
            help_print("Show the latest logs as the number of <lines>:\n");
            help_print("SHOW LOG [line]\n\n");

            help_print("Show a kind of logs:\n");
            help_print("SHOW LOG {INFO|RULE|TRACE|ERROR} [line]\n\n");

            help_print("Show the log of a day:\n");
            help_print("SHOW LOG <yyyy-mm-dd>\n\n");

            help_print("Show the log of a period of time:\n");
            help_print("SHOW LOG FROM <yyyy-mm-dd> TO <yyyy-mm-dd>\n\n");
        }
    }
    |   HELP LINK_PROTECT EOS
    {
        if (is_master_om_board() == 0)
        {
            help_print("link_protect [slot <slot-id>] create <id> [name=<string>] <device=<dev_type>> [ip=<dev_ip>] <port=<port_val>>\n");
            help_print("link_protect [slot <slot-id>] destroy <id>\n"); 
            help_print("link_protect [slot <slot-id>] interface add <id> <smps> \n");
            help_print("link_protect [slot <slot-id>] interface delete <id> <smps> \n");
            help_print("link_protect [slot <slot-id>] interface type set <id> [smps] <normal|loopback> \n");
            help_print("link_protect [slot <slot-id>] monitor <id> [link|traffic|fcs|all] <on|off> \n");
            help_print("link_protect [slot <slot-id>] set min rx_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set inline min rx_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set inline min tx_rx_percent <INT> \n");
            help_print("link_protect [slot <slot-id>] set inline min rx_fcs_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set inline min tx_fcs_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set monitor min tx_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set monitor min rx_tx_percent <INT> \n");
            help_print("link_protect [slot <slot-id>] set monitor min rx_fcs_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set monitor min tx_fcs_rate <INT> \n");
            help_print("link_protect [slot <slot-id>] set heartbeat interval <interval> \n");
            help_print("link_protect [slot <slot-id>] set heartbeat timeout <time> \n");
            help_print("link_protect [slot <slot-id>] status show [id] \n");
            help_print("link_protect [slot <slot-id>] interface concatenate type <id> {loop|cross}\n");
        }
    }*/
    ;

snmpcmds:
   SNMP snmp_op EOS 
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_OPTION);
    }
    | SNMP STATUS EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SHOW_STATUS);
    }
    | SNMP SHOW CONFIGURATION EOS
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SHOW_CONF);
    }
    | SNMP SET AUTH TYPE snmp_auth_type EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
    | SNMP SET PRIV TYPE snmp_priv_type EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
    | SNMP SET ACCESS snmp_access EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
    | SNMP SET snmp_param _STRING_ EOS
    {
		convert_spec_str_to_json($4);	
        if(oe_check_snmp_param($3, $4) == 0){
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
		}
    }
    | SNMP ADD TRAP IP _IPADDR_ EOS
    {
        char ip[64] = {0};
        struct in_addr in;
        in.s_addr = htonl($5);

        strcpy(ip, (char *)inet_ntoa(in));
		convert_val_to_json(SNMP_ADD_TRAP_IP);
		convert_spec_str_to_json(ip);	
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
    | SNMP DELETE TRAP IP ip_addr EOS
    {
        char ip[64] = {0};
        struct in_addr in;
        in.s_addr = htonl($5);

        strcpy(ip, (char *)inet_ntoa(in));
        strcpy(ip, (char *)inet_ntoa(in));
		convert_val_to_json(SNMP_DEL_TRAP_IP);
		convert_spec_str_to_json(ip);	
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
    /*|SNMP TRAP trap_type THRESHOLD _INT_ EOS
    {
    	if ($3 != TRAP_TYPE_CPU && $3 != TRAP_TYPE_MEM && $3 != TRAP_TYPE_FCS && $3 != TRAP_TYPE_DROP) {
		//__printf_buf("trap type should be cpu or memory or fcs or drop\n");
		EN = 100;
		break;
	}
	
	int max_v = ($3 == TRAP_TYPE_FCS || $3 == TRAP_TYPE_DROP) ? 100000: 100;
	if ($5 < 1 || $5 > max_v) {
		//__printf_buf("value should be from 1 to %d.\n", max_v);
		EN = 100;
		break;
	}else {
		EN = oe_set_snmp_trap($3, $5);
	}
    }
    | SNMP SHOW TRAP THRESHOLD VALUE EOS
    {
    	EN = oe_get_snmp_trap();
    }*/
    ;

logcmds:
    /*LOG able log_enable EOS
    {
        EN = oe_log_enable($2, $3, 1);
    }
    |  LOG HOST log_config EOS
    {
        g_log.mask = g_log.mask & ~LOG_ACTION_MASK;
        g_log.mask |= LOG_ACTION_SET;
        g_log.mask |= LOG_CONFIG;
        EN = oe_log_config(&g_log, LOG_HOST_REMOTE_ADD);
    }
    |  LOG NO HOST log_config EOS
    {
        g_log.mask = g_log.mask & ~LOG_ACTION_MASK;
        g_log.mask |= LOG_ACTION_SET;
        g_log.mask |= LOG_CONFIG;
        EN = oe_log_config(&g_log, LOG_HOST_REMOTE_DEL);
    }
    |  LOG HOST LOCAL EOS
    {
        g_log.mask = g_log.mask & ~LOG_ACTION_MASK;
        g_log.mask |= LOG_ACTION_SET;
        g_log.mask |= LOG_CONFIG;
        g_log.type = EMS_LOG_CONFIG_LOCAL;
        g_log.hostip = 0;
        g_log.port = 0;
        EN = oe_log_config(&g_log, LOG_HOST_LOCAL);
    }
    |  SHOW LOG log_show EOS
    {
        g_log.mask = g_log.mask & ~LOG_ACTION_MASK;
        g_log.mask |= LOG_ACTION_SHOW;
        EN = oe_log_show(&g_log);
    }
    |   RESET LOG EOS
    {
        EN = oe_log_reset();
    }*/
    ;

vermgrcmds:
    /*SHOW CURRENT APPLICATION EOS
    {
        EN = oe_get_current_application();
    }
    |  SHOW AVAILABLE APPLICATION EOS
    {
        EN = oe_get_available_application();
    }
    |  ADD APPLICATION _URL_ EOS
    {
        EN = oe_add_application($3, NULL);
    }
    |  ADD APPLICATION _URL_ _STRING_ EOS
    {
        EN = oe_add_application($3, $4);
    }
    |  DELETE APPLICATION _STRING_ EOS
    {
        EN = oe_delete_application($3);
    }
    |   SELECT APPLICATION _STRING_ EOS
    {
        EN = oe_select_application($3, 0);
    }
    |   SELECT APPLICATION _STRING_ _INT_ EOS
    {
        if ($4 <= 0) {
            //__printf_buf("The sequence must be > 0.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
            EN = oe_select_application($3, $4);
        }
    }
    |   UNDO SELECT APPLICATION _STRING_ EOS
    {
        EN = oe_undo_select_application($4);
    }
    |   OM LIST APPLICATION EOS
    {
        EN = oe_om_list_application();
    }
    |   OM ADD APPLICATION _URL_ EOS
    {
        EN = oe_om_add_application($4);
    }
    |   OM DELETE APPLICATION _STRING_ EOS
    {
        EN = oe_om_delete_application($4);
    }
    |   OM SELECT APPLICATION _STRING_ _INT_ EOS
    {
        if ($5 <= 1 || $5 > SLOT_NUMBER_PER_SYSTEM) {
            //__printf_buf("The slot_min must be from 2 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
            EN = oe_om_select_application($4, $5, 0);
        }
    }

    |   OM SELECT APPLICATION _STRING_ _INT_ _INT_ EOS
    {
        if ($5 <= 1 || $5 > SLOT_NUMBER_PER_SYSTEM) {
            //__printf_buf("The slot_min must be from 2 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else if ($6 <= 1 || $6 > SLOT_NUMBER_PER_SYSTEM) {
            //__printf_buf("The slot_max must be from 2 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else if ($6 < $5) {
            //__printf_buf("The slot_max must be >= slot_min.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
            EN = oe_om_select_application($4, $5, $6);
        }
    }
    |   OM UNDO SELECT APPLICATION _STRING_ _INT_ EOS
    {
        if ($6 <= 1 || $6 > SLOT_NUMBER_PER_SYSTEM) {
            //__printf_buf("The slot_min must be from 2 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
            EN = oe_om_undo_select_application($5, $6, 0);
        }      
    }
    |   OM UNDO SELECT APPLICATION _STRING_ _INT_ _INT_ EOS
    {
        if ($6 <= 1 || $6 > SLOT_NUMBER_PER_SYSTEM) {
            //__printf_buf("The slot_min must be from 2 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else if ($7 <= 1 || $7 > SLOT_NUMBER_PER_SYSTEM) {
            //__printf_buf("The slot_max must be from 2 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else if ($7 < $6) {
            //__printf_buf("The slot_max must be >= slot_min.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
            EN = oe_om_undo_select_application($5, $6, $7);
        }   
    }*/
    ;

op_mode:
    FR_16XE_FB_11QXE_2DXE
    {
        $$ = 1;
    }
    |   FR_20XE_FB_9QXE_4DXE
    {
        $$ = 0;
    }
    |   FR_24XE_FB_8QXE_4DXE
    {
        $$ = 2;
    }

olpcmds:
    LINK_PROTECT slot_info CREATE olpid EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_CREATE);
    }
    | LINK_PROTECT slot_info CREATE olpid olp_propertys EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_PROPERTY);
    }
    |   LINK_PROTECT slot_info DESTROY olpid EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_DESTROY);
    }
    /*|   LINK_PROTECT slot_info PROPERTY SET olpid olp_propertys EOS
    {
        EN = oe_lkpt_link_property_set($5, &g_olp_property);
        free($5);
    }*/
    |   LINK_PROTECT slot_info INTERFACE ADD olpid smps EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_ADD);
    }
    |   LINK_PROTECT slot_info INTERFACE DELETE olpid smps EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_DEL);
    }
    |   LINK_PROTECT slot_info INTERFACE TYPE SET olpid if_type EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_SET_WITFHOUT_SMPS);
    }
    |   LINK_PROTECT slot_info INTERFACE TYPE SET olpid smps if_type EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_SET);
    }
    |   LINK_PROTECT slot_info MONITOR olpid link_protect_types onoff EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_WORK);
    }
    |   LINK_PROTECT slot_info MONITOR ALL link_protect_types onoff EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_WORK_ALL);
    }
    |   LINK_PROTECT slot_info STATUS SHOW olpid EOS
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_STATUS_SHOW);
    }
    |   LINK_PROTECT slot_info SET INLINE MIN RX_RATE _INT_ EOS
    {  
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_RX_RATE);
    }
    |   LINK_PROTECT slot_info SET INLINE MIN TX_RX_PERCENT _INT_ EOS
    {   
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_TX_RX_PERCENT);
    }   
    |   LINK_PROTECT slot_info SET INLINE MIN RX_FCS_RATE _INT_ EOS
    {   
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_RX_FCS_RATE);
    }   
    |   LINK_PROTECT slot_info SET INLINE MIN TX_FCS_RATE _INT_ EOS
    {           
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_TX_FCS_RATE);
    }           
    /*|   LINK_PROTECT slot_info SET MONITOR MIN TX_RATE _INT_ EOS
    {           
        EN = oe_lkpt_link_srv_min_tx_rate($7);
    }*/  
    |   LINK_PROTECT slot_info SET MONITOR MIN RX_TX_PERCENT _INT_ EOS
    {   
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_SRV_RX_TX_PERCENT);
    }   
    |   LINK_PROTECT slot_info SET MONITOR MIN RX_FCS_RATE _INT_ EOS
    {           
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_SRV_RX_FCS_RATE);
    }           
    |   LINK_PROTECT slot_info SET MONITOR MIN TX_FCS_RATE _INT_ EOS
    {       
		convert_val_to_json($7); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_SRV_TX_FCS_RATE);
    }   
    |   LINK_PROTECT slot_info INTERFACE CONCATENATE TYPE olpid LOOP EOS
    {
		convert_val_to_json(0); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_CONCATENATE);
    }
    |   LINK_PROTECT slot_info INTERFACE CONCATENATE TYPE olpid CROSS EOS
    {
		convert_val_to_json(1); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_CONCATENATE);
    }
    |   LINK_PROTECT slot_info SET HEARTBEAT INTERVAL _INT_ EOS
    {
		convert_val_to_json($6); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_HEART_INTERVAL);
    }
    |   LINK_PROTECT slot_info SET HEARTBEAT TIMEOUT _INT_ EOS
    {
		convert_val_to_json($6); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_HEART_TIMEOUT);
    }
    /*|   LINK_PROTECT SHOW CONFIGURATION EOS
    {
        EN = oe_lkpt_link_show_config();
    }*/
    ;

slot_info:
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    |   SLOT _INT_
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();
        if ($2 != g_my_slot) {
            YYERROR;
        }
        $$ = $2;
		convert_val_to_json($$);
    }
    ;

olpid:
    _INT_'-'_INT_
    {
		#if 0
        olp_id_t *olp_id = calloc(1, sizeof(olp_id_t));
        if (olp_id == NULL) {
            EN = OE_RST_NO_MEM;
            YYERROR;
        }
        olp_id->dev_id = $1;
        olp_id->link_id = $3;
        $$ = olp_id;
		#endif
		convert_val_to_json($1);
		convert_val_to_json($3);
    }
    ;


olp_propertys:
    olp_property
    {
    }
    ;

olp_property:
    NAME '=' _STRING_ DEVICE '=' olp_device IP '=' _IPADDR_ PORT '=' _INT_
    {
        if (strlen($3) >= 128) {
            //__printf_buf("The len of the name should be form 1 to 127\n"); 
			EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
		convert_spec_str_to_json($3);	
		convert_val_to_json($6);
		convert_val_to_json($9);
		convert_val_to_json($12);
	}
    ;

olp_device:
    _INT_
    {
        $$ = $1;
    }
    ;

description:
   _STRING_
    {
        if (strlen($1) >= 128) {
            //__printf_buf("The len of the name should be form 1 to 127\n"); 
                       EN = OE_RST_INVLD_PRM;
                       //__printf_rv(EN);
            YYERROR;
        }
        memcpy(g_description, $1, strlen($1));
    }
    ;

sys_name:
   _STRING_
    {
        if (strlen($1) >= 128 || strlen($1) < 4) {
            //__printf_buf("The len of the name should be form 4 to 127\n"); 
                       EN = OE_RST_INVLD_PRM;
                       //__printf_rv(EN);
            YYERROR;
        }
        memcpy(g_name, $1, strlen($1));
    }
    ;          
 
link_protect_types:
    link_protect_type
    {
        $$ = $1;
		convert_val_to_json($$);
    }
    |   link_protect_type','link_protect_types
    {
        $$ = $1|$3;
		convert_val_to_json($$);
    }
    ;

link_protect_type:
    {
        $$ = E_LINK_PROTECT_CHECK_ALL_MASK & (~E_LINK_PROTECT_OPTPOWER_CHECK_MASK);
    }
    |   RX
    {
        $$ = E_LINK_PROTECT_RX_CNT_CHECK_MASK;
    }
    |   TX
    {
        $$ = E_LINK_PROTECT_TX_CNT_CHECK_MASK;
    }
    |   RX_FCS
    {
        $$ = E_LINK_PROTECT_RX_FCS_CNT_CHECK_MASK;
    }
    |   TX_FCS
    {
        $$ = E_LINK_PROTECT_TX_FCS_CNT_CHECK_MASK;
    }
    |   ALL
    {
        $$ = E_LINK_PROTECT_CHECK_ALL_MASK;
    }
    |   TRAFFIC
    {
        $$ = E_LINK_PROTECT_RX_CNT_CHECK_MASK | E_LINK_PROTECT_TX_CNT_CHECK_MASK;
    }
    |   LINK
    {
        $$ = E_LINK_PROTECT_LINK_STAT_CHECK_MASK;
    }
    |   FCS
    {
        $$ = E_LINK_PROTECT_RX_FCS_CNT_CHECK_MASK | E_LINK_PROTECT_TX_FCS_CNT_CHECK_MASK;
    }
    ;

snmp_op:
    START
    {
        $$ = SNMP_START;
		convert_val_to_json($$);
    }
    | RESTART
    {
        $$ = SNMP_RESTART;
		convert_val_to_json($$);
    }
    | STOP
    {
        $$ = SNMP_STOP;
		convert_val_to_json($$);
    }
    | RESTORE CONFIGURATION
    {
                $$ = SNMP_RESTORE;
		convert_val_to_json($$);
        }
    ;   

snmp_param:
    SYSNAME
    {
        $$ = SNMP_SYSNAME;
		convert_val_to_json($$);
    }
    | SYSCONTACT
    {
        $$ = SNMP_SYSCONTACT;
		convert_val_to_json($$);
    }
    | SYSLOCATION
    {
        $$ = SNMP_SYSLOCATION;
		convert_val_to_json($$);
    }
    | ROCOMMUNITY
    {
        $$ = SNMP_ROCOMMUNITY;
		convert_val_to_json($$);
    }
    | RWCOMMUNITY
    {
        $$ = SNMP_RWCOMMUNITY;
		convert_val_to_json($$);
    }
    | TRAP VERSION
    {
        $$ = SNMP_TRAP_VERSION;
		convert_val_to_json($$);
    }
    | USERNAME
    {
        $$ = SNMP_USERNAME;
		convert_val_to_json($$);
    }
    | AUTH PASSWORD
    {
        $$ = SNMP_AUTH_PASSWD;
		convert_val_to_json($$);
    }
    | PRIV PASSWORD
    {
        $$ = SNMP_PRIV_PASSWD;
		convert_val_to_json($$);
    }
    ;

snmp_auth_type:
    MD5
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    | SHA
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    ;

snmp_priv_type:
    DES
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    | AES
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    ;

snmp_access:
    NOAUTH
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    | AUTH
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    | PRIV
    {
        $$ = 2;
		convert_val_to_json($$);
    }
    ;

smps:
    _SMPS_
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();	
        if (OeAp_Get_Platform() == PLATFORM_S40 && $1.s != 1 << (g_my_slot - 1)) {
            //__printf_buf("Invalid input. Please input right slot_id :%d\n", g_my_slot);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
		//fprintf(stderr,"%d\n",$1.s);
		slot_mask = $1.s;
		convert_val_to_json($1.s);
		convert_val_to_json($1.m);

        {
            int i=0;
            for(i=0; i<MAX_PORTS_WIDTH; i++)
            {
				//fprintf(stderr,"port[%d]:%lld\n",i,$1.p[i]);
                convert_val_to_json($1.p[i]);
            }
        }
        
    }
    |   _MPS_
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();	
        if (OeAp_Get_Platform() == PLATFORM_S320) {
            //__printf_buf("Invalid input.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        slot_mask = 1<<(g_my_slot-1);
		convert_val_to_json(slot_mask);
		convert_val_to_json($1.m);
        {
            int i=0;
            for(i=0; i<MAX_PORTS_WIDTH; i++)
            {
                convert_val_to_json($1.p[i]);
            }
        }
    }
    ;

psc:
    DIP
    {
        $$ = TRUNK_LOAD_BALANCE_DSTIP;
		convert_val_to_json($$);
    }
    |   DMAC
    {
        $$ = TRUNK_LOAD_BALANCE_DSTMAC;
		convert_val_to_json($$);
    }
    |   SIP
    {
        $$ = TRUNK_LOAD_BALANCE_SRCIP;
		convert_val_to_json($$);
    }
    |   SMAC
    {
        $$ = TRUNK_LOAD_BALANCE_SRCMAC;
		convert_val_to_json($$);
    }
    |   IP
    {
        $$ = TRUNK_LOAD_BALANCE_SRCDSTIP;
		convert_val_to_json($$);
    }
    |   MAC
    {
        $$ = TRUNK_LOAD_BALANCE_SRCDSTMAC;
		convert_val_to_json($$);
    }
    ;
    
ip_addr:
    _IPADDR_
    {
        if ((0 == $1) || (0xffffffff == $1)) {
            //__printf_buf("ip address should be NOT 0.0.0.0 or 255.255.255.255.\n");
            EN = OE_RST_INVLD_PRM; 
			//__printf_rv(EN);
            YYERROR;
        }
        
        $$ = $1;
		convert_val_to_json($$);
    }
    ;
    
mask:
    _IPADDR_
    {
        $$ = $1;
		convert_val_to_json($$);
    }
    ;

trap_type:
    INTERFACE FCS
    {
	$$ = TRAP_TYPE_FCS;
    }
    |INTERFACE DROP
    {
	$$ = TRAP_TYPE_DROP;
    }
    |	CPU OVERLOAD
    {
	$$ = TRAP_TYPE_CPU;
    }
    |	MEMORY OVERLOAD
    {
	$$ = TRAP_TYPE_MEM;
    };
    
onoff:
    ON
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    |   OFF
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    ;
    
mac_addr:
    _MAC_
    {
        unsigned char *tmp_mac = malloc(sizeof(unsigned char) * 6);
        if (tmp_mac == NULL) {
            EN = OE_RST_NO_MEM;
            YYERROR;
        }
        memset(tmp_mac, 0, sizeof(unsigned char) * 6);
        memcpy(tmp_mac, $1, 6);
        
        $$ = tmp_mac;
    }
    ;
    
agingtime:
    _INT_
    {
        if ((0 != $1) && (($1 < L2_MAC_AGING_TIME_MIN) || ($1 > L2_MAC_AGING_TIME_MAX))) {
            //__printf_buf("aging time should be 0 or from %d to %d (s).\n", L2_MAC_AGING_TIME_MIN, L2_MAC_AGING_TIME_MAX);
            EN = OE_RST_INVLD_PRM; 
			//__printf_rv(EN);
            YYERROR;
        }

        $$ = $1;
    }
    ;

able:
    ENABLE
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    |   DISABLE
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    ;

verbose:
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    |   VERBOSE
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    ;

rule_id:
    _INT_
    {
        if ($1 < 1 || $1 > DEFAULT_ACLS_NUMBER) {
            //__printf_buf("rule id should be from 1 to %d.\n", DEFAULT_ACLS_NUMBER);
            EN = OE_RST_INVLD_PRM; 
			//__printf_rv(EN);
            YYERROR;            
        }     
        $$ = $1;
		convert_val_to_json($$);
    }
    ;

entry_id:
	_INT_
	{
        g_class_entry.entry_index = $1;
    }

duplex:
    FULL
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    |   HALF
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    ;

//TODO by caicai
/*
hash_outports_id_attr:
    outports_id
    {
        g_hash_list.list[g_hash_list.num++] = $1;
    }
    |   outports_id'-'outports_id
    {
        int i;
        for (i = $1; i <= $3; i++) {
            g_hash_list.list[g_hash_list.num++] = i;
        }
    }

hash_outports_id_list:
    hash_outports_id_attr
    {
    }
    |   hash_outports_id_attr','hash_outports_id_list
    {
    }
    ;
*/

//TODO by caicai
/*    
class_entry:
    ce
    {

    }
    |   class_entry ce
    {

    }
    ;

ce:
    ce_protocol
    |   ce_ethertype
    |   ce_sip
    |   ce_dip
    |   ce_sport
    |   ce_dport
    |   ce_smac
    |   ce_dmac
    |   ce_tcpflag
    |   ce_userdata
    |   ce_innervlan
    |   ce_range_outervlan
    |   ce_range_l3packetlen
    {

    }
    ;

ce_protocol:
    PROTOCOL '=' TCP
    {
        g_class_entry.key_mask |= OE_KEY_MASK_PROTOCOL;

        g_class_entry.protocol = 0x06;
    }
    |   PROTOCOL '=' UDP
    {
        g_class_entry.key_mask |= OE_KEY_MASK_PROTOCOL;

        g_class_entry.protocol = 0x11;
    }
    |   PROTOCOL '=' ICMP
    {
        g_class_entry.key_mask |= OE_KEY_MASK_PROTOCOL;

        g_class_entry.protocol = 0x01;
    }
    |   PROTOCOL '=' _INT_
    {
        if ($3 < 0 || $3 > 255) {
            //__printf_buf("protocol should be from 0 to 255.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
    
        g_class_entry.key_mask |= OE_KEY_MASK_PROTOCOL;

        g_class_entry.protocol = $3;
    }
    ;
       
ce_ethertype:
    ETHERTYPE '=' _INT_
    {
        g_class_entry.ethertype = $3;
        g_class_entry.key_mask |= OE_KEY_MASK_ETHERTYPE;
    }
    ;
 
ce_sip:
    SIP '=' ip
    {
        g_class_entry.key_mask |= OE_KEY_MASK_SIP;

        g_class_entry.sip = $3;
        g_class_entry.sip_mask = 0xffffffff;
    }
    |   SIP '=' ip '/' ip
    {
        g_class_entry.key_mask |= OE_KEY_MASK_SIP;

        g_class_entry.sip = $3;
        g_class_entry.sip_mask = $5;
    }
    |   SIP '=' _IPV6ADDR_
    {    
        g_class_entry.key_mask |= OE_KEY_MASK_SIPV6;
        memcpy(g_class_entry.sipv6, $3, sizeof(g_class_entry.sipv6));
        memset(g_class_entry.sipv6_mask, 0xff, sizeof(g_class_entry.sipv6));
    }    
    |   SIP '=' _IPV6ADDR_ '/' _IPV6ADDR_
    {    
        g_class_entry.key_mask |= OE_KEY_MASK_SIPV6;
        memcpy(g_class_entry.sipv6, $3, sizeof(g_class_entry.sipv6));
        memcpy(g_class_entry.sipv6_mask, $5, sizeof(g_class_entry.sipv6_mask));
    } 
    ;

ce_dip:
    DIP '=' ip
    {
        g_class_entry.key_mask |= OE_KEY_MASK_DIP;

        g_class_entry.dip = $3;
        g_class_entry.dip_mask = 0xffffffff;
    }
    |   DIP '=' ip '/' ip
    {
        g_class_entry.key_mask |= OE_KEY_MASK_DIP;

        g_class_entry.dip = $3;
        g_class_entry.dip_mask = $5;
    }
    |   DIP '=' _IPV6ADDR_
    {
        g_class_entry.key_mask |= OE_KEY_MASK_DIPV6;
        memcpy(g_class_entry.dipv6, $3, sizeof(g_class_entry.dipv6));
        memset(g_class_entry.dipv6_mask, 0xff, sizeof(g_class_entry.dipv6));
    }
    |   DIP '=' _IPV6ADDR_ '/' _IPV6ADDR_
    {
        g_class_entry.key_mask |= OE_KEY_MASK_DIPV6;
        memcpy(g_class_entry.dipv6, $3, sizeof(g_class_entry.dipv6));
        memcpy(g_class_entry.dipv6_mask, $5, sizeof(g_class_entry.dipv6_mask));
    }
    ;

ce_sport:
    SPORT '=' _INT_
    {
        if ($3 < 0 || $3 > 65535) {
            //__printf_buf("sport should be from 0 to 65535.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.key_mask |= OE_KEY_MASK_SPORT;
        
        g_class_entry.sport = $3;
        g_class_entry.sport_mask = 0xffff;         
    }
    |   SPORT '=' _INT_ '/' _INT_
    {
        if ($3 < 0 || $3 > 65535 || $5 < 0 || $5 > 65535) {
            //__printf_buf("sport should be from 0 to 65535.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.key_mask |= OE_KEY_MASK_SPORT;
        
        g_class_entry.sport = $3;
        g_class_entry.sport_mask = $5;        
    }    
    |   SPORT '=' _INT_ '-' _INT_
    {
        if(($3>$5) || ($3<0) || ($5>0xffff))
        {
            //__printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            //__printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=L4_SRC_PORT;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_num++; 
    }
    |SPORT '=' _INT_ '-' _INT_ '/' INVERT
    {
        if(($3>$5) || ($3<0) || ($5>0xffff))
        {
            //__printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
           // __printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=L4_SRC_PORT;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_invert_flag[g_class_entry.range_num]=1;
        g_class_entry.range_num++;    
    }
    ;

ce_dport:
    DPORT '=' _INT_
    {
        if ($3 < 0 || $3 > 65535) {
            //__printf_buf("dport should be from 0 to 65535.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.key_mask |= OE_KEY_MASK_DPORT;
        
        g_class_entry.dport = $3;
        g_class_entry.dport_mask = 0xffff;      
    }
    |   DPORT '=' _INT_ '/' _INT_
    {
        if ($3 < 0 || $3 > 65535 || $5 < 0 || $5 > 65535) {
            //__printf_buf("dport should be from 0 to 65535.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.key_mask |= OE_KEY_MASK_DPORT;
        
        g_class_entry.dport = $3;
        g_class_entry.dport_mask = $5;        
    }     
    |    DPORT '=' _INT_ '-' _INT_
    {
        if(($3>$5) || ($3<0) || ($5>0xffff))
        {
            //__printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            //__printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=L4_DST_PORT;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_num++; 
    }
    | DPORT '=' _INT_ '-' _INT_ '/' INVERT
    {
        if(($3>$5) || ($3<0) || ($5>0xffff))
        {
            //__printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            //__printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            //__printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=L4_DST_PORT;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_invert_flag[g_class_entry.range_num]=1;
        g_class_entry.range_num++;    
    }
    ;

ce_smac:
    SMAC '=' _MAC_
    {
        g_class_entry.key_mask |= OE_KEY_MASK_SMAC;
        memcpy(g_class_entry.smac, $3, 6);
        memset(g_class_entry.smac_mask, 0xff, sizeof(g_class_entry.smac_mask));
    }
    |   SMAC '=' _MAC_ '/' _MAC_
    {
        g_class_entry.key_mask |= OE_KEY_MASK_SMAC;

        memcpy(g_class_entry.smac, $3, 6);
        memcpy(g_class_entry.smac_mask, $5, 6);
    }
    ;

ce_dmac:
    DMAC '=' _MAC_
    {
        g_class_entry.key_mask |= OE_KEY_MASK_DMAC;

        memcpy(g_class_entry.dmac, $3, 6);
        memset(g_class_entry.dmac_mask, 0xff, sizeof(g_class_entry.dmac_mask));
    }
    |   DMAC '=' _MAC_ '/' _MAC_
    {
        g_class_entry.key_mask |= OE_KEY_MASK_DMAC;
    
        memcpy(g_class_entry.dmac, $3, 6);
        memcpy(g_class_entry.dmac_mask, $5, 6);
    }
    ;

ce_tcpflag:
    TCPFLAG '=' tcpflags
    {
    }
    ;
 
 tcpflag:
    FIN
    {
        g_class_entry.key_mask |= 0x0040;
        g_class_entry.tcp_flag |= 0x01;
    }
    |    SYN
    {
        g_class_entry.key_mask |= 0x0080;
        g_class_entry.tcp_flag |= 0x02;
    }
    |    RST
    {
        g_class_entry.key_mask |= 0x0100;
        g_class_entry.tcp_flag |= 0x04;
    }
    |    PSH
    {
        g_class_entry.key_mask |= 0x0200;
        g_class_entry.tcp_flag |= 0x08;
    }
    |    ACK
    {
        g_class_entry.key_mask |= 0x0400;
        g_class_entry.tcp_flag |= 0x10;
    }
    |    URG
    {
        g_class_entry.key_mask |= 0x0800;
        g_class_entry.tcp_flag |= 0x20;
    }
    |    '!' FIN
    {
        g_class_entry.key_mask |= 0x0040;
        g_class_entry.tcp_flag &= (~0x01);
    }
    |    '!' SYN
    {
        g_class_entry.key_mask |= 0x0080;
        g_class_entry.tcp_flag &= (~0x02);
    }
    |    '!' RST
    {
        g_class_entry.key_mask |= 0x0100;
        g_class_entry.tcp_flag &= (~0x04);
    }
    |    '!' PSH
    {
        g_class_entry.key_mask |= 0x0200;
        g_class_entry.tcp_flag &= (~0x08);
    }
    |    '!' ACK
    {
        g_class_entry.key_mask |= 0x0400;
        g_class_entry.tcp_flag &= (~0x10);
    }
    |    '!' URG
    {
        g_class_entry.key_mask |= 0x0800;
        g_class_entry.tcp_flag &= (~0x20);
    }
    ;

tcpflags:
    tcpflag
    |    tcpflags ',' tcpflag
    ;


ce_innervlan:
	INNERVLAN '=' _INT_
    {
		g_class_entry.inner_vlan = $3;
		g_class_entry.inner_vlan_mask = 0xfff;
		g_class_entry.key_mask |= OE_KEY_MASK_INNERVLAN;
    }
	|   INNERVLAN '=' _INT_ '/' _INT_
	{
		g_class_entry.inner_vlan = $3;
		g_class_entry.inner_vlan_mask = $5;
		g_class_entry.key_mask |= OE_KEY_MASK_INNERVLAN;
	}
    ;

ud_id:
    UD0
    {
        $$=0;
		convert_val_to_json($$);
    }
    |   UD1
    {
        $$=1;
		convert_val_to_json($$);
    }
    |   UD2
    {
        $$=2;
		convert_val_to_json($$);
    }
    |   UD3
    {
        $$=3;
		convert_val_to_json($$);
    }
    |   UD4
    {
        $$=4;
		convert_val_to_json($$);
    }
    |   UD5
    {
        $$=5;
		convert_val_to_json($$);
    }
    |   UD6
    {
        $$=6;
		convert_val_to_json($$);
    }
    |   UD7
    {
        $$=7;
		convert_val_to_json($$);
    }
    ;

ce_userdata:
    ud_id '=' _INT_ '/' _INT_
    {
        if ((g_class_entry.ud_mask & (1 << $1)) == 0) {
            g_class_entry.ud_mask |= (1 << $1);

        if($3 > 0xffffffff) {
            __printf_buf("Invalid input.ud data can't greater than 0xffffffff\n");
                EN = OE_RST_INVLD_PRM;
				__printf_rv(EN);
                YYERROR;
        }
        if($5 > 0xffffffff) {
            __printf_buf("Invalid input.ud mask can't greater than 0xffffffff\n");
                EN = OE_RST_INVLD_PRM;
				__printf_rv(EN);
                YYERROR;
        }

            g_class_entry.uds[$1].data = $3;
            g_class_entry.uds[$1].mask = $5;
            break;
        }
        g_class_entry.ud_mask |= (1<<$1);
    }
    ;

ce_range_outervlan:
    OUTERVLAN '=' _INT_ '-' _INT_
    {
        if(($3>$5) || ($3<2) || ($5>4093))
        {
            __printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            __printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=OUTVLAN;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_num++; 
    }
    | OUTERVLAN '=' _INT_ '-' _INT_ '/' INVERT
    {
        if(($3>$5) || ($3<0) || ($5>0xfff))
        {
            __printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            __printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=OUTVLAN;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_invert_flag[g_class_entry.range_num]=1;
        g_class_entry.range_num++;    
    }
    ;

ce_range_l3packetlen:
    SIZE '=' _INT_ '-' _INT_
    {
        if(($3>$5) || ($3<0))
        {
            __printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            __printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=L3_PACKET_LEN;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_num++; 
    }
    | SIZE '=' _INT_ '-' _INT_ '/' INVERT
    {
        if(($3>$5) || ($3<0))
        {
            __printf_buf("Invalid input: Invalid range\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        if(g_class_entry.range_num>32)
        {
            __printf_buf("Range resource is full\n");
            EN = OE_RST_INVLD_PRM;
            __printf_rv(EN);
            YYERROR;
        }
        g_class_entry.range[g_class_entry.range_num].used=1;
        g_class_entry.range[g_class_entry.range_num].range_type=L3_PACKET_LEN;
        g_class_entry.range[g_class_entry.range_num].min=$3;
        g_class_entry.range[g_class_entry.range_num].max=$5;
        g_class_entry.range_invert_flag[g_class_entry.range_num]=1;
        g_class_entry.range_num++;   
    }
    ;

ud_mode:
    HEAD
    {
        $$ = UD_TYPE_HEAD;
		convert_val_to_json($$);
    }
    |   L3
    {
        $$ = UD_TYPE_L3;
		convert_val_to_json($$);
    }
    |   L4
    {
        $$ = UD_TYPE_L4;
		convert_val_to_json($$);
    }
    ;

ud_offset:
    _INT_
    {
        $$ = $1;
		convert_val_to_json($$);
    }
    ;
*/

if_type:
    NORMAL
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    |   LOOPBACK
    {
        $$ = 1;
		convert_val_to_json($$);
    }

ip:
    _IPADDR_
    {
        $$ = $1;
		convert_val_to_json($$);
    }
    ;

shutdown:
    SHUTDOWN
    {
        $$ = 1;
		convert_val_to_json($$);
    }
    |   NO_SHUTDOWN
    {
        $$ = 0;
		convert_val_to_json($$);
    }
    ;

slot_id:
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();
		slot_mask = (1 << (g_my_slot - 1));
        $$ = g_my_slot;
		convert_val_to_json($$);
    }
    |   _INT_
    {
        if (($1 < 1) || ($1 > SLOT_NUMBER_PER_SYSTEM)) {
            //__printf_buf("slot id should be from 1 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM; 
			//__printf_rv(EN);
            YYERROR;
        }
		slot_mask = (1 << ($1 - 1));

        $$ = $1;
		convert_val_to_json($$);
    }
    ;

platform:
    S320
    {
        $$ = PLATFORM_S320;
    }
    |   S40
    {
        $$ = PLATFORM_S40;
    }
    |   ST
    {
        $$ = PLATFORM_S40;
    }
    |   OM
    {
        $$ = PLATFORM_S320;
    }
    ;

log_enable:
    log_enable_level
    {
        $$ = $1;
    }
    |   '|' log_enable_level
    {
        $$ = $2;
    }
    |   log_enable_level '|' log_enable
    {
        $$ = $1 | $3;
    }
    ;

log_enable_level:
    RULE 
    {
        $$ = EMS_LOG_LEVEL_RULE;
    }
    |  INFO
    {
        $$ = EMS_LOG_LEVEL_INFO;
    }
    |  ERROR
    {
        $$ = EMS_LOG_LEVEL_ERROR;
    }
    |   WARN
    {
        $$ = EMS_LOG_LEVEL_WARN;
    }
    |  ALL
    {
        $$ = EMS_LOG_LEVEL_RULE|EMS_LOG_LEVEL_INFO|EMS_LOG_LEVEL_ERROR|EMS_LOG_LEVEL_WARN;
    }
    ;
/*
log_config:
    ip_addr
    {
        g_log.type = EMS_LOG_CONFIG_REMOTE;
        g_log.hostip = $1;
        g_log.port = EMS_LOG_SYSLOG_DEFAULT_PORT;
    }
    |  ip_addr _INT_
    {
        if ($2 < 0 || $2 > 65535) {
            __printf_buf("port should be from 0 to 65535.\n");
            EN = OE_RST_INVLD_PRM;
			__printf_rv(EN);
            YYERROR;
        }
        g_log.type = EMS_LOG_CONFIG_REMOTE;
        g_log.hostip = $1;
        g_log.port = $2;
    }
    ;

log_lines:
    {
        $$ = LOG_DEFAULT_PRINT_LINE;
    }
    |  _INT_
    {
        if ($1 < 0 || $1 > 0x7fffffff) {
            __printf_buf("The line must be <= %d.\n", 0x7fffffff);
            EN = OE_RST_INVLD_PRM;
			__printf_rv(EN);
            YYERROR;
        }
        $$ = $1;
    }
    ;

log_level:
    RULE
    {
        $$ = EMS_LOG_LEVEL_RULE;
    }
    |  INFO
    {
        $$ = EMS_LOG_LEVEL_INFO;
    }
    |  ERROR
    {
        $$ = EMS_LOG_LEVEL_ERROR;
    }
    |  TRACE
    {
        $$ = EMS_LOG_LEVEL_TRACE;
    }
    |   WARN
    {
        $$ = EMS_LOG_LEVEL_WARN;
    }
    ;

log_show:
    log_lines
    {
        g_log.lines = $1;
        g_log.mask |= LOG_OPT_LINE;
    }
    |   log_level log_lines
    {
        //g_log.mask |= (($1 & (g_log_level_mask_init | EMS_LOG_LEVEL_TRACE)) << LOG_LEVEL_OFFSET);
        g_log.mask |= (($1 & (g_log_level_mask_init | EMS_LOG_LEVEL_ALL_MASK)) << LOG_LEVEL_OFFSET);
        if ($1 == g_log_level_mask_init)
            g_log.lines = INT_MAX;
        else
            g_log.lines = $2;
        g_log.mask |= LOG_OPT_LINE;
    }
    |   _TIME_FORMAT_ 
    {
        strncpy(g_log.start_time, $1, 11);
        g_log.mask |= LOG_OPT_TIME;
    }
    |   FROM _TIME_FORMAT_ TO _TIME_FORMAT_
    {
        unsigned int s_year = 0, s_month = 0, s_day = 0;
        unsigned int e_year = 0, e_month = 0, e_day = 0;
        sscanf($2, "%u-%02u-%02u", &s_year, &s_month, &s_day);
        sscanf($4, "%u-%02u-%02u", &e_year, &e_month, &e_day);
        if (e_year < s_year || (e_year == s_year && e_month < s_month)
            || (e_year == s_year && e_month == s_month && e_day < s_day)){
            __printf_buf("The endtime must be >= the starttime.\n");
            EN = OE_RST_INVLD_PRM;
			__printf_rv(EN);
            YYERROR;
        }
        strncpy(g_log.start_time, $2, 11);
        strncpy(g_log.end_time, $4, 11);
        g_log.mask |= LOG_OPT_TIME_RANGE;
    }
    ;
*/

quiet:
    {
        $$ = 0;
    }
    |    QUIET
    {
        $$ = 1;
    }
    ;

rxtx_mode:
    RECEIVE
    {
        $$ = 0;
    }
    | TRANSCEIVING
    {
        $$ = 1;
    };
    
pwd_str:
	_STRING_
	{
		$$=$1;
		convert_spec_str_to_json((char *)$$);
	}
	| _PWD_STRING_
	{
		$$=$1;
		convert_spec_str_to_json((char *)$$);
	};
	
fpga_type:
    AB
    {
        $$ = 1;
    }
    | A9
    {
        $$ = 2;
    };

%%


static void cli_reset_command(void)
{
    clear_name_table();
	//TODO by caicai
	#if 0
    memset(&g_olp_property, 0, sizeof(olp_property_t));
    memset(&g_hash_list, 0, sizeof(struct outports_id_list_t));
    memset(&g_mirror_list, 0, sizeof(struct outports_id_list_t));
	#endif
    memset(g_description, 0, sizeof(g_description));
    memset(g_name,0,sizeof(g_name));    
	memset(&g_sb,0,sizeof(string_buf));
	in_list = 0;
	in_map = 0;
	in_pair = 0;
	int_key = 0;
}

int start_cli_cmd(char *cmd)
{
    int rv = 0;
    hy_switch_to_buffer(cmd);
    rv = yyparse();
    hy_delete_buffer();
    return rv;
}

int start_cli_parser(char *cmd, FILE *fp)
{
    int rv = 0;

    if (fp) {
        hy_switch_to_file(fp);
        rv = yyparse();
        fclose(fp);
        hy_delete_buffer();
        return rv;
    }

    if(cmd) {
        hy_switch_to_buffer(cmd);
        rv = yyparse();
        //free(cmd);
        hy_delete_buffer();
        return rv;
    }

    //rl_variable_bind("disable-completion", "On");
    using_history();
    stifle_history(20);

    printf("Optiway %s Software\nCopyright (C) 2013, Embedway Co., Ltd.\n", (OeAp_Get_Platform() == PLATFORM_S320)?"S320":"S40");
    //printf("\nLBSwitch Software\nCopyright (C) 2013, Chanct Inc.\n");

    hy_switch_to_file(stdin);
    printPrompt();
    yyparse();
    hy_delete_buffer();

    return 0;
}

int convert_val_to_json(unsigned long long value)
{
	char val[128];
	int l = 0;

	if(g_sb.buf_size == 0){
		string_buf_init(&g_sb,128);
	}

	sprintf(val,"%lld",value);
	l = strlen(val);

    if ( in_list == 0 ) {
        string_buf_puts(&g_sb, "[", 1 );
		in_list = 1;
    }
    else {
        if ( !in_pair ) string_buf_puts(&g_sb, ",", 1 );
    }
	
	string_buf_puts_format(&g_sb, "%.*s", l, val);

	in_pair=0;
	int_key = 0;

	return 0;
}

int convert_spec_str_to_json(char *val)
{
	int l = 0;
	l = strlen(val);
	
	if(g_sb.buf_size == 0){
		string_buf_init(&g_sb,128);
	}

    if ( in_list == 0 ) {
        string_buf_puts(&g_sb, "[", 1 );
		in_list = 1;
    }
    else {
        if ( !in_pair ) string_buf_puts(&g_sb, ",", 1 );
    }
	
	string_buf_puts_format(&g_sb, "\"%.*s\"", l, val);

	in_pair=0;
	int_key = 0;
	
	return 0;
}

int convert_key_to_json(char *val)
{
	int l = 0;
	l = strlen(val);
	
	if(g_sb.buf_size == 0){
		string_buf_init(&g_sb,128);
	}

    if ( in_list == 0 ) {
        string_buf_puts(&g_sb, "[", 1 );
		in_list = 1;
    }
    else {
        if ( !in_pair ) string_buf_puts(&g_sb, ",", 1 );
    }
	
    if( in_map == 0 ) {
		string_buf_puts(&g_sb, "{", 1 );
        in_map = 1;
    }

	int_key = get_int2_key(val,l);
	if(int_key > 0){
		string_buf_puts_format(&g_sb, "%d:", int_key );
	}else{
		string_buf_puts_format(&g_sb, "%.*s:", l, val);
	}
    in_pair=1;
	
	return 0;
}

int convert_spec_key_to_json(char *val)
{
	int l = 0;
	l = strlen(val);
	if(g_sb.buf_size == 0){
		string_buf_init(&g_sb,128);
	}

    if ( in_list == 0 ) {
        string_buf_puts(&g_sb, "[", 1 );
		in_list = 1;
    }
    else {
        if ( !in_pair ) string_buf_puts(&g_sb, ",", 1 );
    }

	if( in_map == 0 ) {
		string_buf_puts(&g_sb, "{", 1 );
        in_map = 1;
    }
	string_buf_puts_format(&g_sb, "\"%.*s\":", l, val);
    in_pair=1;

	
	return 0;
}

int close_json()
{
    if ( in_map ) {
		string_buf_puts(&g_sb, "}", 1 );
    }
    if ( in_list ) {
		string_buf_puts(&g_sb, "]", 1 );
    }
	
	return 0;
}

int oe_check_snmp_param(int param, char *value)
{
    int res = 0;
    int i = 0;
    int len = strlen(value);
    #define MAX_SNMP_STRING_LEN 31

    for (i = 0; i < len; i++)
    {   
        if (!((value[i] >= 'a' && value[i] <= 'z')
            || (value[i] >= 'A' && value[i] <= 'Z')
            || (value[i] >= '0' && value[i] <= '9')
            || (value[i] == '.') || (value[i] == '-')
            || (value[i] == '_') || (value[i] == '@')))
        {   
            //__printf_buf("error: snmp not support the character \"%c\"\n", value[i]);
            return -1; 
        }   
    }

    switch (param)
    {
        case SNMP_USERNAME:
            if (len <= 0 || len > MAX_SNMP_STRING_LEN) {
                //__printf_buf("error: the length of the snmp username must be 1~%d .\n", MAX_SNMP_STRING_LEN);
                res = -1;
            }
            break;

        case SNMP_TRAP_VERSION:
            if (strcmp(value, "2c") && strcmp(value, "2C") && strcmp(value, "3")) {
                //__printf_buf("error: snmp trap version must be \"2c\" or \"3\".\n");
                res = -1;
            }
            break;
        case SNMP_AUTH_PASSWD:
            if (len < 8 || len > MAX_SNMP_STRING_LEN) {
                //__printf_buf("error: the length of the snmp auth password must be 8~%d .\n", MAX_SNMP_STRING_LEN);
                res = -1;
            }
            break;

        case SNMP_PRIV_PASSWD:
            if (len < 8 || len > MAX_SNMP_STRING_LEN) {
                //__printf_buf("error: the length of the snmp priv password must be 8~%d .\n", MAX_SNMP_STRING_LEN);
                res = -1;
            }
            break;

        default:
            break;
    }

    return res;

}
