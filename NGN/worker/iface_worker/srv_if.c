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
#include "module.h"
#include "app.h"

extern int my_slot_id;

int process_if_set_port_autoneg(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int autoneg = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	autoneg = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_autoneg(&smp, autoneg);
	if(rv == 0){
		OeMdl_Set_Interface_Autoneg(smp.modules, smp.ports, autoneg);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;
}

int process_if_set_port_pause(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int able = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	able = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_pause(&smp,able);
	if(rv == 0){
		OeMdl_Set_Interface_Autoneg(smp.modules, smp.ports, able);
	}


END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;
}

int process_if_set_port_shutdown(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int able = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	able = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_shutdown(&smp,able);
	if(rv == 0){
		OeMdl_Set_Interface_Shutdown(smp.modules, smp.ports, able);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

int process_if_set_port_duplex(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int duplex = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	duplex = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_duplex(&smp,duplex);
	if(rv == 0){
		OeMdl_Set_Interface_Duplex(smp.modules, smp.ports, duplex);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

int process_if_set_port_speed(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int speed = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	speed = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_speed(&smp,speed);
	if(rv == 0){
		OeMdl_Set_Interface_Speed(smp.modules, smp.ports, speed);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

int process_if_set_port_mac_address_learning(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int able = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	able = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_mac_address_learning(&smp,able);
	if(rv == 0){
		OeMdl_Set_Interface_Mac_Address_Learning(smp.modules, smp.ports, able);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

static int _interface_status_convert_res(char **res_buf, size_t *size, struct oe_status_t *status)
{
    #if 0
    int rv = OE_RST_OK;
    int i = 0, j = 0;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, res_buf, size);

    mpack_start_array(&writer, 2);
    mpack_write_i32(&writer, status->n_list_status);
    mpack_start_array(&writer, status->n_list_status);
    for (i=0; i<status->n_list_status; i++) {
    	mpack_start_array(&writer, 4);
    	mpack_write_i32(&writer, status->list_status[i].sm_common.slot_id);
    	mpack_write_i32(&writer, status->list_status[i].sm_common.module_id);
    	mpack_write_i32(&writer, status->list_status[i].n_port_status);
    	mpack_start_array(&writer, status->list_status[i].n_port_status);
    	for (j=0; j< status->list_status[i].n_port_status; j++) {
    		mpack_start_array(&writer, 11);
    		mpack_write_i32(&writer, status->list_status[i].port_status[j].smp_common.port_id);
    		mpack_write_i32(&writer, status->list_status[i].port_status[j].smp_common.port_valid_flag);
    		mpack_write_i32(&writer, status->list_status[i].port_status[j].smp_common.port_insert_flag);
    		mpack_write_i32(&writer, status->list_status[i].port_status[j].link);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].autoneg);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].pause);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].duplex);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].speed);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].mac_address_learning);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].mtu);
			mpack_write_str(&writer,status->list_status[i].port_status[j].port_name,MAX_PB_STR_LEN);
    		mpack_finish_array(&writer);
    	}
    	mpack_finish_array(&writer);
    	mpack_finish_array(&writer);
    }
    mpack_finish_array(&writer);
    mpack_finish_array(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok) {
    	rv = ENCODE_DATA_ERR;
    }
    #endif

    int rv = OE_RST_OK;
    int i = 0, j = 0;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, res_buf, size);
    mpack_start_array(&writer, 13);
    mpack_write_cstr(&writer, "slot-id");
    mpack_write_cstr(&writer, "module-id");
    mpack_write_cstr(&writer, "port-id");
    mpack_write_cstr(&writer, "port-valid-flag");
    mpack_write_cstr(&writer, "port-insert-flag");
    mpack_write_cstr(&writer, "link");
    mpack_write_cstr(&writer, "autoneg");
    mpack_write_cstr(&writer, "pause");
    mpack_write_cstr(&writer, "duplex");
    mpack_write_cstr(&writer, "speed");
    mpack_write_cstr(&writer, "mac-address-learning");
    mpack_write_cstr(&writer, "mtu");
    mpack_write_cstr(&writer, "port_name");
    mpack_finish_array(&writer);

    for (i=0; i<status->n_list_status; i++)
    {
        for (j=0; j< status->list_status[i].n_port_status; j++)
        {
            mpack_start_array(&writer, 13);
            mpack_write_i32(&writer, status->list_status[i].sm_common.slot_id);
            mpack_write_i32(&writer, status->list_status[i].sm_common.module_id);
            mpack_write_i32(&writer, status->list_status[i].port_status[j].smp_common.port_id);
            mpack_write_i32(&writer, status->list_status[i].port_status[j].smp_common.port_valid_flag);
    		mpack_write_i32(&writer, status->list_status[i].port_status[j].smp_common.port_insert_flag);
    		mpack_write_i32(&writer, status->list_status[i].port_status[j].link);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].autoneg);
    	    mpack_write_i32(&writer, status->list_status[i].port_status[j].pause);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].duplex);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].speed);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].mac_address_learning);
			mpack_write_i32(&writer, status->list_status[i].port_status[j].mtu);
			mpack_write_str(&writer,status->list_status[i].port_status[j].port_name,MAX_PB_STR_LEN);
    		mpack_finish_array(&writer);
        }
    }

    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
    	rv = ENCODE_DATA_ERR;
    }
    return rv;
}


