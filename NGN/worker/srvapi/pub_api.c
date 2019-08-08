#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "def.h"
#include "appdefs.h"
#include "imatrix/im-types.h"
#include "imatrix/im-port.h"
#include "imatrix/im-field.h"
#include "imatrix/im-vlan.h"
#include "imatrix/im-trunk.h"
#include "app.h"
#include "sfp_agent_api.h"
#include "sfp_spec_database.h"

unsigned char g_port_sequence[156];
unsigned char g_port_sequence_num=0;

//TODO by caicai remove acl debug switch 
#if 0
extern int ACL_DEBUG_SWITCH;
#endif
extern struct oe_outports_t g_outports[MAX_OUTPORTS_ID];

unsigned char* get_seq_group(void)
{
    return g_port_sequence;
}

int ow_pub_sequence_init(void)
{
    int i = 0;
    g_port_sequence_num=0;
    /*f*/
    bcm_port_map_t *bpm = NULL;
    bpm = OeAp_Get_Bcm_Portmap_By_Module(MODULE_FRONT);
    if(bpm)
    {
       for(i=0; i<MAX_PORT_NUMBER;i++)
       {
            if(bpm->port_map[i] >= 0)
            {
                g_port_sequence[g_port_sequence_num]=bpm->port_map[i];
                g_port_sequence_num++;
            }
       } 
    }

    /*r*/
    bpm = OeAp_Get_Bcm_Portmap_By_Module(MODULE_REAR);
    if(bpm)
    {
        for(i=0; i<MAX_PORT_NUMBER;i++)
        {
            if(bpm->port_map[i] >= 0)
            {
                g_port_sequence[g_port_sequence_num]=bpm->port_map[i];
                g_port_sequence_num++;
            }
        } 
    }
    
    /*b*/
    bpm = OeAp_Get_Bcm_Portmap_By_Module(MODULE_BACK);
    if(bpm!=NULL)
    {
        for(i=0; i<MAX_PORT_NUMBER;i++)
        {
            if(bpm->port_map[i] >= 0)
            {
                g_port_sequence[g_port_sequence_num]=bpm->port_map[i];
                g_port_sequence_num++;
            }
        } 
    }

    printf("PORTS SEQUENCE: g_port_sequence_num:%d \n", g_port_sequence_num);
    for(i=0;i<g_port_sequence_num;i++)
    {
        if(i%16==0)
            printf("\n");

        printf("%d ", g_port_sequence[i]);
   }
   printf("\n");

   return 0;
}

//#if (defined SW_BCM_5684X || defined SW_BCM_56960)
static int _get_module_ports_from_smp(struct smp_t* smp, opt_pbmp_t * pbmp)
{
    bcm_port_map_t *bcm_pm = NULL;
    int module =0;
    int port=0;  // r/r/b ports

    if( (NULL==smp)||(pbmp==NULL) ) {
        return -1;
    }

    for (module = MODULE_REAR; module <= MODULE_BACK; module++) {
        if (smp->modules & (1 << module)) {
            bcm_pm = OeAp_Get_Bcm_Portmap_By_Module(module);
            if (bcm_pm == NULL) {
                continue;
            }

            for (port = 0; port < MAX_PORT_NUMBER; port++) {
                if (smp->ports & (1ull << port)) {
                    if (bcm_pm->port_map[port] >= 0) {
                        PORT_BMP_ADD((*pbmp), bcm_pm->port_map[port]);
                    }
                }
            }
        }
    }
    
    return 0;
}

static int
_set_smp_pvlan_enable(struct smp_t* smp,int enable)
{
    bcm_port_map_t *bcm_pm = NULL;
    int module =0;
    int port=0;  // r/r/b ports
    int bcm_port=0;

    if( (NULL==smp) ) {
        return -1;
    }
    
    for (module = MODULE_REAR; module <= MODULE_BACK; module++) {
        if (smp->modules & (1 << module)) {
            bcm_pm = OeAp_Get_Bcm_Portmap_By_Module(module);
            if (bcm_pm == NULL) {
                continue;
            }

            for (port = 0; port < MAX_PORT_NUMBER; port++) {
                if (smp->ports & (1ull << port)) {
                    if (bcm_pm->port_map[port] >= 0) {
                        bcm_port = bcm_pm->port_map[port];
                        if(enable)
                            im_mod_port_table(BCM_UNIT0, bcm_port, 1, "trust_incoming_vid=0");
                        else
                            im_mod_port_table(BCM_UNIT0, bcm_port, 1, "trust_incoming_vid=1");
                    }
                }
            }
        }
    }

    return 0;

}

int ow_pub_create_vlan_range(int index, int min, int max)
{
    im_fp_range_t fp_range_t;

    memset(&fp_range_t, 0x00, sizeof(im_fp_range_t));
    if((min<=0) || (min>4095))
        return -1;
    
    if((max<=0) || (max>4095))
        return -1;

    if(min>max)
        return -1;

    fp_range_t.rid=index;
    fp_range_t.range_type=0x40; //0x40 means vlan
    fp_range_t.low=min;
    fp_range_t.up=max;

    return im_field_range_create(0, &fp_range_t);
}

int ow_pub_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
{
    int rv = 0;
    im_pbmp_t pbmp;
    opt_pbmp_t opt_pbmp;
    int slot=0;

    memset(&pbmp, 0x00, sizeof(im_pbmp_t));
    memset(&opt_pbmp, 0x00, sizeof(opt_pbmp_t));
    for (slot = 0; slot < SLOT_NUMBER_PER_SYSTEM; slot++) {
        if (smp->slots & (1 << slot)) {
            break;
        }    
    }    
    
    //only work on self-slot
    if(slot != OeAp_Get_My_Slot_Id()) //Only work on target slot
    {
        return 0;
    }

    _get_module_ports_from_smp(smp, &opt_pbmp);
    memcpy(&pbmp, &opt_pbmp, sizeof(opt_pbmp_t));
    rv = im_pvlan_set(0, pbmp, pvid);
    _set_smp_pvlan_enable(smp,pvlan_able);
    return rv;
}

static int _port_bmp_2_uport_bmp(int module, opt_pbmp_t port_bmp, opt_pbmp_t *uport_pbmp)
{
    int pi = 0;
    int uport = 0;
    bcm_port_map_t *bcm_pm= NULL;
    
    if (!uport_pbmp) {
        return -1;
    }

    bcm_pm = OeAp_Get_Bcm_Portmap_By_Module(module);
    memset(uport_pbmp, 0, sizeof(opt_pbmp_t));
    
    for(pi=0; pi<=MAX_PORT_NUMBER; pi++) {
        if (!IS_PORT_IN_BMP(port_bmp, pi)) {
            continue;
        }
        uport = bcm_pm->port_map[pi];
        //printf("%s line%d uport=%d\n",__func__,__LINE__,uport);
        if (uport < 0) {
            return -1;
        }
        
        PORT_BMP_ADD((*uport_pbmp), uport);
    }

    return 0;
}

