#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> 
#include "nngn_struct.h"
#include "nngn.h"
#include "mpack.h"
#include "mpack-config.h"
#include "srv_mpack_common.h"

enum{
	KEY_UD,
	KEY_PROTOCOL,
	KEY_SIP,
	KEY_DIP,
	KEY_SPORT,
	KEY_DPORT,
	KEY_SIZE,
	KEY_SMAC,
	KEY_DMAC,
	KEY_INNERVLAN,
	KEY_ETHERTYPE,
	KEY_COUNT
};

int process_set_rule_ud_mode(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	mpack_writer_t writer;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	int ud_id = 0, mode = 0, offset = 0;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,3);
	ud_id = mpack_expect_uint(&reader);
	mode = mpack_expect_uint(&reader);
	offset = mpack_expect_uint(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	fprintf(stderr,"ud_id:%d mode:%d offset:%d\n",ud_id,mode,offset);
	
END:	
	mpack_writer_init_growable(&writer, &res_buf, &res_size);
	mpack_start_array(&writer, 1);
	mpack_write_uint(&writer, rv);
	mpack_finish_array(&writer);
	if (mpack_writer_destroy(&writer) != mpack_ok) {
		fprintf(stderr, "An error occurred encoding the data!\n");
		rv = ENCODE_DATA_ERR;
	}
	res->rv = rv;
	res->size = res_size;
	res->content = malloc(res_size);
	memcpy(res->content,res_buf,res_size);
	free(res_buf);

	return rv;

}

int process_add_rule_class_entry(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	mpack_writer_t writer;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	int rule_id = 0;
	int map_ele_num = 0, key_val = 0;
	bool found[KEY_COUNT] = {0};

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,2);

	rule_id = mpack_expect_uint(&reader);
	map_ele_num = mpack_expect_map(&reader);
	for(i = map_ele_num; i > 0 && mpack_reader_error(&reader) == mpack_ok; i--){
		switch(mpack_expect_key_uint(&reader,found,KEY_COUNT)){
		}
	}

	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	fprintf(stderr,"ud_id:%d mode:%d offset:%d\n",ud_id,mode,offset);
	
END:	
	mpack_writer_init_growable(&writer, &res_buf, &res_size);
	mpack_start_array(&writer, 1);
	mpack_write_uint(&writer, rv);
	mpack_finish_array(&writer);
	if (mpack_writer_destroy(&writer) != mpack_ok) {
		fprintf(stderr, "An error occurred encoding the data!\n");
		rv = ENCODE_DATA_ERR;
	}
	res->rv = rv;
	res->size = res_size;
	res->content = malloc(res_size);
	memcpy(res->content,res_buf,res_size);
	free(res_buf);

	return rv;

}


void init_msg_process_handle(void)
{
}

int main(int argc, char **argv)
{
	int rv = 0;
	int opt;
	sinit_data_t init_data;
	int slot_id = 0;
	char *work_id = NULL;

	memset(&init_data,0,sizeof(init_data));
	strcpy(init_data.broker_addr,"127.0.0.1");

	while ((opt = getopt(argc, argv, "i:w:")) > 0) {
        switch (opt) {
    	case 'i':
			memset(init_data.broker_addr,0,strlen(init_data.broker_addr));
			strcpy(init_data.broker_addr,optarg);
			break;
		case 'w':
			work_id = strdup(optarg);
			break;
        case 'h':
        default:
            printf("Usage: system_worker [-i broker_addr] [-w word_id] \n");
            exit(0);
            break;
        }
    }

	if(work_id){
		sprintf(init_data.worker_id,"%d-%s",slot_id,work_id);
		free(work_id);
	}

	rv = nsrv_new(&init_data);
	if(rv != 0){
		return rv;
	}

	init_msg_process_handle();
	while(1){
	}
	nsrv_destroy();

	return rv;
}
