#ifndef __EXP_DEF_H__
#define __EXP_DEF_H__

//#define SLOT_NUMBER_PER_SYSTEM  14

enum {
    OE_RST_OK,                      /* ok */

    OE_RST_NO_MEM = 1,              /* malloc failed */
    OE_RST_RS_FULL = 2,             /* resource full */

    OE_RST_INVLD_PRM = 3,           /* invalid parameter */
    OE_RST_FILE_ERR = 4,            /* file operate error */
    OE_RST_INT_ERR = 5,             /* internal error */

    OE_RST_CE_NOT_FOUND = 6,  /*class entry not found*/
    OE_RST_CE_EXIST = 7,     /*class entry already exist*/
    OE_RST_RULE_INSTALL = 8,    /*need rule uninstall*/
    OE_RST_INPORT_REPEAT = 9,  /*inport repeat */
    OE_RST_OUTPORTS_NUM = 10, /*hash outports number must be smaller than 8 */
    OE_RST_HASH_ERR = 11,  /*hash outports isn't in the same module */
    OE_RST_SET_CAM_ERR = 12,        /* set cam error */
    OE_RST_GET_CAM_ERR = 13,        /* get cam error */
    OE_RST_DEAD_LOOP = 14,  /*dead loop*/
    OE_RST_INIT = 15,             /*init config, pls wait*/
    OE_RST_UD_ERR_CE = 16, /*ud and mac and five-tuple can't be in the same class entry*/
    OE_RST_UD_ERR_RULE = 17, /*ud and mac and five-tuple can't be in the same rule*/
    OE_RST_UD_ERR_UD = 18, /*ud0-ud3 and ud4-ud7 can't be in the same class entry*/
    OE_RST_NO_PORT_IN = 19, /*no port in the inports*/
    OE_RST_NO_PORT_OUT = 20, /*no port in the outports*/
    OE_RST_RULE_CONTENT_CONFLICT = 21, /*ud can't be used in the situation*/

    OE_RST_SNMP_PARSE_FILE_ERR = 22, /*paser snmp_conf file error*/

    OE_RST_OUTPORTS = 23, /*hash outports number must be smaller than 8 */

    OE_RST_MIRROR_ERR = 24, /*set mirror err*/

    OE_RST_OUT_OF_RANGE = 25, /*value out of range*/
    OE_RST_COMM_ERR = 26, /*common error*/
    OE_RST_NOT_SUPPORT = 27, /*not support*/
    OE_RST_NO_LOG = 28, /*no log to show*/
    OE_RST_DISK_FULL = 29, /*disk space full*/
    OE_RST_NO_ACL = 30,
    OE_RST_NO_VLAN = 31,
    OE_RST_NO_TRUNK = 32,
    OE_RST_ACL_TYPE_ERR = 33,
    OE_RST_IF_REPEAT = 34,
    OE_RST_OUTPORTS_REPEAT = 35,
    OE_RST_IF_NOT_FOUND = 36,
    OE_RST_PORTS_NOT_IN_INPORTS = 37,
    OE_RST_PORTS_NOT_IN_OUTPORTS = 38,
    OE_RST_INPORTS_NOT_EXIST = 39,
    OE_RST_OUTPORTS_NOT_EXIST = 40,
    OE_RST_TRUNK_MEMBER_NOT_FOUND = 41,
    OE_RST_INPORT_CONFLICT = 42,
    OE_RST_BD_STATUS_CONFIG_FAILED = 43,
    OE_RST_NO_VLANIF = 44,
    OE_RST_USER_EXIST_ERR = 45,
    OE_RST_USER_NOT_EXIST_ERR = 46,
    OE_RST_USER_MAX_LIMIT_ERR = 47,
    OE_RST_SYNC_CONFIG = 48, /*sync config, pls wait*/
    OE_RST_NOT_PERMIT = 49, /*permission denied*/
    OE_RST_SNMP_TRAP_IP_NUM_ERR = 50,
    OE_RST_DUPLICATE_SNMP_TRAP_IP_ERR = 51,
    OE_RST_NO_SUCH_SNMP_TRAP_IP_ERR = 52,
    OE_RST_ENTRY_CLASH = 53,
    OE_RST_FTP_DOWNLOAD_FAILED = 54,
    OE_RST_LOG_HOST_NOT_LOCAL = 55,
    OE_RST_LOG_HOST_NUM_ERR = 56,
    OE_RST_DUPLICATE_LOG_HOST_ERR = 57,
    OE_RST_NO_SUCH_LOG_HOST_ERR = 58,
    OE_RST_L2_MAC_ST_NOT_FOUND = 59,    /*mac_static not found*/
    OE_RST_L2_MAC_ST_EXIST = 60,        /*mac_static already exist*/
    OE_RST_L2_MAC_ST_MAC_EXIST = 61,    /*mac_static mac already exist*/
    OE_RST_L3_ROUTE_ST_NOT_FOUND = 62,    /*route_static not found*/
    OE_RST_L3_ROUTE_ST_EXIST = 63,        /*route_static already exist*/
    OE_RST_L3_ROUTE_ST_DIP_EXIST = 64,    /*route_static dip already exist*/
    OE_RST_TRUNK_DEFAULT_VLAN_ERROR = 65,
    OE_RST_TRUNK_IN_USE = 66,
    OE_RST_IF_USED_BY_TRUNK = 67,
    OE_RST_SINGLE_FIBER_SET_CONFLICT = 68,
    OE_RST_GROUP_CLASH = 69,
    OE_RST_ACL_NO_SLOT = 70,
    OE_RST_PORT_IN_OTHER_SLOT = 71,
    OE_RST_SNMP_IS_RUNNING = 72,
    OE_RST_SNMP_IS_NOT_RUNNING = 73,
    OE_RST_ACL_BEEN_INSTALL = 74,
    OE_RST_ACL_BEEN_UNINSTALL = 75,
    OE_RST_ACL_WARNING = 76,
    OE_RST_LOAD_CONFIG_ERR = 77,
    OE_RST_L2_MAC_SLOT_MULTIPLE = 78,
    OE_RST_TRUNK_INVLD_SMP = 79,

