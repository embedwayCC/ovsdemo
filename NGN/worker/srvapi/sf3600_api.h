#ifndef _OPTIWAY_EXPII_SF3600_API_H_
#define _OPTIWAY_EXPII_SF3600_API_H_

#include "def.h"

#ifdef SW_BCM_5684X
int ow_sf3600_dev_open(void);
int ow_sf3600_dev_close(void);
int ow_sf3600_init(void);
int ow_sf3600_bcm_init(void);
int ow_sf3600_sequence_init(void);
int ow_sf3600_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able);
int ow_sf3600_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec);
int ow_sf3600_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec);
int ow_sf3600_create_vlan_range(int index, int min, int max);
int ow_sf3600_destroy_vlan_range(int index, int min, int max);
int ow_sf3600_enable_acl_on_slot(int rule_id, 
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
int ow_sf3600_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry);
int ow_sf3600_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index);
int ow_sf3600_trunk_del(int outid, struct smp_t *smp, int type);
int ow_sf3600_ud_set(int index, int type, int value);
int ow_sf3600_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp);
int ow_sf3600_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode);
int ow_sf3600_egress_fp_remove(int index);
int ow_sf3600_trunk_psc_set(int outid, int load_balance);
int ow_sf3600_enable_backgroup_drop(void);
int ow_sf3600_slot_load_balance_set(int all_load_balance, int slot_load_balance);
int ow_sf3600_create_nexthop_on_slot(int rule_id, 
                                 int slot,
                                 int action,
                                 opt_pbmp_t* f_ports,
                                 opt_pbmp_t* r_ports,
                                 opt_pbmp_t* b_ports,
                                 opt_pbmp_t* cross_back,
                                 unsigned int* outports,
                                 unsigned int num_outports);
int ow_sf3600_destroy_nexthop_on_slot(int rule_id);
int ow_sf3600_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p);
int ow_sf3600_printf_interface_sfp(struct board_sfp_t *board_sfp);
int ow_sf3600_set_interface_force_up(struct smp_t *smp, int onoff);
int ow_sf3600_soc_install(void);
int ow_sf3600_soc_uninstall(void);
int ow_sf3600_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight);
#endif

#endif
