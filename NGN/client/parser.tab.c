/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */


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



#line 179 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
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
#line 136 "parser.y" /* yacc.c:355  */

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

#line 520 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_YXS_NGN_PIPE_NGN_CLIENT_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 537 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   363

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  288
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  175
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  392

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   536

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   284,   283,     2,   285,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   282,     2,
       2,   286,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   287,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   192,   192,   193,   202,   206,   210,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   238,
     242,   246,   250,   256,   287,   292,   297,   302,   307,   312,
     339,   344,   349,   354,   359,   366,   371,   376,   381,   387,
     399,   404,   409,   414,   419,   424,   430,   435,   439,   444,
     448,   453,   470,   486,   521,   538,   543,   550,   567,   635,
     761,   766,   771,   780,   784,   799,   814,   829,   844,   850,
     854,   860,   866,   871,   875,   880,   917,   924,  1468,  1473,
    1477,  1481,  1486,  1491,  1496,  1504,  1516,  1552,  1593,  1718,
    1723,  1728,  1738,  1743,  1748,  1753,  1758,  1763,  1768,  1773,
    1779,  1785,  1791,  1801,  1807,  1813,  1819,  1825,  1831,  1837,
    1850,  1854,  1866,  1885,  1891,  1907,  1940,  1945,  1953,  1956,
    1960,  1964,  1968,  1972,  1976,  1980,  1984,  1991,  1996,  2001,
    2006,  2014,  2019,  2024,  2029,  2034,  2039,  2044,  2049,  2054,
    2062,  2067,  2075,  2080,  2088,  2093,  2098,  2106,  2130,  2153,
    2158,  2163,  2168,  2173,  2178,  2186,  2201,  2227,  2232,  2269,
    2274,  2282,  2286,  2314,  2319,  2948,  2953,  2960,  2968,  2973,
    2981,  2987,  3161,  3164,  3181,  3186
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "A9", "AB", "ABNORMAL", "AC1200",
  "ACCESS", "ACK", "ACL", "ADD", "AES", "AGING_TIME", "ALL", "APPLICATION",
  "AUTH", "AUTO", "AUTONEG", "AVAILABLE", "B", "BACKPLANE", "BOARD", "BY",
  "CHANGE_TTL", "CHASSIS", "CLEAR", "CONCATENATE", "CONFIGURATION",
  "CONNECT", "COUNTER", "CPU", "CREATE", "CROSS", "CURRENT", "D",
  "DATETIME", "DEFAULT", "DEFAULT_PVLAN", "DELETE", "DES", "DESCRIPT",
  "DESTROY", "DEVICE", "DIP", "DISABLE", "DISCONNECT", "DMAC", "DPORT",
  "DROP", "DST_IP", "DST_MAC", "DUAL_DUAL_STAR", "DUAL_STAR", "DUPLEX",
  "DYNAMIC", "ENABLE", "ENTRY", "EOS", "ERROR", "ETHER", "ETHERTYPE",
  "EXIT", "EXITSHELL", "EXPORT", "F", "FAILOVER", "FCS", "FCS_RATE", "FIN",
  "FIVE_TUPLE", "FORCE", "FPGA", "FR_16XE_FB_11QXE_2DXE",
  "FR_20XE_FB_9QXE_4DXE", "FR_24XE_FB_8QXE_4DXE", "FROM", "FTP", "FULL",
  "FW", "GATEWAY", "GLOBAL", "GRULE", "HALF", "HASH", "HEAD", "HEARTBEAT",
  "HELP", "HOST", "HOSTIP", "ICMP", "ID", "IMPORT", "INFO", "INFOSHOW",
  "INLINE", "INNERVLAN", "INPORTS", "INSTALL", "INT", "INTERFACE",
  "INTERVAL", "INVERT", "IP", "IP_ROUTE_STATIC", "IPCONFIG", "IPV6",
  "IPV6_ROUTE_STATIC", "L2", "L2CMDS", "L3", "L3CMDS", "L3PACKETLEN", "L4",
  "LEARNING", "LINE", "LINK", "LINK_PROTECT", "LIST", "LOAD_BALANCE",
  "LOCAL", "LOG", "LOOP", "LOOPBACK", "M", "MAC", "MAC_ADDRESS",
  "MAC_IGNORE", "MASK", "MAX", "MC", "MD5", "MEMORY", "MIN", "MIRROR",
  "MODE", "MODE_ROUTE", "MODULE", "MONITOR", "NAME", "NO", "NO_SHUTDOWN",
  "NO_VLAN", "NOAUTH", "NONE", "NORMAL", "OFF", "OFFSET", "OLP", "OM",
  "ON", "OPMODE", "OUTERVLAN", "OUTPORTS", "OUTPUT_INTERFACE", "OVERLOAD",
  "PASSWORD", "PAUSE", "PERCENT", "PLATFORM", "PORT", "PORTMAP", "PRIV",
  "PROTOCOL", "PSH", "PVLAN", "QUIET", "R", "RATE", "RCLOAD", "RECEIVE",
  "REPLACE", "RESERVE", "RESET", "RESTART", "RESTORE", "ROCOMMUNITY",
  "ROUTE_STATIC", "ROUTING_TABLE", "RST", "RULE", "RULESET", "RWCOMMUNITY",
  "RX", "RX_FCS", "RX_FCS_RATE", "RX_RATE", "RX_TX_PERCENT", "RX_VLANTAG",
  "RXTX", "S", "S320", "S40", "SAVE", "SCAN_FREQUENCY", "SELECT",
  "SEQUENCE", "SERVER", "SET", "SHA", "SHOW", "SHUTDOWN", "SINGLE_FIBER",
  "SIP", "SIZE", "SLOT", "SMAC", "SMP", "SNMP", "SOC", "SPEED", "SPORT",
  "SRC_DST_IP", "SRC_DST_MAC", "SRC_IP", "SRC_MAC", "ST", "START",
  "STATIC", "STATUS", "STOP", "STRING", "SYN", "SYNCONFIG", "SYSCONTACT",
  "SYSLOCATION", "SYSNAME", "SYSTEM", "TCP", "TCPFLAG", "THRESHOLD",
  "TIMEOUT", "TIMEZONE", "TO", "TRACE", "TRAFFIC", "TRANSCEIVING", "TRAP",
  "TRUNK", "TX", "TX_FCS", "TX_FCS_RATE", "TX_RATE", "TX_RX_PERCENT",
  "TYPE", "UD0", "UD1", "UD2", "UD3", "UD4", "UD5", "UD6", "UD7", "UDP",
  "UNDO", "UNINSTALL", "UNTAG", "UP", "URG", "URL", "USERNAME", "VALUE",
  "VERBOSE", "VERSION", "VLAN", "VLAN_FILTER", "VLAN_MAPPING", "VLAN_S",
  "VLANMASK", "WARM", "WARN", "WEIGHT", "_INT_", "_NAME_", "_STRING_",
  "_URL_", "_PWD_STRING_", "_IPADDR_", "_IPV6ADDR_", "_MAC_",
  "_TIME_FORMAT_", "_SMPS_", "_MPS_", "':'", "'-'", "','", "'/'", "'='",
  "'|'", "$accept", "input", "line", "command", "infocmds", "ifcmds",
  "trunkcmds", "vlan_id", "inports_id", "outports_type", "outports_id",
  "l2cmds", "rulecmds", "syscmds", "helpcmds", "snmpcmds", "logcmds",
  "vermgrcmds", "olpcmds", "slot_info", "olpid", "olp_propertys",
  "olp_property", "olp_device", "link_protect_types", "link_protect_type",
  "snmp_op", "snmp_param", "snmp_auth_type", "snmp_priv_type",
  "snmp_access", "smps", "psc", "ip_addr", "mask", "onoff", "able",
  "verbose", "duplex", "if_type", "ip", "shutdown", "slot_id", "quiet",
  "pwd_str", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,    58,    45,    44,    47,    61,   124
};
# endif

