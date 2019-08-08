#ifndef __MY_PARSER_H__
#define __MY_PARSER_H__

#include <stdio.h>

#define MAX_CMD_NUM  512
#define MAX_CMD_WORD_NUM 258
#define MAX_CMD_WORD_LEN 16
#define MAX_BUF_SIZE (sizeof(char) * MAX_CMD_WORD_NUM * MAX_CMD_WORD_LEN)

extern int end_of_line;
extern char *helplist[MAX_CMD_NUM][MAX_CMD_WORD_NUM];
extern char *tokenlist[MAX_CMD_WORD_NUM][MAX_CMD_NUM];

extern int start_cli_cmd(char *cmd);
extern int start_cli_parser(char *cmd, FILE *fp);
extern char *back_token_str(char *input);
extern int find_token(char *input);
extern void init_readline(void);
extern char *command_generator(const char *text, int state);
//extern int cli_print_help(char *helplist);
extern int variable_match_check(char *var, const char *str);
extern int convert_val_to_json(unsigned long long val);
extern int convert_spec_str_to_json(char *val);
extern int convert_key_to_json(char *val);
extern int convert_spec_key_to_json(char *val);
extern int close_json();
extern int oe_check_snmp_param(int param, char *value);
#endif
