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
#include "global.h"
#include "tna_exact_match.h"
#include <getopt.h>
#include "copy_to_cpu.h"
#include <syslog.h>

#define RPC_CLI_MAX_STR_LEN 1024
#define MAX_SLOT_NUM 14

int rpc_process_msg(string_buf *sb, unsigned long long slot_mask, int opcode, res_t **res, int *nres);
int rpc_oe_get_info(int *slot_id, int *platform);
void delete_msg(void *value);

typedef  struct _pipemsg_broadcast
{
	int expect_num;
	char pipe_id[NNGX_WKID_MAX_LENGTH];
	res_t res[MAX_PIPEMSG_NUM];
	uint64_t start_time;
	uint32_t timeout;
}pipemsg_broadcast;

zhash_t *pipe_res_list = NULL;
int g_slot_id;

#if 1
int rpc_get_process_msg(req_t *pReq,char *buf, size_t size, int op,int slot, res_t* ret, int *res_num)  
{                                                    
	int rv = 0;                                      
	int i = 0;                                       
	res_t *res[MAX_SLOT_NUM];                                   
	res_t res0[MAX_SLOT_NUM];                                   
	req_t req;                                       
	int nres = 0;                 
	int slot_mask = 0;

	for(i=0; i<MAX_SLOT_NUM; i++){
		memset(&res0[i], 0, sizeof(res_t));              
		res[i] = &res0[i];                               
	}
	
	slot_mask  |= 1 << (slot - 1);
	nclt_req_fill(pReq, size, op , slot_mask, buf);          

	#if 0
	rv = nclt_sr_plus(&req, res, &nres, 200000);     

	fprintf(stderr, "res->contex:%p\n", res[0]->content);
	for(i=0; i<MAX_SLOT_NUM; i++){
		ret[i] = *(res[i]);
	}
	 *res_num = nres;

	if(buf){                                         
		free(buf);                                   
	}                                                

	fprintf(stderr, "res->rv:%d ret.rv:%d\n", res[0]->rv, ret[0].rv);
	#endif

	return rv;                                       
}


static int _rpc_oe_get_system_datetime(req_t *pReq,int slot,int op)
{
	int rv = 0, i = 0;                                                   
	size_t size = 0;                                                     
	char *buf = NULL;
	res_t res[MAX_SLOT_NUM];
	int nres = 0;      
	char log_buf[1024] = {0};

	rv = rpc_get_process_msg(pReq,buf, size, op,slot, res, &nres);   
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
	char sys_time[MAX_PB_STR_LEN] = {0};
	char pipe_id[NNGX_WKID_MAX_LENGTH] = {0};
	pipe_msg msg[4];
	memset(msg[0].next_worker,0,NNGX_WKID_MAX_LENGTH);
	memset(msg[0].worker,0,NNGX_WKID_MAX_LENGTH);	
	strcpy(msg[0].worker,"6-sys_worker");
	strcpy(msg[0].next_worker,"5-if_worker");
	rv = _rpc_oe_get_system_datetime(&msg[0].req,6,23);

	memset(msg[1].next_worker,0,NNGX_WKID_MAX_LENGTH);
	memset(msg[1].worker,0,NNGX_WKID_MAX_LENGTH);	
	strcpy(msg[1].worker,"5-if_worker");
	strcpy(msg[1].next_worker,"6-sys_worker");	
	rv = _rpc_oe_get_system_datetime(&msg[1].req,5,23);

	memset(msg[2].next_worker,0,NNGX_WKID_MAX_LENGTH);
	memset(msg[2].worker,0,NNGX_WKID_MAX_LENGTH);	
	strcpy(msg[2].worker,"6-sys_worker");
	strcpy(msg[2].next_worker,"5-if_worker");	
	rv = _rpc_oe_get_system_datetime(&msg[2].req,6,23);
	
	memset(msg[3].next_worker,0,NNGX_WKID_MAX_LENGTH);
	memset(msg[3].worker,0,NNGX_WKID_MAX_LENGTH);	
	strcpy(msg[3].worker,"5-if_worker");
	strcpy(msg[3].next_worker,OM_PIPE_ID);	
	rv = _rpc_oe_get_system_datetime(&msg[3].req,5,23);
	
	create_pipe_msg(msg, 4,NNGN_BROAD_CAST_DISABLE,pipe_id);
	pipemsg_broadcast *pMsg = calloc(sizeof(pipemsg_broadcast),1);
	pMsg->expect_num = 1;
	strcpy(pMsg->pipe_id,pipe_id);
	pMsg->start_time = zclock_time();
	pMsg->timeout = req->timeout;
	zhash_insert(pipe_res_list, pMsg->pipe_id,pMsg);
	zhash_freefn(pipe_res_list,pMsg->pipe_id,delete_msg);

	res->rv = NNGN_PIPEMSG_RET;

	return rv;
}