#define YYPACT_NINF -245

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-245)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -245,    19,  -245,    22,   -40,    15,   -42,   -37,  -245,    40,
      46,    25,  -204,    80,  -204,   -71,  -185,     7,   110,   111,
      -7,     5,    33,  -134,  -130,  -245,  -245,  -245,  -245,  -245,
    -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -128,
    -125,    91,  -155,  -245,  -245,  -122,  -128,  -125,  -245,  -245,
      74,  -245,    96,    78,  -116,  -107,    -8,  -245,  -245,    98,
    -130,    -3,   130,   112,    11,  -128,  -155,   -59,   -76,   113,
     -12,  -155,   -10,   114,   -69,   -68,  -245,   145,    -2,   148,
    -245,   123,  -245,   124,   125,   -88,  -245,  -155,  -245,   -36,
    -245,  -245,  -245,   157,   131,   -13,   -11,   -89,  -245,   -89,
    -245,   133,  -245,   -79,   -79,    -1,    -9,    24,     4,  -245,
     137,  -245,   141,    -3,  -245,  -245,  -245,   -57,   158,    38,
       0,    99,  -100,   162,   168,   135,  -245,   179,   180,    -5,
     181,   184,  -245,   143,   144,  -245,    16,  -121,  -115,  -245,
    -245,  -245,  -245,  -245,   -19,  -245,   -25,   196,  -245,  -245,
    -245,   197,   198,  -245,  -245,  -155,   199,  -245,   201,   203,
     206,   208,  -245,     2,     6,  -245,     8,    -6,   214,   -79,
      31,   -79,    75,     1,     1,   -78,   146,   149,   -79,  -245,
    -245,   219,   220,  -245,    30,   -80,    17,    27,   -44,  -245,
     -80,  -245,    13,   223,     3,     3,    32,  -245,  -245,   225,
    -245,  -245,    28,   228,   229,  -245,  -245,    12,  -204,  -245,
    -245,  -245,   233,  -245,   -85,  -245,    57,  -245,   235,  -245,
    -245,  -245,   -50,  -245,  -245,  -245,  -245,  -245,    20,    21,
      34,  -245,    10,   238,  -245,  -245,  -155,   -79,  -155,   -79,
    -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,   -80,    14,
     -80,    35,    36,  -123,  -109,   240,  -245,  -245,    37,   242,
    -245,  -245,   243,  -245,  -245,   244,    29,  -245,  -245,   245,
     246,   247,  -245,  -245,  -245,  -245,  -245,  -245,  -245,   252,
     253,    29,  -245,  -245,   254,  -245,  -245,   255,   256,  -245,
    -245,  -245,   257,  -245,  -245,   258,  -245,  -245,    45,    44,
      47,  -245,    48,  -245,   261,     9,   262,   -84,   265,     1,
     266,   267,   268,    55,    56,    58,    59,    60,    62,    64,
    -245,  -245,   271,  -245,  -245,  -245,   279,  -245,  -245,  -245,
    -245,  -245,   280,  -245,  -245,  -245,  -245,  -245,   281,   -49,
     -45,   297,  -245,   283,   284,  -245,   -44,   285,  -245,  -245,
    -245,  -245,  -245,   286,   287,   288,   289,   290,   291,   292,
    -245,  -245,  -245,  -245,  -245,   293,  -245,   294,    66,  -245,
    -245,   296,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,
    -245,  -245,    83,  -245,  -245,   259,    69,    81,   200,    70,
      87,  -245
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,   170,     0,     4,     0,
       0,     0,     0,     0,     0,   110,     0,     0,     0,     0,
       0,     0,     0,     0,   170,     3,     5,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,     6,     0,
       0,     0,     0,    69,   171,     0,     0,     0,    18,    73,
       0,   155,     0,     0,     0,     0,     0,   174,   175,     0,
     170,   172,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   128,     0,     0,     0,
     127,     0,   129,     0,     0,     0,    54,     0,    57,     0,
      63,   147,   148,     0,     0,     0,     0,     0,    75,     0,
     156,     0,   111,     0,     0,     0,     0,     0,     0,    76,
       0,   173,     0,   172,    62,   160,   159,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,    20,     0,     0,   130,     0,     0,     0,   134,
     135,   132,   133,   131,     0,   137,     0,     0,    79,    78,
      68,     0,     0,    55,    56,     0,     0,    70,     0,     0,
       0,     0,   167,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,   118,   118,     0,     0,     0,     0,    72,
      60,     0,     0,    44,     0,     0,     0,     0,     0,   169,
       0,   168,     0,     0,     0,     0,     0,    21,    23,     0,
      49,    48,   161,     0,     0,    47,    46,     0,     0,   145,
     144,   146,     0,   138,     0,   139,     0,   136,     0,    80,
      71,    35,     0,    33,    37,    36,    41,    40,     0,     0,
       0,    89,     0,     0,   113,    91,     0,     0,     0,     0,
     123,   126,   125,   119,   121,   124,   120,   122,     0,   116,
       0,     0,     0,     0,     0,     0,    61,    45,     0,     0,
     158,   157,     0,   163,   164,     0,     0,   166,   165,     0,
       0,     0,    26,   149,   150,   153,   154,   151,   152,     0,
       0,     0,    22,   162,     0,    30,    31,     0,     0,    83,
     140,   141,     0,   143,   142,     0,    84,    38,     0,     0,
       0,   112,     0,    90,     0,     0,     0,     0,     0,   118,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,    53,     0,    50,    24,    28,     0,    34,    25,    29,
      43,    42,     0,    32,    85,    86,    81,    82,     0,     0,
       0,     0,    92,     0,     0,    93,     0,     0,    97,   117,
      96,   108,   109,     0,     0,     0,     0,     0,     0,     0,
      51,    27,    52,    39,    66,     0,    64,     0,     0,   107,
     106,     0,    94,   101,    99,   102,   100,   104,   103,   105,
      67,    65,     0,    95,   115,     0,     0,     0,     0,     0,
       0,   114
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,    18,  -245,
      77,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,
      95,  -245,  -245,  -245,  -144,  -245,  -245,  -245,  -245,  -245,
    -245,   -60,   165,    -4,  -245,  -142,  -145,  -245,  -245,  -244,
     263,  -245,    26,   250,  -245
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    25,    26,    27,    28,    29,   322,    87,   155,
      89,    30,    31,    32,    33,    34,    35,    36,    37,    56,
     167,   233,   234,   385,   248,   249,    83,   146,   292,   295,
     212,    93,   279,    52,   101,   262,   118,   284,   265,   269,
     163,   193,    45,   112,    59
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     158,   127,   160,   130,   173,   136,   120,   297,   364,   169,
      54,   129,   366,   137,   240,    43,   123,   185,   195,     2,
       3,   121,   251,   103,   202,   170,    68,   152,    60,     4,
     250,   209,    69,   104,   213,   159,   161,   171,   267,   259,
     215,   343,    41,    74,     5,   290,   273,   153,   270,   274,
      85,   231,    50,   186,     6,   115,    39,     7,    64,    46,
     268,   313,   314,   347,    95,   260,   116,   241,   293,   261,
     187,    75,    51,   115,   115,   317,     8,   318,   267,    38,
       9,    10,    11,   119,   116,   116,   110,   203,    57,    65,
      58,   105,    66,   154,   263,   222,   294,    48,    12,   264,
     268,    70,   371,    49,    71,   275,   308,    53,   310,   175,
      13,   117,    40,   291,    42,    47,   242,   315,   176,   316,
     258,   326,   214,    14,    96,    91,    92,   276,   216,   106,
     344,   319,   232,    55,   188,    15,   332,    62,    63,    84,
     189,    44,   124,    86,   122,    67,    88,   128,    90,   131,
      97,    94,   252,    98,    99,   109,   190,    72,   210,   138,
     100,   177,   111,   196,   102,   349,   113,   133,   134,   114,
     126,   132,   135,   139,    16,   147,   304,   211,   306,   140,
     148,   149,   150,   243,   244,   151,   156,   162,   157,   107,
     165,    17,   166,    18,   179,   125,    91,    92,   180,   168,
     191,   174,   184,   178,   288,   277,    76,    77,   278,   192,
     108,    19,    88,   204,   182,   183,    20,   194,    21,   197,
     298,   141,   142,   143,   365,   198,    22,   199,   367,    44,
      78,    73,    79,    61,   144,   245,   200,   201,   205,   246,
     247,   206,   172,   217,    23,   207,   208,   346,   218,    80,
      24,    81,    82,   219,   220,   221,   223,   145,   224,    88,
     225,    88,   166,   226,   236,   227,   238,    91,    92,    91,
      92,   235,   239,   255,   237,   228,   256,   257,   253,   229,
     272,   254,   282,   266,   271,   285,   286,   281,   287,   283,
     289,   230,   296,   299,   300,   303,   302,   320,   309,   323,
     324,   325,   327,   328,   329,   301,   311,   312,   321,   330,
     331,   333,   334,   335,   336,   337,   338,   339,   342,   345,
     340,   341,   348,   350,   351,   352,   353,   354,   360,   355,
     356,   357,   305,   358,   307,   359,   361,   362,   363,   368,
     369,   370,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   387,   390,   388,   391,   389,
     280,   386,   164,   181
};