//TODO by caicai remove acl debug switch 
#if 0
extern int ACL_DEBUG_SWITCH;
#endif
static int _module_bmp_2_uport_bmp(int module, opt_pbmp_t *port_bmp, opt_pbmp_t *uport_pbmp)
{
    int rv = 0;
    opt_pbmp_t pbmp;
    opt_pbmp_t u_pbmp;

    if (!uport_pbmp) {
        return -1;
    }
    
    memset(&pbmp, 0, sizeof(opt_pbmp_t));
    memset(uport_pbmp, 0, sizeof(opt_pbmp_t));

    if (IS_EMPTY_BMP((*port_bmp))) {
        return 0;
    }
    
    memcpy(&pbmp, port_bmp, sizeof(opt_pbmp_t));
//TODO by caicai remove acl debug switch 
#if 0
    if(ACL_DEBUG_SWITCH) {
        printf("                   pbmp:0x%x, 0x%x, 0x%x, 0x%x\n", pbmp.pbits[0],pbmp.pbits[1],pbmp.pbits[2],pbmp.pbits[3]);
    }
#endif

    memset(&u_pbmp, 0, sizeof(opt_pbmp_t));
    _port_bmp_2_uport_bmp(module, pbmp, &u_pbmp);
//TODO by caicai remove acl debug switch 
#if 0
    if(ACL_DEBUG_SWITCH) {
        printf("                   u_pbmp:0x%x, 0x%x, 0x%x, 0x%x\n", u_pbmp.pbits[0],u_pbmp.pbits[1],u_pbmp.pbits[2],u_pbmp.pbits[3]);
    }
#endif
    
    memcpy(uport_pbmp, &u_pbmp, sizeof(opt_pbmp_t));
    return rv;
}

extern oe_rule_t g_rule[DEFAULT_ACLS_NUMBER];
static int _get_rule_del_vlan_ports(int rule_id, int vid, int module, 
    opt_pbmp_t *port_bmp, opt_pbmp_t *uport_pbmp)
{
    int rv = 0;
    int ri = 0;
    int pi = 0;
    int slot = 0;
    opt_pbmp_t total_bmp;
    opt_pbmp_t del_bmp;
    
    if (!port_bmp) {
        return -1;
    }

    /* only work on self-slot */
    slot = OeAp_Get_My_Slot_Id();
    
    memset(&total_bmp, 0, sizeof(opt_pbmp_t));
    memset(&del_bmp, 0, sizeof(opt_pbmp_t));
    MERGE_BMP(del_bmp,(*port_bmp));

    printf("%s line%d module=%d,vid=%d,del_bmp.pbmp:0x%x,0x%x,0x%x,0x%x\n", __func__,__LINE__,module,
                    vid, del_bmp.pbits[0],del_bmp.pbits[1],del_bmp.pbits[2],del_bmp.pbits[3]);
    for (ri = 0; ri < DEFAULT_ACLS_NUMBER; ri++) {
        if (!g_rule[ri].used || ri == rule_id) {
            continue;
        }

        if ((g_rule[ri].outer_vlan_work == vid) ||  
            (vid >=g_rule[ri].vlan_min &&
            vid <= g_rule[ri].vlan_max) ) {
            if (module == MODULE_FRONT) {
                MERGE_BMP(total_bmp,g_rule[ri].vlan_record[slot][VLAN_TYPE_TAG].front);
                MERGE_BMP(total_bmp,g_rule[ri].vlan_record[slot][VLAN_TYPE_UNTAG].front);
            } else if (module == MODULE_BACK) {
                MERGE_BMP(total_bmp,g_rule[ri].vlan_record[slot][VLAN_TYPE_TAG].back);
                MERGE_BMP(total_bmp,g_rule[ri].vlan_record[slot][VLAN_TYPE_UNTAG].back);
            } else if (module == MODULE_REAR) {
                MERGE_BMP(total_bmp,g_rule[ri].vlan_record[slot][VLAN_TYPE_TAG].rear);
                MERGE_BMP(total_bmp,g_rule[ri].vlan_record[slot][VLAN_TYPE_UNTAG].rear);
            }
        }
    }

    for(pi=0; pi<=MAX_PORT_NUMBER; pi++) {
        if (IS_PORT_IN_BMP(del_bmp, pi) && IS_PORT_IN_BMP(total_bmp, pi)) {
            //printf("%s line%d pi=%d\n",__func__,__LINE__,pi);
            PORT_BMP_DEL(del_bmp, pi);
        }
    }

    rv = _port_bmp_2_uport_bmp(module, del_bmp, uport_pbmp); 
//TODO by caicai remove acl debug switch 
#if 0
    if(ACL_DEBUG_SWITCH)
    {
        printf("rule[%d] del-vlan[%d]:\n", rule_id, vid );
        printf("                   u-pbmp:0x%x, 0x%x, 0x%x, 0x%x\n", uport_pbmp->pbits[0], uport_pbmp->pbits[1],
            uport_pbmp->pbits[2], uport_pbmp->pbits[3]);
    }
#endif
    return rv;
}

static int _module_vlan_add(int vid, int module, opt_pbmp_t *tag_pbmp, opt_pbmp_t *untag_pbmp)
{
    int rv = 0;
    opt_pbmp_t uport_pbmp;
    opt_pbmp_t uport_upbmp;
    im_pbmp_t im_pbmp;
    im_pbmp_t im_ubmp;
    
    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    if ( (rv = _module_bmp_2_uport_bmp(module, tag_pbmp, &uport_pbmp)) !=0) {
        return rv;
    }

    memset(&uport_upbmp, 0, sizeof(opt_pbmp_t));
    if ( (rv = _module_bmp_2_uport_bmp(module, untag_pbmp, &uport_upbmp)) !=0) {
        return rv;
    }

    /* nothing to be done for empty pbmp */
    if (IS_EMPTY_BMP(uport_pbmp) && 
        IS_EMPTY_BMP(uport_upbmp) ) {
        return 0;
    }

    im_vlan_create(0, vid);
    memset(&im_pbmp, 0, sizeof(im_pbmp_t));
    memcpy(&im_pbmp, &uport_pbmp, sizeof(opt_pbmp_t));
    memset(&im_ubmp, 0, sizeof(im_pbmp_t));
    memcpy(&im_ubmp, &uport_upbmp, sizeof(opt_pbmp_t));

    MERGE_BMP(im_pbmp,im_ubmp);
    /*im_pbmp:TAG + UNTAG, im_upbmp:UNTAG*/
    rv = im_vlan_port_add(0, vid, im_pbmp, im_ubmp);
#if 0
    printf("%s line%d module=%d, rv=%d, im_pbmp:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__, module, rv,
                        im_pbmp.pbits[0], im_pbmp.pbits[1], im_pbmp.pbits[2], im_pbmp.pbits[3]);
    printf("%s line%d  im_ubmp:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__, 
                        im_ubmp.pbits[0], im_ubmp.pbits[1], im_ubmp.pbits[2], im_ubmp.pbits[3]);
#endif
    return rv;
}

int ow_pub_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec)
{
    int rv = 0;

    _module_vlan_add(vid, MODULE_FRONT, &tag_vlan_rec->front, &untag_vlan_rec->front);
    _module_vlan_add(vid, MODULE_BACK, &tag_vlan_rec->back, &untag_vlan_rec->back);
    _module_vlan_add(vid, MODULE_REAR, &tag_vlan_rec->rear, &untag_vlan_rec->rear);
    
    return rv;
}

//remove for the moment  by caicai
#if 0
static int _module_vlan_del(int rule_id, int vid, int module, opt_pbmp_t *port_bmp)
{
    int rv = 0;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t im_bmp;
    int slot = 0;
    
    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));

    if ((rv=_get_rule_del_vlan_ports(rule_id, vid, module,port_bmp, &uport_pbmp)) != 0) {
        return rv;
    }

    /* nothing to be done for empty pbmp */
    if (IS_EMPTY_BMP(uport_pbmp)) {
        goto end;
    }

    memset(&im_bmp, 0, sizeof(im_pbmp_t));
    memcpy(&im_bmp, &uport_pbmp, sizeof(opt_pbmp_t));
    if ((rv = im_vlan_port_remove(0, vid, im_bmp)) != 0) {
        return rv;
    }

    slot = OeAp_Get_My_Slot_Id();

