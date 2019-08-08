#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "app.h"
#include "pub_api.h"
#include "appdefs.h"

#ifdef SW_BCM_56960
int handle = 0;
int ow_sf4800_dev_open(void)
{
    int rv = 0;

    if ( (rv=rpc_create_handle(&handle)) != 0) {
        printf("RPC connect to bcm.user failed \r\n");
        rv = -1;
    }

    return rv;
}

int ow_sf4800_dev_close(void)
{
    int rv = 0;

    if (!handle) {
        rv = rpc_destroy_handle(handle);
    }

    return rv;
}

int ow_sf4800_init(void)
{
    printf("%s:%d SF4800 SOFTWARE INIT\n", __func__, __LINE__);
    
    im_process_bcm_cmd(0,"vlan remove 1 pbm=all");
    im_process_bcm_cmd(0,"port all txpause=off rxpause=off");
    im_process_bcm_cmd(0,"mod ing_vlan_tag_action_profile 0 1 SOT_OTAG_ACTION=1 DT_OTAG_ACTION=1");
    im_process_bcm_cmd(0,"mod ing_vlan_tag_action_profile 0 1 SOT_OPRI_ACTION=1 DT_OPRI_ACTION=1");
    im_process_bcm_cmd(0,"port all lrn=4");
    im_process_bcm_cmd(0,"l2 cache clear");
    im_process_bcm_cmd(0,"mod port 1 133 port_bridge=1");

    //RTAG7 SET
    im_process_bcm_cmd(0,"modreg hash_control USE_TCP_UDP_PORTS=0");
    im_process_bcm_cmd(0,"modreg hash_control NON_UC_TRUNK_HASH_USE_RTAG7=1");
    im_process_bcm_cmd(0,"modreg RTAG7_HASH_CONTROL_3 HASH_A1_FUNCTION_SELECT=0x7 HASH_A0_FUNCTION_SELECT=0x7");
    im_process_bcm_cmd(0,"setreg RTAG7_HASH_FIELD_BMAP_1 0xf00");
    im_process_bcm_cmd(0,"setreg rtag7_ipv4_tcp_udp_hash_field_bmap_1 0xf00");
    im_process_bcm_cmd(0,"setreg rtag7_ipv4_tcp_udp_hash_field_bmap_2 0xf00");
    im_process_bcm_cmd(0,"setreg rtag7_mpls_outer_hash_field_bmap 0x1e0");
    im_process_bcm_cmd(0,"setreg rtag7_ipv6_tcp_udp_hash_field_bmap_1 0xf00");
    im_process_bcm_cmd(0,"setreg rtag7_ipv6_tcp_udp_hash_field_bmap_2 0xf00");

	return 0;
}   

int ow_sf4800_bcm_init(void)
{
    int rv=0;

    rv = ow_pub_bcm_init();
    return rv;
} 

int ow_sf4800_sequence_init(void)
{
    int rv=0;

    rv = ow_pub_sequence_init();
    return rv;
}

int ow_sf4800_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
{
    int rv = 0;

    rv = ow_pub_pvlan_set(pvid, smp,pvlan_able);
    return rv;
}

int ow_sf4800_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec)
{
    int rv = 0;
    
    rv = ow_pub_vlan_add(vid, tag_vlan_rec, untag_vlan_rec);
    return rv;
}

int ow_sf4800_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec)
{
    int rv = 0;
	
	//TODO by caicai
	#if 0	
    rv = ow_pub_vlan_delete(rule_id, vid, vlan_rec);
	#endif
    return rv;
}

int ow_sf4800_create_vlan_range(int index, int min, int max)
{
    int rv = 0;
    
    rv = ow_pub_create_vlan_range(index, min, max);
    return rv;
}

int ow_sf4800_destroy_vlan_range(int index, int min, int max)
{
    int rv = 0;
    
    rv = ow_pub_destroy_vlan_range(index, min, max);
    return rv;
}