static const yytype_uint16 yycheck[] =
{
      13,    13,    13,    13,    13,     7,    66,    57,    57,    10,
      14,    71,    57,    15,    13,    57,    92,    17,   118,     0,
       1,    80,   100,    31,    29,    26,    21,    87,    21,    10,
     174,    15,    27,    41,   155,    95,    96,    38,   122,   184,
     155,    32,    27,    10,    25,   130,    43,    83,   190,    46,
      24,    57,    27,    53,    35,    44,    96,    38,    65,    96,
     144,   184,   185,   307,    46,   145,    55,    66,    11,   149,
      70,    38,   276,    44,    44,   184,    57,   186,   122,    57,
      61,    62,    63,    65,    55,    55,    60,    92,   273,    96,
     275,    99,    99,   129,    77,   155,    39,    57,    79,    82,
     144,    96,   346,    57,    99,   102,   248,    27,   250,    85,
      91,   100,   152,   198,    99,   152,   115,   240,    94,   242,
      90,   266,   243,   104,    47,   280,   281,   124,   243,   137,
     121,   240,   138,   204,   134,   116,   281,    27,    27,   273,
     140,   271,   218,   271,    67,   152,   271,    70,    57,    72,
      76,   273,   230,    57,    76,    57,   156,   152,   142,   161,
     276,   137,   165,   263,   271,   309,    36,   236,   236,    57,
      57,    57,    27,   175,   155,    27,   236,   161,   238,   181,
      57,    57,    57,   182,   183,   273,    29,   276,    57,   197,
      57,   172,   271,   174,    57,   271,   280,   281,    57,   104,
     200,   106,   164,   199,   208,   202,   173,   174,   205,   209,
     218,   192,   271,   218,   271,    57,   197,   118,   199,    57,
     270,   223,   224,   225,   273,    57,   207,    92,   273,   271,
     197,   226,   199,   226,   236,   234,    57,    57,    57,   238,
     239,    57,   243,   262,   225,   102,   102,   307,   273,   216,
     231,   218,   219,    57,    57,    57,    57,   259,    57,   271,
      57,   271,   271,    57,   169,    57,   171,   280,   281,   280,
     281,    57,   197,   178,   243,   273,    57,    57,   132,   273,
      57,   132,    57,   256,   271,    57,    57,   255,   276,   261,
      57,   283,    57,   273,   273,    57,   286,    57,   284,    57,
      57,    57,    57,    57,    57,   271,   271,   271,   271,    57,
      57,    57,    57,    57,    57,    57,   271,   273,    57,    57,
     273,   273,    57,    57,    57,    57,   271,   271,    57,   271,
     271,   271,   237,   271,   239,   271,    57,    57,    57,    42,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,   286,    57,   271,   286,   286,   276,   271,   159,
     195,   102,    99,   113
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   289,     0,     1,    10,    25,    35,    38,    57,    61,
      62,    63,    79,    91,   104,   116,   155,   172,   174,   192,
     197,   199,   207,   225,   231,   290,   291,   292,   293,   294,
     299,   300,   301,   302,   303,   304,   305,   306,    57,    96,
     152,    27,    99,    57,   271,   330,    96,   152,    57,    57,
      27,   276,   321,    27,   321,   204,   307,   273,   275,   332,
      21,   226,    27,    27,    65,    96,    99,   152,    21,    27,
      96,    99,   152,   226,    10,    38,   173,   174,   197,   199,
     216,   218,   219,   314,   273,   330,   271,   296,   271,   298,
      57,   280,   281,   319,   273,   296,   298,    76,    57,    76,
     276,   322,   271,    31,    41,    99,   137,   197,   218,    57,
     330,   165,   331,    36,    57,    44,    55,   100,   324,   296,
     319,    80,   298,    92,   218,   271,    57,    13,   298,   319,
      13,   298,    57,   236,   236,    27,     7,    15,   161,   175,
     181,   223,   224,   225,   236,   259,   315,    27,    57,    57,
      57,   273,   319,    83,   129,   297,    29,    57,    13,   319,
      13,   319,   276,   328,   328,    57,   271,   308,   308,    10,
      26,    38,   243,    13,   308,    85,    94,   137,   199,    57,
      57,   331,   271,    57,   164,    17,    53,    70,   134,   140,
     156,   200,   209,   329,   118,   118,   263,    57,    57,    92,
      57,    57,    29,    92,   218,    57,    57,   102,   102,    15,
     142,   161,   318,   155,   243,   155,   243,   262,   273,    57,
      57,    57,   319,    57,    57,    57,    57,    57,   273,   273,
     283,    57,   138,   309,   310,    57,   308,   243,   308,   197,
      13,    66,   115,   182,   183,   234,   238,   239,   312,   313,
     312,   100,   230,   132,   132,   308,    57,    57,    90,   324,
     145,   149,   323,    77,    82,   326,   256,   122,   144,   327,
     323,   271,    57,    43,    46,   102,   124,   202,   205,   320,
     320,   255,    57,   261,   325,    57,    57,   276,   321,    57,
     130,   198,   316,    11,    39,   317,    57,    57,   270,   273,
     273,   271,   286,    57,   319,   308,   319,   308,   323,   284,
     323,   271,   271,   184,   185,   240,   242,   184,   186,   240,
      57,   271,   295,    57,    57,    57,   324,    57,    57,    57,
      57,    57,   324,    57,    57,    57,    57,    57,   271,   273,
     273,   273,    57,    32,   121,    57,   319,   327,    57,   312,
      57,    57,    57,   271,   271,   271,   271,   271,   271,   271,
      57,    57,    57,    57,    57,   273,    57,   273,    42,    57,
      57,   327,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,   286,    57,   271,   311,   102,   286,   276,   159,
     286,   271
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   288,   289,   289,   290,   290,   290,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   292,
     292,   292,   292,   292,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   295,   296,   297,   297,   298,   299,   300,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   302,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   304,   305,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     307,   307,   308,   309,   310,   311,   312,   312,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   314,   314,   314,
     314,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     316,   316,   317,   317,   318,   318,   318,   319,   319,   320,
     320,   320,   320,   320,   320,   321,   322,   323,   323,   324,
     324,   325,   325,   326,   326,   327,   327,   328,   329,   329,
     330,   330,   331,   331,   332,   332
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       3,     4,     5,     4,     6,     6,     5,     7,     6,     6,
       5,     5,     6,     5,     6,     5,     5,     5,     6,     8,
       5,     5,     6,     6,     4,     5,     4,     4,     4,     4,
       6,     7,     7,     1,     1,     1,     1,     1,     0,     0,
       4,     5,     3,     3,     8,     9,     8,     9,     3,     2,
       4,     4,     4,     2,     4,     3,     3,     0,     3,     3,
       4,     6,     6,     5,     5,     6,     6,     0,     0,     5,
       6,     5,     7,     7,     8,     9,     7,     7,     6,     8,
       8,     8,     8,     8,     8,     8,     8,     8,     7,     7,
       0,     2,     3,     1,    12,     1,     1,     3,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     2,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 194 "parser.y" /* yacc.c:1646  */
    {
        if (end_of_line) {
            printPrompt();
        }
    }
#line 1959 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 203 "parser.y" /* yacc.c:1646  */
    {
        cli_reset_command();
    }
#line 1967 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 207 "parser.y" /* yacc.c:1646  */
    {
        cli_reset_command();
    }
#line 1975 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 211 "parser.y" /* yacc.c:1646  */
    {
        cli_reset_command();
        yyerrok;
        EN = -1;
    }
#line 1985 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 231 "parser.y" /* yacc.c:1646  */
    {
        YYACCEPT;
    }
#line 1993 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 239 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_CONFIG);
    }