int process_if_get_interface_status(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	struct smp_t smp;
	struct oe_status_t if_status_attr;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,3);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	memset(&if_status_attr, 0, sizeof(if_status_attr));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_get_interface_status(&smp, &if_status_attr);

	if(rv == 0){
		rv = _interface_status_convert_res(&res_buf, &res_size,&if_status_attr);
	}

//dbug
#if 0
	mpack_tree_t tree;
	mpack_tree_init(&tree,res_buf,res_size);
	mpack_node_print_file(mpack_tree_root(&tree), stdout);
	mpack_tree_destroy(&tree);
#endif

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

static int _slots_interface_counter_convert_to_res(char **res_buf, size_t *size, struct oe_counter_t *if_counter)
{
    int rv = OE_RST_OK;
    int i = 0, j = 0, ci = 0;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, res_buf, size);

    mpack_start_array(&writer, 2);
    mpack_write_i32(&writer, if_counter->n_board_counter);
    mpack_start_array(&writer, if_counter->n_board_counter);
    for (i=0; i<if_counter->n_board_counter; i++) {
        mpack_start_array(&writer, 4);
        mpack_write_i32(&writer, if_counter->board_counter[i].sm_common.slot_id);
        mpack_write_i32(&writer, if_counter->board_counter[i].sm_common.module_id);
        mpack_write_i32(&writer, if_counter->board_counter[i].n_port_counter);
        mpack_start_array(&writer, if_counter->board_counter[i].n_port_counter);
        for (j=0; j< if_counter->board_counter[i].n_port_counter; j++) {
            mpack_start_array(&writer, 5);
            mpack_write_i32(&writer, if_counter->board_counter[i].port_counter[j].smp_common.port_id);
            mpack_write_i32(&writer, if_counter->board_counter[i].port_counter[j].smp_common.port_valid_flag);
            mpack_write_i32(&writer, if_counter->board_counter[i].port_counter[j].smp_common.port_insert_flag);
            mpack_write_i32(&writer, if_counter->board_counter[i].port_counter[j].n_counter);
            mpack_start_array(&writer, if_counter->board_counter[i].port_counter[j].n_counter);
            for (ci=0; ci< if_counter->board_counter[i].port_counter[j].n_counter; ci++) {
                mpack_start_array(&writer, 4);
                mpack_write_i32(&writer, if_counter->board_counter[i].port_counter[j].counter[ci].type);
                mpack_write_str(&writer, if_counter->board_counter[i].port_counter[j].counter[ci].name,
                    sizeof(if_counter->board_counter[i].port_counter[j].counter[ci].name));
                mpack_write_u64(&writer, if_counter->board_counter[i].port_counter[j].counter[ci].total);
                mpack_write_u64(&writer, if_counter->board_counter[i].port_counter[j].counter[ci].rate);
                mpack_finish_array(&writer);
            }
            mpack_finish_array(&writer);
            mpack_finish_array(&writer);
        }
        mpack_finish_array(&writer);
        mpack_finish_array(&writer);
    }
    mpack_finish_array(&writer);
    mpack_finish_array(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok) {
    	rv = OE_RST_INVLD_PRM;
    }

    return rv;
}