int ow_sf4800_enable_acl_on_slot(int rule_id, 
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

int ow_sf4800_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry)
{
    int rv = 0;

    rv = ow_pub_disable_acl_on_slot(slot, p_class_entry);
    return rv;
}

int ow_sf4800_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index)
{
    int rv = 0;
	#if 1
    if (type == OUTPORT_MC) {
        return 0;
    }
    
    rv = ow_pub_trunk_add(outid, smp, load_balance, index);  
	#endif
    return rv;
}

int ow_sf4800_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight)
{
    return ow_sf4800_trunk_add(outid,smp,type,load_balance,index);
}

int ow_sf4800_trunk_del(int outid, struct smp_t *smp, int type)
{
    int rv = 0;
   
	#if 1
    if (type == OUTPORT_MC) {
        return 0;
    }
    
    rv = ow_pub_trunk_del(outid, smp);
	#endif
    return rv;
}

int ow_sf4800_ud_set(int index, int type, int value)
{
    int rv = 0;

    rv = ow_pub_ud_set(index, type, value);
    return rv;
}

int ow_sf4800_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp)
{
    int rv = 0;

    rv = ow_pub_crossback_pbmp_get(slot_bmp, pbmp);
    return rv;
}

int ow_sf4800_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode)
{
    int rv = 0;

    rv = ow_pub_egress_fp_set(port, old_vid, mask, new_vid, index, mode);
    return rv;
}

int ow_sf4800_egress_fp_remove(int index)
{
    int rv = 0;

    rv = ow_pub_egress_fp_remove(index);
    return rv;
}

int ow_sf4800_trunk_psc_set(int outid, int load_balance)
{
    int rv = 0;

    rv = ow_pub_trunk_psc_set(outid, load_balance);
    return rv;
}

int ow_sf4800_enable_backgroup_drop(void)
{
    int rv=0;
    rv = ow_pub_enable_backgroup_drop();
    return rv;
}

int ow_sf4800_slot_load_balance_set(int all_load_balance, int slot_load_balance)
{
    FILE *fp = NULL;
    char file_name[64]={0};
    char buf[256] = {0};
    int value = 0;
    int mpls_value = 0;

    printf("%s line%d all_load_balance=0x%x,slot_load_balance=%d\n",__func__,__LINE__,all_load_balance, slot_load_balance);
    
    sprintf(file_name, "/usr/local/bcm/tmp.soc");
    if((fp = fopen(file_name, "w"))) {
        fprintf(fp, "0:\nmodreg hash_control USE_TCP_UDP_PORTS=%d\n", slot_load_balance);
        fprintf(fp, "modreg hash_control NON_UC_TRUNK_HASH_USE_RTAG7=1\n");
        fprintf(fp, "modreg RTAG7_HASH_CONTROL_3 HASH_A1_FUNCTION_SELECT=0x7 HASH_A0_FUNCTION_SELECT=0x7\n");
        switch(all_load_balance){
            case TRUNK_LOAD_BALANCE_SRCIP: 
                if (slot_load_balance == 0) {
                    fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_1 0xc00\n"); value = 0xc00;
                    mpls_value = 0x60;
                } else {
                    fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_1 0xc40\n"); value = 0xc40;
                    mpls_value = 0x60;
                }
                break;
            case TRUNK_LOAD_BALANCE_DSTIP: 
                if (slot_load_balance == 0) {
                    fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_1 0x300\n"); value = 0x300;
                    mpls_value = 0x180;
                } else {
                    fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_1 0x320\n"); value = 0x320;
                    mpls_value = 0x180;
                }
                break;
            case TRUNK_LOAD_BALANCE_SRCDSTIP: 
                if (slot_load_balance == 0) {
                    fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_1 0xf00\n"); value = 0xf00;
                    mpls_value = 0x1e0;
                } else {
                    fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_1 0xf60\n"); value = 0xf60;
                    mpls_value = 0x1e0;
                }
                break;
            case TRUNK_LOAD_BALANCE_SRCDSTMAC: 
                fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_3 0xfc0\n"); value = 0xfc0;
                mpls_value = 0x1e0;
                break;
            case TRUNK_LOAD_BALANCE_DSTMAC: 
                fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_3 0x1c0\n"); value = 0x1c0;
                mpls_value = 0x1e0;
                break;
            case TRUNK_LOAD_BALANCE_SRCMAC: 
                fprintf(fp, "setreg RTAG7_HASH_FIELD_BMAP_3 0xe00\n"); value = 0xe00;
                mpls_value = 0x1e0;
                break;
            default:
                break;
        }
        fprintf(fp, "setreg rtag7_ipv4_tcp_udp_hash_field_bmap_1 0x%x\n", value);
        fprintf(fp, "setreg rtag7_ipv4_tcp_udp_hash_field_bmap_2 0x%x\n", value);
        fprintf(fp, "setreg rtag7_mpls_outer_hash_field_bmap 0x%x\n", mpls_value);
        fprintf(fp, "setreg rtag7_ipv6_tcp_udp_hash_field_bmap_1 0x%x\n", value);
        fprintf(fp, "setreg rtag7_ipv6_tcp_udp_hash_field_bmap_2 0x%x\n", value);
        fclose(fp);
        
        sprintf(buf, "%s", "echo -e \"rcload /usr/local/bcm/tmp.soc\\nexit\" | /usr/local/bcm-user/bcmcli 1>/dev/null 2>&1");
        if (system(buf) != 0)
            return OE_RST_INT_ERR;
    } else {
        return OE_RST_FILE_ERR;
    }

    return OE_RST_OK; 
}

