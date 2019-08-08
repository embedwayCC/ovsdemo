#ifndef _OPTIWAY_EXPII_PUB_API_H_
#define _OPTIWAY_EXPII_PUB_API_H_

#include "rpc_def.h"
#include "def.h"
#include "imatrix/im-rpc.h"
#include "imatrix/im-init.h"

//for all solutions
#if (defined SW_BCM_5684X || defined SW_BCM_56960 || defined SW_CTC8096)
int ow_pub_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec);
int ow_pub_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec);
#endif

// SAME behavior in 56960 and ctc8096 defined here
#if (defined SW_BCM_56960 || defined SW_CTC8096)
int ow_pub_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p);
int ow_pub_printf_interface_sfp(struct board_sfp_t *board_sfp);
#endif

// SAME behavior in 5684X and 56960 defined here
#if (defined SW_BCM_5684X || defined SW_BCM_56960)
int ow_pub_sequence_init(void);
int ow_pub_create_vlan_range(int index, int min, int max);
int ow_pub_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able);
int ow_pub_enable_acl_on_slot(int rule_id, 
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
int ow_pub_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry);
int ow_pub_trunk_add(int outid, struct smp_t *smp, int load_balance, int *index);
int ow_pub_trunk_add_weight(int outid, struct smp_t *smp, int load_balance, int *index, int weight);
int ow_pub_trunk_del(int outid, struct smp_t *smp);
int ow_pub_ud_set(int index, int type, int value);
int ow_pub_destroy_vlan_range(int index, int min, int max);
int ow_pub_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp);
int ow_pub_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode);
int ow_pub_egress_fp_remove(int index);
int ow_pub_trunk_psc_set(int outid, int load_balance);
int ow_pub_slot_load_balance_set(int all_load_balance, int slot_load_balance);
int ow_pub_enable_backgroup_drop(void);
int ow_pub_bcm_init(void);

#endif

#endif

