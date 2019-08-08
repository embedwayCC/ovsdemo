/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_HOME_YXS_NGN_PIPE_NGN_CLIENT_PARSER_TAB_H_INCLUDED
# define YY_YY_HOME_YXS_NGN_PIPE_NGN_CLIENT_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    A9 = 258,
    AB = 259,
    ABNORMAL = 260,
    AC1200 = 261,
    ACCESS = 262,
    ACK = 263,
    ACL = 264,
    ADD = 265,
    AES = 266,
    AGING_TIME = 267,
    ALL = 268,
    APPLICATION = 269,
    AUTH = 270,
    AUTO = 271,
    AUTONEG = 272,
    AVAILABLE = 273,
    B = 274,
    BACKPLANE = 275,
    BOARD = 276,
    BY = 277,
    CHANGE_TTL = 278,
    CHASSIS = 279,
    CLEAR = 280,
    CONCATENATE = 281,
    CONFIGURATION = 282,
    CONNECT = 283,
    COUNTER = 284,
    CPU = 285,
    CREATE = 286,
    CROSS = 287,
    CURRENT = 288,
    D = 289,
    DATETIME = 290,
    DEFAULT = 291,
    DEFAULT_PVLAN = 292,
    DELETE = 293,
    DES = 294,
    DESCRIPT = 295,
    DESTROY = 296,
    DEVICE = 297,
    DIP = 298,
    DISABLE = 299,
    DISCONNECT = 300,
    DMAC = 301,
    DPORT = 302,
    DROP = 303,
    DST_IP = 304,
    DST_MAC = 305,
    DUAL_DUAL_STAR = 306,
    DUAL_STAR = 307,
    DUPLEX = 308,
    DYNAMIC = 309,
    ENABLE = 310,
    ENTRY = 311,
    EOS = 312,
    ERROR = 313,
    ETHER = 314,
    ETHERTYPE = 315,
    EXIT = 316,
    EXITSHELL = 317,
    EXPORT = 318,
    F = 319,
    FAILOVER = 320,
    FCS = 321,
    FCS_RATE = 322,
    FIN = 323,
    FIVE_TUPLE = 324,
    FORCE = 325,
    FPGA = 326,
    FR_16XE_FB_11QXE_2DXE = 327,
    FR_20XE_FB_9QXE_4DXE = 328,
    FR_24XE_FB_8QXE_4DXE = 329,
    FROM = 330,
    FTP = 331,
    FULL = 332,
    FW = 333,
    GATEWAY = 334,
    GLOBAL = 335,
    GRULE = 336,
    HALF = 337,
    HASH = 338,
    HEAD = 339,
    HEARTBEAT = 340,
    HELP = 341,
    HOST = 342,
    HOSTIP = 343,
    ICMP = 344,
    ID = 345,
    IMPORT = 346,
    INFO = 347,
    INFOSHOW = 348,
    INLINE = 349,
    INNERVLAN = 350,
    INPORTS = 351,
    INSTALL = 352,
    INT = 353,
    INTERFACE = 354,
    INTERVAL = 355,
    INVERT = 356,
    IP = 357,
    IP_ROUTE_STATIC = 358,
    IPCONFIG = 359,
    IPV6 = 360,
    IPV6_ROUTE_STATIC = 361,
    L2 = 362,
    L2CMDS = 363,
    L3 = 364,
    L3CMDS = 365,
    L3PACKETLEN = 366,
    L4 = 367,
    LEARNING = 368,
    LINE = 369,
    LINK = 370,
    LINK_PROTECT = 371,
    LIST = 372,
    LOAD_BALANCE = 373,
    LOCAL = 374,
    LOG = 375,
    LOOP = 376,
    LOOPBACK = 377,
    M = 378,
    MAC = 379,
    MAC_ADDRESS = 380,
    MAC_IGNORE = 381,
    MASK = 382,
    MAX = 383,
    MC = 384,
    MD5 = 385,
    MEMORY = 386,
    MIN = 387,
    MIRROR = 388,
    MODE = 389,
    MODE_ROUTE = 390,
    MODULE = 391,
    MONITOR = 392,
    NAME = 393,
    NO = 394,
    NO_SHUTDOWN = 395,
    NO_VLAN = 396,
    NOAUTH = 397,
    NONE = 398,
    NORMAL = 399,
    OFF = 400,
    OFFSET = 401,
    OLP = 402,
    OM = 403,
    ON = 404,
    OPMODE = 405,
    OUTERVLAN = 406,
    OUTPORTS = 407,
    OUTPUT_INTERFACE = 408,
    OVERLOAD = 409,
    PASSWORD = 410,
    PAUSE = 411,
    PERCENT = 412,
    PLATFORM = 413,
    PORT = 414,
    PORTMAP = 415,
    PRIV = 416,
    PROTOCOL = 417,
    PSH = 418,
    PVLAN = 419,
    QUIET = 420,
    R = 421,
    RATE = 422,
    RCLOAD = 423,
    RECEIVE = 424,
    REPLACE = 425,
    RESERVE = 426,
    RESET = 427,
    RESTART = 428,
    RESTORE = 429,
    ROCOMMUNITY = 430,
    ROUTE_STATIC = 431,
    ROUTING_TABLE = 432,
    RST = 433,
    RULE = 434,
    RULESET = 435,
    RWCOMMUNITY = 436,
    RX = 437,
    RX_FCS = 438,
    RX_FCS_RATE = 439,
    RX_RATE = 440,
    RX_TX_PERCENT = 441,
    RX_VLANTAG = 442,
    RXTX = 443,
    S = 444,
    S320 = 445,
    S40 = 446,
    SAVE = 447,
    SCAN_FREQUENCY = 448,
    SELECT = 449,
    SEQUENCE = 450,
    SERVER = 451,
    SET = 452,
    SHA = 453,
    SHOW = 454,
    SHUTDOWN = 455,
    SINGLE_FIBER = 456,
    SIP = 457,
    SIZE = 458,
    SLOT = 459,
    SMAC = 460,
    SMP = 461,
    SNMP = 462,
    SOC = 463,
    SPEED = 464,
    SPORT = 465,
    SRC_DST_IP = 466,
    SRC_DST_MAC = 467,
    SRC_IP = 468,
    SRC_MAC = 469,
    ST = 470,
    START = 471,
    STATIC = 472,
    STATUS = 473,
    STOP = 474,
    STRING = 475,
    SYN = 476,
    SYNCONFIG = 477,
    SYSCONTACT = 478,
    SYSLOCATION = 479,
    SYSNAME = 480,
    SYSTEM = 481,
    TCP = 482,
    TCPFLAG = 483,
    THRESHOLD = 484,
    TIMEOUT = 485,
    TIMEZONE = 486,
    TO = 487,
    TRACE = 488,
    TRAFFIC = 489,
    TRANSCEIVING = 490,
    TRAP = 491,
    TRUNK = 492,
    TX = 493,
    TX_FCS = 494,
    TX_FCS_RATE = 495,
    TX_RATE = 496,
    TX_RX_PERCENT = 497,
    TYPE = 498,
    UD0 = 499,
    UD1 = 500,
    UD2 = 501,
    UD3 = 502,
    UD4 = 503,
    UD5 = 504,
    UD6 = 505,
    UD7 = 506,
    UDP = 507,
    UNDO = 508,
    UNINSTALL = 509,
    UNTAG = 510,
    UP = 511,
    URG = 512,
    URL = 513,
    USERNAME = 514,
    VALUE = 515,
    VERBOSE = 516,
    VERSION = 517,
    VLAN = 518,
    VLAN_FILTER = 519,
    VLAN_MAPPING = 520,
    VLAN_S = 521,
    VLANMASK = 522,
    WARM = 523,
    WARN = 524,
    WEIGHT = 525,
    _INT_ = 526,
    _NAME_ = 527,
    _STRING_ = 528,
    _URL_ = 529,
    _PWD_STRING_ = 530,
    _IPADDR_ = 531,
    _IPV6ADDR_ = 532,
    _MAC_ = 533,
    _TIME_FORMAT_ = 534,
    _SMPS_ = 535,
    _MPS_ = 536
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 136 "parser.y" /* yacc.c:1909  */

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

#line 355 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_YXS_NGN_PIPE_NGN_CLIENT_PARSER_TAB_H_INCLUDED  */