end:
    if (module == MODULE_FRONT) {
        CLEAR_BMP(g_rule[rule_id].vlan_record[slot][VLAN_TYPE_TAG].front);
        CLEAR_BMP(g_rule[rule_id].vlan_record[slot][VLAN_TYPE_UNTAG].front);
    } else if (module == MODULE_BACK) {
        CLEAR_BMP(g_rule[rule_id].vlan_record[slot][VLAN_TYPE_TAG].back);
        CLEAR_BMP(g_rule[rule_id].vlan_record[slot][VLAN_TYPE_UNTAG].back);
    } else {
        CLEAR_BMP(g_rule[rule_id].vlan_record[slot][VLAN_TYPE_TAG].rear);
        CLEAR_BMP(g_rule[rule_id].vlan_record[slot][VLAN_TYPE_UNTAG].rear);
    }
    return rv;
}

static int _destroy_vlan(int vid)
{
    int rv = 0;
    int ri = 0;
    int slot = 0;
    opt_pbmp_t total_bmp[3];

    /* only work on self-slot */
    slot = OeAp_Get_My_Slot_Id();

    memset(total_bmp, 0, sizeof(total_bmp));
    for (ri = 0; ri < DEFAULT_ACLS_NUMBER; ri++) {
        if (!g_rule[ri].used) {
            continue;
        }

        if ((g_rule[ri].outer_vlan_work == vid) ||  
            (vid >=g_rule[ri].vlan_min &&
            vid <= g_rule[ri].vlan_max) ) {
            
            MERGE_BMP(total_bmp[0],g_rule[ri].vlan_record[slot][VLAN_TYPE_TAG].front);
            MERGE_BMP(total_bmp[0],g_rule[ri].vlan_record[slot][VLAN_TYPE_UNTAG].front);
            printf("%s line%d vid=%d front:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__,vid,total_bmp[0].pbits[0],
                total_bmp[0].pbits[1],total_bmp[0].pbits[2],total_bmp[0].pbits[3]);
            
            MERGE_BMP(total_bmp[1],g_rule[ri].vlan_record[slot][VLAN_TYPE_TAG].back);
            MERGE_BMP(total_bmp[1],g_rule[ri].vlan_record[slot][VLAN_TYPE_UNTAG].back);

            MERGE_BMP(total_bmp[2],g_rule[ri].vlan_record[slot][VLAN_TYPE_TAG].rear);
            MERGE_BMP(total_bmp[2],g_rule[ri].vlan_record[slot][VLAN_TYPE_UNTAG].rear);
        }
    }
    
    if (IS_EMPTY_BMP(total_bmp[0]) &&
        IS_EMPTY_BMP(total_bmp[1]) &&
        IS_EMPTY_BMP(total_bmp[2])) {
        printf("\r\n %s line%d vid=%d, destroy \n\r",__func__,__LINE__,vid);
        //rv = im_vlan_destroy(vid);
    }
    return rv;
}

int ow_pub_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec)
{
    int rv = 0;

    _module_vlan_del(rule_id, vid, MODULE_FRONT, &vlan_rec->front);
    _module_vlan_del(rule_id, vid, MODULE_BACK, &vlan_rec->back);
    _module_vlan_del(rule_id, vid, MODULE_REAR, &vlan_rec->rear);
    _destroy_vlan(vid);
    return rv;
}
#endif

#if (defined SW_BCM_56960 || defined SW_CTC8096)
int ow_pub_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p)
{
    bcm_port_map_t *bpm =   OeAp_Get_Bcm_Portmap_By_Module(module); 
    int port_seq=0; //for SF4800 fr 100G mode, f/1,5,9...
    int i=0;
    int rv=0;
    char module_str[MODULE_NUMBER_PER_BOARD] = {'r', 'f', 'b'};
    int slot = OeAp_Get_My_Slot_Id();
	int index = 0;

    #ifdef _PLATFORM_X86_UBUNTU_
  	 return 0;
    #endif
		

    if(NULL==port_sfp_p)
        return -1;

    if(MODULE_BACK==module)
        return 0;

    if(NULL==bpm || bpm->port_map[port]<0)
        return 0;

    port_sfp_p->smp_common.port_id = port;
    port_sfp_p->smp_common.port_valid_flag = 1;
    sprintf(port_sfp_p->port_name, "%d/%c/%d", slot + 1, module_str[module - MODULE_REAR], port + 1);

    //Get info from qsfp_server_agent
    {
        int g_module_nums=0;
        sfp_card_info_t card_info;
        sfp_card_id_t card_id;
        sfp_agent_t *p_agent = NULL;
        int current_ports=0;
        int last_ports=0;

        p_agent = get_sfp_agent();
        rv=sfp_agent_init(p_agent);
        if(rv!=0)
        {
            printf("Error in sfp_agent_init \n");
            return 0;
        }

        memset(&card_id, 0x00, sizeof(sfp_card_id_t));
        memset(&card_info, 0x00, sizeof(sfp_card_info_t));

        if(MODULE_FRONT==module)
        {
            card_id.card_type=SFP_CARD_TYPE_FM;
            rv=sfp_agent_get_fm_card_num(p_agent, &g_module_nums);
        }
        else
        {
            card_id.card_type=SFP_CARD_TYPE_RM;
            rv=sfp_agent_get_rm_card_num(p_agent, &g_module_nums);
        }
        
        if(rv!=0)
        {
            sfp_agent_exit(p_agent);
            return 0;
        }

        //Get sequence of port; f/1->1  f/5->2  f/9->3
        for(i=0; i<=port; i++)
        {
            if(bpm->port_map[i]>=0)
                port_seq++;
        }

        for(i=0; i<g_module_nums; i++)
        {
            card_id.id=i;
            rv=sfp_agent_get_card_info(p_agent,&card_id,&card_info);
            if(rv==0)
            {
                int ret=0;
                last_ports = current_ports; 
                current_ports += card_info.sfp_num; 
                if(current_ports>=port_seq)
                {
                    index = port_seq-last_ports-1;
                    port_sfp_p->port_type=card_info.sff_type[index];
                    sfp_xyz_t xyz;
                    sfp_present_t present;
                    sfp_vendor_name_t vendor_name;
                    sfp_module_type_t module_type;
                    sfp_temperature_t temperature;
                    sfp_power_t       power;
                    sfp_length_t      length;
                    sfp_center_wavelength_t wavelength;
                    sfp_vendor_pn_t         vendor_pn;
                    const ewx_sfp_spec_t    *p_sfp_spec  = NULL;

                    memset(&xyz, 0x00, sizeof(sfp_xyz_t));   
                    printf("Current module-index:%d   port-index:%d \n", i, index);

                    xyz.card_id.card_type = card_id.card_type;
                    xyz.card_id.id = i;
                    xyz.sfp_id = index;

                    ret = sfp_agent_get_present(p_agent,&xyz,&present);
                    if(ret!=0)
                    {
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    ret = sfp_agent_get_vendor_name(p_agent,
                            &xyz,
                            &present,
                            &vendor_name);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_vendor_name", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    ret = sfp_agent_get_module_type(p_agent,
                            &xyz,
                            &present,
                            &module_type);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_module_type", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    ret = sfp_agent_get_temperature(p_agent,
                            &xyz,
                            &present,
                            &temperature);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_temperature", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    } 

                    ret = sfp_agent_get_power(p_agent,
                            &xyz,
                            &present,
                            &power);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_power", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    ret = sfp_agent_get_length(p_agent,
                            &xyz,
                            &present,
                            &length);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_length", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    ret = sfp_agent_get_center_wavelength(p_agent,
                            &xyz,
                            &present,
                            &wavelength);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_wavelength", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    ret = sfp_agent_get_vendor_pn(p_agent,
                            &xyz,
                            &present,
                            &vendor_pn);
                    if (ret) {
                        printf("error: %s ret %d\n", "sfp_agent_get_vendor_pn", ret);
                        sfp_agent_exit(p_agent);
                        return -1;
                    }

                    p_sfp_spec = ewx_find_sfp_spec_by_name_pn(
                            vendor_name.string,
                            vendor_pn.string);

                    /*convert data from driver*/
                    port_sfp_p->present = present.v;
                    if(!port_sfp_p->present)
                    {
                        sfp_agent_exit(p_agent);
                        return 0; 
                    }

                    /*vendor name*/
                    strcpy(port_sfp_p->vendor_name,vendor_name.string);

                    /*module type*/
                    port_sfp_p->module_type = module_type;

                    /*temperature*/
                    port_sfp_p->temperature = temperature.value;

                    /*power*/
                    port_sfp_p->rx_dbm   = 10 * log10((double)power.rx[0]/10000.0);
                    port_sfp_p->rx_dbm_1 = 10 * log10((double)power.rx[1]/10000.0);
                    port_sfp_p->rx_dbm_2 = 10 * log10((double)power.rx[2]/10000.0);
                    port_sfp_p->rx_dbm_3 = 10 * log10((double)power.rx[3]/10000.0);

                    port_sfp_p->tx_dbm   = 10 * log10((double)power.tx[0]/10000.0);
                    port_sfp_p->tx_dbm_1 = 10 * log10((double)power.tx[1]/10000.0);
                    port_sfp_p->tx_dbm_2 = 10 * log10((double)power.tx[2]/10000.0);
                    port_sfp_p->tx_dbm_3 = 10 * log10((double)power.tx[3]/10000.0);

                    /*wavelength*/
                    port_sfp_p->wavelength = wavelength.wavelength;

                    /*vendor pn*/
                    strcpy(port_sfp_p->vendor_pn, vendor_pn.string); 

                    /*window*/
                    port_sfp_p->rx_min = p_sfp_spec->rx_power_min;
                    port_sfp_p->rx_max = p_sfp_spec->rx_power_max;
                    port_sfp_p->tx_min = p_sfp_spec->tx_power_min;
                    port_sfp_p->tx_max = p_sfp_spec->tx_power_max;
        
                    sfp_agent_exit(p_agent);
                    return 0;
                }
            }
        } 
        sfp_agent_exit(p_agent);
    }
    return 0;
} 

