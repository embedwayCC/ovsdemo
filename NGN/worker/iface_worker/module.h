#ifndef __EXP_MODULE_H__
#define __EXP_MODULE_H__

int module_set_port_autoneg(struct smp_t *smp, int onoff);
int OeMdl_Set_Interface_Autoneg(int modules, unsigned long long ports, int onoff);
int module_set_port_pause(struct smp_t *smp, int onoff);
int OeMdl_Set_Interface_Pause(int modules, unsigned long long ports, int onoff);
int module_set_port_shutdown(struct smp_t *smp, int shutdown);
int OeMdl_Set_Interface_Shutdown(int modules, unsigned long long ports, int shutdown);
int module_set_port_duplex(struct smp_t *smp, int duplex);
int OeMdl_Set_Interface_Duplex(int modules, unsigned long long ports, int duplex);
int module_set_port_speed(struct smp_t *smp, int speed);
int OeMdl_Set_Interface_Speed(int modules, unsigned long long ports, int speed);
int _check_if_used_by_trunk(int module, int port);
int module_set_port_mac_address_learning(struct smp_t *smp, int onoff);
int OeMdl_Set_Interface_Mac_Address_Learning( int modules, unsigned long long ports, int onoff);
int module_get_bcm_port_status(int module, int port, struct port_status_t *port_status_p);
int module_get_interface_status(struct smp_t *smp, struct oe_status_t *if_status_attr);
int module_get_interface_counter(struct smp_t *smp, int verbose, struct oe_counter_t *if_counter_attr);
int module_set_port_mode(struct smp_t *smp, int able);
int OeMd_Set_Interface_Mode(int modules, unsigned long long ports, int mode);
int OeMdl_Set_Interface_Descript(int modules, unsigned long long ports, char* descript);
int module_clear_interface_counter(int module, unsigned long long ports);
int module_get_interface_info(struct smp_t *smp, struct oe_info_t *if_sfp_attr);
int OeMdl_Set_Interface_Single_Fiber_Transmit(int modules, unsigned long long ports, int onoff);
int OeMdl_Initialize(void);
int module_init();
#endif

