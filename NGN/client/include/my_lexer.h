#ifndef __MY_LEXER_H__
#define __MY_LEXER_H__

#include <stdio.h>

extern int hy_is_interactive(void);
extern int hy_switch_to_file(FILE *fp);
extern int hy_switch_to_buffer(char *cmd);
extern int hy_delete_buffer();
extern int yylex(void);
extern void clear_name_table(void);

#endif
