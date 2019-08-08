#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "parser.tab.h"

typedef struct {
	char *name;
	int token;
	unsigned match_len;
} token_ent;

token_ent token_table[] = {

    { "a9",                  A9,                  2 },
    { "ab",                  AB,                  2 },
    { "abnormal",            ABNORMAL,            3 },
    { "ac1200",              AC1200,              3 },
    { "access",              ACCESS,              3 },
    { "ack",                 ACK,                 3 },
    { "acl",                 ACL,                 3 },
    { "add",                 ADD,                 2 },
    { "aes",                 AES,                 2 },
    { "aging_time",          AGING_TIME,          2 },
    { "all",                 ALL,                 2 },
    { "application",         APPLICATION,         2 },
    { "auth",                AUTH,                4 },
    { "auto",                AUTO,                4 },
    { "autoneg",             AUTONEG,             5 },
    { "available",           AVAILABLE,           2 },
    { "b",                   B,                   1 },
    { "backplane",           BACKPLANE,           2 },
    { "board",               BOARD,               2 },
    { "by",                  BY,                  2 },
    { "change_ttl",          CHANGE_TTL,          4 },
    { "chassis",             CHASSIS,             4 },
    { "clear",               CLEAR,               2 },
    { "concatenate",         CONCATENATE,         4 },
    { "configuration",       CONFIGURATION,       4 },
    { "connect",             CONNECT,             4 },
    { "counter",             COUNTER,             3 },
    { "cpu",                 CPU,                 2 },
    { "create",              CREATE,              3 },
    { "cross",               CROSS,               3 },
    { "current",             CURRENT,             2 },
    { "d",                   D,                   1 },
    { "datetime",            DATETIME,            2 },
    { "default",             DEFAULT,             7 },
    { "default_pvlan",       DEFAULT_PVLAN,       8 },
    { "delete",              DELETE,              3 },
    { "des",                 DES,                 3 },
    { "descript",            DESCRIPT,            4 },
    { "destroy",             DESTROY,             4 },
    { "device",              DEVICE,              3 },
    { "dip",                 DIP,                 3 },
    { "disable",             DISABLE,             4 },
    { "disconnect",          DISCONNECT,          4 },
    { "dmac",                DMAC,                2 },
    { "dport",               DPORT,               2 },
    { "drop",                DROP,                2 },
    { "dst_ip",              DST_IP,              5 },
    { "dst_mac",             DST_MAC,             5 },
    { "dual_dual_star",      DUAL_DUAL_STAR,      6 },
    { "dual_star",           DUAL_STAR,           6 },
    { "duplex",              DUPLEX,              3 },
    { "dynamic",             DYNAMIC,             2 },
    { "enable",              ENABLE,              3 },
    { "entry",               ENTRY,               3 },
    { "eos",                 EOS,                 2 },
    { "error",               ERROR,               2 },
    { "ether",               ETHER,               5 },
    { "ethertype",           ETHERTYPE,           6 },
    { "exit",                EXIT,                4 },
    { "exitshell",           EXITSHELL,           5 },
    { "export",              EXPORT,              3 },
    { "f",                   F,                   1 },
    { "failover",            FAILOVER,            2 },
    { "fcs",                 FCS,                 3 },
    { "fcs_rate",            FCS_RATE,            4 },
    { "fin",                 FIN,                 3 },
    { "five_tuple",          FIVE_TUPLE,          3 },
    { "force",               FORCE,               2 },
    { "fpga",                FPGA,                2 },
    { "fr_16xe_fb_11qxe_2dxe",FR_16XE_FB_11QXE_2DXE,4 },
    { "fr_20xe_fb_9qxe_4dxe",FR_20XE_FB_9QXE_4DXE,5 },
    { "fr_24xe_fb_8qxe_4dxe",FR_24XE_FB_8QXE_4DXE,5 },
    { "from",                FROM,                3 },
    { "ftp",                 FTP,                 2 },
    { "full",                FULL,                2 },
    { "fw",                  FW,                  2 },
    { "gateway",             GATEWAY,             2 },
    { "global",              GLOBAL,              2 },
    { "grule",               GRULE,               2 },
    { "half",                HALF,                3 },
    { "hash",                HASH,                3 },
    { "head",                HEAD,                4 },
    { "heartbeat",           HEARTBEAT,           4 },
    { "help",                HELP,                3 },
    { "host",                HOST,                4 },
    { "hostip",              HOSTIP,              5 },
    { "icmp",                ICMP,                2 },
    { "id",                  ID,                  2 },
    { "import",              IMPORT,              2 },
    { "info",                INFO,                4 },
    { "infoshow",            INFOSHOW,            5 },
    { "inline",              INLINE,              3 },
    { "innervlan",           INNERVLAN,           3 },
    { "inports",             INPORTS,             3 },
    { "install",             INSTALL,             3 },
    { "int",                 INT,                 3 },
    { "interface",           INTERFACE,           6 },
    { "interval",            INTERVAL,            6 },
    { "invert",              INVERT,              3 },
    { "ip",                  IP,                  2 },
    { "ip_route_static",     IP_ROUTE_STATIC,     3 },
    { "ipconfig",            IPCONFIG,            3 },
    { "ipv6",                IPV6,                4 },
    { "ipv6_route_static",   IPV6_ROUTE_STATIC,   5 },
    { "l2",                  L2,                  2 },
    { "l2cmds",              L2CMDS,              3 },
    { "l3",                  L3,                  2 },
    { "l3cmds",              L3CMDS,              3 },
    { "l3packetlen",         L3PACKETLEN,         3 },
    { "l4",                  L4,                  2 },
    { "learning",            LEARNING,            2 },
    { "line",                LINE,                4 },
    { "link",                LINK,                4 },
    { "link_protect",        LINK_PROTECT,        5 },
    { "list",                LIST,                3 },
    { "load_balance",        LOAD_BALANCE,        3 },
    { "local",               LOCAL,               3 },
    { "log",                 LOG,                 3 },
    { "loop",                LOOP,                4 },
    { "loopback",            LOOPBACK,            5 },
    { "m",                   M,                   1 },
    { "mac",                 MAC,                 3 },
    { "mac_address",         MAC_ADDRESS,         5 },
    { "mac_ignore",          MAC_IGNORE,          5 },
    { "mask",                MASK,                3 },
    { "max",                 MAX,                 3 },
    { "mc",                  MC,                  2 },
    { "md5",                 MD5,                 2 },
    { "memory",              MEMORY,              2 },
    { "min",                 MIN,                 3 },
    { "mirror",              MIRROR,              3 },
    { "mode",                MODE,                4 },
    { "mode_route",          MODE_ROUTE,          5 },
    { "module",              MODULE,              4 },
    { "monitor",             MONITOR,             3 },
    { "name",                NAME,                2 },
    { "no",                  NO,                  2 },
    { "no_shutdown",         NO_SHUTDOWN,         4 },
    { "no_vlan",             NO_VLAN,             4 },
    { "noauth",              NOAUTH,              3 },
    { "none",                NONE,                3 },
    { "normal",              NORMAL,              3 },
    { "off",                 OFF,                 3 },
    { "offset",              OFFSET,              4 },
    { "olp",                 OLP,                 2 },
    { "om",                  OM,                  2 },
    { "on",                  ON,                  2 },
    { "opmode",              OPMODE,              2 },
    { "outervlan",           OUTERVLAN,           4 },
    { "outports",            OUTPORTS,            5 },
    { "output_interface",    OUTPUT_INTERFACE,    5 },
    { "overload",            OVERLOAD,            2 },
    { "password",            PASSWORD,            3 },
    { "pause",               PAUSE,               3 },
    { "percent",             PERCENT,             2 },
    { "platform",            PLATFORM,            2 },
    { "port",                PORT,                4 },
    { "portmap",             PORTMAP,             5 },
    { "priv",                PRIV,                3 },
    { "protocol",            PROTOCOL,            3 },
    { "psh",                 PSH,                 2 },
    { "pvlan",               PVLAN,               2 },
    { "quiet",               QUIET,               1 },
    { "r",                   R,                   1 },
    { "rate",                RATE,                2 },
    { "rcload",              RCLOAD,              2 },
    { "receive",             RECEIVE,             3 },
    { "replace",             REPLACE,             3 },
    { "reserve",             RESERVE,             5 },
    { "reset",               RESET,               5 },
    { "restart",             RESTART,             5 },
    { "restore",             RESTORE,             5 },
    { "rocommunity",         ROCOMMUNITY,         3 },
    { "route_static",        ROUTE_STATIC,        5 },
    { "routing_table",       ROUTING_TABLE,       5 },
    { "rst",                 RST,                 2 },
    { "rule",                RULE,                4 },
    { "ruleset",             RULESET,             5 },
    { "rwcommunity",         RWCOMMUNITY,         2 },
    { "rx",                  RX,                  2 },
    { "rx_fcs",              RX_FCS,              6 },
    { "rx_fcs_rate",         RX_FCS_RATE,         7 },
    { "rx_rate",             RX_RATE,             4 },
    { "rx_tx_percent",       RX_TX_PERCENT,       4 },
    { "rx_vlantag",          RX_VLANTAG,          4 },
    { "rxtx",                RXTX,                3 },
    { "s",                   S,                   1 },
    { "s320",                S320,                2 },
    { "s40",                 S40,                 2 },
    { "save",                SAVE,                2 },
    { "scan_frequency",      SCAN_FREQUENCY,      2 },
    { "select",              SELECT,              3 },
    { "sequence",            SEQUENCE,            3 },
    { "server",              SERVER,              3 },
    { "set",                 SET,                 3 },
    { "sha",                 SHA,                 3 },
    { "show",                SHOW,                3 },
    { "shutdown",            SHUTDOWN,            3 },
    { "single_fiber",        SINGLE_FIBER,        3 },
    { "sip",                 SIP,                 3 },
    { "size",                SIZE,                3 },
    { "slot",                SLOT,                2 },
    { "smac",                SMAC,                3 },
    { "smp",                 SMP,                 3 },
    { "snmp",                SNMP,                2 },
    { "soc",                 SOC,                 3 },
    { "soc",                 SOC,                 4 },
    { "speed",               SPEED,               3 },
    { "sport",               SPORT,               3 },
    { "src_dst_ip",          SRC_DST_IP,          9 },
    { "src_dst_mac",         SRC_DST_MAC,         9 },
    { "src_ip",              SRC_IP,              5 },
    { "src_mac",             SRC_MAC,             5 },
    { "st",                  ST,                  2 },
    { "start",               START,               4 },
    { "static",              STATIC,              5 },
    { "status",              STATUS,              5 },
    { "stop",                STOP,                3 },
    { "string",              STRING,              3 },
    { "syn",                 SYN,                 3 },
    { "synconfig",           SYNCONFIG,           4 },
    { "syscontact",          SYSCONTACT,          4 },
    { "syslocation",         SYSLOCATION,         4 },
    { "sysname",             SYSNAME,             4 },
    { "system",              SYSTEM,              4 },
    { "tcp",                 TCP,                 3 },
    { "tcpflag",             TCPFLAG,             4 },
    { "threshold",           THRESHOLD,           2 },
    { "timeout",             TIMEOUT,             5 },
    { "timezone",            TIMEZONE,            5 },
    { "to",                  TO,                  2 },
    { "trace",               TRACE,               4 },
    { "traffic",             TRAFFIC,             4 },
    { "transceiving",        TRANSCEIVING,        4 },
    { "trap",                TRAP,                4 },
    { "trunk",               TRUNK,               3 },
    { "tx",                  TX,                  2 },
    { "tx_fcs",              TX_FCS,              6 },
    { "tx_fcs_rate",         TX_FCS_RATE,         7 },
    { "tx_rate",             TX_RATE,             5 },
    { "tx_rx_percent",       TX_RX_PERCENT,       5 },
    { "type",                TYPE,                2 },
    { "ud0",                 UD0,                 3 },
    { "ud1",                 UD1,                 3 },
    { "ud2",                 UD2,                 3 },
    { "ud3",                 UD3,                 3 },
    { "ud4",                 UD4,                 3 },
    { "ud5",                 UD5,                 3 },
    { "ud6",                 UD6,                 3 },
    { "ud7",                 UD7,                 3 },
    { "udp",                 UDP,                 3 },
    { "undo",                UNDO,                3 },
    { "uninstall",           UNINSTALL,           3 },
    { "untag",               UNTAG,               3 },
    { "up",                  UP,                  2 },
    { "urg",                 URG,                 3 },
    { "url",                 URL,                 3 },
    { "username",            USERNAME,            2 },
    { "value",               VALUE,               2 },
    { "verbose",             VERBOSE,             4 },
    { "version",             VERSION,             4 },
    { "vlan",                VLAN,                4 },
    { "vlan_filter",         VLAN_FILTER,         6 },
    { "vlan_mapping",        VLAN_MAPPING,        6 },
    { "vlan_s",              VLAN_S,              6 },
    { "vlanmask",            VLANMASK,            5 },
    { "warm",                WARM,                4 },
    { "warn",                WARN,                4 },
    { "weight",              WEIGHT,              6 },
    { "weight",              WEIGHT,              7 },
    { NULL,                  0,                   0 }
};

int str_match(char *str1, char *str2, int min_match)
{
	int len = 0;

	while ((*str1 != '\0') && (*str2 != '\0')) {
		if (toupper(*str1) != toupper(*str2)) {
			return 1;
		}
		str1++;
		str2++;
		len++;
	}

	if ((*str1 == '\0') && (*str2 != '\0')) {
		return 2;
	}

	return !(len >= min_match);
}

int find_token(char *input)
{
	token_ent *token;

	token = token_table;
	while (token->match_len != 0) {
		if (tolower(input[0]) == token->name[0]) {
			if (!str_match(token->name, input, token->match_len)) {
				return token->token;
			}
		}
		if (tolower(input[0]) < token->name[0])
			break;
		token++;
	}

	return 0;
}

char *back_token_str(char *input)
{
	token_ent *token;

	token = token_table;
	while (token->match_len != 0) {
		if (tolower(input[0]) == token->name[0]) {
			if (!str_match (token->name, input, token->match_len)) {
				return token->name;
			}
		}
		if (tolower(input[0]) < token->name[0])
			break;
		token++;
	}

	return (char *)NULL;
}
