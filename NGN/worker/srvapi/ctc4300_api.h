#ifndef _OPTIWAY_EXPII_CTC4300_API_H_
#define _OPTIWAY_EXPII_CTC4300_API_H_

#ifdef SW_CTC8096
#include "def.h"

#define NULL 0

int ow_ctc4300_dev_open(void);
int ow_ctc4300_dev_close(void);
int ow_ctc4300_init(void);
int ow_ctc4300_ctc_init(void);
int ow_ctc4300_sequence_init(void);
int ow_ctc4300_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able);
int ow_ctc4300_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec);
int ow_ctc4300_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec);
int ow_ctc4300_create_vlan_range(int index, int min, int max);
int ow_ctc4300_destroy_vlan_range(int index, int min, int max);
int ow_ctc4300_enable_acl_on_slot(int rule_id, 
                                int slot,
                                struct class_entry_t* p_class_entry,
                                int outer_vlan, 
                                int range_index, 
                                int action, 
                                opt_pbmp_t* f_ports, 
                                opt_pbmp_t* r_ports, 
                                opt_pbmp_t* b_ports,
                                opt_pbmp_t* cross_back,
                                unsigned int* outports,
                                unsigned int num_outports,
                                unsigned int entry_id);
int ow_ctc4300_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry);
int ow_ctc4300_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index);
int ow_ctc4300_trunk_del(int outid, struct smp_t *smp, int type);
int ow_ctc4300_ud_set(int index, int type, int value);
int ow_ctc4300_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp);
int ow_ctc4300_trunk_psc_set(int outid, int load_balance);
int ow_ctc4300_enable_backgroup_drop();
int ow_ctc4300_slot_load_balance_set(int all_load_balance, int slot_load_balance);
int ow_ctc4300_create_nexthop_on_slot(int rule_id, 
                                 int slot,
                                 int action,
                                 opt_pbmp_t* f_ports,
                                 opt_pbmp_t* r_ports,
                                 opt_pbmp_t* b_ports,
                                 opt_pbmp_t* cross_back,
                                 unsigned int* outports,
                                 unsigned int num_outports);
int ow_ctc4300_destroy_nexthop_on_slot(int rule_id);
int ow_ctc4300_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p);
int ow_ctc4300_printf_interface_sfp(struct board_sfp_t *board_sfp);
int ow_ctc4300_set_interface_force_up(struct smp_t *smp, int onoff);
int ow_ctc4300_soc_install(void);
int ow_ctc4300_soc_uninstall(void);
int ow_ctc4300_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight);
#endif

#endif