/*remove by caicai*/
#if 0
int ow_pub_printf_interface_sfp(struct board_sfp_t *board_sfp)
{
    int rv=0;
    struct sm_common_t *m_common=NULL;
    struct port_sfp_t *port_sfp=NULL;
    char *module_to_str[] = {NULL, NULL, NULL, NULL, "r", "f", "b"};
    struct smp_common_t *smp_common = NULL;
    int i=0;

    if(NULL==board_sfp)
        return 0;

    m_common = &(board_sfp->sm_common);
    __printf_buf("\nSlot %d/Module %s:\n", m_common->slot_id + 1, module_to_str[m_common->module_id]);
    for (i = 0; i < board_sfp->n_port_sfp; i++) {
        port_sfp = &(board_sfp->port_sfp[i]);
        smp_common = &(port_sfp->smp_common);
        if (smp_common->port_valid_flag == 0) {
            continue;
        }
        /*port description*/
        __printf_buf("\nInterface %d/%s/%d: \n", m_common->slot_id + 1, module_to_str[m_common->module_id], smp_common->port_id +1);
        __printf_buf("----------------------------------------\n");
        __printf_buf("%-24s%s\n", "Port Name:", port_sfp->port_name);
        if(!port_sfp->present)
        {
            __printf_buf("\n");
            __printf_buf("----------------------------------------\n");
            continue;
        }   

        /*port properties*/
        __printf_buf("%-24s%s\n", "Vendor Name:", port_sfp->vendor_name);
        __printf_buf("%-24s%s\n", "Vendor PN:", port_sfp->vendor_pn);
        __printf_buf("%-24s%d nm\n", "WaveLength:", port_sfp->wavelength);
        __printf_buf("%-24s%d C\n", "Temperature:", port_sfp->temperature);

        __printf_buf("%-24s\n", "Rx Power:(dbm)");
        __printf_buf("    %-8s%lf\n", "Lane0:", port_sfp->rx_dbm);
        __printf_buf("    %-8s%lf\n", "Lane1:", port_sfp->rx_dbm_1);
        __printf_buf("    %-8s%lf\n", "Lane2:", port_sfp->rx_dbm_2);
        __printf_buf("    %-8s%lf\n", "Lane3:", port_sfp->rx_dbm_3);

        __printf_buf("%-24s\n", "Tx Power:(dbm)");
        __printf_buf("    %-8s%lf\n", "Lane0:", port_sfp->tx_dbm);
        __printf_buf("    %-8s%lf\n", "Lane1:", port_sfp->tx_dbm_1);
        __printf_buf("    %-8s%lf\n", "Lane2:", port_sfp->tx_dbm_2);
        __printf_buf("    %-8s%lf\n", "Lane3:", port_sfp->tx_dbm_3);
       
        __printf_buf("%-24s\n", "Power Window:(dbm)");
        __printf_buf("    %-8s%lf ~ %-41f\n", "Rx Power: ", port_sfp->rx_min, port_sfp->rx_max);
        __printf_buf("    %-8s%lf ~ %-41f\n", "Tx Power: ", port_sfp->tx_min, port_sfp->tx_max);

        __printf_buf("\n");
        __printf_buf("----------------------------------------\n");
    }
     __printf_buf("\n");

    return 0;
}
#endif

#endif


#if (defined SW_BCM_5684X || defined SW_BCM_56960)
static int 
_convert_class_to_entry(im_fp_entry_t* entry, struct class_entry_t* class)
{
    int i=0;
    entry->key_mask=class->key_mask;
    entry->user_data_mask=class->ud_mask;

    //L2 
    memcpy(entry->smac, class->smac, 6);
    memcpy(entry->smac_mask, class->smac_mask, 6);
    memcpy(entry->dmac, class->dmac, 6);
    memcpy(entry->dmac_mask, class->dmac_mask, 6);
    entry->inner_vlan=class->inner_vlan;
    entry->inner_vlan_mask=class->inner_vlan_mask;
    entry->ethertype=class->ethertype;

    //L3
    entry->sip=class->sip;
    entry->sip_mask=class->sip_mask;
    entry->dip=class->dip;
    entry->dip_mask=class->dip_mask;
    entry->protocol=class->protocol;
   
    memcpy(entry->udf,class->sipv6,16);
    memcpy(entry->udf_mask,class->sipv6_mask,16);
    memcpy(entry->udf+16,class->dipv6,16);
    memcpy(entry->udf_mask+16,class->dipv6_mask,16);

    //L4
    entry->tcp_flag=class->tcp_flag;
    entry->min_sport=class->sport;
    entry->max_sport=class->sport;
    entry->sport_mask=class->sport_mask;
    entry->max_dport=class->dport;
    entry->min_dport=class->dport;
    entry->dport_mask=class->dport_mask;

    for (i = 0; i < UD_NUM; i++) {
        if (class->ud_mask & (1 << i)) {
            memcpy(entry->udf + sizeof(class->uds[i].data)*i, &class->uds[i].data, sizeof(class->uds[i].data));
            memcpy(entry->udf_mask + sizeof(class->uds[i].mask)*i, &class->uds[i].mask, sizeof(class->uds[i].mask));
        }    
    }    

	return 0;
}

