#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.h"
#include "mpack.h"
#include "mpack-config.h"

#include "jsmn.h"
//#define MP_SOURCE
//#include "msgpunk.h"
enum { E_UNKNOWN,E_UINT,E_DOUBLE,E_STRING,E_BOOL };

static int str_type(char *str,uint64_t *di,double *dd, int* db)
{
	struct in_addr addr;
    char *endptr=NULL;
    int s = 0;


    *di= strtoll(str,&endptr,0);
    if(*endptr=='\0') {
        return E_UINT;
    }

    endptr=NULL;
    *dd= strtod(str,&endptr);
    if(*endptr=='\0') {
        return E_DOUBLE;
    }

    s = inet_pton(AF_INET, str, (void *)&addr);
    //FIXME:字节序的问题
    if ( s > 0 ) {
        //fprintf(stderr, "%lx\n",*di);
		*di = addr.s_addr;
        return E_UINT;
    }

    if(strcmp(str,"true")==0) {
        *db=1;
        return E_BOOL;
    }

    if(strcmp(str,"false")==0) {
        *db=0;
        return E_BOOL;
    }
                   
    return E_STRING;
}

#if 0
static int dump2mp(const char *js, jsmntok_t *t, size_t count, char *out_msg_data, int *len) 
{
        int i = 0;
        char *str;
        uint64_t number;
        double  number_d;
        int  number_b;
        char msgbuf[MAX_CMD_LEN];
        char *w = msgbuf;

        for( i=0;i<count;i++,t++) {
                if (t->type == JSMN_PRIMITIVE) {
                        str=strndup(js+t->start,t->end - t->start);
                        switch( str_type( str, &number, &number_d, &number_b ) ) {
                        case E_UINT:
                                w = mp_encode_uint(w, number);
                                break;
                        case E_DOUBLE:
                                w = mp_encode_double(w, number_d);
                                break;
                        case E_BOOL:
                                w = mp_encode_bool(w, number_b );
                                break;
                        case E_STRING:
                                w = mp_encode_str(w, str ,t->end - t->start);
                                break;
                        default:
                                break;
                        }
                        free(str);

                } else if (t->type == JSMN_STRING) {
						char tmp_str[CLI_MAX_STR_LEN];
						int mac[MAC_ELEMENT],ip[IPV6_ELEMENT];
						unsigned char real_mac[MAC_ELEMENT];
						unsigned char real_ip[IPV6_ELEMENT];
						int mac_num = 0,j = 0, ip_num = 0;
						char pack_str[CLI_MAX_STR_LEN] = {0};
						int size = 0;

						sprintf(tmp_str,js+t->start,t->end - t->start);
						mac_num = sscanf(tmp_str,"%02x:%02x:%02x:%02x:%02x:%02x",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
						ip_num = sscanf(tmp_str,"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",&ip[0],&ip[1],&ip[2],&ip[3],&ip[4],&ip[5],&ip[6],&ip[7]);
						if(mac_num == MAC_ELEMENT && ip_num == 0){
							for(j = 0; j < MAC_ELEMENT; j++){
								real_mac[j] = (unsigned char)mac[j];
							}
							size =MAC_ELEMENT; 
							memcpy(pack_str,real_mac,size);
						}else if(ip_num == IPV6_ELEMENT){
							for(j = 0; j < IPV6_ELEMENT; j++){
								real_ip[j] = (unsigned char)ip[j];
							}
							size = IPV6_ELEMENT;
							memcpy(pack_str,real_ip,size);
						}else{
							sprintf(pack_str,js+t->start,t->end - t->start);
							size = t->end - t->start;
						}
						w = mp_encode_str(w, (char *)pack_str, size);
						#if 0
							w = mp_encode_str(w, (char *)real_mac, MAC_ELEMENT);
						}else{
							w = mp_encode_str(w, js+t->start, t->end - t->start);
						}
						#endif
                } else if (t->type == JSMN_OBJECT) {
                        w = mp_encode_map(w, t->size);
                } else if (t->type == JSMN_ARRAY) {
                        w = mp_encode_array(w, t->size);
                }

        }

        //fwrite(msgbuf,w-msgbuf,1,stdout);
		memcpy(out_msg_data,msgbuf,w-msgbuf);
		*len = w-msgbuf;

        return 0;
}
#else
static int dump2mp(const char *js, jsmntok_t *t, size_t count, char *out_msg_data, int *len) 
{
        int i = 0;
        char *str;
        uint64_t number;
        double  number_d;
        int  number_b;
		mpack_writer_t writer;
		char *buf = NULL;
		size_t size = 0;
		int map_flag = 0, map_num = 0, map_total_num = 0;
		int arrary_flag = 0, arrary_num = 0, arrary_total_num = 0;

		mpack_writer_init_growable(&writer, &buf, &size);

        for( i=0;i<count;i++,t++) {
                if (t->type == JSMN_PRIMITIVE) {
                        str=strndup(js+t->start,t->end - t->start);
                        switch( str_type( str, &number, &number_d, &number_b ) ) {
                        case E_UINT:
								mpack_write_uint(&writer,number);
                                break;
                        case E_DOUBLE:
								mpack_write_double(&writer,number);
                                break;
                        case E_BOOL:
								mpack_write_bool(&writer,number_b);
                                break;
                        case E_STRING:
								mpack_write_str(&writer,str,t->end - t->start);
                                break;
                        default:
                                break;
                        }
                        free(str);
						if(map_flag == 1 && map_num != map_total_num){
							map_num++;
						}
						if(map_flag == 1 && map_num == map_total_num){
							map_flag = 0;
							map_num = 0;
							map_total_num = 0;
							mpack_finish_map(&writer);
						}
						if(arrary_flag == 1 && arrary_num != arrary_total_num){
							arrary_num++;
						}
						if(arrary_flag == 1 && arrary_num == arrary_total_num){
							arrary_flag = 0;
							arrary_num = 0;
							arrary_total_num = 0;
							mpack_finish_array(&writer);
						}

                } else if (t->type == JSMN_STRING) {
						char tmp_str[CLI_MAX_STR_LEN];
						int mac[MAC_ELEMENT],ip[IPV6_ELEMENT];
						unsigned char real_mac[MAC_ELEMENT];
						unsigned char real_ip[IPV6_ELEMENT];
						int mac_num = 0,j = 0, ip_num = 0;
						char pack_str[CLI_MAX_STR_LEN] = {0};
						int size = 0;

						sprintf(tmp_str,js+t->start,t->end - t->start);
						mac_num = sscanf(tmp_str,"%02x:%02x:%02x:%02x:%02x:%02x",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
						ip_num = sscanf(tmp_str,"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",&ip[0],&ip[1],&ip[2],&ip[3],&ip[4],&ip[5],&ip[6],&ip[7]);
						if(mac_num == MAC_ELEMENT && ip_num == 0){
							for(j = 0; j < MAC_ELEMENT; j++){
								real_mac[j] = (unsigned char)mac[j];
							}
							size =MAC_ELEMENT; 
							memcpy(pack_str,real_mac,size);
						}else if(ip_num == IPV6_ELEMENT){
							for(j = 0; j < IPV6_ELEMENT; j++){
								real_ip[j] = (unsigned char)ip[j];
							}
							size = IPV6_ELEMENT;
							memcpy(pack_str,real_ip,size);
						}else{
							sprintf(pack_str,js+t->start,t->end - t->start);
							size = t->end - t->start;
						}
						mpack_write_str(&writer, (char *)pack_str, size);
						if(map_flag == 1 && map_num != map_total_num){
							map_num++;
						}
						if(map_flag == 1 && map_num == map_total_num){
							map_flag = 0;
							map_num = 0;
							map_total_num = 0;
							mpack_finish_map(&writer);
						}
						if(arrary_flag == 1 && arrary_num != arrary_total_num){
							arrary_num++;
						}
						if(arrary_flag == 1 && arrary_num == arrary_total_num){
							arrary_flag = 0;
							arrary_num = 0;
							arrary_total_num = 0;
							mpack_finish_array(&writer);
						}

                } else if (t->type == JSMN_OBJECT) {
                        mpack_start_map(&writer, t->size);
						map_flag = 1;
						map_total_num = t->size;
                } else if (t->type == JSMN_ARRAY) {
                        mpack_start_array(&writer, t->size);
						arrary_flag = 1;
						arrary_total_num = t->size;
                }

        }

		if (mpack_writer_destroy(&writer) != mpack_ok) {
			fprintf(stderr, "An error occurred encoding the data!\n");
			return 0;
		}

		memcpy(out_msg_data,buf,size);
		*len = size;

        return 0;
}

#endif

int json2msgpack(char *input, char *out_msg_data, int *len) 
{
    jsmn_parser p;
    char js[MAX_CMD_LEN];
    jsmntok_t tok[CLI_MAX_STR_LEN];
    int r;

    strcpy(js,input);

    /* Prepare parser */
    jsmn_init(&p);

    r = jsmn_parse(&p, js, sizeof(js), tok, sizeof(tok)/sizeof(tok[0]));
    
    if (r < 0) {
            return 1;
    } else {
            dump2mp(js, tok, p.toknext, out_msg_data, len);
    }

    return 0;
}

#if 0
int msgpack2json(char *msg_str)
{
	#if 0
    char buf[1024];
    
    fread(buf,1,sizeof(buf),stdin);

    mp_fprint(NULL,buf);
	#endif
	mp_fprint(NULL,msg_str);

    return 0;

}
#else
int msgpack2json(char *msg_str, int len)
{
	mpack_tree_t tree;

	mpack_tree_init(&tree,msg_str,len);
	mpack_node_print_file(mpack_tree_root(&tree), stdout);
	mpack_tree_destroy(&tree);

	return 0;
}
#endif