void delete_msg(void *value)
{
    free(value);
    value = NULL;
}


int process_pipeline_datetime(req_t *req_org, res_t *res_in,res_t *res_out,char *pipemsg_id)
{
	assert(req_org);
	assert(res_in);
	assert(res_out);
	int rv = res_in->rv;
	mpack_writer_t writer;
	char *buf = NULL;
	size_t res_size = 0;
	char sys_time[MAX_PB_STR_LEN] = {0};
	pipemsg_broadcast * pMsg = zhash_lookup(pipe_res_list, pipemsg_id);
	if(pMsg == NULL)
	{
		zsys_error("recv pipe msg id is NULL");
		res_out->rv = NNGN_PIPEMSG_RET;
		return rv;
	}
	if(rv == 0 && pMsg->expect_num == 1)
	{
		//save info from req org
		zhash_delete(pipe_res_list, pipemsg_id);
		memcpy(sys_time,"test end",strlen("test end")+1);
	}
	else  if(rv == 0 && pMsg->expect_num != 1)
	{
		pMsg->expect_num--;
		//zsys_debug("rev msg expec num is %d",pMsg->expect_num);
		res_out->rv = NNGN_PIPEMSG_RET;
	}
	else
	{
		zhash_delete(pipe_res_list, pipemsg_id);
		memcpy(sys_time,"test error",strlen("test error")+1);
	}
	
       
       mpack_writer_init_growable(&writer, &buf, &res_size);
       mpack_start_array(&writer,1);
       mpack_write_str(&writer,sys_time,strlen(sys_time));
       mpack_finish_array(&writer);
       if (mpack_writer_destroy(&writer) != mpack_ok) {
          	rv = ENCODE_DATA_ERR;
       }
	
    res_out->rv = res_out->rv;
   	res_out->seria_op = SERIAL_MPK;
   	res_out->slot = g_slot_id;
   	res_out->size = res_size;
   	res_out->seria_op = SERIAL_MPK;
   if(res_size != 0){
       res_out->content = malloc(res_size);
       memcpy(res_out->content,buf,res_size);
   }
   if(buf){
       free(buf);
   }
   return rv;
}



void init_msg_process_handle(void)
{
	nsrv_register_opcode(0,1,OPCODE_SYSTEM_GET_DATETIME,process_get_datetime,opcode_reg_end);
	nsrv_register_pipeline_opcode(0,1,OPCODE_SYSTEM_GET_DATETIME,process_pipeline_datetime,opcode_reg_end);
}

int statechange_handler(wk_st *worker_status, zlist_t *List)
{
    zsys_debug("wkst_change_handler recv msg slot is %d name is %s current is %d old is %d",worker_status->slot,
                                                                                            worker_status->uname,
                                                                                            worker_status->current_status,
                                                                                            worker_status->old_status);
	return 0;
}