#line 2001 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 243 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_SYSTEM);
    }
#line 2009 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 247 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_BOARDINFO);
    }
#line 2017 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 251 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json((yyvsp[-2].ival));
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_BOARDINFO);
    }
#line 2027 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 257 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_BOARDSTATUS);
    }
#line 2035 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 288 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_AUTONEG);
    }
#line 2044 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 293 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_PAUSE);
    }
#line 2053 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 298 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_SHUTDOWN);
    }
#line 2062 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 303 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_SINGLEFIBER_TRANSMIT);
    }
#line 2071 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 308 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_DUPLEX);
    }
#line 2080 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 313 "parser.y" /* yacc.c:1646  */
    {
        if ((yyvsp[-1].ival) != 1000 && (yyvsp[-1].ival) != 10000) {
            //__printf_buf("the speed should be 1000|10000 Mbps.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_val_to_json((yyvsp[-1].ival));
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_PORT_SPEED);
        }
    }
#line 2096 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 340 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_GET_INFO);
    }
#line 2105 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 345 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_GET_STATUS);
    }
#line 2114 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 350 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_SHOW_COUNTER);
    }
#line 2123 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 355 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_CLEAR_COUNTER);
    }
#line 2132 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 360 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_INTERFACE_SET_PORTMODE);
    }
