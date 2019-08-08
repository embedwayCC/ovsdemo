#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "look_up.h"

spec_key_t special_key_array[]={
	{"ud",0},
	{"protocol",1},
	{"sip",2},
	{"dip",3},
	{"sport",4},
	{"dport",5},
	{"size",6},
	{"smac",7},
	{"dmac",8},
	{"innervlan",9},
	{"ethertype",10},
};

/* This little parsing function finds one word at a time from the
 *  * input line. It supports double quotes to group words together. */
const int ws[256] = {[' ']=1, ['\t']=1, ['\n']=1 };
char *find_word(char *c, char **start, char **end, int *token )
// token：
// 1  普通值，可以转换成对应的json形式，
// 2  双引号字符串， 不可以转换，加双引号
// 3  普通key，可以转换成对应的json形式，后边跟':' 
{// 4 双引号字符串key，不可以转换，加双引号，后边跟':' 
// 这个版本增加了对‘=’的解析。token一直解析到后面的第一个‘=’，
// 有‘=’的是key（3或4）
    int in_qot=0;
    *token=0;
    while ((*c != '\0') && ws[(unsigned char)*c]) c++; // skip leading whitespace
    if (*c == '"') {
        in_qot=1;
        *token=2;
        c++;
    }
    *start=c;
    if (in_qot) {
        while ((*c != '\0') && (*c != '"')) c++;
        *end = c;
        if (*c == '"') {
            in_qot=0;
            c++;
            if ((*c != '\0') && !ws[(unsigned char)*c] && (*c != '=' )) {
                fprintf(stderr,"text follows quoted text without space\n");
                return NULL;
            }
            while ((*c != '\0') && ws[(unsigned char)*c] ) c++;
            if ( *c == '=' ) {
                *token=4;
                c++;
            }
        }
        else {
            fprintf(stderr,"quote mismatch\n");
            return NULL;
        }
    }
    else {
        while ((*c != '\0') && !(ws[(unsigned char)*c]) && (*c != '=' )) {
            if (*c == '"') {
                fprintf(stderr,"start-quote within word\n");
                return NULL;
            }
            c++;
            *token=1;
        }
        *end = c;
        while ((*c != '\0') && ws[(unsigned char)*c] ) c++;
        if ( *c == '=' ) {
            *token=3;
            c++;
        }
    }
    return c;
}

static int get_ud_num(char *ud_str)
{
	int rv = 0;
	int ud_num = 0, num = 0;

	num = sscanf(ud_str,"ud%d", &ud_num);
	if(num == 1){
		rv = ud_num + UD_NUM_SOURCE; 
	}

	return rv;
}

int get_int2_key(char *key, int l)
{
	int total_num = 0;
	int i = 0;
	int rv = -1;
    char *p =  NULL;
	size_t size = 0;

    if(l<=0) return -1;
    p=strndup(key,l);

	total_num = sizeof(special_key_array)/sizeof(special_key_array[0]);
	for(i = 0; i < total_num; i++){
		size = strlen(special_key_array[i].key_name);
		if(strncasecmp(special_key_array[i].key_name,p,size) == 0){
			rv = special_key_array[i].key_val;
			if(rv == 0){
				rv = get_ud_num(p);
			}
		}
	}
	
	return rv;
}

int find_str_times(char *s_str, char dest, int l)
{
	size_t len;
	int i = 0, times = 0;
	char *d_str = NULL;

	d_str = (char *)malloc(128);
	memcpy(d_str,s_str,l);
	len = strlen(d_str);
	for(i = 0; i < len; i++){
		if(d_str[i] == dest){
			times++;
		}
	}

	if(d_str){
		free(d_str);
	}
	return times;
}