    /***********olp return**********/
    OE_RST_LKPT_CREATE_ERR = 80,
    OE_RST_LKPT_REUSED_ERR = 81,
    OE_RST_LKPT_IF_NOT_EXIST_ERR = 82,
    OE_RST_LKPT_SOCK_SET_ERR = 83,
    OE_RST_LKPT_SOCK_CONNECT_ERR = 84,
    OE_RST_LKPT_SOCK_RECV_ERR = 85,
    OE_RST_LKPT_NETWORK_ERR = 86,

    /* Parser: syntax error */
    OE_RST_SYNTAX_ERR = 90,

    OE_RST_API_NOT_SUPPORT = 100,
    OE_RST_UD_USED_BY_RULE = 101,
    OE_RST_INPORTS_CROSS_BRD = 102,
    OE_RST_INPORTS_UESD_BY_RULE = 103,
    OE_RST_OUTPORTS_UESD_BY_RULE = 104,
    OE_RST_OUTPORTS_CROSS_BRD = 105,
    OE_RST_OUTPORTS_NOT_HASH = 106,
    OE_RST_EFP_OUT_OF_RANGE = 107,
    OE_RST_EFP_INSTALL = 108,
    OE_RST_EFP_UNINSTALL = 109,
    OE_RST_NO_ACL_ENTRY = 110,
    OE_RST_OUTPORTS_OUT_OF_RANGE = 111,

    OE_RST_INTERfACE_CONFLICT = 112,
    OE_RST_WEIGHT_OUT_RANGE = 113,
    OE_RST_NO_ACTION = 114,

    DECODE_DATA_ERR = 115,
    ENCODE_DATA_ERR = 116,
    /* admin's password set error */
    OE_RST_PASSWD_SIMPLE = 120,
    OE_RST_PASSWD_LEN_INVALID = 121,
    OE_RST_PASSWD_LASTEST_SETTED = 122,
    OE_RST_PASSWD_INVALID = 123,

    /*****NOTE: Need modify the cli_errorcode_string in log_api.c when add the new enum.*****/

    OE_RST_MAX                      /* don't use this errorcode, and when you
                                     * want add an errorcode, please add before this entry
                                     */
};