static int 
_get_b_port_from_slot(int my_slot, int to_slot)
{
    if(my_slot > to_slot)
    {
        return to_slot*4;
    }
    else
    {
        return (to_slot-1)*4;
    }
}

static int
_convert_crossback_to_pbmp(im_pbmp_t* im_pbmp, opt_pbmp_t* opt_pbmp)
{
    bcm_port_map_t *bpm = NULL;
    int bcm_port=0;
    int slot=0;
    int my_slot=OeAp_Get_My_Slot_Id();
    int b_port=0;
    im_pbmp_t tmp;
    opt_pbmp_t opt_tmp;

    memset(&opt_tmp, 0x00, sizeof(opt_pbmp_t));
    memset(&tmp,0x00,sizeof(im_pbmp_t));
    memcpy(&tmp, im_pbmp, sizeof(im_pbmp_t));
    memcpy(&opt_tmp, opt_pbmp, sizeof(opt_pbmp_t));

    bpm=OeAp_Get_Bcm_Portmap_By_Module(MODULE_BACK);
    ITERATOR_BMP_START(opt_tmp, slot)
    {    
        b_port=_get_b_port_from_slot(my_slot, slot);   
        bcm_port=bpm->port_map[b_port];
        //insert bcm_port to im_pbmp
        PORT_BMP_ADD(tmp, bcm_port);
    }    
    ITERATOR_BMP_END;

    memcpy(im_pbmp, &tmp, sizeof(im_pbmp_t));

    return 0;
}

static int
_convert_smp_to_pbmp(im_pbmp_t* im_pbmp, opt_pbmp_t* opt_pbmp, int module)
{
    bcm_port_map_t *bpm = NULL;
    int port=0;
    int bcm_port=0;
    im_pbmp_t tmp;
    opt_pbmp_t opt_tmp;

    memset(&opt_tmp, 0x00, sizeof(opt_pbmp_t));
    memset(&tmp,0x00,sizeof(im_pbmp_t));
    memcpy(&tmp,im_pbmp,sizeof(im_pbmp_t));
    memcpy(&opt_tmp, opt_pbmp, sizeof(opt_pbmp_t));

    if((im_pbmp==0) || (opt_pbmp==0))
        return -1;
    
    bpm=OeAp_Get_Bcm_Portmap_By_Module(module);
    ITERATOR_BMP_START(opt_tmp, port)
    {    
        bcm_port=bpm->port_map[port];
        //insert bcm_port to im_pbmp
        printf("port=%d u_port=%d \n", port, bcm_port);
        PORT_BMP_ADD(tmp, bcm_port);
    }    
    ITERATOR_BMP_END;

    memcpy(im_pbmp, &tmp, sizeof(im_pbmp_t));
    return 0;
}

static int
print_debug_entry(im_fp_entry_t* fp)
{
    if(NULL==fp)
        return 0;

    printf("KEY MASK :0x%02x \n", fp->key_mask);

    if(fp->key_mask&KEY_MASK_OUTERVLAN)
    {
        printf("[ENTRY-DEBUG] outervlan :%d \n", fp->outer_vlan);
    }
    if(fp->key_mask&KEY_MASK_INNERVLAN)
    {
        printf("[ENTRY-DEBUG] innervlan :%d \n", fp->inner_vlan);
    }
    if(fp->key_mask&KEY_MASK_SIP)
    {
        printf("[ENTRY-DEBUG] sip :0x%x 0x%x \n", fp->sip, fp->sip_mask);
    }
    if(fp->key_mask&KEY_MASK_DIP)
    {
        printf("[ENTRY-DEBUG] dip :0x%x 0x%x \n", fp->dip, fp->dip_mask);
    }
    if(fp->key_mask&KEY_MASK_ETHERTYPE)
    {
        printf("[ENTRY-DEBUG] ethertype :0x%x \n", fp->ethertype);
    }

    return 0;
}

#define HASH_CONDITION_1 1
#define HASH_CONDITION_2 2
#define HASH_CONDITION_3 3
#define HASH_CONDITION_4 4

//if HASH :
// 1. if only one trunk , then redirect trunk with trunk number  HASH_CONDITION_1
// 2. if more then one trunk, then egressmask                    HASH_CONDITION_2
// 3. if cross_back and trunk both exist, then egressmask        HASH_CONDITION_3
// 4. if only cross_back , then redirect pbmp                    HASH_CONDITION_4
static int
_get_hash_condition(int num_outports, opt_pbmp_t* cross_back)
{
    int cross_back_flag=0;

    if(NULL==cross_back)
        return -1;

    if(cross_back->pbits[0]!=0)
    {
        cross_back_flag=1;
    } 

    if((cross_back_flag==0)&&(num_outports==1))
        return HASH_CONDITION_1;

    if((cross_back_flag==0)&&(num_outports>1))
        return HASH_CONDITION_2;

    if((cross_back_flag==1)&&(num_outports>=1))
        return HASH_CONDITION_3;

    if((cross_back_flag==1)&&(num_outports==0))
        return HASH_CONDITION_4;

	return 0;
}

static int _get_egress_mask_uport_pbmp(im_pbmp_t *mask_pbmp, 
                            int module, 
                            opt_pbmp_t* ports,
                            int cross_flag
                            )
{
    int i = 0;
    im_pbmp_t tmp;
    
    if (!ports || !mask_pbmp) {
        return -1;
    }

//TODO by caicai remove acl debug switch 
#if 0
    if(ACL_DEBUG_SWITCH)
    {
        printf("                   mask_pbmp:0x%x, 0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x,0x%x\n", mask_pbmp->pbits[0], mask_pbmp->pbits[1],
            mask_pbmp->pbits[2], mask_pbmp->pbits[3],mask_pbmp->pbits[4],mask_pbmp->pbits[5],
            mask_pbmp->pbits[6],mask_pbmp->pbits[7]);
    }
#endif
    
    memset(&tmp, 0, sizeof(im_pbmp_t));

    if (cross_flag == 0) {
        _convert_smp_to_pbmp(&tmp, ports, module);
    } else {
        _convert_crossback_to_pbmp(&tmp, ports);
    }
    //for (i = 0; i < MAX_PORT_NUMBER; i++) {
    for (i = 0; i < 256; i++) {
        if (IS_PORT_IN_BMP(tmp, i)) {
            PORT_BMP_DEL((*mask_pbmp), i);
        }
    }
//TODO by caicai remove acl debug switch 
#if 0
    if(ACL_DEBUG_SWITCH)
    {
        printf("                   mask_pbmp:0x%x, 0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x,0x%x\n", mask_pbmp->pbits[0], mask_pbmp->pbits[1],
            mask_pbmp->pbits[2], mask_pbmp->pbits[3],mask_pbmp->pbits[4],mask_pbmp->pbits[5],
            mask_pbmp->pbits[6],mask_pbmp->pbits[7]);
    }
#endif
    return 0;
}

