#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "common.h"
#include "string_buf.h"
#include "look_up.h"
#include "get_special_param.h"
#include "js.h"
#include "nngn_struct.h"
#include "nngn.h"
#include "error_code.h"


static int parseline(char *buf, unsigned long long *slot_mask, char *data_buf, int *size)
//增加了对key的的处理
{
	int rv = 0;
    char *c = NULL;
    char *start = NULL;
    char *end = NULL;
    int t=0;
    int in_list = 0;
    int in_map = 0;
    int in_pair = 0;
	int int_key = 0;
	int mask_num = 0;
	string_buf sb;
#if 0
	int data_len = 0;
	char msg_data[MAX_CMD_LEN];
#endif

    if( buf == NULL ) return 0;
	string_buf_init(&sb,CLI_MAX_STR_LEN);

    c = buf;
    while(*c != '\0') {
        if ( (c = find_word(c,&start,&end, &t)) == NULL) return -1;
        if ( t == 0 ) break;
        if ( in_list == 0 ) {
            string_buf_puts(&sb, "[", 1 );
			in_list = 1;
        }
        else {
            if ( !in_pair ) string_buf_puts(&sb, ",", 1 );
        }

		//fprintf(stderr,"line:%d func:%s in_map:%d in_pair:%d int_key:%d mask_num:%d t:%d\n",__LINE__,__func__,in_map,in_pair,int_key,mask_num,t);

        switch(t) {
        case 1:
			mask_num = find_str_times(start,'/',(int)(end-start));
			if(mask_num == 2){
				get_smp_val(&sb,start,(int)(end-start),slot_mask);
			}else{
				get_key_mask(&sb,start,(int)(end-start));
			}
            in_pair=0;
			int_key = 0;
            break;
        case 2:
            string_buf_puts_format(&sb, "\"%.*s\"", (int)(end-start), start);
            in_pair=0;
			int_key = 0;
            break;
        case 3:
            if( in_map == 0 ) {
				string_buf_puts(&sb, "{", 1 );
                in_map = 1;
            }
			int_key = get_int2_key(start,(int)(end-start));
			if(int_key > 0){
				string_buf_puts_format(&sb, "%d:", int_key );
			}else{
				string_buf_puts_format(&sb, "%.*s:", (int)(end-start), start);
			}
            in_pair=1;
            break;
        case 4:
            if( in_map == 0 ) {
				string_buf_puts(&sb, "{", 1 );
                in_map = 1;
            }
			string_buf_puts_format(&sb, "\"%.*s\":", (int)(end-start), start);
            in_pair=1;
            break;
        }

        start = end = NULL;
    }

    if ( in_map ) {
		string_buf_puts(&sb, "}", 1 );
    }
    if ( in_list ) {
		string_buf_puts(&sb, "]", 1 );
    }

	fprintf(stderr,"js_str:%s\n",sb.buf);
	json2msgpack(sb.buf,data_buf,size);
#if 0	
	fprintf(stderr,"js_str:%s\n",sb.buf);
	rv = json2msgpack(sb.buf,msg_data,&data_len);
	msgpack2json(msg_data,data_len);
#endif

	string_buf_free(&sb);

	if(rv != 0){
		return rv;
	}

    return rv;
}

static int parsefile(FILE *fp, unsigned long long *slot_mask, char *data_buf, int *size)
{
    ssize_t read;
    size_t  len = 0 ;
    char   *line = NULL;

    while ((read = getline(&line, &len, fp)) != -1) {
        parseline(line, slot_mask, data_buf, size);
    }
    free(line);
    return 0;
}

int main(int argc, char **argv)
{
	int rv = 0;
    int opt;
    char *cmdstr = NULL;
    FILE *fp=NULL;
	char *slot_str = NULL;
	unsigned long long slot_mask = 0x3fff;
	uint32_t opcode = 0;
	char data_buf[MAX_CMD_LEN];
	int size = 0, i = 0;
	req_t req_data;
	res_t *res[MAX_SLOT_NUM];
	res_t res0[MAX_SLOT_NUM];
	int slot_num = 0;
	uint32_t nres = 0;
	int nNum = 1;
	int nIndex = 0;

	memset(&req_data, 0, sizeof(req_data));

    while ((opt = getopt(argc, argv, "c:f:s:o:n:")) > 0) {
        switch (opt) {
        case 'c':
            cmdstr = strdup(optarg);
            break;
        case 'f':
            if( (fp = fopen( optarg, "r"))== NULL) {
                perror("open");
                exit(1);
            }
            break;
		case 's':
            slot_str = strdup(optarg);
            break;
		case 'o':
			opcode = strtoul(optarg,NULL,0);
			opcode = (opcode & 0xff);
			break;
		case 'n':
			nNum = atoi(optarg);
	              break;
        case 'h':
        default:
            printf("Usage: cli [-c <cmd>] [ -f <file> ] [-s slot_info] [-o opcode]\n");
            exit(0);
            break;
        }
    }

	if(slot_str){
		str2map(slot_str, &slot_mask);
		free(slot_str);
	}

#if 0
    if( (cmdstr==NULL)&&(fp==NULL)){
        parsefile(stdin,&slot_mask,data_buf,&size);
	}
#endif

    if(cmdstr) {
        parseline(cmdstr,&slot_mask,data_buf,&size);
        free(cmdstr);
    }

    if(fp) {
        parsefile(fp,&slot_mask,data_buf,&size);
        fclose(fp);
    }

	fprintf(stderr,"line:%d slot_mask:0x%llx\n",__LINE__,slot_mask);

	slot_num = get_slot_num(slot_mask);
	//fprintf(stderr,"line:%d slot_num:%d\n",__LINE__,slot_num);

	nclt_new();
	nclt_req_fill(&req_data, size, opcode, slot_mask, data_buf);

	for(i = 0; i < MAX_SLOT_NUM; i++){
		memset(res0, 0, sizeof(res0));
		res[i] = &res0[i];
	}
	struct timeval t1,t2;
	gettimeofday(&t1,NULL);
	for(nIndex = 0;nIndex<nNum;nIndex++)
	{
		rv = nclt_sr_plus(&req_data, res, &nres,1000);
		#if 1
		for(i = 0; i < nres; i++){
			if(res[i]->rv == 0){
				if(res[i]->content){
					fprintf(stderr,"slot %d:\n",res[i]->slot);
					msgpack2json((char *)res[i]->content, res[i]->size);
					free(res[i]->content);
				}
					fprintf(stderr,"\n\n");
				}else{
					fprintf(stderr,"slot %d %s\n",res[i]->slot,_oe_error(res[i]->rv));
			}
		}
		#endif
	}
       gettimeofday(&t2,NULL);
	long deltal = (t2.tv_sec-t1.tv_sec)*1000000+t2.tv_usec-t1.tv_usec;
	printf("time is %ld\n",deltal);
	nclt_destroy();

	return rv;
}

