#ifndef GET_SPECIAL_PARAM_H
#define GET_SPECIAL_PARAM_H

#include "string_buf.h"
#include "common.h"

int get_key_mask(string_buf *buf,char *val_str, int l);
int get_smp_val(string_buf *buf,char *val_str, int l, unsigned long long *);
int str2map(char *s, unsigned long long *map);int get_slot_num(unsigned long long);

#endif