static int _set_fp_slice_map(void)
{
    FILE *fp = NULL;
    char file_name[64]={0};
    char buf[256] = {0};
    int i;

    if (OeAp_Get_My_Board_Type() != BOARD_TYPE_AC1200) {
        sprintf(file_name, "/usr/local/bcm/tmp.soc");
        if((fp = fopen(file_name, "w"))) {
            for (i = 0; i < 10; i++) {
                fprintf(fp, "0:\nmod fp_slice_map 0 1 VIRTUAL_SLICE_%d_VIRTUAL_SLICE_GROUP_ENTRY_0=0\n", i);
            }
            fclose(fp);
            sprintf(buf, "%s", "echo -e \"rcload /usr/local/bcm/tmp.soc\\nexit\" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1");
            if(system("killall -9 bcm.user.proxy 1>/dev/null 2>&1")){
			}
            if (system(buf) != 0)
                return OE_RST_INT_ERR;
        }
    }
    return OE_RST_OK;
}

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
                              unsigned int entry_id)
{
    int rv=0;
    im_fp_entry_t fp_entry;
    im_fp_action_t fp_action;
    im_pbmp_t im_pbmp;
    int i=0;

    if(NULL==p_class_entry)
        return -1;

    memset(&fp_entry, 0x00, sizeof(im_fp_entry_t));
    memset(&fp_action, 0x00, sizeof(im_fp_action_t));

    if(slot != OeAp_Get_My_Slot_Id()) //Only work on target slot
    {
        return 0;
    }

    //fp entry assginment
    _convert_class_to_entry(&fp_entry,p_class_entry); 

    if(range_index>=0)
    {
        printf("range index :%d \n", range_index);
        fp_entry.rid_flag[range_index]=1;
    }else if(outer_vlan)
    {
        fp_entry.key_mask |= KEY_MASK_OUTERVLAN;
        fp_entry.outer_vlan=outer_vlan;
        fp_entry.outer_vlan_mask=0xfff;
    }
	
    fp_entry.eid=entry_id;

    {
//TODO by caicai remove acl debug switch 
#if 0
        if(ACL_DEBUG_SWITCH)
        {
            print_debug_entry(&fp_entry); 
        }        
#endif
    }

    if (im_field_entry_add(0, &fp_entry) != 0)
    {
        return OE_RST_SET_CAM_ERR;
    }

    if (im_field_entry_prio_set(0, entry_id , 2049-rule_id) != 0)
    {    
        return OE_RST_SET_CAM_ERR;
    }
    
    //action assignment
    fp_action.eid = entry_id;
    if(action==FW_MODE_DROP) // if drop, to BCM 
    {
        memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
        for(i=0;i<8;i++)
        {
            im_pbmp.pbits[i]=0xffffffff;
        }
        fp_action.action=imFieldActionRedirectPort;
        fp_action.param0=30;
        fp_action.param1=28;
        memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
    }
    if(action==FW_MODE_MC) //if mcast, for BCM, redirect all relevant ports
    { 
        memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
        _convert_smp_to_pbmp(&im_pbmp, f_ports, MODULE_FRONT);
        _convert_smp_to_pbmp(&im_pbmp, r_ports, MODULE_REAR);
        _convert_smp_to_pbmp(&im_pbmp, b_ports, MODULE_BACK);
        _convert_crossback_to_pbmp(&im_pbmp, cross_back);
        fp_action.action=imFieldActionRedirectPbmp;
        memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
    }
    
    //if HASH :
    // 1. if only one trunk , then redirect trunk with trunk number  HASH_CONDITION_1
    // 2. if more then one trunk, then egressmask                    HASH_CONDITION_2
    // 3. if cross_back and trunk both exist, then egressmask        HASH_CONDITION_3
    // 4. if only cross_back , then redirect pbmp                    HASH_CONDITION_4
    if(action==FW_MODE_HASH)    
    {
        int hash_condition=_get_hash_condition(num_outports,cross_back);
        int i = 0;
//TODO by caicai remove acl debug switch 
#if 0
        if(ACL_DEBUG_SWITCH)
        {
            printf("[HASH-ACTION] hash condition:%d,outid=%d,g_outports=%p\n", hash_condition,outports[0],g_outports);
            printf("[HASH-ACTION] hash condition:%d, index=%d\n", hash_condition,g_outports[outports[0]].index);
        } 
#endif

        switch(hash_condition)
        {
            case HASH_CONDITION_1:
                fp_action.action=imFieldActionRedirectTrunk;
				/*TODO by caicai remove at the moment*/
				#if 0
                fp_action.param0=g_outports[outports[0]].index; 
				#endif
                break;
            case HASH_CONDITION_2:
            case HASH_CONDITION_3:
                fp_action.action = imFieldActionEgressMask;
                memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
                for (i =0; i< 8; i++) {
                    im_pbmp.pbits[i] = 0xffffffff;
                }
                _get_egress_mask_uport_pbmp(&im_pbmp, MODULE_FRONT, f_ports,0);
                _get_egress_mask_uport_pbmp(&im_pbmp, MODULE_REAR, r_ports,0);
                _get_egress_mask_uport_pbmp(&im_pbmp, MODULE_BACK, b_ports,0);
                _get_egress_mask_uport_pbmp(&im_pbmp, -1, cross_back,1);
                memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
                break;
            case HASH_CONDITION_4:
                fp_action.action = imFieldActionRedirectPbmp;
                memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
                _convert_crossback_to_pbmp(&im_pbmp, cross_back);
                memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
                break;
            default:
                break;
        }
    }

    if (im_field_action_add(0, &fp_action) != 0)
    {
        return OE_RST_SET_CAM_ERR;
    }

    {
        //assign counters for each entry
        im_fp_stat_t stat;
        int stat_id=0;
        memset(&stat, 0x00, sizeof(im_fp_stat_t));
        stat.group = fp_entry.gid;          
        stat.stat_arr_sz = 2;         
        stat.stat_arr[0] = imFieldStatBytes;
        stat.stat_arr[1] = imFieldStatPackets;
        rv=im_field_stat_create(0, &stat, &stat_id);
        rv=im_field_stat_attach(0, stat_id,entry_id);
        p_class_entry->stat_id=stat_id;  //pass this id back to g_rule list
    }
    
    if (im_field_entry_install(0, entry_id) != 0)
    {
        return OE_RST_SET_CAM_ERR;
    }

    return 0;
}

void  PORT_BMP_AD_2(opt_pbmp_t *pbmp ,int port) 
{
    pbmp->pbits[port /32] |= (1<< (port % 32)); 
}

int _get_slot_module_ports_from_smp(struct smp_t* smp, int *slot, opt_pbmp_t * pbmp)
{
    bcm_port_map_t *bcm_pm = NULL;
    int s=0;
    int m=0;
    int port=0;  // r/r/b ports
    int uport=0;       // CTC Pannel ports

    if((NULL==smp)||(pbmp==NULL)||(NULL==slot))
        return -1;

    for(s=0;s<SLOT_NUMBER_PER_SYSTEM;s++)
    {
        if(smp->slots & (1 << s))
            break; 
    }
    *slot=s;

    for(m=0;m<MODULE_BACK;m++)
    {
        if(smp->modules & (1 << m))
            break;
    }

    bcm_pm=OeAp_Get_Bcm_Portmap_By_Sm(s,m);

    if(bcm_pm == NULL)
    {	
        return -1;
    }

    for(port=0; port<=MAX_PORT_NUMBER; port++)
    {
        if(smp->ports & 1ull<<(port))
        {
            uport=bcm_pm->port_map[port];
            if(uport>0)
            {
                PORT_BMP_AD_2(pbmp,uport);   
            }
        }
    } 

    return 0;
}