int process_if_get_interface_counter(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	struct smp_t smp;
	struct oe_counter_t if_counter_attr;
	int verbose = 0;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	verbose = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	memset(&if_counter_attr, 0, sizeof(if_counter_attr));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_get_interface_counter(&smp, verbose, &if_counter_attr);

	if(rv == 0){
		rv = _slots_interface_counter_convert_to_res(&res_buf, &res_size, &if_counter_attr);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

int process_if_clear_interface_counter(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int i = 0;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,3);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	for(i = MODULE_REAR; i < MODULE_REAR + MODULE_NUMBER_PER_BOARD; i++) {
		if(module & 1 << i) {
			rv = module_clear_interface_counter(i,port);
			if (rv != 0) {
				break;
			}
		}
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

int process_if_set_port_mode(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	struct smp_t smp;
	uint64_t mode = 0;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	mode = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_set_port_mode(&smp, mode);
	if(rv == 0){
		OeMd_Set_Interface_Mode(module, port, mode);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

int process_if_set_port_descript(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	struct smp_t smp;
	char desc[128] = {0};

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	mpack_expect_str_buf(&reader,desc,sizeof(desc));
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	if(rv == 0){
		OeMdl_Set_Interface_Descript(module, port, desc);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

static int _interface_info_convert(char **res_buf, size_t *size, struct oe_info_t *info)
{
    int rv = OE_RST_OK;
    int i = 0, j = 0;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, res_buf, size);

    mpack_start_array(&writer, 2);
    mpack_write_i32(&writer, info->n_board_sfp);
    mpack_start_array(&writer, info->n_board_sfp);
    for (i=0; i< info->n_board_sfp; i++) {
    	mpack_start_array(&writer, 4);
    	mpack_write_i32(&writer, info->board_sfp[i].sm_common.slot_id);
    	mpack_write_i32(&writer,  info->board_sfp[i].sm_common.module_id);
    	mpack_write_i32(&writer,  info->board_sfp[i].n_port_sfp);
    	mpack_start_array(&writer,info->board_sfp[i].n_port_sfp);
    	for (j=0; j< info->board_sfp[i].n_port_sfp; j++) {
    		mpack_start_array(&writer, 33);
    		mpack_write_i32(&writer, info->board_sfp[i].port_sfp[j].smp_common.port_id);
    		mpack_write_i32(&writer, info->board_sfp[i].port_sfp[j].smp_common.port_valid_flag);
    		mpack_write_i32(&writer, info->board_sfp[i].port_sfp[j].smp_common.port_insert_flag);
			mpack_write_str(&writer, info->board_sfp[i].port_sfp[j].port_name,MAX_PB_STR_LEN);
			mpack_write_str(&writer, info->board_sfp[i].port_sfp[j].port_descr,MAX_PB_STR_LEN);
    		mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].port_type);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].link_proto);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].running_status);
    		mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].present);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_power);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].tx_power_flag);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_dbm);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_dbm_1);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_dbm_2);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_dbm_3);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_power);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].rx_power_flag);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_dbm);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_dbm_1);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_dbm_2);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_dbm_3);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].valid_window_flag);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_min);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].rx_max);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_min);
			mpack_write_i64(&writer,  info->board_sfp[i].port_sfp[j].tx_max);
			mpack_write_str(&writer, info->board_sfp[i].port_sfp[j].vendor_name,64);
			mpack_write_str(&writer, info->board_sfp[i].port_sfp[j].vendor_pn,64);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].wavelength);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].module_type);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].speed);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].temperature);
			mpack_write_i32(&writer,  info->board_sfp[i].port_sfp[j].temperature_flag);
    		mpack_finish_array(&writer);
    	}
    	mpack_finish_array(&writer);
    	mpack_finish_array(&writer);
    }
    mpack_finish_array(&writer);
    mpack_finish_array(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok) {
    	rv = OE_RST_INVLD_PRM;
    }

    return rv;

}


