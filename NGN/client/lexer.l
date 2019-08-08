%{

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "parser.tab.h"
#include "my_parser.h"
#include "my_lexer.h"
#include "def.h"


//TODO by caicai
#if 0
#include "log_def.h"

struct log_api g_log;

#define YY_INPUT(buf, result, max_size)             \
    memset(&g_log, 0, sizeof(g_log));               \
    if (hy_is_interactive()) {                      \
        char *s = NULL;                             \
        while(s == NULL || *s == 0)                 \
            s = readline("CLI> ");                  \
        add_history(s);                             \
        strncpy(buf, s, max_size);                  \
        strcat(buf, "\n");                          \
        result = strlen(buf);                       \
        free(s);                                    \
    } else {                                        \
        int c = fgetc(yyin);                        \
        result = (c == EOF) ? 0 : (buf[0] = c, 1);  \
    }
#endif

#define YY_INPUT(buf, result, max_size)             \
    if (hy_is_interactive()) {                      \
        char *s = NULL;                             \
        while(s == NULL || *s == 0)                 \
            s = readline("CLI> ");                  \
        add_history(s);                             \
        strncpy(buf, s, max_size);                  \
        strcat(buf, "\n");                          \
        result = strlen(buf);                       \
        free(s);                                    \
    } else {                                        \
        int c = fgetc(yyin);                        \
        result = (c == EOF) ? 0 : (buf[0] = c, 1);  \
    }


#define MAXNAMES 64
#define MAXWORDLEN  128

static char g_name[MAXNAMES][MAXWORDLEN];
static int  g_nameindex = 0;

static char *add_name_table(char *name)
{
    if (g_nameindex < MAXNAMES) {
        strncpy(g_name[g_nameindex], name, MAXWORDLEN);
        g_name[g_nameindex][MAXWORDLEN - 1] = '\0';
        return g_name[g_nameindex++];
    }
		
    fprintf(stderr,"Can't use more than %d names, '%s' discarded!\n", MAXNAMES, name);

    return g_name[g_nameindex];
}

void clear_name_table(void)
{
    g_nameindex = 0;
}

int insert_port_to_p(int port, unsigned long long *p)
{
    int index=0;
    int offset=0;
    if((port<1) || (port>MAX_PORT_NUMBER))
        return -1;

    if(NULL==p)
        return -1;

    index = (port-1)/64;
    offset = (port-1)%64;

    p[index] |= 1ull<<offset;

    return 0;
}

//This function runs only for SLOT&MODULE
static int str2map_slot_module(char *s, unsigned long long *sm)
{
    char *ret;
    char buf[80] = {0};
    char *str = buf;
    int a,b,i;

    if (!s)
        return 1;

    strcpy(buf,s);
    *sm = 0;

    while (1) {
        ret = (char *)strsep(&str,",");
        if (ret == NULL)
            break;

        if (strlen(ret) == 1) {
            if ((ret[0] == 'r') || (ret[0] == 'R'))
                *sm |= 1ull << 4;
            else if ((ret[0] == 'f') || (ret[0] == 'F'))
                *sm |= 1ull << 5;
            else if ((ret[0] == 'b') || (ret[0] == 'B'))
                *sm |= 1ull << 6;
            else if(ret[0] == '*')
                {
                    memset(sm, 0xff, sizeof(unsigned long long));
                }
            else {
                a = atoi(ret);
                if ((a > SLOT_NUMBER_PER_SYSTEM) || (a < 1))
                    return 1;
                *sm |= 1ull << (a - 1);
            }
        }     
    }

    return 0;
}

//This function funs for Ports smp->p[]
static int str2map_ports(char *s, unsigned long long *p)
{
    char *ret;
    char buf[80] = {0};
    char *str = buf;
    int a,b,i;
    int port = 0;

    if (!s)
        return 1;

    strcpy(buf,s);
    memset(p, 0x00, sizeof(unsigned long long )*MAX_PORT_NUMBER);

    while (1) {
        ret = (char *)strsep(&str,",");
        if (ret == NULL)
            break;

        if(strstr(ret,"-"))  //a-b format
        {
            if (sscanf(ret, "%d-%d", &a, &b) != 2)
                return 1;

            if ((a > b) || (b > MAX_PORT_NUMBER) || (a < 1))
                return 1;

            for(i = a; i <= b; i++)
            {
                insert_port_to_p(i,p);
            }
        }
        else                //f/x, f/xx, f/xxx, f/*  format
        {
            if((strlen(ret)<1) || (strlen(ret)>3))
                return 1;

            if(ret[0]=='*')
            {
                memset(&p[0], 0xff, sizeof(unsigned long long));
                memset(&p[1], 0xff, sizeof(unsigned long long));
            }
            else
            {
                port = atoi(ret); 
                if((port<1) || (port>MAX_PORT_NUMBER))
                    return 1;
                insert_port_to_p(port,p);
            }
        }
    }

    return 0;
}

static int str2map(char *s, unsigned long long *map)
{
    char *ret;
    char buf[80] = {0};
    char *str = buf;
    int a,b,i;

    if (!s)
        return 1;

    strcpy(buf,s);
    *map = 0;

    while (1) {
        ret = (char *)strsep(&str,",");
        if (ret == NULL)
            break;

        if (strlen(ret) > 2) {     //
            if (sscanf(ret, "%d-%d", &a, &b) != 2)
                return 1;

            if ((a > b) || (b > MAX_PORT_NUMBER) || (a < 1))
                return 1;

            for(i = a; i <= b; i++)
                *map |= 1ull << (i - 1);
        } else if (strlen(ret) == 1) {
            if ((ret[0] == 'r') || (ret[0] == 'R'))
                *map |= 1ull << 4;
            else if ((ret[0] == 'f') || (ret[0] == 'F'))
                *map |= 1ull << 5;
            else if ((ret[0] == 'b') || (ret[0] == 'B'))
                *map |= 1ull << 6;
            else if ((ret[0] == '*'))
                *map |= PORT_BIT_MASK;
            else {
                a = atoi(ret);
                if ((a > MAX_PORT_NUMBER) || (a < 1))
                    return 1;
                *map |= 1ull << (a - 1);
            }
        } else if (strlen(ret) == 2) {
            a = atoi(ret);
            if ((a > MAX_PORT_NUMBER) || (a < 1))
                return 1;
            *map |= 1ull << (a - 1);
        }
    }

    return 0;
}

%}

