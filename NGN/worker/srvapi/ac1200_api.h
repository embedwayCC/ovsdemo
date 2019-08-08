#ifndef _OPTIWAY_EXPII_AC1200_API_H_
#define _OPTIWAY_EXPII_AC1200_API_H_

#include "def.h"
#include "rpc_def.h"

#ifdef SW_BCM_56334
int ow_ac1200_dev_open(void);
int ow_ac1200_dev_close(void);
int ow_ac1200_sequence_init(void);
int ow_ac1200_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able);
int ow_ac1200_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec);
int ow_ac1200_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec);
int ow_ac1200_enable_acl_on_slot(int rule_id, 
                                int slot,
                                oe_class_entry_t* p_class_entry,
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
int ow_ac1200_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index);
int ow_ac1200_trunk_del(int outid, struct smp_t *smp, int type);

#endif

#endif

