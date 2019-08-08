#include <stdio.h>

#include "ow_api.h"
#include "def.h"

ow_api_t *ow_api = NULL;
int g_ow_api_en = 0;

#ifdef SW_BCM_5684X
extern ow_api_t ow_sf3600_api;
#endif
#ifdef SW_BCM_56960
extern ow_api_t ow_sf4800_api;
#endif
#ifdef SW_CTC8096
extern ow_api_t ow_ctc4300_api;
#endif
#ifdef SW_BCM_56334
extern ow_api_t ow_ac1200_api;
#endif

int ow_install_api(void)
{
#ifdef SW_BCM_5684X
    ow_api = &ow_sf3600_api;
#endif

#ifdef SW_BCM_56960
    ow_api = &ow_sf4800_api;
#endif

#ifdef SW_CTC8096
    ow_api = &ow_ctc4300_api;
#endif

#ifdef SW_BCM_56334
        ow_api = &ow_ac1200_api;
#endif

    g_ow_api_en = 1;
    return 0;
}

int ow_uninstall_api(void)
{
    ow_api = NULL;
    g_ow_api_en = 0;
    
    return 0;
}

int ow_dev_open(void)
{
    int rv = OE_RST_OK;
    
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_dev_open ? ow_api->ow_dev_open() : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_dev_close(void)
{
    int rv = OE_RST_OK;
    
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_dev_close ? ow_api->ow_dev_close() : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_init(void)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_init ? ow_api->ow_init() : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_bcm_init(void)
{
   int rv = OE_RST_OK;

   OW_PTR_VALID_CHECK(ow_api);
   rv = ow_api->ow_bcm_init ? ow_api->ow_bcm_init() : OE_RST_API_NOT_SUPPORT;
   return rv;
}

int ow_sequence_init(void)
{
    int rv = OE_RST_OK;

   OW_PTR_VALID_CHECK(ow_api);
   rv = ow_api->ow_sequence_init ? ow_api->ow_sequence_init() : OE_RST_API_NOT_SUPPORT;
   return rv;
}

int ow_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
{
    int rv = OE_RST_OK;
  
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_pvlan_set ? ow_api->ow_pvlan_set(pvid, smp, pvlan_able) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec)
{
     int rv = OE_RST_OK;
    
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_vlan_delete ? ow_api->ow_vlan_delete(rule_id, vid, vlan_rec) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec)
{
     int rv = OE_RST_OK;
    
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_vlan_add ? ow_api->ow_vlan_add(vid, tag_vlan_rec, untag_vlan_rec) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_create_vlan_range(int index, int min, int max)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_create_vlan_range ? ow_api->ow_create_vlan_range(index,min,max) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_destroy_vlan_range(int index, int min, int max)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_destroy_vlan_range ? ow_api->ow_destroy_vlan_range(index,min,max) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_enable_acl_on_slot(int rule_id, 
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
                                unsigned int entry_id)
{
    int rv = OE_RST_OK;

    printf("%s line%d outid=%d\n",__func__,__LINE__,outports[0]);
    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_enable_acl_on_slot ? ow_api->ow_enable_acl_on_slot(rule_id,
                                                                       slot,
                                                                       p_class_entry,
                                                                       outer_vlan,
                                                                       range_index,
                                                                       action,
                                                                       &f_ports,
                                                                       &r_ports,
                                                                       &b_ports,
                                                                       &cross_back,
                                                                       outports,
                                                                       num_outports,
                                                                       entry_id) : OE_RST_API_NOT_SUPPORT;

    return rv;
}


int ow_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_disable_acl_on_slot ? ow_api->ow_disable_acl_on_slot(slot, p_class_entry) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index)
{
    int rv = OE_RST_OK;
	
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_trunk_add ? ow_api->ow_trunk_add(outid, smp, type, load_balance, index) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight)
{
    int rv = OE_RST_OK;
	
    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_trunk_add_weight ? ow_api->ow_trunk_add_weight(outid, smp, type, load_balance, index, weight) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_trunk_del(int outid, struct smp_t *smp, int type)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_trunk_del ? ow_api->ow_trunk_del(outid, smp, type) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_ud_set(int index, int type, int value)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_ud_set ? ow_api->ow_ud_set(index, type, value) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_crossback_pbmp_get ? ow_api->ow_crossback_pbmp_get(slot_bmp, pbmp) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_egress_fp_set ? ow_api->ow_egress_fp_set(port, old_vid, mask, new_vid, index, mode) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_egress_fp_remove(int index)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_egress_fp_remove ? ow_api->ow_egress_fp_remove(index) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_trunk_psc_set(int outid, int load_balance)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_trunk_psc_set ? ow_api->ow_trunk_psc_set(outid, load_balance) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_enable_backgroup_drop(void)
{
     int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_enable_backgroup_drop ? ow_api->ow_enable_backgroup_drop() : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_slot_load_balance_set(int all_load_balance, int slot_load_balance)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);
    rv = ow_api->ow_slot_load_balance_set ? ow_api->ow_slot_load_balance_set(all_load_balance, 
        slot_load_balance) : OE_RST_API_NOT_SUPPORT;
    return rv;
}

int ow_create_nexthop_on_slot(int rule_id, 
                                int slot,
                                int action, 
                                opt_pbmp_t f_ports, 
                                opt_pbmp_t r_ports, 
                                opt_pbmp_t b_ports,
                                opt_pbmp_t cross_back,
                                unsigned int* outports,
                                unsigned int num_outports)
{
    int rv = OE_RST_OK;

    printf("%s line%d outid=%d\n",__func__,__LINE__,outports[0]);
    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_create_nexthop_on_slot ? ow_api->ow_create_nexthop_on_slot(rule_id,
                                                                       slot,
                                                                       action,
                                                                       &f_ports,
                                                                       &r_ports,
                                                                       &b_ports,
                                                                       &cross_back,
                                                                       outports,
                                                                       num_outports) : OE_RST_API_NOT_SUPPORT;

    return rv;
}

int ow_destroy_nexthop_on_slot(int rule_id)
{
    int rv = OE_RST_OK;

    printf("%s line%d rule_id=%d\n",__func__,__LINE__,rule_id);
    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_destroy_nexthop_on_slot ? ow_api->ow_destroy_nexthop_on_slot(rule_id) : OE_RST_API_NOT_SUPPORT;

    return rv;
}

int ow_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_get_port_info ? ow_api->ow_get_port_info(module,port,port_sfp_p) : OE_RST_API_NOT_SUPPORT;

    return rv;
}

int ow_printf_interface_sfp(struct board_sfp_t *board_sfp)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_printf_interface_sfp ? ow_api->ow_printf_interface_sfp(board_sfp) : OE_RST_API_NOT_SUPPORT;

    return rv;
}

int ow_set_interface_force_up(struct smp_t *smp, int onoff)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_set_interface_force_up ? ow_api->ow_set_interface_force_up(smp, onoff) : OE_RST_API_NOT_SUPPORT;

    return rv;
}

int ow_soc_install(void)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_soc_install ? ow_api->ow_soc_install() : OE_RST_API_NOT_SUPPORT;

    return rv;
}

int ow_soc_uninstall(void)
{
    int rv = OE_RST_OK;

    OW_PTR_VALID_CHECK(ow_api);

    rv = ow_api->ow_soc_uninstall ? ow_api->ow_soc_uninstall() : OE_RST_API_NOT_SUPPORT;

    return rv;
}
