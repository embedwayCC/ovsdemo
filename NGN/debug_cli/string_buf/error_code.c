#include <stdio.h>
#include "exp_def.h"
#include "srv_mpack_common.h"

char *_oe_error(int errcode)
{
    switch (errcode) {
        case 0:
            return ("OK.");
            
        case OE_RST_NO_MEM:
            return ("Memory exhausted.");
        case OE_RST_RS_FULL:
            return ("Resource full.");
        case OE_RST_INVLD_PRM:
            return ("Invalid arguement.");
        case OE_RST_FILE_ERR:
            return ("File operate error.");
        case OE_RST_INT_ERR:
            return ("Internal error.");
        case OE_RST_CE_NOT_FOUND:
            return ("Class entry not found.");
        case OE_RST_CE_EXIST:
            return ("Class entry exist.");
        case OE_RST_RULE_INSTALL:
            return ("Acl action has been set,delete it before modify.");
        case OE_RST_INPORT_REPEAT:
            return ("Inports port conflict.");
        case OE_RST_OUTPORTS_NUM:
            return ("Hash outports port number isn't <= 8.");
        case OE_RST_HASH_ERR:
            return ("Hash outports isn't in the same module.");
        case OE_RST_SET_CAM_ERR:
            return ("Set cam error.");
        case OE_RST_GET_CAM_ERR:
            return ("Get cam error.");
        case OE_RST_DEAD_LOOP:
            return ("Dead loop in the rule.");
        case OE_RST_INIT:
            return ("Init config. Please wait.");
        case OE_RST_UD_ERR_CE:
            return ("UD , MAC and the five-tuple shouldn't be in the same class entry.");
        case OE_RST_UD_ERR_RULE:
            return ("UD , MAC and the five-tuple shouldn't be in the same rule.");
        case OE_RST_UD_ERR_UD:
            return ("UD0,UD2 and UD4,UD6 shouldn't be in the same class entry.");
        case OE_RST_NO_PORT_IN:
            return ("No port is in the inports.");
        case OE_RST_NO_PORT_OUT:
            return ("No port is in the outports.");
        case OE_RST_RULE_CONTENT_CONFLICT:
            return ("Rule content conflict.");
        case OE_RST_SNMP_PARSE_FILE_ERR:
            return ("Parser snmp config file error.");
        case OE_RST_OUTPORTS:
            return ("Outport error.");
        case OE_RST_MIRROR_ERR:
            return ("Mirror must be in the same slot & unit.");
        case OE_RST_OUT_OF_RANGE:
            return ("Value out of range.");
        case OE_RST_COMM_ERR:
            return ("Common error.");
        case OE_RST_NOT_SUPPORT:
            return ("Not support.");
        case OE_RST_NO_LOG:
            return ("No log to show.");
        case OE_RST_DISK_FULL:
            return ("Disk space is full.");
        case OE_RST_NO_ACL:
            return "Error: No such ACL.";
        case OE_RST_NO_VLAN:
            return "Error: No such VLAN.";
        case OE_RST_NO_TRUNK:
            return "Error: No such TRUNK.";
        case OE_RST_ACL_TYPE_ERR:
            return ("Acl type error.");
        case OE_RST_IF_REPEAT:
            return ("Interface repeat.");
        case OE_RST_OUTPORTS_REPEAT:
            return "Outports repeat.";
        case OE_RST_IF_NOT_FOUND:
            return "Interface not found.";
        case OE_RST_PORTS_NOT_IN_INPORTS:
            return "Error: Ports not exist in inports.";
        case OE_RST_PORTS_NOT_IN_OUTPORTS:
            return "Error: Ports not exist in outports.";
        case OE_RST_INPORTS_NOT_EXIST:
            return "Error: Inports not exist.";
        case OE_RST_OUTPORTS_NOT_EXIST:
            return "Error: Outports not exist.";
        case OE_RST_TRUNK_MEMBER_NOT_FOUND:
            return "Error: The interface NOT added to this trunk.";
        case OE_RST_INPORT_CONFLICT:
            return "Error: inport conflict.";
        case OE_RST_BD_STATUS_CONFIG_FAILED:
            return "Error: The slot offline or config failed.";
        case OE_RST_NO_VLANIF:
            return "Error: No such VLAN interface.";
        case OE_RST_USER_EXIST_ERR:
            return ("User already exists.");
        case OE_RST_USER_NOT_EXIST_ERR:
            return ("User does not exist.");
        case OE_RST_USER_MAX_LIMIT_ERR:
            return ("User number isn't <= maximum number.");
        case OE_RST_SYNC_CONFIG:
            return ("Sync config. Please wait.");
        case OE_RST_NOT_PERMIT:
            return ("Permission denied.");
        case OE_RST_SNMP_TRAP_IP_NUM_ERR:
            return ("Snmp trap ip num must be 0~5.");
        case OE_RST_DUPLICATE_SNMP_TRAP_IP_ERR:
            return ("Duplicate snmp trap ip.");
        case OE_RST_NO_SUCH_SNMP_TRAP_IP_ERR:
            return ("No such snmp trap ip.");
        case OE_RST_ENTRY_CLASH:
            return ("Entry clash.");
        case OE_RST_FTP_DOWNLOAD_FAILED:
            return ("Ftp download failed.");
        case OE_RST_LOG_HOST_NOT_LOCAL:
            return ("Log host not local.");
        case OE_RST_LOG_HOST_NUM_ERR:
            return ("Log host num must be 0~5.");
        case OE_RST_DUPLICATE_LOG_HOST_ERR:
            return ("Duplicate log host.");
        case OE_RST_NO_SUCH_LOG_HOST_ERR:
            return ("No such log host.");
        case OE_RST_L2_MAC_ST_NOT_FOUND:
            return ("Error: MAC static does not exist.");
        case OE_RST_L2_MAC_ST_EXIST:
            return ("Error: MAC static already exist.");
        case OE_RST_L2_MAC_ST_MAC_EXIST:
            return ("Error: This MAC address already exist.");
        case OE_RST_L2_MAC_SLOT_MULTIPLE:
            return ("Error: Slot conflict.");
        case OE_RST_TRUNK_INVLD_SMP:
            return ("Error: Invalid smp. (<slot-id>/<module-id>/<portmap>)");
        case OE_RST_L3_ROUTE_ST_NOT_FOUND:
            return ("Error: Route static does not exist.");
        case OE_RST_L3_ROUTE_ST_EXIST:
            return ("Error: Route static already exist.");
        case OE_RST_L3_ROUTE_ST_DIP_EXIST:
            return ("Error: This destination IP address does not exist.");
        case OE_RST_TRUNK_DEFAULT_VLAN_ERROR:
            return ("Error: The trunk default vlan and the vlan don't match.");
        case OE_RST_TRUNK_IN_USE:
            return "Error: The trunk in use, can NOT be deleted.";
		case OE_RST_IF_USED_BY_TRUNK:
			return ("The interface is used by trunk");
		case OE_RST_SINGLE_FIBER_SET_CONFLICT:
			return ("The interface single_fiber transmit and receive only one can be on at the same time.");
		case OE_RST_GROUP_CLASH:
			return ("Group clash, plase change the priority.");
		case OE_RST_ACL_NO_SLOT:
			return ("Please specify the acl on which slot is installed.");
		case OE_RST_PORT_IN_OTHER_SLOT :
			return ("Inports has other board's ports.");
		case OE_RST_SNMP_IS_RUNNING:
			return ("Snmp is running.");
		case OE_RST_SNMP_IS_NOT_RUNNING:
			return ("Snmp is not running.");
        case OE_RST_ACL_BEEN_INSTALL: 
			return ("Acl has been installed.");
        case OE_RST_ACL_BEEN_UNINSTALL:
			return ("Acl has been uninstalled.");
        case OE_RST_ACL_WARNING:
			return ("Acl has been installed and cannot be set !");
        case OE_RST_LOAD_CONFIG_ERR:
			return ("Loading configuration, please wait.");
        case OE_RST_LKPT_CREATE_ERR:
            return ("Link protect create error.");
        case OE_RST_LKPT_REUSED_ERR:
            return ("Link protect reused.");
        case OE_RST_LKPT_IF_NOT_EXIST_ERR:
            return ("Link protect interface not exist.");
        case OE_RST_LKPT_SOCK_SET_ERR:
            return ("Link protect socket set error.");
        case OE_RST_LKPT_SOCK_CONNECT_ERR:
            return ("Link protect socket connect error.");
        case OE_RST_LKPT_SOCK_RECV_ERR:
            return ("Link protect socket receive error.");
        case OE_RST_LKPT_NETWORK_ERR:
            return ("Link protect network error.");
        case OE_RST_SYNTAX_ERR:
            return ("Parser syntax error.");
        case OE_RST_API_NOT_SUPPORT:
            return ("OW api not support.");
        case OE_RST_UD_USED_BY_RULE:
            return ("This ud has beed used by rule.");
        case OE_RST_INPORTS_UESD_BY_RULE:
            return ("This inports has beed used by rule.");
        case OE_RST_OUTPORTS_UESD_BY_RULE:
            return ("This outports has beed used by rule.");
        case OE_RST_INPORTS_CROSS_BRD:
            return ("Inports's member can not cross board.");
        case OE_RST_OUTPORTS_CROSS_BRD:
            return ("Hash outports's member can not cross board.");
        case OE_RST_OUTPORTS_NOT_HASH:
            return ("Outports's not hash type.");
        case OE_RST_EFP_OUT_OF_RANGE:
            return ("Egress fp out of range.");
        case OE_RST_EFP_INSTALL:
            return ("Interface outervlan has been installed.");
        case OE_RST_EFP_UNINSTALL:
            return ("Interface outervlan is not installed.");
        case OE_RST_NO_ACL_ENTRY:
            return ("Error: No such ACL Entry.");
        case OE_RST_OUTPORTS_OUT_OF_RANGE:
            return ("hash outports number should <= 8.");
	 case OE_RST_INTERfACE_CONFLICT:
	     return ("Outports port conflict !!");
	 case OE_RST_WEIGHT_OUT_RANGE:
	     return("The total weight must be 1~128 !");
	 case OE_RST_NO_ACTION:
	     return("Error:Need set acl action first.");		 
	 case DECODE_DATA_ERR:
		 return("Error:msg decode error.");
	 case ENCODE_DATA_ERR:
		 return("Error:msg encode error.");
    default:
        return ("Unknown error.");
    }

    return NULL;
}