/* %x helps */

/* suppresee "yyunput defined but not used" */
%option nounput
/* suppresee "input defined but not used" */
%option noinput

smp_num  [1-9][0-9]*
smp_m1 [fF]|[rR]|[bB]
smp_m2 {smp_m1}|{smp_m1}[,]{smp_m1}|{smp_m1}[,]{smp_m1},{smp_m1}
smp_m3 {smp_m2}|{smp_m2}[,]{smp_m2}|[*]
smp_nums {smp_num}|{smp_num}[-]{smp_num}
smp_elem1 {smp_nums}|{smp_nums}[,]{smp_nums}
smp_elem {smp_elem1}|{smp_elem1}([,]{smp_elem1})*|[*]
smp_data {smp_elem}[/]{smp_m3}[/]{smp_elem}
sm_data {smp_elem}[/]{smp_m3}
mp_data {smp_m2}[/]{smp_elem}
smps_data {smp_elem}[/]{smp_m3}[/]{smp_elem}
mps_data {smp_m2}[/]{smp_elem}

integer  [0-9]+
hexdigit [0-9a-fA-F]
ident    [@a-zA-Z]+[a-zA-Z0-9_-]*
char     [A-Za-z0-9.+-_/@]
string	 \"[A-Za-z0-9_+-\\*/@ ]+\"
hexint   {hexdigit}+
ipnum    [0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]
ipaddr   {ipnum}\.{ipnum}\.{ipnum}\.{ipnum}
ipv6num  {hexdigit}|{hexdigit}{2}|{hexdigit}{3}|{hexdigit}{4}
ipv6addr {ipv6num}:{ipv6num}:{ipv6num}:{ipv6num}:{ipv6num}:{ipv6num}:{ipv6num}:{ipv6num}
macnum   [0-9a-fA-F][0-9a-fA-F]
mac      {macnum}\:{macnum}\:{macnum}\:{macnum}\:{macnum}\:{macnum}
time_format (([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})-(((0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)-(0[1-9]|[12][0-9]|30))|(02-(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|((0[48]|[2468][048]|[3579][26])00))-02-29)
pwd_string	\"[A-Za-z0-9_+-\\*/~!@#$%\^&\(\)=\|\[\]\{\}\;\:\'\"\,\<\>\.\?\/\` ]+\"
%%

{time_format} {
     memset(yylval.time, '\0', 12);
     strncpy(yylval.time, yytext, 10);
     return _TIME_FORMAT_;
            }

{smps_data}  {
                char *s;
                char *m;
                char *p;
                char * str = yytext;

                s = (char *)strsep(&str, "/");
                m = (char *)strsep(&str, "/");
                p = (char *)strsep(&str, "/");
                if(str2map_slot_module(s, &yylval.smps.s) != 0 || 
                    (yylval.smps.s & SLOT_BIT_MASK) != yylval.smps.s) { 
                    memset(&yylval.smps.s, 0, sizeof(yylval.smps.s)); 
                    return _SMPS_; 
                }
                if(str2map_slot_module(m, &yylval.smps.m) != 0 || 
                    (yylval.smps.m & MODULE_BIT_MASK) != yylval.smps.m) { 
                    memset(&yylval.smps.m, 0, sizeof(yylval.smps.m)); 
                    return _SMPS_;
                }
                if(str2map_ports(p, yylval.smps.p) != 0) { 
                    memset(yylval.smps.p, 0, sizeof(yylval.smps.p)); 
                    return _SMPS_; 
                }
                yylval.smps.s &= SLOT_BIT_MASK;
                yylval.smps.m &= MODULE_BIT_MASK;
                return _SMPS_;
            }
{mps_data}	{
                char *m;
                char *p;
                char * str = yytext;
                m = (char *)strsep(&str, "/");
                p = (char *)strsep(&str, "/");

                if(str2map_slot_module(m, &yylval.mps.m) != 0 || 
                    (yylval.mps.m & MODULE_BIT_MASK) != yylval.mps.m) { 
                    memset(&yylval.mps.m, 0, sizeof(yylval.mps.m)); 
                    return _MPS_;
                }
                if(str2map_ports(p, yylval.mps.p) != 0) { 
                    memset(yylval.mps.p, 0, sizeof(yylval.mps.p)); 
                    return _MPS_; 
                }
                yylval.mps.m &= MODULE_BIT_MASK;
                return _MPS_;
            }
{integer}  {
    yylval.ival = atoll(yytext);
                return _INT_;
            }
0x{hexint}  {
                sscanf(yytext, "0x%llx", &(yylval.ival));
                return _INT_;
            }
{ident}		{
                int token;
                token = find_token(yytext);
                if (token > 0) {
                    return token; 
                }
                yylval.sptr = add_name_table(yytext);
                return _NAME_;
            }
(ftp):\/\/({char}+(:{char}+)*@)*{ipaddr}(:{integer})*\/{char}+	{
                yylval.sptr = add_name_table(yytext);
                return _URL_;
            }
{string}	{
                int len = strlen(yytext);
                yytext[len - 1] = 0;
                yylval.sptr = add_name_table(yytext + 1);
                return _STRING_;
            }
{pwd_string} {
		 int len = strlen(yytext);
                yytext[len - 1] = 0;
                yylval.sptr = add_name_table(yytext + 1);
                return _PWD_STRING_;
		}
{ipaddr}	{
                yylval.ipaddr = inet_network(yytext);
                return _IPADDR_;
            }
{ipv6addr}  {
            int i8[8];

            sscanf(yytext, "%x:%x:%x:%x:%x:%x:%x:%x", i8, i8 + 1, i8 + 2, i8 + 3,  
            i8 + 4, i8 + 5, i8 + 6, i8 + 7); 

            yylval.ipv6addr[0] = i8[0];
            yylval.ipv6addr[1] = i8[1];
            yylval.ipv6addr[2] = i8[2];
            yylval.ipv6addr[3] = i8[3];
            yylval.ipv6addr[4] = i8[4];
            yylval.ipv6addr[5] = i8[5];
            yylval.ipv6addr[6] = i8[6];
            yylval.ipv6addr[7] = i8[7];

            return _IPV6ADDR_;
        }
{mac}       {
                int tmp_mac[6], i;
                sscanf(yytext, "%02x:%02x:%02x:%02x:%02x:%02x", &tmp_mac[0], &tmp_mac[1],
                                &tmp_mac[2], &tmp_mac[3], &tmp_mac[4], &tmp_mac[5]);
                for (i = 0; i < 6; i++) {
                    yylval.mac[i] = (unsigned char)tmp_mac[i];
                }
                return _MAC_;
            }
[\n]        { 
                end_of_line = 1; 
                return EOS;
            }
;           {
                end_of_line = 0; 
                return EOS;
            }
[ \t]+      /* ignore whitespace */;
.           return yytext[0];

%%

int yywrap(void)
{
	return 1;
}

int hy_is_interactive(void)
{
#ifdef YY_FLEX_SUBMINOR_VERSION
	return YY_CURRENT_BUFFER->yy_is_interactive;
#else
    return yy_current_buffer->yy_is_interactive;
#endif    
}

int hy_switch_to_file(FILE* fp)
{
	yy_switch_to_buffer(yy_create_buffer(fp, YY_BUF_SIZE));
	return 0;
}

int hy_switch_to_buffer(char *cmd)
{
	if (cmd) {
        yy_switch_to_buffer(yy_scan_string(cmd));
    }
	return 0;
}

int hy_delete_buffer()
{
    yy_delete_buffer(YY_CURRENT_BUFFER);
    return 0;
}
