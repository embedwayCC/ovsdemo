#include <stdio.h>
#include <stdlib.h>

#include "imatrix/im-types.h"

#ifdef SW_BCM_56334
unsigned char g_port_sequence_num = 25;
unsigned char g_port_sequence_front[] = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, AC1200_TO_S1_S2};
unsigned char g_port_sequence_rear[]  = {23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, AC1200_TO_S1_S2};
unsigned char* get_seq_group(int unit)
{
    unsigned char* rv = NULL;
    if(unit==1)
        rv = g_port_sequence_front;
    else
        rv = g_port_sequence_rear;

    return rv;
}

static int _get_module_ports_from_smp(struct smp_t* smp, opt_pbmp_t * pbmp)
{

    int slot = 0;
    bcm_port_map_t *bcm_pm = NULL;
    int module =0;
    int port=0;  // r/r/b ports
    //int uport=0;       // CTC Pannel ports

    if( (NULL==smp)||(pbmp==NULL) ) {
        return -1;
    }

    for(slot=0; slot<SLOT_NUMBER_PER_SYSTEM; slot++)
    {
        if(smp->slots & (1 << slot))
            break; 
    }
    
    for (module = MODULE_REAR; module <= MODULE_BACK; module++) {
        if (smp->modules & (1 << module)) {
            bcm_pm = OeAp_Get_Bcm_Portmap_By_Sm(slot, module);
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

int ow_ac1200_bcm_init(void)
{
    int j;
    int unit_num = 2;
    im_pbmp_t pbmp;

    #if 0
    if ( (rv=_all_ports_pvlan_enable_init()) !=0) {
        return rv;
    }
    #endif
    
    //for AC1200 , set receiving b-class port(xe3: 27) dtag mode to internal
    im_port_dtag_mode_set(0, 27, IM_PORT_DTAG_MODE_INTERNAL);
    im_port_dtag_mode_set(1, 27, IM_PORT_DTAG_MODE_INTERNAL);    
    
    for (j = 0; j < unit_num; j++) {
        if (im_trunk_init(j) != 0)
            return OE_RST_SET_CAM_ERR;

        memset(&pbmp, 0xff, sizeof(im_pbmp_t));
        if (im_pvlan_set(j, pbmp, 1) != 0)
            return OE_RST_SET_CAM_ERR;

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

int ow_ac1200_sequence_init(void)
{
    return 0;
}

int ow_ac1200_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
{
    int rv = 0;
    im_pbmp_t pbmp;
    opt_pbmp_t opt_pbmp;

    printf("%s line%d\n", __func__,__LINE__);
    memset(&pbmp, 0x00, sizeof(im_pbmp_t));
    memset(&opt_pbmp, 0x00, sizeof(opt_pbmp_t));

    _get_module_ports_from_smp(smp, &opt_pbmp);
    rv = im_pvlan_set(0, pbmp, pvid);
    
    return rv;
}

int ow_ac1200_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec)
{
#if 0
    int rv = 0;
    im_pbmp_t im_pbmp;
    im_pbmp_t im_ubmp;

    /* nothing to be done for empty pbmp */
    if (IS_EMPTY_BMP(pbmp) && 
        IS_EMPTY_BMP(ubmp) ) {
        return 0;
    }

    if ( (rv = im_vlan_create(0, vid)) != 0) {
        return rv;
    }

    memset(&im_pbmp, 0, sizeof(im_pbmp_t));
    memcpy(&im_pbmp, &pbmp, sizeof(opt_pbmp_t));
    memset(&im_ubmp, 0, sizeof(im_pbmp_t));
    memcpy(&im_pbmp, &ubmp, sizeof(opt_pbmp_t));
    
    rv = im_vlan_port_add(0, vid, im_pbmp, im_ubmp);
#endif
    return 0;
}

int ow_ac1200_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec)
{
#if 0
     int rv = 0;
    im_pbmp_t im_bmp;

    /* nothing to be done for empty pbmp */
    if (IS_EMPTY_BMP(pbmp)) {
        return 0;
    }

    if ( (rv = im_vlan_create(0, vid)) != 0) {
        return rv;
    }

    memset(&im_bmp, 0, sizeof(im_pbmp_t));
    memcpy(&im_bmp, &pbmp, sizeof(opt_pbmp_t));
    rv = im_vlan_port_remove(0, vid, im_bmp);
    
    return rv;
#endif
    return 0;
}

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
                                unsigned int entry_id)
{
    int rv = 0;
    
    printf("%s line%d\n", __func__,__LINE__);
    return rv;
}

extern struct oe_outports_t g_outports[MAX_OUTPORTS_ID];
static int _get_outports_uport_pbmp(int outid, opt_pbmp_t *uport_pbmp)
{
    int slot = 0;
    opt_pbmp_t pbm;
    port_list_t *list = NULL;

    if (!uport_pbmp) {
        return -1;
    }

    list = g_outports[outid].p_list;
    while (list) {
        memset(&pbm, 0x00, sizeof(opt_pbmp_t));
        _get_slot_module_ports_from_smp(&(list->smp), &slot, &pbm);
        MERGE_BMP((*uport_pbmp), pbm);
        list = list->p_next;        
    }
    
    return 0;
}

extern int _get_slot_module_ports_from_smp(struct smp_t* smp, int *slot, opt_pbmp_t * pbmp);
int ow_ac1200_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index)
{
    int rv = 0;
    int trunk_id = 0;
    int slot = 0;
    int modid = 0;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t bmp;
    unsigned char* seq = NULL;
    int seq_num = 0;

    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_slot_module_ports_from_smp(smp, &slot, &uport_pbmp);

    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }
    
    printf("%s line%d\n", __func__,__LINE__);
    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_outports_uport_pbmp(outid, &uport_pbmp);

    memset(&bmp, 0, sizeof(im_pbmp_t));
    memcpy(&bmp, &uport_pbmp, sizeof(opt_pbmp_t));

    seq = get_seq_group(0);
    seq_num = get_seq_num();
    trunk_id = outid;
    *index = trunk_id;
    modid = slot * 2;
    rv = im_trunk_add(0, trunk_id, load_balance, bmp, modid, seq_num, seq);
    return rv;
}

int ow_ac1200_trunk_del(int outid, struct smp_t *smp, int type)
{
    return 0;
}

#endif

