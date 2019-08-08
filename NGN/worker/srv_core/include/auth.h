#ifndef __RCP_AUTH_H__
#define __RCP_AUTH_H__

#include <sys/time.h>
#include "rpc_def.h"

#define RCP_NISAC_MAX_USER_NAME	16

#define RCP_AUTH_CRYPT_PASSWD_OFFSET 12
#define RCP_AUTH_CRYPT_PASSWD_LEN 22

#define  CONFIG_DIR		"/sysfs"
#define AUTH_ACCOUNT_CONFIG CONFIG_DIR"/account"

#define ACCOUNT_HOME_DIR "/home/%s"

#define ACCOUNT_DEL_TOOL "/usr/local/bin/account_del.sh"

typedef struct USER_MGR_S {
	char login[MAX_USER_NAME_LEN];
	char password[RCP_AUTH_CRYPT_PASSWD_LEN+ 1];
	int active;
} USER_MGR_T;

typedef struct user_info_t USER_INFO_T;

extern void init_multi_account(void);
extern int get_user_info(int, int, USER_INFO_T *); //show users
extern int add_new_user(char *, char *, int); //set user username password pwd
extern int del_user(char *); //no user username
extern int get_user_level(char *, int *);
extern int set_user_level(char *, int); // set user username level type
extern int set_user_passwd(char *, char *); // set user username password pwd
extern int get_user_passwd(char *, char *);
extern int reset_user_passwd(char *);
extern int set_user_active(char *, unsigned int);
extern int auth_save_account_config(void);
#endif

