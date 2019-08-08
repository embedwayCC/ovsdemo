#ifndef __EXP_SYS_H__
#define __EXP_SYS_H__

#define DEFAULT_DEVICE_NAME "ExTAP 5100"
#define DUAL_STAR_MODE 0
#define DUAL_DUAL_STAR_MODE 1


void system_reset_system(void);
int set_board_platform(int platform);
int system_set_platform(int slot_id, int platform);
int system_set_sysname(char *device_name);
int system_set_datetime(int slot, char *sys_time);
int system_get_datetime(char *sys_time);
int system_set_timezone(int slot, char *timezone);
int system_ipconfig(char *iface, int ip, int mask);
int system_get_gateway_static(void);
int system_gateway(int ip);
int system_opmode_set(int slot_id, int mode);
int system_rcload_bcm_config(int slot_id, char *filename);
int system_set_chassis_id(int id);
int system_set_warm_syn(int able);
int set_chassis_mode(int mode);
int system_cpu_usage_info_thread(void);
#endif