/* function: check whether given bcmport is in the given trunk
 * return 1:yes ; 0:no
 */
static int _check_bcmport_in_trunk(int unit ,int trunk_id, opt_pbmp_t *bcm_ports)
{
    im_trunk_add_info_t trunk_info;
    int port_num=0;
    int pi=0;
    int ret=0;

    if((trunk_id<0) || IS_EMPTY_BMP((*bcm_ports)))
        return 0;

    memset(&trunk_info, 0x00, sizeof(im_trunk_add_info_t));
    ret=im_trunk_get(unit, trunk_id, &trunk_info);
    if(ret!=0)
        return 0;

    port_num=trunk_info.num_ports;
    for(pi=0;pi<port_num;pi++) {
        if (IS_PORT_IN_BMP((*bcm_ports), trunk_info.tp[pi])) {
	     return -1;
        }
    }

    return 0;
}

int ow_pub_trunk_add(int outid, struct smp_t *smp, int load_balance, int *index)
{
    int rv = 0;
    int trunk_id = 0;
    int slot = 0;
    int modid = 0;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t bmp;
    struct smp_t add_smp;

    memset(&add_smp, 0, sizeof(struct smp_t));    
    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_slot_module_ports_from_smp(smp, &slot, &uport_pbmp);
    //printf("%s line%d uport_pbmp:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__,
    //    uport_pbmp.pbits[0],uport_pbmp.pbits[1],uport_pbmp.pbits[2],uport_pbmp.pbits[3]);
    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }

    rv = _check_bcmport_in_trunk(0, outid, &uport_pbmp);
    if(rv == -1)
    {
	return OE_RST_INTERfACE_CONFLICT;
    }

    _check_bcmport_in_trunk(0, outid, &uport_pbmp);
    printf("%s line%d uport_pbmp:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__,
        uport_pbmp.pbits[0],uport_pbmp.pbits[1],uport_pbmp.pbits[2],uport_pbmp.pbits[3]);
    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }
    
    memset(&bmp, 0, sizeof(im_pbmp_t));
    memcpy(&bmp, &uport_pbmp, sizeof(opt_pbmp_t));

    trunk_id = outid;
    *index = trunk_id;
    printf("[INLINE-DEBUG---------] index=%d outid=%d \n", *index, outid);
    //modid = slot * 2;
    modid = 0;
 
    rv = im_trunk_add(0, trunk_id, load_balance, bmp, modid, 
        g_port_sequence_num, g_port_sequence);
    return rv;
}

int ow_pub_trunk_del(int outid, struct smp_t *smp)
{
    int rv = 0;
    int trunk_id = 0;
    int slot = 0;
    int modid = 0;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t bmp;

    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_slot_module_ports_from_smp(smp, &slot, &uport_pbmp);

    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }
    
    memset(&bmp, 0, sizeof(im_pbmp_t));
    memcpy(&bmp, &uport_pbmp, sizeof(opt_pbmp_t));

    trunk_id = outid;
    modid = 0;
    rv = im_trunk_remove(0, trunk_id, bmp, modid);
    
    return rv;
}

int ow_pub_ud_set(int index, int type, int value)
{
    int rv = 0;

    rv = im_field_udf_set(0, index, type, value);
    return rv;
}

int ow_pub_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry)
{
    int rv = 0;
    im_fp_entry_t fp_entry;
    //im_fp_action_t fp_action; 

    if(NULL==p_class_entry) {
        return -1;
    }
    
    if(slot != OeAp_Get_My_Slot_Id()) //Only work on target slot
    {
        return 0;
    }

    //de-attach the counter from the entry, Then destroy the counter
    {
        printf("eid:%d stat:%d \n" ,p_class_entry->entry_id,p_class_entry->stat_id);
        rv=im_field_stat_detach(0, p_class_entry->stat_id, p_class_entry->entry_id);
        if(rv!=0)
            printf("%s:%d Error in detach \n", __func__, __LINE__);
        rv=im_field_stat_destroy(0, p_class_entry->stat_id);
        if(rv!=0)
            printf("%s:%d Error in destroy \n", __func__, __LINE__);
    }

    memset(&fp_entry, 0, sizeof(im_fp_entry_t));
    fp_entry.eid=p_class_entry->entry_id;
    if ( (rv = im_field_entry_remove(0, &fp_entry)) !=0) {
        return rv;
    }

    /*fix me later ??? */
    /*
    memset(&fp_action, 0, sizeof(im_fp_action_t));
    fp_action.eid=p_class_entry->entry_index;
    rv = im_field_action_remove(0, &fp_action);
    */
    return rv;
}

int ow_pub_destroy_vlan_range(int index, int min, int max)
{
    //int rv=0;
    im_fp_range_t fp_range_t;

    memset(&fp_range_t, 0x00, sizeof(im_fp_range_t));
    if((min<=0) || (min>4095))
        return -1;
    
    if((max<=0) || (max>4095))
        return -1;

    if(min>max)
        return -1;

    fp_range_t.rid=index;
    fp_range_t.range_type=0x40; //0x40 means vlan
    fp_range_t.low=min;
    fp_range_t.up=max;

    return im_field_range_destroy(0, &fp_range_t);
}


int ow_pub_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp)
{
    int slot=0;
    int my_slot=OeAp_Get_My_Slot_Id();
    int port=0;
    opt_pbmp_t opt_tmp;

    memset(&opt_tmp, 0, sizeof(opt_pbmp_t));
    memcpy(&opt_tmp, slot_bmp, sizeof(opt_pbmp_t));

    ITERATOR_BMP_START(opt_tmp, slot)
    {    
        port=_get_b_port_from_slot(my_slot, slot);   
        //printf("%s line%d port=%d\n", __func__, __LINE__, port);
        PORT_BMP_ADD((*pbmp), port);
    }    
    ITERATOR_BMP_END;

    return 0;
}

int ow_pub_egress_fp_set(int port, int old_vid, int mask,int new_vid, int index, int mode)
{
    int rv = OE_RST_OK;
    im_fp_entry_t fp;
    im_fp_action_t fp_action;

    memset(&fp, 0, sizeof(im_fp_entry_t));
    memset(&fp_action, 0, sizeof(im_fp_action_t));

    if (mode == OUTER_VLAN_REPLACE) {
        fp.key_mask = KEY_MASK_OUTPORT | KEY_MASK_OUTERVLAN | KEY_MASK_ECAP;
        fp.outport = port;
        fp.outport_mask = 0xffff;
        fp.outer_vlan = old_vid;
        fp.outer_vlan_mask = mask;
        fp.eid = 3072 + index;

        fp_action.eid = fp.eid;
        fp_action.action = imFieldActionOuterVlanNew;
        fp_action.param0 = new_vid;
    } else {
        fp.key_mask = KEY_MASK_INPORT | KEY_MASK_INNERVLAN | KEY_MASK_ECAP;
        fp.inport = port;
        fp.inport_mask = 0xffff;
        fp.inner_vlan = old_vid;
        fp.inner_vlan_mask = mask;
        fp.eid = 3072 + index;

        fp_action.eid = fp.eid;
        fp_action.action = imFieldActionInnerVlanNew;
        fp_action.param0 = new_vid;
    }

    if ( (rv=im_field_entry_add(BCM_UNIT0, &fp)) != 0)
        return rv;

    if ( (rv=im_field_entry_prio_set(BCM_UNIT0, fp.eid, 1)) != 0)
        return rv;

    if ( (rv=im_field_action_add(BCM_UNIT0, &fp_action)) != 0)
        return rv;

    if ( (rv=im_field_entry_install(BCM_UNIT0, fp.eid)) != 0)
        return rv;

    return rv;
}

