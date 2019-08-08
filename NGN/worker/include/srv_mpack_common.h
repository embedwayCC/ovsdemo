#ifndef SRV_MPACK_COMMON_H
#define SRV_MPACK_COMMON_H

#define MAX_RES_BUF_LEN 1024

typedef void (*sighandler_t)(int);

enum{
	NOT_SERIAL = 0,
	SERIAL_MPK = 1,
	SERIAL_PROTO = 2	
};

#endif