#line 2141 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 367 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_INPORTS);
    }
#line 2150 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 372 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_INPORTS);
    }
#line 2159 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 377 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_INPORTS_ALL);
    }
#line 2168 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 382 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json(1);
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_OUTPORTS);
    }
#line 2178 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 388 "parser.y" /* yacc.c:1646  */
    {
        if(((yyvsp[-1].ival)<2)||((yyvsp[-1].ival)>128))
        {
            EN=OE_RST_INVLD_PRM;
            YYERROR;
        }
		convert_val_to_json((yyvsp[-1].ival));
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_OUTPORTS);
    }
#line 2193 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 400 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_OUTPORTS);
    }
#line 2202 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 405 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_DEL_OUTPORTS_ALL);
    }
#line 2211 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 410 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_ADD_PSC);
    }
#line 2220 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 415 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_GLOBLE_LOAD_BALANCE);
    }
#line 2229 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 420 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_FAILOVER);
	}
#line 2238 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 425 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json((yyvsp[-1].ival));
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_FAILOVER_INTERVAL);
    }
#line 2248 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 431 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_OUTPORTS);
    }
#line 2257 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 436 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_OUTPORTS_ALL);
    }
#line 2265 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 440 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_INPORTS);
    }
#line 2274 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 445 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_GET_INPORTS_ALL);
    }
#line 2282 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 449 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_INPORTS_PVLAN_ENABLE);
    }
#line 2291 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 454 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_INPORTS_PVID);
    }
#line 2300 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 471 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_ACL_SET_OUTPORTS_UNTAG_ENABLE);
    }
#line 2309 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 487 "parser.y" /* yacc.c:1646  */
    {
        if ((yyvsp[0].ival) < 2 || (yyvsp[0].ival) >= 4096) {
            //__printf_buf("Vlan id should be from 2 to 4095\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        (yyval.ival) = (yyvsp[0].ival);
		convert_val_to_json((yyval.ival));
    }
#line 2324 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 522 "parser.y" /* yacc.c:1646  */
    {
        if((yyvsp[0].ival) < 1 || (yyvsp[0].ival) > MAX_INPORTS_ID )
        {
            //__printf_buf("Inport id should be from 1 to %d\n", MAX_INPORTS_ID);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        else{
            (yyval.ival) = (yyvsp[0].ival);
		}
		convert_val_to_json((yyval.ival));
    }
#line 2342 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 539 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival)=OUTPORT_HASH;
		convert_val_to_json((yyval.ival));
    }
#line 2351 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 544 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival)=OUTPORT_MC;
		convert_val_to_json((yyval.ival));
    }