int process_if_get_interface_info(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	struct smp_t smp;
	struct oe_info_t if_sfp_attr;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,3);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = module_get_interface_info(&smp, &if_sfp_attr);
	if(rv == 0){
		_interface_info_convert(&res_buf, &res_size, &if_sfp_attr);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;

}

extern int ow_set_interface_force_up(struct smp_t *,int);
int process_if_set_port_single_fiber_transmit(req_t *req, res_t *res)
{
	int rv = 0;
	mpack_reader_t reader;
	char *res_buf = NULL;
	size_t size = 0, res_size = 0;
	uint64_t slot = 0, module = 0, port = 0;
	int able = 0;
	struct smp_t smp;

	size = req->size;
	mpack_reader_init(&reader,(char *)req->content,sizeof((char *)req->content),size);
	mpack_expect_array_match(&reader,4);
	slot = mpack_expect_u64(&reader);
	module = mpack_expect_u64(&reader);
	port = mpack_expect_u64(&reader);
	able = mpack_expect_u64(&reader);
	mpack_done_array(&reader);
	if (mpack_reader_destroy(&reader) != mpack_ok) {
		rv = DECODE_DATA_ERR;
		goto END;
	}

	memset(&smp, 0, sizeof(smp));
	smp.slots = slot;
	smp.modules = module;
	smp.ports = port;
	rv = ow_set_interface_force_up(&smp,able);
	if(rv == 0){
		OeMdl_Set_Interface_Single_Fiber_Transmit(smp.modules, smp.ports, able);
	}

END:
	res->rv = rv;
	res->slot = my_slot_id + 1;
	res->size = res_size;
	res->seria_op = SERIAL_MPK;
	if(res_size != 0){
		res->content = malloc(res_size);
		memcpy(res->content,res_buf,res_size);
	}
	if(res_buf){
		free(res_buf);
	}

	return rv;
}

void init_msg_process_handle(void)
{
	nsrv_register_opcode(0,1,21,process_if_set_port_autoneg,opcode_reg_more);
	nsrv_register_opcode(0,1,22,process_if_get_interface_status,opcode_reg_more);
	nsrv_register_opcode(0,1,23,process_if_set_port_pause,opcode_reg_more);
	nsrv_register_opcode(0,1,24,process_if_set_port_shutdown,opcode_reg_more);
	nsrv_register_opcode(0,1,25,process_if_set_port_duplex,opcode_reg_more);
	nsrv_register_opcode(0,1,26,process_if_set_port_speed,opcode_reg_more);
	nsrv_register_opcode(0,1,27,process_if_set_port_mac_address_learning,opcode_reg_more);
	nsrv_register_opcode(0,1,28,process_if_get_interface_counter,opcode_reg_more);
	nsrv_register_opcode(0,1,29,process_if_clear_interface_counter,opcode_reg_more);
	nsrv_register_opcode(0,1,30,process_if_set_port_mode,opcode_reg_more);
	nsrv_register_opcode(0,1,31,process_if_set_port_descript,opcode_reg_more);
	nsrv_register_opcode(0,1,32,process_if_get_interface_info,opcode_reg_more);
	nsrv_register_opcode(0,1,33,process_if_set_port_single_fiber_transmit,opcode_reg_end);
}


int main(int argc, char **argv)
{
	int rv = 0;
	int opt;
	sinit_data_t init_data;
	char *work_id = NULL;
	int slot_id = 0;

	memset(&init_data,0,sizeof(init_data));
	strcpy(init_data.broker_addr,"127.0.0.1");
	strcpy(init_data.worker_id,"iface");

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
            printf("Usage: system_worker [-i broker_addr] [-w work_id] \n");
            exit(0);
            break;
        }
    }

	if(work_id){
		memset(init_data.worker_id,0,sizeof(init_data.worker_id));
		sprintf(init_data.worker_id,"%s",work_id);
		free(work_id);
	}

	/*need define bcm type before init*/
	rv = module_init();
	if(rv != 0){
		fprintf(stderr,"init error\n");
		exit(0);
	}

	slot_id = OeAp_Get_My_Slot_Id();
	init_data.slot = slot_id + 1;

	rv = nsrv_new(&init_data);
	if(rv != 0){
		return rv;
	}

	init_msg_process_handle();
	rv = setup_fault_trap();
	return rv;
}