int ow_sf4800_create_nexthop_on_slot(int rule_id, 
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

int ow_sf4800_destroy_nexthop_on_slot(int rule_id)
{
    int rv = 0;
    return rv;
}

int ow_sf4800_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p)
{
    return ow_pub_get_port_info(module,port,port_sfp_p);
}

int ow_sf4800_printf_interface_sfp(struct board_sfp_t *board_sfp)
{
	int rv = 0;
   
	//TODO by caicai
	#if 0	
	rv = ow_pub_printf_interface_sfp(board_sfp);
	#endif

	return rv;
}

extern int __oe_set_smp_single_fiber_transmit_56960(struct smp_t *, bcm_port_map_t *, int, int);
int ow_sf4800_set_interface_force_up(struct smp_t *smp, int onoff)
{
    int module;
    bcm_port_map_t *bpm;

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if ((smp->modules & (1 << module)) == 0)
            continue;
        bpm = OeAp_Get_Bcm_Portmap_By_Module(module);
        if(bpm == NULL) 
            continue;   

        __oe_set_smp_single_fiber_transmit_56960(smp, bpm, module, onoff);
    }   

    return OE_RST_OK;
}

int ow_sf4800_soc_install(void)
{
    char buf[256];               

    if (access("/appfs/exp_user_def/config.soc", F_OK) == 0)
    {                            
        sprintf(buf, "%s", "echo -e \"rcload /appfs/exp_user_def/config.soc\\nexit\" | /usr/local/bcm-user/bcmcli 1>/dev/null 2>&1");
        if(system(buf)){
			fprintf(stderr,"rcload /appfs/exp_user_def/config.soc error\n");
			return -1;
		}			
    }                            

    return 0;
}

int ow_sf4800_soc_uninstall(void)
{
    char buf[256];               

    if (access("/appfs/exp_user_def/config_uninstall.soc", F_OK) == 0)
    {                            
        sprintf(buf, "%s", "echo -e \"rcload /appfs/exp_user_def/config_uninstall.soc\\nexit\" | /usr/local/bcm-user/bcmcli 1>/dev/null 2>&1");
        if(system(buf)){
			fprintf(stderr,"rcload /appfs/exp_user_def/config_uninstall.soc erroe\n");
			return -1;
		}
    }                            

    return 0;
}

#endif