#line 2360 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 551 "parser.y" /* yacc.c:1646  */
    {
        if((yyvsp[0].ival) < 1 || (yyvsp[0].ival) > MAX_OUTPORTS_ID)
        {
            //__printf_buf("Outport id should be from 1 to %d\n", MAX_OUTPORTS_ID);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
        else{
		
            (yyval.ival) = (yyvsp[0].ival);
		}
		convert_val_to_json((yyval.ival));
    }
#line 2379 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 762 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_RESET_SYSTEM);
    }
#line 2388 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 767 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_RESTORE_CONFIG);
    }
#line 2397 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 772 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_SAVE_CONFIG);
        if (EN == 0) {
            printf("save configuration success.\n");
        } else {
            printf("save configuration error.\n");
        }
    }
#line 2410 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 781 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_CLEAR_CONFIG);
    }
#line 2418 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 785 "parser.y" /* yacc.c:1646  */
    {
        if (strlen((yyvsp[-3].sptr)) == 0 || strlen((yyvsp[-2].sptr)) == 0 || strlen((yyvsp[-1].sptr)) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json((yyvsp[-3].sptr));
			convert_spec_str_to_json((yyvsp[-2].sptr));
			convert_spec_str_to_json((yyvsp[-1].sptr));
			convert_spec_str_to_json(".");
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_IMPORT_CONFIG);
        }
    }
#line 2437 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 800 "parser.y" /* yacc.c:1646  */
    {
        if (strlen((yyvsp[-4].sptr)) == 0 || strlen((yyvsp[-3].sptr)) == 0 || strlen((yyvsp[-2].sptr)) == 0 || strlen((yyvsp[-1].sptr)) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json((yyvsp[-4].sptr));
			convert_spec_str_to_json((yyvsp[-3].sptr));
			convert_spec_str_to_json((yyvsp[-2].sptr));
			convert_spec_str_to_json((yyvsp[-1].sptr));
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_IMPORT_CONFIG);
        }
    }
#line 2456 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 815 "parser.y" /* yacc.c:1646  */
    {
        if (strlen((yyvsp[-3].sptr)) == 0 || strlen((yyvsp[-2].sptr)) == 0 || strlen((yyvsp[-1].sptr)) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json((yyvsp[-3].sptr));
			convert_spec_str_to_json((yyvsp[-2].sptr));
			convert_spec_str_to_json((yyvsp[-1].sptr));
			convert_spec_str_to_json(".");
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_EXPORT_CONFIG);
        }
    }
#line 2475 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 830 "parser.y" /* yacc.c:1646  */
    {
        if (strlen((yyvsp[-4].sptr)) == 0 || strlen((yyvsp[-3].sptr)) == 0 || strlen((yyvsp[-2].sptr)) == 0 || strlen((yyvsp[-1].sptr)) == 0) {
            //__printf_buf("The string type param must be not empty.\n");
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
        } else {
			convert_spec_str_to_json((yyvsp[-4].sptr));
			convert_spec_str_to_json((yyvsp[-3].sptr));
			convert_spec_str_to_json((yyvsp[-2].sptr));
			convert_spec_str_to_json((yyvsp[-1].sptr));
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_OMSERVER_EXPORT_CONFIG);
        }
    }
#line 2494 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 845 "parser.y" /* yacc.c:1646  */
    {
		convert_spec_str_to_json((yyvsp[-1].sptr));
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_SYSNAME);
    }
#line 2504 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 851 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GET_DATETIME);
    }
#line 2512 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 855 "parser.y" /* yacc.c:1646  */
    {
		convert_spec_str_to_json((yyvsp[-1].sptr));
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_DATEIIME);
    }
#line 2522 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 861 "parser.y" /* yacc.c:1646  */
    {
		convert_spec_str_to_json((yyvsp[-1].sptr));
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_TIMEZONE);
    }
#line 2532 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 867 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_RESET_BOARD);
    }
#line 2541 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 872 "parser.y" /* yacc.c:1646  */
    {
        EN = 0;
    }
#line 2549 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 876 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_IPCONFIG);
    }
#line 2558 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 881 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_GATEWAY);
    }
#line 2567 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 918 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SYSTEM_SET_PASSWORD);
    }
#line 2576 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1469 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_OPTION);
    }
#line 2585 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1474 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SHOW_STATUS);
    }
#line 2593 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1478 "parser.y" /* yacc.c:1646  */
    {
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SHOW_CONF);
    }
#line 2601 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1482 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
#line 2610 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1487 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
#line 2619 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1492 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
#line 2628 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1497 "parser.y" /* yacc.c:1646  */
    {
		convert_spec_str_to_json((yyvsp[-1].sptr));	
        if(oe_check_snmp_param((yyvsp[-2].ival), (yyvsp[-1].sptr)) == 0){
			close_json();
			EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
		}
    }
#line 2640 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1505 "parser.y" /* yacc.c:1646  */
    {
        char ip[64] = {0};
        struct in_addr in;
        in.s_addr = htonl((yyvsp[-1].ipaddr));

        strcpy(ip, (char *)inet_ntoa(in));
		convert_val_to_json(SNMP_ADD_TRAP_IP);
		convert_spec_str_to_json(ip);	
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
#line 2656 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1517 "parser.y" /* yacc.c:1646  */
    {
        char ip[64] = {0};
        struct in_addr in;
        in.s_addr = htonl((yyvsp[-1].ival));

        strcpy(ip, (char *)inet_ntoa(in));
        strcpy(ip, (char *)inet_ntoa(in));
		convert_val_to_json(SNMP_DEL_TRAP_IP);
		convert_spec_str_to_json(ip);	
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_SNMP_SET_CONF);
    }
#line 2673 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1719 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_CREATE);
    }
