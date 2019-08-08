#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "string_buf.h"
#include "js.h"
#include "nngn_struct.h"
#include "nngn.h"
#include "error_code.h"
#include "get_special_param.h"
#include "mpack-config.h"	
#include "mpack.h"

static int process_show_cmds(char * content, int size)
{
	int rv = 0;
	int array_num = 0, list_num = 0;
	char list_name[128] = {0};
	char str_val[128] = {0};
	int i = 0;
	mpack_reader_t reader;
	mpack_tag_t tag;

	mpack_reader_init(&reader,content,sizeof(content),size);

	list_num = mpack_expect_array(&reader);
	if(list_num > 1){
		for(i = list_num; i > 0 && mpack_reader_error(&reader) == mpack_ok; i--){
			tag = mpack_read_tag(&reader);
			if(tag.type != mpack_type_str){
				fprintf(stderr,"%ld  ",tag.v.u);
			}else{
				memset(list_name, 0, sizeof(list_name));
				mpack_read_bytes(&reader, list_name, tag.v.l);
				fprintf(stderr,"%s  ",list_name);
			}
		}
		mpack_done_array(&reader);
		fprintf(stderr,"\n");

		do{
			array_num = mpack_expect_array(&reader);
			if(array_num != 0){
				for(i = array_num; i > 0 && mpack_reader_error(&reader) == mpack_ok; i--){
					tag = mpack_read_tag(&reader);
					if(tag.type != mpack_type_str){
						fprintf(stderr,"%ld  ",tag.v.u);
					}else{
						memset(str_val, 0, sizeof(str_val));
						mpack_read_bytes(&reader, str_val, tag.v.l);
						fprintf(stderr,"%s  ",str_val);
					}
				}
				fprintf(stderr,"\n");
			}
		}while(array_num != 0);

	}else{					
		tag = mpack_read_tag(&reader);
		if(tag.type != mpack_type_str){
			fprintf(stderr,"%ld  ",tag.v.u);
		}else{
			mpack_read_bytes(&reader, str_val, tag.v.l);
			fprintf(stderr,"%s  ",str_val);
		}

	}
	fprintf(stderr,"\n");

	return rv;
}


int process_msg(string_buf *sb, unsigned long long slot_mask, int opcode)
{
	int rv = 0;
	char data_buf[MAX_CMD_LEN]={0};
	int size = 0, i = 0;
	req_t req_data;
	res_t *res[MAX_SLOT_NUM];
	res_t res0[MAX_SLOT_NUM];
	int slot_num = 0;
	uint32_t nres = 0;

	if(sb->buf){
		fprintf(stderr,"js_str:%s\n",sb->buf);
		json2msgpack(sb->buf,data_buf,&size);
		string_buf_free(sb);
	}

	slot_num = get_slot_num(slot_mask);
	nclt_req_fill(&req_data, size, opcode, slot_mask, data_buf);

	for(i = 0; i < MAX_SLOT_NUM; i++){
		memset(res0, 0, sizeof(res0));
		res[i] = &res0[i];
	}

	if(slot_num == 1){
		nclt_sr(&req_data, res[0]);	
		/*unpack res data*/
		if(res[0]->rv == 0){
			if(res[0]->content){
				fprintf(stderr,"slot %d:\n",res[0]->slot);
				#if 0
				msgpack2json((char *)res[0]->content, res[0]->size);
				#endif
				rv = process_show_cmds((char *)res[0]->content, res[0]->size);
				if(rv != 0){
					fprintf(stderr,"slot %d %s\n",res[0]->slot,_oe_error(res[0]->rv));
				}
				free(res[0]->content);
			}else{
				fprintf(stderr,"set ok\n");
			}
		}else{
			fprintf(stderr,"slot %d %s\n",res[0]->slot,_oe_error(res[0]->rv));
			return rv;
		}
	}else{
		rv = nclt_sr_plus(&req_data, res, &nres, 10000);
		for(i = 0; i < nres; i++){
			if(res[i]->rv == 0){
				if(res[i]->content){
					fprintf(stderr,"slot %d:\n",res[i]->slot);
					#if 0
					msgpack2json((char *)res[i]->content, res[i]->size);
					#endif
					rv = process_show_cmds((char *)res[0]->content, res[0]->size);
					if(rv != 0){
						fprintf(stderr,"slot %d %s\n",res[0]->slot,_oe_error(res[0]->rv));
					}
					free(res[i]->content);
				}else{
					fprintf(stderr,"set ok\n");
				}
				fprintf(stderr,"\n\n");
			}else{
				fprintf(stderr,"slot %d %s\n",res[i]->slot,_oe_error(res[i]->rv));
				break;
			}
		}
	}

	return rv;
}