int ow_pub_egress_fp_remove(int index)
{
    int rv = OE_RST_OK;
    im_fp_entry_t fp;

    memset(&fp, 0, sizeof(im_fp_entry_t));

    fp.eid= 3072 + index;
    if ( (rv = im_field_entry_remove(0, &fp)) !=0) {
        return rv;
    }
   
    return rv;
}

int ow_pub_trunk_psc_set(int outid, int load_balance)
{
    int rv = OE_RST_OK;

    rv = im_trunk_psc_set(0, outid, load_balance);
    return rv;
}

int ow_pub_enable_backgroup_drop(void)
{
    im_fp_entry_t fp_v4;
    im_fp_action_t fp_action_v4;
    int rc;

    memset(&fp_v4, 0x00, sizeof(im_fp_entry_t));
    memset(&fp_action_v4, 0x00, sizeof(im_fp_action_t));

    //Make sure all groups in the same V-group
    _set_fp_slice_map();        

    fp_v4.eid = 3000;
    fp_action_v4.eid =3000;
    fp_action_v4.action=imFieldActionRedirectPort;
    fp_action_v4.param0=30;
    fp_action_v4.param1=28;

    rc = im_field_entry_add(0, &fp_v4);
    if (rc != 0)
    {    
        return OE_RST_SET_CAM_ERR;
    }

    rc = im_field_entry_prio_set(0,3000,0);
    if (rc != 0)
    {    
        return OE_RST_SET_CAM_ERR;
    }

    rc = im_field_action_add(0, &fp_action_v4) ;
    if (rc!= 0)
    {
        return OE_RST_SET_CAM_ERR;
    }
    
    rc = im_field_entry_install(0, fp_v4.eid);
    if (rc != 0)
    {
        return OE_RST_SET_CAM_ERR;
    }

    return OE_RST_OK;
}

int ow_pub_slot_load_balance_set(int all_load_balance, int slot_load_balance)
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
                if (slot_load_balance== 0) {
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
        
        sprintf(buf, "%s", "echo -e \"rcload /usr/local/bcm/tmp.soc\\nexit\" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1");
        if(system("killall -9 bcm.user.proxy 1>/dev/null 2>&1"))
		{
		}
        if (system(buf) != 0)
            return OE_RST_INT_ERR;
    } else {
        return OE_RST_FILE_ERR;
    }

    return OE_RST_OK; 
}

static int _all_ports_pvlan_enable_init(void)
{
    int port = 0;
    bcm_port_map_t *bpm = NULL;
    
     /*for SF3600 , set all b class ports with trust_incoming_vid=1; */
    bpm = OeAp_Get_Bcm_Portmap_By_Module(MODULE_BACK);
    if(NULL != bpm)
    {    
        for(port=0; port<MAX_PORT_NUMBER; port++)
        {
            if(bpm->port_map[port] >= 0)
            {
                if(im_mod_port_table(BCM_UNIT0, bpm->port_map[port], 1, "trust_incoming_vid=1") != 0)
                    return OE_RST_SET_CAM_ERR;
            }
        }
    }

    /* set all f class ports with trust_incoming_vid=0; */
    bpm = OeAp_Get_Bcm_Portmap_By_Module(MODULE_FRONT);
    if(NULL != bpm)
    {    
        for(port=0; port<MAX_PORT_NUMBER; port++)
        {
            if(bpm->port_map[port] >= 0)
            {
                if(im_mod_port_table(BCM_UNIT0, bpm->port_map[port], 1, "trust_incoming_vid=0") != 0)
                    return OE_RST_SET_CAM_ERR;
            }
        }
    }

    /* set all r class ports with trust_incoming_vid=0; */
    bpm = OeAp_Get_Bcm_Portmap_By_Module(MODULE_REAR);
    if(NULL != bpm)
    {    
        for(port=0; port<MAX_PORT_NUMBER; port++)
        {
            if(bpm->port_map[port] >= 0)
            {
                if(im_mod_port_table(BCM_UNIT0, bpm->port_map[port], 1, "trust_incoming_vid=0") != 0)
                    return OE_RST_SET_CAM_ERR;
            }
        }
    }
    
    return OE_RST_OK;
}

int ow_pub_trunk_add_weight(int outid, struct smp_t *smp, int load_balance, int *index, int weight)
{
    int rv = 0;
    int trunk_id = 0;
    int slot = 0;
    int modid = 0;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t bmp;
    struct smp_t add_smp;

    memset(&add_smp, 0, sizeof(struct smp_t));    
    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_slot_module_ports_from_smp(smp, &slot, &uport_pbmp);
 //   printf("%s line%d uport_pbmp:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__,
 //       uport_pbmp.pbits[0],uport_pbmp.pbits[1],uport_pbmp.pbits[2],uport_pbmp.pbits[3]);
    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }


    rv = _check_bcmport_in_trunk(0, outid, &uport_pbmp);
    if(rv == -1)
    {
	return OE_RST_INTERfACE_CONFLICT;
    }
	
    printf("%s line%d uport_pbmp:0x%x,0x%x,0x%x,0x%x\n",__func__,__LINE__,
        uport_pbmp.pbits[0],uport_pbmp.pbits[1],uport_pbmp.pbits[2],uport_pbmp.pbits[3]);
    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }
    
    memset(&bmp, 0, sizeof(im_pbmp_t));
    memcpy(&bmp, &uport_pbmp, sizeof(opt_pbmp_t));

    trunk_id = outid;
    *index = trunk_id;
    printf("[INLINE-DEBUG---------] index=%d outid=%d \n", *index, outid);
    //modid = slot * 2;
    modid = 0;
    /* fix me later */
#ifndef SW_BCM_56960
    rv = im_trunk_add_weight(0, trunk_id, load_balance, bmp, modid, 
        g_port_sequence_num, g_port_sequence, weight);
#endif
    return rv;
}

int ow_pub_bcm_init(void)
{
    int rv = 0;
    int j;
    int unit_num = 1;
    im_pbmp_t pbmp;
    
    if ( (rv=_all_ports_pvlan_enable_init()) !=0) {
        return rv;
    }
    
    for (j = 0; j < unit_num; j++) {
        if (im_trunk_init(j) != 0)
            return OE_RST_SET_CAM_ERR;

        memset(&pbmp, 0xff, sizeof(im_pbmp_t));
		
        im_pvlan_set(j, pbmp, 1) ;

        if (im_vlan_init(j) != 0)
            return OE_RST_SET_CAM_ERR;

        if (im_vlan_port_remove(j, 1, pbmp) != 0)
            return OE_RST_SET_CAM_ERR;

        if (im_field_init(j) != 0)
            return OE_RST_SET_CAM_ERR;
#ifdef _PLATFORM_MIPS_ 
        if (im_l2_clear(j) != 0)
            return OE_RST_SET_CAM_ERR;
#endif
    }
   
    return OE_RST_OK;
}
#endif