#line 2682 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1724 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_PROPERTY);
    }
#line 2691 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1729 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_DESTROY);
    }
#line 2700 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1739 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_ADD);
    }
#line 2709 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1744 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_DEL);
    }
#line 2718 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1749 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_SET_WITFHOUT_SMPS);
    }
#line 2727 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1754 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_SET);
    }
#line 2736 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1759 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_WORK);
    }
#line 2745 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1764 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_WORK_ALL);
    }
#line 2754 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1769 "parser.y" /* yacc.c:1646  */
    {
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_STATUS_SHOW);
    }
#line 2763 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1774 "parser.y" /* yacc.c:1646  */
    {  
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_RX_RATE);
    }
#line 2773 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1780 "parser.y" /* yacc.c:1646  */
    {   
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_TX_RX_PERCENT);
    }
#line 2783 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1786 "parser.y" /* yacc.c:1646  */
    {   
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_RX_FCS_RATE);
    }
#line 2793 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1792 "parser.y" /* yacc.c:1646  */
    {           
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_TX_FCS_RATE);
    }
#line 2803 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1802 "parser.y" /* yacc.c:1646  */
    {   
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_SRV_RX_TX_PERCENT);
    }
#line 2813 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1808 "parser.y" /* yacc.c:1646  */
    {           
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_SRV_RX_FCS_RATE);
    }
#line 2823 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1814 "parser.y" /* yacc.c:1646  */
    {       
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_MIN_SRV_TX_FCS_RATE);
    }
#line 2833 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1820 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json(0); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_CONCATENATE);
    }
#line 2843 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1826 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json(1); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_IF_CONCATENATE);
    }
#line 2853 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1832 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_HEART_INTERVAL);
    }
#line 2863 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1838 "parser.y" /* yacc.c:1646  */
    {
		convert_val_to_json((yyvsp[-1].ival)); 
		close_json();
		EN = process_msg(&g_sb, slot_mask, OPCODE_LKPT_LINK_HEART_TIMEOUT);
    }
#line 2873 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1850 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 2882 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1855 "parser.y" /* yacc.c:1646  */
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();
        if ((yyvsp[0].ival) != g_my_slot) {
            YYERROR;
        }
        (yyval.ival) = (yyvsp[0].ival);
		convert_val_to_json((yyval.ival));
    }
#line 2895 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1867 "parser.y" /* yacc.c:1646  */
    {
		#if 0
        olp_id_t *olp_id = calloc(1, sizeof(olp_id_t));
        if (olp_id == NULL) {
            EN = OE_RST_NO_MEM;
            YYERROR;
        }
        olp_id->dev_id = (yyvsp[-2].ival);
        olp_id->link_id = (yyvsp[0].ival);
        (yyval.vptr) = olp_id;
		#endif
		convert_val_to_json((yyvsp[-2].ival));
		convert_val_to_json((yyvsp[0].ival));
    }
#line 2914 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1886 "parser.y" /* yacc.c:1646  */
    {
    }
#line 2921 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1892 "parser.y" /* yacc.c:1646  */
    {
        if (strlen((yyvsp[-9].sptr)) >= 128) {
            //__printf_buf("The len of the name should be form 1 to 127\n"); 
			EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
		convert_spec_str_to_json((yyvsp[-9].sptr));	
		convert_val_to_json((yyvsp[-6].ival));
		convert_val_to_json((yyvsp[-3].ipaddr));
		convert_val_to_json((yyvsp[0].ival));
	}
#line 2938 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1908 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = (yyvsp[0].ival);
    }
#line 2946 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1941 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = (yyvsp[0].ival);
		convert_val_to_json((yyval.ival));
    }
#line 2955 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1946 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = (yyvsp[-2].ival)|(yyvsp[0].ival);
		convert_val_to_json((yyval.ival));
    }
#line 2964 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1953 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_CHECK_ALL_MASK & (~E_LINK_PROTECT_OPTPOWER_CHECK_MASK);
    }
#line 2972 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1957 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_RX_CNT_CHECK_MASK;
    }
#line 2980 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1961 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_TX_CNT_CHECK_MASK;
    }
#line 2988 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1965 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_RX_FCS_CNT_CHECK_MASK;
    }
#line 2996 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1969 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_TX_FCS_CNT_CHECK_MASK;
    }
#line 3004 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1973 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_CHECK_ALL_MASK;
    }
#line 3012 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1977 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_RX_CNT_CHECK_MASK | E_LINK_PROTECT_TX_CNT_CHECK_MASK;
    }
#line 3020 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1981 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_LINK_STAT_CHECK_MASK;
    }
#line 3028 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1985 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = E_LINK_PROTECT_RX_FCS_CNT_CHECK_MASK | E_LINK_PROTECT_TX_FCS_CNT_CHECK_MASK;
    }
#line 3036 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1992 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_START;
		convert_val_to_json((yyval.ival));
    }
#line 3045 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1997 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_RESTART;
		convert_val_to_json((yyval.ival));
    }
#line 3054 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 2002 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_STOP;
		convert_val_to_json((yyval.ival));
    }
#line 3063 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 2007 "parser.y" /* yacc.c:1646  */
    {
                (yyval.ival) = SNMP_RESTORE;
		convert_val_to_json((yyval.ival));
        }
#line 3072 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 2015 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_SYSNAME;
		convert_val_to_json((yyval.ival));
    }
#line 3081 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 2020 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_SYSCONTACT;
		convert_val_to_json((yyval.ival));
    }
#line 3090 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 2025 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_SYSLOCATION;
		convert_val_to_json((yyval.ival));
    }
#line 3099 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 2030 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_ROCOMMUNITY;
		convert_val_to_json((yyval.ival));
    }
#line 3108 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 2035 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_RWCOMMUNITY;
		convert_val_to_json((yyval.ival));
    }
