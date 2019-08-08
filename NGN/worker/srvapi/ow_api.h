#ifndef _OPTIWAY_EXPII_OW_API_H_
#define _OPTIWAY_EXPII_OW_API_H_

#include "rpc_def.h"
#include "def.h"


typedef struct ow_api_s
{
    int (*ow_dev_open)(void);
    int (*ow_dev_close)(void);
    int (*ow_init)(void);
    int (*ow_bcm_init)(void);
    int (*ow_sequence_init)(void);
    int(*ow_pvlan_set)(int, struct smp_t *, int);
    int(*ow_vlan_add)(int, slot_vlan_record_t * , slot_vlan_record_t *);
    int(*ow_vlan_delete)(int, int, slot_vlan_record_t * );
    int(*ow_create_vlan_range)(int, int, int);
    int(*ow_destroy_vlan_range)(int, int, int);
    int(*ow_enable_acl_on_slot)(int rule_id, 
                                int slot,
                                struct class_entry_t* p_class_entry,
                                int outer_vlan, 
                                int range_index, 
                                int action, 
                                opt_pbmp_t *f_ports, 
                                opt_pbmp_t *r_ports, 
                                opt_pbmp_t *b_ports,
                                opt_pbmp_t *cross_back,
                                unsigned int* outports,
                                unsigned int num_outports,
                                unsigned int entry_id);
    int (*ow_disable_acl_on_slot)(int, oe_class_entry_t*);
    int (*ow_trunk_add)(int, struct smp_t *, int, int, int*);
    int (*ow_trunk_del)(int, struct smp_t *, int);
    int (*ow_ud_set)(int, int, int);
    int (*ow_crossback_pbmp_get)(opt_pbmp_t *, opt_pbmp_t *);
    int (*ow_egress_fp_set)(int, int, int, int, int, int);
    int (*ow_egress_fp_remove)(int);
    int (*ow_trunk_psc_set)(int, int);
    int (*ow_enable_backgroup_drop)(void);
    int (*ow_slot_load_balance_set)(int, int);
    int (*ow_create_nexthop_on_slot)(int rule_id, 
                                int slot,
                                int action, 
                                opt_pbmp_t *f_ports, 
                                opt_pbmp_t *r_ports, 
                                opt_pbmp_t *b_ports,
                                opt_pbmp_t *cross_back,
                                unsigned int* outports,
                                unsigned int num_outports);
   int (*ow_destroy_nexthop_on_slot)(int);
   int (*ow_get_port_info)(int, int, struct port_sfp_t *);
   int (*ow_printf_interface_sfp)(struct board_sfp_t *board_sfp);
   int (*ow_set_interface_force_up)(struct smp_t *smp, int onoff);
   int (*ow_soc_install)(void);
   int (*ow_soc_uninstall)(void);
   int (*ow_trunk_add_weight)(int, struct smp_t *, int, int, int*, int);
}ow_api_t;

extern int ow_install_api();
extern int ow_uninstall_api();
extern int ow_dev_open(void);
extern int ow_dev_close(void);
extern int ow_init(void);
extern int ow_bcm_init(void);
extern int ow_sequence_init(void);
extern int ow_pvlan_set(int, struct smp_t *, int pvlan_able);
extern int ow_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec);
extern int ow_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec);
extern int ow_create_vlan_range(int index, int min, int max);
extern int ow_destroy_vlan_range(int index, int min, int max);
extern int ow_enable_acl_on_slot(int rule_id, 
                                int slot,
                                struct class_entry_t* p_class_entry,
                                int outer_vlan, 
                                int range_index, 
                                int action, 
                                opt_pbmp_t f_ports, 
                                opt_pbmp_t r_ports, 
                                opt_pbmp_t b_ports,
                                opt_pbmp_t cross_back,
                                unsigned int* outports,
                                unsigned int num_outports,
                                unsigned int entry_id);
extern int ow_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry);
extern int ow_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index);
extern int ow_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight);
extern int ow_trunk_del(int outid, struct smp_t *smp, int type);
extern int ow_ud_set(int index, int type, int value);
extern int ow_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp);
extern int ow_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode);
extern int ow_egress_fp_remove(int index);
extern int ow_trunk_psc_set(int outid, int load_balance);
extern int ow_enable_backgroup_drop(void);
extern int ow_slot_load_balance_set(int all_load_balance, int slot_load_balance);
extern int ow_create_nexthop_on_slot(int rule_id, 
                                int slot,
                                int action, 
                                opt_pbmp_t f_ports, 
                                opt_pbmp_t r_ports, 
                                opt_pbmp_t b_ports,
                                opt_pbmp_t cross_back,
                                unsigned int* outports,
                                unsigned int num_outports);
extern int ow_destroy_nexthop_on_slot(int rule_id);
extern int ow_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p);
extern int ow_printf_interface_sfp(struct board_sfp_t *board_sfp);
extern int ow_set_interface_force_up(struct smp_t *smp, int onoff);
extern int ow_soc_install(void);
extern int ow_soc_uninstall(void);
#endif
