#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "appdefs.h"
#include "pub_api.h"
#include "app.h"

#ifdef SW_BCM_5684X
int ow_sf3600_dev_open(void)
{
    int rv = 0;

    rv = im_dev_open();
    return rv;
}

int ow_sf3600_dev_close(void)
{
    int rv = 0;

    rv = im_dev_close();
    return rv;
}

int ow_sf3600_init(void)
{
    int rv=0;

    return rv;
}   

int ow_sf3600_bcm_init(void)
{
    int rv=0;

    rv = ow_pub_bcm_init();
    return rv;
} 

int ow_sf3600_sequence_init(void)
{
    int rv=0;

    rv = ow_pub_sequence_init();
    return rv;
}

int ow_sf3600_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
{
    int rv = 0;

    rv = ow_pub_pvlan_set(pvid, smp, pvlan_able);
    return rv;
}

int ow_sf3600_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec)
{
    int rv = 0;
    
    rv = ow_pub_vlan_add(vid, tag_vlan_rec, untag_vlan_rec);
    return rv;
}

int ow_sf3600_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec)
{
    int rv = 0;
    
    rv = ow_pub_vlan_delete(rule_id, vid, vlan_rec);
    return rv;
}

int ow_sf3600_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index)
{
    int rv = 0;
    
    if (type == OUTPORT_MC) {
        return 0;
    }

    rv = ow_pub_trunk_add(outid, smp, load_balance, index);
    return rv;
}

int ow_sf3600_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight)
{
    int rv = 0;
    
    if (type == OUTPORT_MC) {
        return 0;
    }

    rv = ow_pub_trunk_add_weight(outid, smp, load_balance, index, weight);
    return rv;
}

int ow_sf3600_trunk_del(int outid, struct smp_t *smp, int type)
{
    int rv = 0;
    
    if (type == OUTPORT_MC) {
        return 0;
    }
    
    rv = ow_pub_trunk_del(outid, smp);
    return rv;
}

int ow_sf3600_create_vlan_range(int index, int min, int max)
{
    int rv = 0;
    
    rv = ow_pub_create_vlan_range(index, min, max);
    return rv;
}

int ow_sf3600_destroy_vlan_range(int index, int min, int max)
{
    int rv = 0;
    
    rv = ow_pub_destroy_vlan_range(index, min, max);
    return rv;
}

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
                                 unsigned int entry_id)
{
    int rv=0;

    rv=ow_pub_enable_acl_on_slot(rule_id, 
                                 slot,
                                 p_class_entry,
                                 outer_vlan,
                                 range_index,
                                 action,
                                 f_ports,
                                 r_ports,
                                 b_ports,
                                 cross_back,
                                 outports,
                                 num_outports,
                                 entry_id);
    return rv;
}

int ow_sf3600_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry)
{
    int rv = 0;

    rv = ow_pub_disable_acl_on_slot(slot, p_class_entry);
    return rv;
}

int ow_sf3600_ud_set(int index, int type, int value)
{
    int rv = 0;

    rv = ow_pub_ud_set(index, type, value);
    return rv;
}

int ow_sf3600_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp)
{
    int rv = 0;

    rv = ow_pub_crossback_pbmp_get(slot_bmp, pbmp);
    return rv;
}

int ow_sf3600_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode)
{
    int rv = 0;

    rv = ow_pub_egress_fp_set(port, old_vid, mask, new_vid, index, mode);
    return rv;
}

int ow_sf3600_egress_fp_remove(int index)
{
    int rv = 0;

    rv = ow_pub_egress_fp_remove(index);
    return rv;
}

int ow_sf3600_trunk_psc_set(int outid, int load_balance)
{
    int rv = 0;

    rv = ow_pub_trunk_psc_set(outid, load_balance);
    return rv;
}

int ow_sf3600_enable_backgroup_drop()
{
    int rv=0;
    rv = ow_pub_enable_backgroup_drop();
    return rv;
}

int ow_sf3600_slot_load_balance_set(int all_load_balance, int slot_load_balance)
{
    int rv = 0;

    rv = ow_pub_slot_load_balance_set(all_load_balance, slot_load_balance);
    return rv;
}

int ow_sf3600_create_nexthop_on_slot(int rule_id, 
                                 int slot,
                                 int action,
                                 opt_pbmp_t* f_ports,
                                 opt_pbmp_t* r_ports,
                                 opt_pbmp_t* b_ports,
                                 opt_pbmp_t* cross_back,
                                 unsigned int* outports,
                                 unsigned int num_outports)
{
    int rv = 0;
    return rv;
}

int ow_sf3600_destroy_nexthop_on_slot(int rule_id)
{
    int rv = 0;
    return rv;
}

int ow_sf3600_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p)
{
    return module_get_port_info(module, port , port_sfp_p);    
}

int ow_sf3600_printf_interface_sfp(struct board_sfp_t *board_sfp)
{
    _cli_printf_interface_sfp(board_sfp);
    return 0;    
}

int ow_sf3600_set_interface_force_up(struct smp_t *smp, int onoff)
{
    int module=0;
    bcm_port_map_t *bpm=NULL;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if ((smp->modules & (1 << module)) == 0)
            continue;
        bpm = OeAp_Get_Bcm_Portmap_By_Module(module);
        if(bpm == NULL) 
            continue;   

        __oe_set_smp_single_fiber_transmit(smp, bpm, module, onoff);
    }

    return OE_RST_OK;

}

int ow_sf3600_soc_install(void)
{
    char buf[256];

    if (access("/appfs/exp_user_def/config.soc", F_OK) == 0)
    {
        sprintf(buf, "%s", "echo -e \"0:\\nrcload /appfs/exp_user_def/config.soc\\nexit\" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1");
        system(buf);
    }

    return 0;
}

int ow_sf3600_soc_uninstall(void)
{
    char buf[256];

    if (access("/appfs/exp_user_def/config_uninstall.soc", F_OK) == 0)
    {
        sprintf(buf, "%s", "echo -e \"0:\\nrcload /appfs/exp_user_def/config_uninstall.soc\\nexit\" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1");
        system(buf);
    }


    return 0;
} 
 
#endif
