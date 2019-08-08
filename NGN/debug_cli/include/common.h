#ifndef COMMON_H
#define COMMON_H

#define CLI_MAX_STR_LEN 128
#define MAX_CMD_LEN 1024
#define MAC_ELEMENT 6
#define IPV6_ELEMENT 8

#define ALL_SLOT_MASK (0x3fff)
#define ALL_MODULE_MASK (0x7f)
#define ALL_PORT_MASK (0xFFFFFFFFFFFFFFLL)

#define MAX_SLOT_NUM 14

enum{
	MODULE_REAR = 4,
	MODULE_FRONT,
	MODULE_BACKPLANE
};

int common_init();

#endif