int timeout_msg_handle()
{
    zsys_debug("this is callback function called timeout_msg_handle!");
    pipemsg_broadcast *pMsg = NULL;
    for (pMsg = zhash_first (pipe_res_list); pMsg != NULL; pMsg = zhash_next(pipe_res_list))
    {
        if((zclock_time() - pMsg->start_time) > pMsg->timeout)
        {
            zsys_debug("msg timeout,pipe_id is %s", pMsg->pipe_id);
            zhash_delete(pipe_res_list, pMsg->pipe_id);
        }
    }
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
	strcpy(init_data.worker_id,"om");

	#if 0
	while ((opt = getopt(argc, argv, "w:s:d")) > 0) {
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
	#endif

	int option_index = 0;
	enum opts {
		OPT_INSTALLDIR = 1,
		OPT_CONFFILE,
	};
	static struct option options[] = {
		{"help", no_argument, 0, 'h'},
		{"install-dir", required_argument, 0, OPT_INSTALLDIR},
		{"conf-file", required_argument, 0, OPT_CONFFILE}};
	char *install_dir = NULL;
	char *conf_file = NULL;
	
	while (1) {
		int c = getopt_long(argc, argv, "hd", options, &option_index);

		if (c == -1) {
			break;
		}
		switch (c) {
			case OPT_INSTALLDIR:
				install_dir = strdup(optarg);
				printf("Install Dir: %s\n", install_dir);
				break;
			case OPT_CONFFILE:
				conf_file = strdup(optarg);
				printf("Conf-file : %s\n", conf_file);
				break;
			case 'd':
				debug =1;
				break;
			case 'h':
			case '?':
				printf("tna_exact_match \n");
				printf(
				"Usage : tna_exact_match --install-dir <path to where the SDE is "
				"installed> --conf-file <full path to the conf file "
				"(tna_exact_match.conf)>\n");
				exit(c == 'h' ? 0 : 1);
				break;
			default:
				printf("Invalid option\n");
				exit(0);
				break;
		}
	}
	if (install_dir == NULL) {
	  printf("ERROR : --install-dir must be specified\n");
	  exit(0);
	}
	
	if (conf_file == NULL) {
	  printf("ERROR : --conf-file must be specified\n");
	  exit(0);
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

    openlog("copy_to_cpu", LOG_CONS | LOG_PID, LOG_USER);
	bfrt_test(1);
	config(install_dir, conf_file);
	copy_to_cpu_callback_register(0);
	bool add = 1;
	#if 0
	ipRoute_entry_add_or_modify_with_route(11, 12, 13, 14, 15, add);
	table_traversal();
	ipRoute_entry_add_or_modify_with_route(11, 12, 13, 14, 17, 0);
	table_traversal();
	ipRoute_entry_add_or_modify_with_route(21, 22, 23, 24, 25, add);
	table_traversal();
	printf("======================================================\n");
	ipRoute_entry_add_or_modify_with_nat(31, 32, 3232237009, 2887063802, 35, add);
	table_traversal();
	ipRoute_entry_add_or_modify_with_nat(31, 32, 3232237009, 2887063802, 40, 0);
	table_traversal();
	ipRoute_entry_add_or_modify_with_nat(41, 42, 43, 44, 45, add);
	table_traversal();
	printf("======================================================\n");
	ipRoute_entry_delete(11, 12);
	ipRoute_entry_delete(41, 42);
	table_traversal();
	printf("======================================================\n");
	ipRoute_entry_get(21, 22);
	ipRoute_entry_get(31, 32);
	#endif
	forward_entry_add_or_modify_with_hit(0x000000070809,138,add);
	forward_entry_add_or_modify_with_copy_to_cpu(0x000000040506,add);
	forward_table_traversal();
	forward_entry_get(0x000000040506);
	closelog();
	/*forward_entry_add_or_modify_with_hit(0xc0a805d1,134,0);
	forward_table_traversal();
	forward_entry_add_or_modify_with_hit(0xac1518fa,123,add);
	forward_table_traversal();
	printf("======================================================\n");
	forward_entry_delete(0xac1518fa);
	forward_table_traversal();
	printf("======================================================\n");
	forward_entry_get(0xc0a805d1);*/
	
	//fprintf(stderr,"slot_id:%d\n",g_slot_id);
	init_data.slot = g_slot_id;
	//create_om_pipe_line();
	pipe_res_list = zhash_new();
	assert(pipe_res_list);

	pipe_node worker_topo = NNGN_TOPO_NODE__INIT;
	worker_topo.port = 4007;
	strcpy(worker_topo.worker_id,"5-if_worker");
	//rv = add_pipeline_topo(&worker_topo,worker_top_more);

	//worker_topo = NNGN_TOPO_NODE__INIT;
	memset(worker_topo.worker_id,0,NNGX_ID_MAX_LENGTH);
	worker_topo.port = 4008;
	strcpy(worker_topo.worker_id,"6-sys_worker");
	//rv = add_pipeline_topo(&worker_topo,worker_top_end);

	//create_worker_pipe_line(g_slot_id , init_data.worker_id);

	#if 0
	rv = nsrv_new(&init_data);
	if(rv != 0){
		return rv;
	}
	#endif

	//_rpc_oe_get_system_datetime(6,23);

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

    ncb.type = CALLBACK_MSG_TIMEOUT;
    ncb.func = timeout_msg_handle;
    ngn_reg_cb(&ncb);

    //init_msg_process_handle();
    rv = setup_fault_trap();
   

    return rv;
}