enum{
     //for omserver
     OPCODE_OMSERVER_GET_CONFIG = 1,                    //"/config/conf",
     OPCODE_OMSERVER_SAVE_CONFIG,                   //"/config/save",
     OPCODE_OMSERVER_IMPORT_CONFIG,                 //"/config/import",
     OPCODE_OMSERVER_EXPORT_CONFIG,                 //"/config/export",
	 OPCODE_OMSERVER_RESTORE_CONFIG,				//"restore config  add by caicai
	 OPCODE_OMSERVER_CLEAR_CONFIG,					//"clear config  add by caicai
     OPCODE_OMSERVER_GET_INPORTS,                   //"/trunk/get/inports",
     OPCODE_OMSERVER_GET_INPORTS_ALL,               //"/trunk/get/inports/all",
     OPCODE_OMSERVER_GET_OUTPORTS,                  //"/trunk/get/outports",
     OPCODE_OMSERVER_GET_OUTPORTS_ALL,              //"/trunk/get/outports/all",
     OPCODE_OMSERVER_SHOW_RULE,                     //"/rule/show",
     OPCODE_OMSERVER_GET_RULE,                      //REGISTER_URL("/rule/get", rpc_get_rule);
     OPCODE_OMSERVER_GET_ENTRY,                     //REGISTER_URL("/entry/get", rpc_rule_entry_get);
     OPCODE_OMSERVER_GET_RULE_NUM,                  //REGISTER_URL("/rule/getnum", rpc_get_rule_num);
     OPCODE_OMSERVER_GET_ENTRY_NUM,                 //REGISTER_URL("/entry/getnum", rpc_rule_entry_get_num);
     OPCODE_SYSTEM_GET_BOARDSTATUS,                 //"/board/status",

     //for system
     OPCODE_SYSTEM_GET_SYSTEM = 16,                      //"/system/info",
     OPCODE_SYSTEM_GET_BOARDINFO,                   //"/board/info",
     OPCODE_SYSTEM_RESET_SYSTEM,                    //"/system/reset",
     OPCODE_SYSTEM_GET_PLATFORM,                    //"/info",
     OPCODE_SYSTEM_SET_SYSNAME,                     //"/sysname/set",
     OPCODE_SYSTEM_SET_DATEIIME,                    //"/datetime/set",
     OPCODE_SYSTEM_GET_DATETIME,                    //"/datetime/get",
     OPCODE_SYSTEM_SET_TIMEZONE,                    //"/timezone/set",
     OPCODE_SYSTEM_RESET_BOARD,                     //"/board/reset",
     OPCODE_SYSTEM_IPCONFIG = 26,                        //"/ipconfig",
     OPCODE_SYSTEM_GATEWAY,                         //"/gateway",
     OPCODE_SYSTEM_GET_USEDINFO,                    //"/usedinfo/get",
     OPCODE_SYSTEM_SET_USEDINFO,                    //"/usedinfo/set",
	 OPCODE_SYSTEM_SET_PASSWORD,					//oe_set_usr_passwd add by caicai


     //for interface
     OPCODE_INTERFACE_PORT_AUTONEG,                 //"/port/autoneg",
     OPCODE_INTERFACE_PORT_PAUSE,                   //"/port/pause",
     OPCODE_INTERFACE_PORT_SHUTDOWN,                //"/port/shut",
     OPCODE_INTERFACE_SINGLEFIBER_TRANSMIT,         //"/port/single_fiber/transmit",
     OPCODE_INTERFACE_PORT_DUPLEX,                  //"/port/duplex",
     OPCODE_INTERFACE_PORT_SPEED,                   //"/port/speed",
     OPCODE_INTERFACE_GET_INFO,                     //"/interface/info",
     OPCODE_INTERFACE_GET_STATUS,                   //"/interface/status",
     OPCODE_INTERFACE_SHOW_COUNTER,                 //"/interface/counter/show",
     OPCODE_INTERFACE_CLEAR_COUNTER,                //"/interface/counter/clear",
     OPCODE_INTERFACE_SET_PORTMODE,                 //"/port/mode",

     //for acl
     OPCODE_ACL_ADD_INPORTS,                        //"/trunk/add/inports",
     OPCODE_ACL_DEL_INPORTS,                        //"/trunk/del/inports",
     OPCODE_ACL_DEL_INPORTS_ALL,                    //"/trunk/del/inports/all",
     OPCODE_ACL_ADD_OUTPORTS,                       //"/trunk/add/outports",
     OPCODE_ACL_DEL_OUTPORTS,                       //"/trunk/del/outports",
     OPCODE_ACL_DEL_OUTPORTS_ALL,                   //"/trunk/del/outports/all",
     OPCODE_ACL_SET_INPORTS_PVID,                   //"/trunk/set/inports/pvid",
     OPCODE_ACL_SET_INPORTS_PVLAN_ENABLE,           //"/trunk/set/inports/pvlan/enable",
     OPCODE_ACL_SET_OUTPORTS_UNTAG_ENABLE,          //"/trunk/set/outports/untag/enable",
     OPCODE_ACL_ADD_PSC,                            //"/trunk/add/psc",
     OPCODE_ACL_SET_FAILOVER,                       //"/failover/able",
     OPCODE_ACL_SET_GLOBLE_LOAD_BALANCE,            //"/globle/psc",
	 OPCODE_ACL_SET_FAILOVER_INTERVAL,				//set failover interval add by caicai
     OPCODE_ACL_SET_RULE_UD,                        //"/rule/set/ud",
     OPCODE_ACL_ADD_RULE,                           //"/rule/add",
     OPCODE_ACL_ADD_RULE_ALL,                       //add by caicai
     OPCODE_ACL_SET_RULE_CONFIG,                    //"/rule/set/config",
     OPCODE_ACL_DEL_RULE_ENTRYID,                   //"/rule/del/entryid",
     OPCODE_ACL_DEL_RULE_ID,                        //"/rule/del/id"
     OPCODE_ACL_CLEAR_RULE,                         //"/rule/clear", rpc_rule_clear);


