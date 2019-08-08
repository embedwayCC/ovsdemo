#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "nngn.h"
#include "mpack.h"
#include "mpack-config.h"
#include "srv_mpack_common.h"

#include "auth.h"
#include "appdefs.h"
#include "app.h"
#include "string_buf.h"

#define RPC_CLI_MAX_STR_LEN 1024
#define MAX_SLOT_NUM 14

int rpc_process_msg(string_buf *sb, unsigned long long slot_mask, int opcode, res_t **res, int *nres);

int g_slot_id;

#if 0
int rpc_get_process_msg(char *buf, size_t size, int op,int slot, res_t* ret, int *res_num)  
{                                                    
	int rv = 0;                                      
	int i = 0;                                       
	res_t *res[MAX_SLOT_NUM];                                   
	res_t res0[MAX_SLOT_NUM];                                   
	req_t req;                                       
	int nres = 0;                                    

	for(i=0; i<MAX_SLOT_NUM; i++){
		memset(&res0[i], 0, sizeof(res_t));              
		res[i] = &res0[i];                               
	}

	nclt_req_fill(&req, size, op , 0X20, buf);          
	printf("op is %d\n",op);
	rv = nclt_sr_plus(&req, res, &nres, 200000);     
	printf("rv is %d\n",rv);
//	fprintf(stderr, "res->contex:%p\n", res[0]->content);
	for(i=0; i<MAX_SLOT_NUM; i++){
		ret[i] = *(res[i]);
	}
	 *res_num = nres;

	if(buf){                                         
		free(buf);                                   
	}                                                

//	fprintf(stderr, "res->rv:%d ret.rv:%d\n", res[0]->rv, ret[0].rv);

	return rv;                                       
}


static int _rpc_oe_get_system_datetime(int slot,int op)
{
	int rv = 0, i = 0;                                                   
	size_t size = 0;                                                     
	char *buf = NULL;
	res_t res[MAX_SLOT_NUM];
	int nres = 0;      
	char log_buf[1024] = {0};

	rv = rpc_get_process_msg(buf, size, op,slot, res, &nres);   
	if(rv != 0 || nres == 0){                                          
		return -1;                                          
	}                                                       

	for(i=0; i<nres; i++){                                                
		rv = res[i].rv;                                                   
		if((rv == 0) && res[i].content) {
			fprintf(stderr,"slot %d:\n",res[i].slot);
			msgpack2json((char *)res[i].content, res[i].size);    
			free(res[i].content);                                         
		}                                                                 
		if(rv != 0){                                                      
			break;                                                        
		}                                                                 
	}        
											
	return rv;
}

#endif

int system_get_datetime(char *sys_time)
{   
    struct tm *p;
    struct timeval timeval;
    FILE *fd = NULL;
    char *pchar;
    char buf[16] = {0};
    char tz[16] = {0};
    int tz_hour = 0;

    fd = fopen(TIME_ZONE_FILE, "r");
    if (fd) {
        if (fgets(buf, sizeof(buf), fd)) {
            sscanf(buf, "TZ=%s\r\n", tz);
        } else {
            strcpy(tz, "GMT+8");
        }
        fclose(fd);
    } else {
        strcpy(tz, "GMT+8");
    }

    memset(buf, 0, 16);
    if (strncmp(tz, "GMT", 3) == 0) {
        pchar = tz + 3;
        tz_hour = atoi(pchar);
        if (tz_hour < 0) {
            sprintf(buf, "GMT%s%d", "+", -tz_hour);
        } else if (tz_hour > 0) {
            sprintf(buf, "GMT%s%d", "-", tz_hour);
        } else {
            sprintf(buf, "GMT");
        }
    }

    setenv("TZ", buf, 1);
    gettimeofday(&timeval, NULL);
    p = localtime(&timeval.tv_sec);

    snprintf(sys_time, MAX_PB_STR_LEN, "%d-%02d-%02d %02d:%02d:%02d %s", 
        (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tz);
    return OE_RST_OK;
}

int process_get_datetime(req_t *req, res_t *res)
{
	int rv = 0;
	char *buf = NULL;
	size_t res_size = 0;
	mpack_writer_t writer;
	int slot_id = 0;
	char sys_time[MAX_PB_STR_LEN];
  
	rv = system_get_datetime(sys_time);

	if(rv == 0){
		mpack_writer_init_growable(&writer, &buf, &res_size);
		mpack_start_array(&writer,1);
		mpack_write_str(&writer,sys_time,strlen(sys_time));
		mpack_finish_array(&writer);
		if (mpack_writer_destroy(&writer) != mpack_ok) {
			rv = ENCODE_DATA_ERR;
		}
	}else{
		goto END;
	}


END:
	slot_id = g_slot_id;
	res->rv = 0;
	res->seria_op = SERIAL_MPK;
	res->slot = slot_id;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,buf,res_size);
	}
	if(buf){
		free(buf);
	}

	return rv;
}


void init_msg_process_handle(void)
{
	nsrv_register_opcode(0,1,23,process_get_datetime,opcode_reg_end);
}

int statechange_handler(wk_st *worker_status, zlist_t *List)
{
    zsys_debug("wkst_change_handler recv msg slot is %d name is %s current is %d old is %d",worker_status->slot,
                                                                                            worker_status->uname,
                                                                                            worker_status->current_status,
                                                                                            worker_status->old_status);
	return 0;
}

int main(int argc, char **argv)
{
	int rv = 0;
	int opt;
	sinit_data_t init_data;
	char *work_id = NULL;
	int debug = 0;

	memset(&init_data,0,sizeof(init_data));
	strcpy(init_data.worker_id,"system");

	while ((opt = getopt(argc, argv, "w:s:")) > 0) {
        switch (opt) {
	case 'w':
			work_id = strdup(optarg);
			if(work_id){
				memset(init_data.worker_id,0,sizeof(init_data.worker_id));
				sprintf(init_data.worker_id,"%s",work_id);
				free(work_id);
			}
			break;
	case 's':
			g_slot_id =atoi(optarg);
			break;
	case 'd':
			debug =1;
			break;		
	 case 'h':
        default:
            printf("Usage: system_worker [-w work_id] \n");
            exit(0);
            break;
        }
    }

	if(!debug)
    	{
       	 rv =daemon(1, 0);
    	}
	/*if(work_id){
		memset(init_data.worker_id,0,sizeof(init_data.worker_id));
		sprintf(init_data.worker_id,"%s",work_id);
		free(work_id);
	}*/

	fprintf(stderr,"slot_id:%d\n",g_slot_id);
	init_data.slot = g_slot_id;

	create_worker_pipe_line(g_slot_id , init_data.worker_id);

	rv = nsrv_new(&init_data);
	if(rv != 0){
		return rv;
	}

    #if 0
    char *buf = NULL;
    int size = 0;
    int rc = 0;
    database_init();
    rc = put_data("test key", strlen("test key"), "hello db", strlen("hello db"));
    if(rc != 0)
    {
        zsys_error("putdata error is %d",rc);
    }
    rc = get_data("test key", strlen("test key"),&buf,&size);
    if(rc != 0)
    {
        zsys_error("getdata error is %d",rc);
    }
    zsys_debug("get data is %s size is %d",buf,size);
    database_exit();
    #endif	

    ncb_spec_t ncb;
    ncb.type = CALLBACK_STATGE;
    ncb.func = statechange_handler;
    ngn_reg_cb(&ncb);

    init_msg_process_handle();
    rv = setup_fault_trap();

    return rv;
}