#line 3117 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 2040 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_TRAP_VERSION;
		convert_val_to_json((yyval.ival));
    }
#line 3126 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 2045 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_USERNAME;
		convert_val_to_json((yyval.ival));
    }
#line 3135 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 2050 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_AUTH_PASSWD;
		convert_val_to_json((yyval.ival));
    }
#line 3144 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 2055 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = SNMP_PRIV_PASSWD;
		convert_val_to_json((yyval.ival));
    }
#line 3153 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 2063 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3162 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 2068 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3171 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 2076 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3180 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 2081 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3189 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 2089 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3198 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 2094 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3207 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 2099 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 2;
		convert_val_to_json((yyval.ival));
    }
#line 3216 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 2107 "parser.y" /* yacc.c:1646  */
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();	
        if (OeAp_Get_Platform() == PLATFORM_S40 && (yyvsp[0].smps).s != 1 << (g_my_slot - 1)) {
            //__printf_buf("Invalid input. Please input right slot_id :%d\n", g_my_slot);
            EN = OE_RST_INVLD_PRM;
			//__printf_rv(EN);
            YYERROR;
        }
		//fprintf(stderr,"%d\n",$1.s);
		slot_mask = (yyvsp[0].smps).s;
		convert_val_to_json((yyvsp[0].smps).s);
		convert_val_to_json((yyvsp[0].smps).m);

        {
            int i=0;
            for(i=0; i<MAX_PORTS_WIDTH; i++)
            {
				//fprintf(stderr,"port[%d]:%lld\n",i,$1.p[i]);
                convert_val_to_json((yyvsp[0].smps).p[i]);
            }
        }
        
    }
#line 3244 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 2131 "parser.y" /* yacc.c:1646  */
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
		convert_val_to_json((yyvsp[0].mps).m);
        {
            int i=0;
            for(i=0; i<MAX_PORTS_WIDTH; i++)
            {
                convert_val_to_json((yyvsp[0].mps).p[i]);
            }
        }
    }
#line 3268 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 2154 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = TRUNK_LOAD_BALANCE_DSTIP;
		convert_val_to_json((yyval.ival));
    }
#line 3277 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 2159 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = TRUNK_LOAD_BALANCE_DSTMAC;
		convert_val_to_json((yyval.ival));
    }
#line 3286 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 2164 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = TRUNK_LOAD_BALANCE_SRCIP;
		convert_val_to_json((yyval.ival));
    }
#line 3295 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 2169 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = TRUNK_LOAD_BALANCE_SRCMAC;
		convert_val_to_json((yyval.ival));
    }
#line 3304 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 2174 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = TRUNK_LOAD_BALANCE_SRCDSTIP;
		convert_val_to_json((yyval.ival));
    }
#line 3313 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 2179 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = TRUNK_LOAD_BALANCE_SRCDSTMAC;
		convert_val_to_json((yyval.ival));
    }
#line 3322 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 2187 "parser.y" /* yacc.c:1646  */
    {
        if ((0 == (yyvsp[0].ipaddr)) || (0xffffffff == (yyvsp[0].ipaddr))) {
            //__printf_buf("ip address should be NOT 0.0.0.0 or 255.255.255.255.\n");
            EN = OE_RST_INVLD_PRM; 
			//__printf_rv(EN);
            YYERROR;
        }
        
        (yyval.ival) = (yyvsp[0].ipaddr);
		convert_val_to_json((yyval.ival));
    }
#line 3338 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 2202 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = (yyvsp[0].ipaddr);
		convert_val_to_json((yyval.ival));
    }
#line 3347 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 2228 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3356 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 2233 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3365 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 2270 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3374 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 2275 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3383 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 2282 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3392 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 2287 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3401 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 2315 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3410 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 2320 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3419 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 2949 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3428 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 2954 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3437 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 2961 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = (yyvsp[0].ipaddr);
		convert_val_to_json((yyval.ival));
    }
#line 3446 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 2969 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
		convert_val_to_json((yyval.ival));
    }
#line 3455 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 2974 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
		convert_val_to_json((yyval.ival));
    }
#line 3464 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 2981 "parser.y" /* yacc.c:1646  */
    {
		g_my_slot = 1 + OeAp_Get_My_Slot_Id();
		slot_mask = (1 << (g_my_slot - 1));
        (yyval.ival) = g_my_slot;
		convert_val_to_json((yyval.ival));
    }
#line 3475 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 2988 "parser.y" /* yacc.c:1646  */
    {
        if (((yyvsp[0].ival) < 1) || ((yyvsp[0].ival) > SLOT_NUMBER_PER_SYSTEM)) {
            //__printf_buf("slot id should be from 1 to %d.\n", SLOT_NUMBER_PER_SYSTEM);
            EN = OE_RST_INVLD_PRM; 
			//__printf_rv(EN);
            YYERROR;
        }
		slot_mask = (1 << ((yyvsp[0].ival) - 1));

        (yyval.ival) = (yyvsp[0].ival);
		convert_val_to_json((yyval.ival));
    }
#line 3492 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 3161 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 0;
    }
#line 3500 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 3165 "parser.y" /* yacc.c:1646  */
    {
        (yyval.ival) = 1;
    }
#line 3508 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 3182 "parser.y" /* yacc.c:1646  */
    {
		(yyval.ival)=(yyvsp[0].sptr);
		convert_spec_str_to_json((char *)(yyval.ival));
	}
#line 3517 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 3187 "parser.y" /* yacc.c:1646  */
    {
		(yyval.ival)=(yyvsp[0].sptr);
		convert_spec_str_to_json((char *)(yyval.ival));
	}
#line 3526 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
    break;


#line 3530 "/home/yxs/NGN-PIPE/NGN/client/parser.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 3202 "parser.y" /* yacc.c:1906  */



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