     //for snmp
     OPCODE_SNMP_OPTION,                            //REGISTER_URL("/snmp/option", rpc_snmp_option);
     OPCODE_SNMP_SHOW_STATUS,                       //REGISTER_URL("/snmp/status", rpc_show_snmp_status);
     OPCODE_SNMP_SHOW_CONF,                         //REGISTER_URL("/snmp/config", rpc_show_snmp_conf);
     OPCODE_SNMP_SET_CONF,                          //REGISTER_URL("/snmp/set", rpc_set_snmp_conf);
     OPCODE_SNMP_GET_ENTRY,                         //REGISTER_URL("/snmp/entry", rpc_get_snmp_entry);

     //for lkpt
     OPCODE_LKPT_LINK_CREATE,                       //REGISTER_URL("/lkpt/link/create", rpc_lkpt_link_create);
     OPCODE_LKPT_LINK_DESTROY,                      //REGISTER_URL("/lkpt/link/destroy", rpc_lkpt_link_destroy);
     OPCODE_LKPT_LINK_PROPERTY,                     //REGISTER_URL("/lkpt/link/property/set", rpc_lkpt_link_property_set);
     OPCODE_LKPT_LINK_IF_ADD,                       //REGISTER_URL("/lkpt/link/if/add", rpc_lkpt_link_if_add);
     OPCODE_LKPT_LINK_IF_DEL,                       //REGISTER_URL("/lkpt/link/if/del", rpc_lkpt_link_if_del);
     OPCODE_LKPT_LINK_IF_SET,                       //REGISTER_URL("/lkpt/link/if/set", rpc_lkpt_link_if_set);
	 OPCODE_LKPT_LINK_IF_SET_WITFHOUT_SMPS,			//rpc_lkpt_link_if_set without smps add by caicai
     OPCODE_LKPT_LINK_WORK,                         //REGISTER_URL("/lkpt/link/work", rpc_lkpt_link_work);
	 OPCODE_LKPT_LINK_WORK_ALL,						//rpc_lkpt_link_work add by caicai
     OPCODE_LKPT_LINK_MIN_RX_RATE,                  //REGISTER_URL("/lkpt/link/min/rxrate", rpc_lkpt_link_min_rx_rate);
     OPCODE_LKPT_LINK_MIN_TX_RX_PERCENT,                  //REGISTER_URL("/lkpt/link/min/percent", rpc_lkpt_link_min_percent);
     OPCODE_LKPT_LINK_STATUS_SHOW,                  //REGISTER_URL("/lkpt/link/status/show", rpc_lkpt_link_status_show);
     OPCODE_LKPT_LINK_IF_CONCATENATE,               //REGISTER_URL("/lkpt/link/if/concatenate", rpc_lkpt_link_if_concatenate);
     OPCODE_LKPT_LINK_HEART_INTERVAL,               //REGISTER_URL("/lkpt/link/heartbeat/interval", rpc_lkpt_link_heartbeat_interval);
     OPCODE_LKPT_LINK_HEART_TIMEOUT,                //REGISTER_URL("/lkpt/link/heartbeat/timeout", rpc_lkpt_link_heartbeat_timeout);
	 OPCODE_LKPT_LINK_MIN_RX_FCS_RATE,				//rpc_lkpt_link_min_rx_fcs_rate add by caicai
	 OPCODE_LKPT_LINK_MIN_TX_FCS_RATE,				//rpc_lkpt_link_min_rx_fcs_rate add by caicai
     OPCODE_LKPT_LINK_MIN_SRV_RX_TX_PERCENT,		//rpc_lkpt_link_srv_min_percent add by caicai
	 OPCODE_LKPT_LINK_MIN_SRV_RX_FCS_RATE,			//rpc_lkpt_link_srv_min_rx_fcs_rate add by caicai
	 OPCODE_LKPT_LINK_MIN_SRV_TX_FCS_RATE,				//rpc_lkpt_link_srv_min_tx_fcs_rate add by caicai


};


#endif
