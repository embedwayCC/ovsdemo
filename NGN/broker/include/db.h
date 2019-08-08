#ifndef _DB_H_
#define _DB_H_
#include "global.h"
#include "nngn_struct.h"
#include <lmdb.h>


#define DB_TIMEOUT 3000
#define LMDB_MAP_SIZE 1073741824//lmdbµÄ´óÐ¡
#define DB_PATH_MAX_LEN  128

#define DB_FILE_PATH_REMOTE  "/usr/local/ngn/db"
#define DB_FILE_PATH_LOCAL    "/usr/local/ngn-local/db"

#define DB_FLAG_LOCAL 1
#define DB_FLAG_REMOTE 0

int MDB_CHECK(int mdb_status);
uint32_t database_init_local();
void database_exit_local();
uint32_t put_data_local(char *key,int key_size,char *value,int value_size,int db_flag);
uint32_t get_data_local(char *key,int key_size,char **value,int *value_size,int db_flag);

#endif

