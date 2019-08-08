#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "pub_api.h"
#include "def.h"
#include "appdefs.h"
#include "imatrix/im-types.h"
#include "imatrix/im-field.h"
#include "app.h"

#ifdef SW_CTC8096
#define MAX_OUTPORT_ID_NUM 1024
#define NEXTHOPID_OFFSET 4096

int g_outid_to_lnkid[MAX_OUTPORT_ID_NUM];
static int linkagg_id[8];

static int cur_load_balance ;

static short nexthop_id_record[DEFAULT_ACLS_NUMBER];
static short stats_id_record[DEFAULT_ACLS_NUMBER];

extern int ACL_DEBUG_SWITCH;
extern unsigned char g_port_sequence_num;
extern unsigned char g_port_sequence[156];
extern struct oe_outports_t g_outports[MAX_OUTPORTS_ID];
extern int g_inports_rx_vlantag[SLOT_NUMBER_PER_SYSTEM];
extern oe_rule_t g_rule[DEFAULT_ACLS_NUMBER];

#define OUTID_CHECK(outid)  \
do {    \
    if((outid) < 0 || (outid) >= MAX_OUTPORT_ID_NUM) {\
        return -1;\
    }\
}while(0)


#define CTC_HASH_CONDITION_1 1
#define CTC_HASH_CONDITION_2 2

//if HASH :
// 1. if only one trunk , then redirect trunk with trunk number  CTC_HASH_CONDITION_1
// 2. if more then one trunk, then egressmask                    CTC_HASH_CONDITION_1
// 3. if cross_back and trunk both exist, then egressmask        CTC_HASH_CONDITION_1
// 4. if only cross_back , then redirect pbmp                    CTC_HASH_CONDITION_2
static int ctc4300_get_hash_condition(int num_outports, opt_pbmp_t* cross_back)
{
    int cross_back_flag=0;

    printf("\r\n %s %d num_outports=%d cross_back=0x%p \n\r",__FUNCTION__,__LINE__,num_outports,cross_back);
	
    if(NULL==cross_back)
    {
    	printf("\r\n %s %d \n\r",__FUNCTION__,__LINE__);
        return -1;
    }
    if(cross_back->pbits[0]!=0)
    {
        cross_back_flag=1;
    } 

    if((cross_back_flag==1)&&(num_outports==0))
    {
	printf("\r\n %s %d \n\r",__FUNCTION__,__LINE__);
    	return CTC_HASH_CONDITION_2;
    }

    return CTC_HASH_CONDITION_1;

}

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

static void _init_outid_2_lnkid(void)
{
    int i = 0;

    for(i = 0; i < MAX_OUTPORT_ID_NUM; i++) {
        g_outid_to_lnkid[i] = -1;
    }
}

void init_ctc4300(void)
{
	memset(linkagg_id,0x0,sizeof(linkagg_id));
	memset(nexthop_id_record,0x0,sizeof(nexthop_id_record));
	memset(stats_id_record,0x0,sizeof(stats_id_record));
	_init_outid_2_lnkid();
}


int handle=0;
int ow_ctc4300_dev_open(void)
{
    int rv = 0;
   
    if(0 != rpc_create_handle(&handle)) {
        printf("\r\n RPC connect to ctcsdk failed \r\n");
        return -1;
    }
    return rv;
}

int ow_ctc4300_dev_close(void)
{
    if (handle != 0)
        return (rpc_destroy_handle(handle));
}

int ow_ctc4300_init(void)
{
    int rv=0;
    init_ctc4300();
    return rv;
}   

static int _ctc4300_all_ports_pvlan_enable_init(void)
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
		  if ((im_port_dtag_mode_set(BCM_UNIT0, bpm->port_map[port],IM_PORT_VLAN_NONE))!=0) 
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
                if(im_port_dtag_mode_set(0, bpm->port_map[port],IM_PORT_VLAN_APPEND)!=0)
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
                if(im_port_dtag_mode_set(0, bpm->port_map[port],IM_PORT_VLAN_APPEND)!=0)
                    return OE_RST_SET_CAM_ERR;
            }
        }
    }
    
    return OE_RST_OK;
}


int ow_ctc4300_ctc_init(void)
{
    int rv=0;
    int ix = 0;
    im_pbmp_t pbmp;
	 
    if ( (rv=_ctc4300_all_ports_pvlan_enable_init()) !=0) {
	 printf("\r\n %s %d rv=%d \n\r",__FUNCTION__,__LINE__,rv);
        return rv;
    }	
	
    memset(&pbmp, 0xff, sizeof(im_pbmp_t));
	
    im_pvlan_set(1, pbmp, 1) ;
		
    im_vlan_init(0);
    im_trunk_init(0);
    im_field_init(0);	
    for(ix=0;ix<DEFAULT_ACLS_NUMBER;ix++)
    {
	if(nexthop_id_record[ix] == 1)
	{
	 	rv =  im_field_nexthop_destroy(0, ix+NEXTHOPID_OFFSET);
		printf("\r\n %s %d nexthop rv=%d ix=%d \n\r",__FUNCTION__,__LINE__,rv,ix);
	}

	if(stats_id_record[ix] == 1)
	{
		rv=im_field_stat_destroy(0, ix+1);
		printf("\r\n %s %d stats rv=%d ix=%d \n\r",__FUNCTION__,__LINE__,rv,ix);
	}
    }
    init_ctc4300();
    return rv;
} 

int ow_ctc4300_sequence_init(void)
{
    int rv=0;

    rv = ow_pub_sequence_init();
    return rv;
}

static int _ctc4300_set_smp_pvlan_enable(struct smp_t* smp,int enable)
{
    bcm_port_map_t *bcm_pm = NULL;
    int module =0;
    int port=0;  // r/r/b ports
    int bcm_port=0;
    int my_slot = OeAp_Get_My_Slot_Id();
	
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
                        {  
				printf("\r\n  %s %d g_inports_rx_vlantag[my_slot]=%d \n\r", __FUNCTION__,__LINE__,g_inports_rx_vlantag[my_slot]); 
				if(g_inports_rx_vlantag[my_slot]==1)
				{
					if((im_port_dtag_mode_set(0, bcm_port,IM_PORT_VLAN_APPEND))!=0)
						return OE_RST_SET_CAM_ERR;
				}
				else
				{
				   if(im_port_dtag_mode_set(0, bcm_port,IM_PORT_VLAN_REPLACE)!=0)
						return OE_RST_SET_CAM_ERR;
				}									
			   }
                        else
                         {
                          	if((im_port_dtag_mode_set(BCM_UNIT0, bcm_port,IM_PORT_VLAN_NONE))!=0)
                                        return OE_RST_SET_CAM_ERR;
			    }
                    }
                }
            }
        }
    }

    return 0;

}

int  _ctc4300_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
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
    printf("\r\n  %s %d pvlan_able=%d \n\r", __FUNCTION__,__LINE__,pvlan_able); 
    _ctc4300_set_smp_pvlan_enable(smp,pvlan_able);
    return rv;
}

int ow_ctc4300_pvlan_set(int pvid, struct smp_t *smp, int pvlan_able)
{
    int rv = 0;

    printf("%s line%d\n", __func__,__LINE__);
    rv = _ctc4300_pvlan_set(pvid, smp, pvlan_able);
    printf("%s line%d  rv=%d \n", __func__,__LINE__,rv);
    return rv;
}

int ow_ctc4300_vlan_add(int vid, slot_vlan_record_t *tag_vlan_rec, slot_vlan_record_t *untag_vlan_rec)
{
    int rv = 0;

    printf("%s line%d\n", __func__,__LINE__);
    rv = ow_pub_vlan_add(vid, tag_vlan_rec, untag_vlan_rec);
    return rv;
}

int ow_ctc4300_vlan_delete(int rule_id, int vid, slot_vlan_record_t *vlan_rec)
{
    int rv = 0;

    printf("%s line%d\n", __func__,__LINE__);
    rv = ow_pub_vlan_delete(rule_id, vid, vlan_rec);
    return rv;
}

int _ctc4300_create_vlan_range(int index, int min, int max)
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

int ow_ctc4300_create_vlan_range(int index, int min, int max)
{
    int rv = 0;
    
    rv = _ctc4300_create_vlan_range(index, min, max);
    return rv;
}

int _ctc4300_destroy_vlan_range(int index, int min, int max)
{
    int rv=0;
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

    rv = im_field_range_destroy(0, &fp_range_t);
    return rv;
}
int ow_ctc4300_destroy_vlan_range(int index, int min, int max)
{
    int rv = 0;
    
    rv =  _ctc4300_destroy_vlan_range(index, min, max);
    return rv;
}

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

static int _get_egress_mask_uport_pbmp(im_pbmp_t *mask_pbmp,  int module, opt_pbmp_t* ports,int cross_flag)
{
    int i = 0;
    im_pbmp_t tmp;
    
    if (!ports || !mask_pbmp) {
        return -1;
    }

    if(ACL_DEBUG_SWITCH)
    {
        printf("                   mask_pbmp:0x%x, 0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x,0x%x\n", mask_pbmp->pbits[0], mask_pbmp->pbits[1],
            mask_pbmp->pbits[2], mask_pbmp->pbits[3],mask_pbmp->pbits[4],mask_pbmp->pbits[5],
            mask_pbmp->pbits[6],mask_pbmp->pbits[7]);
    }
    
    memset(&tmp, 0, sizeof(im_pbmp_t));

    if (cross_flag == 0) {
        _convert_smp_to_pbmp(&tmp, ports, module);
    } else {
        _convert_crossback_to_pbmp(&tmp, ports);
    }
    for (i = 0; i < MAX_PORT_NUMBER; i++) {
        if (IS_PORT_IN_BMP(tmp, i)) {
            PORT_BMP_DEL((*mask_pbmp), i);
        }
    }
    if(ACL_DEBUG_SWITCH)
    {
        printf("                   mask_pbmp:0x%x, 0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x,0x%x\n", mask_pbmp->pbits[0], mask_pbmp->pbits[1],
            mask_pbmp->pbits[2], mask_pbmp->pbits[3],mask_pbmp->pbits[4],mask_pbmp->pbits[5],
            mask_pbmp->pbits[6],mask_pbmp->pbits[7]);
    }
    return 0;
}


int _ctc4300_enable_acl_on_slot(int rule_id, 
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
    int nexthopid;
    im_fp_stat_t stat;
    int stat_id=entry_id +1;

    stats_id_record[entry_id] = 1;
	
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
        fp_entry.rid_flag[range_index]=1;
	 fp_entry.range_id[0] = g_rule[rule_id].vlan_min;
	 fp_entry.range_id[1] = g_rule[rule_id].vlan_max;
	 fp_entry.outer_vlan_mask=0xfff;
	 printf("\r\n  %s %d  range index :%d  min:%d  max:%d \n\r", __FUNCTION__,__LINE__,range_index,g_rule[rule_id].vlan_min,g_rule[rule_id].vlan_max);
    }else if(outer_vlan)
    {
        fp_entry.key_mask |= KEY_MASK_OUTERVLAN;
        fp_entry.outer_vlan=outer_vlan;
        fp_entry.outer_vlan_mask=0xfff;
    }
    fp_entry.eid=entry_id;

    {
        if(ACL_DEBUG_SWITCH)
        {
            print_debug_entry(&fp_entry); 
        }        
    }

    if (im_field_entry_add(0, &fp_entry) != 0)
    {
        return OE_RST_SET_CAM_ERR;
    }
    
    if (im_field_entry_prio_set(0, entry_id , 2048-rule_id) != 0)
    {    
        return OE_RST_SET_CAM_ERR;
    }

    if(action == FW_MODE_DROP)	
    {
    	 nexthopid = 1;
    }
    else
    {
        nexthopid = rule_id + NEXTHOPID_OFFSET;
    }

    fp_action.eid = entry_id;
    fp_action.param1=nexthopid;

	
    memset(&stat, 0x00, sizeof(im_fp_stat_t));
    stat.group = fp_entry.gid;          
    stat.stat_arr_sz = 2;         
    stat.stat_arr[0] = imFieldStatBytes;
    stat.stat_arr[1] = imFieldStatPackets;
    rv=im_field_stat_create(0, &stat, stat_id);
    printf("\r\n %s %d im_field_stat_create rv=%d stat_id=%d \n\r",__FUNCTION__,__LINE__,rv,stat_id);
    p_class_entry->stat_id=stat_id;  //pass this id back to g_rule list
    fp_action.param0 = stat_id;
	
     printf("\r\n %s %d \n\r",__FUNCTION__,__LINE__);
     if (im_field_action_add(0, &fp_action) != 0)
     {
     	 printf("\r\n %s %d rv=%d\n\r",__FUNCTION__,__LINE__,rv);
         return OE_RST_SET_CAM_ERR;
     }


    
    if (im_field_entry_install(0, entry_id) != 0)
    {
        return OE_RST_SET_CAM_ERR;
    }

    return 0;
}

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
                                 unsigned int entry_id)
{
    int rv = 0;
    
    printf("%s line%d\n", __func__,__LINE__);
    rv = _ctc4300_enable_acl_on_slot(rule_id, slot,p_class_entry, outer_vlan, range_index,
         action, f_ports, r_ports, b_ports, cross_back, outports,num_outports,entry_id);
    return rv;
}



int _ctc4300_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry)
{
    int rv = 0;
    im_fp_entry_t fp_entry;

    printf("\r\n %s %d slot=%d p_class_entry=0x%p\n\r",__FUNCTION__,__LINE__,slot,p_class_entry);
    if(NULL==p_class_entry) {
        return -1;
    }
    
    if(slot != OeAp_Get_My_Slot_Id()) //Only work on target slot
    {
        return 0;
    }

    //de-attach the counter from the entry, Then destroy the counter
    {
        printf("\r\n %s %d  eid:%d stat:%d \n" ,__func__, __LINE__,p_class_entry->entry_id,p_class_entry->stat_id);
        rv=im_field_stat_destroy(0, p_class_entry->stat_id);
	 stats_id_record[p_class_entry->stat_id - 1] = 0;
        if(rv!=0)
            printf("\r\n %s:%d Error in destroy \n", __func__, __LINE__);
    }

    memset(&fp_entry, 0, sizeof(im_fp_entry_t));
    fp_entry.eid=p_class_entry->entry_id;
    if ( (rv = im_field_entry_remove(0, &fp_entry)) !=0) {
	 printf("\r\n %s %d rv=%d \n\r",__FUNCTION__,__LINE__,rv);
        return rv;
    }

    return rv;
}


int ow_ctc4300_disable_acl_on_slot(int slot, oe_class_entry_t* p_class_entry)
{
    int rv = 0;

    rv = _ctc4300_disable_acl_on_slot(slot, p_class_entry);
    return rv;
}

static int _get_lnkid_by_outid(int outid, int *lnkid)
{
    OUTID_CHECK(outid);

    *lnkid = g_outid_to_lnkid[outid];
    return 0;
}

static int _update_outid_to_lnkid(int outid, int lnkid)
{
    OUTID_CHECK(outid);

    g_outid_to_lnkid[outid] = lnkid;
    return 0;
}

extern int _get_slot_module_ports_from_smp(struct smp_t* smp, int *slot, opt_pbmp_t * pbmp);
int ow_ctc4300_trunk_add(int outid, struct smp_t *smp, int type, int load_balance, int *index)
{
    int rv = 0;
    int slot = 0;
    int modid = 0;
    int lnkid = -1;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t im_pbmp;
    int i;

    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_slot_module_ports_from_smp(smp, &slot, &uport_pbmp);

    if (IS_EMPTY_BMP(uport_pbmp))
    {
        printf("\r\n  %s %d rv=%d \n\r", __FUNCTION__,__LINE__,rv); 
        return 0;
    }

    memset(&im_pbmp,0x0,sizeof(im_pbmp_t));
    memcpy(&im_pbmp, &uport_pbmp, sizeof(im_pbmp_t));

    _get_lnkid_by_outid(outid,&lnkid);

    if(lnkid == -1)
    {	    
        for(i=0;i<8;i++)
        {
            if(linkagg_id[i] == 0)
            {
                lnkid = i;
                linkagg_id[i] = 1;
                break;
            }
        }
    }

    if(lnkid == -1)
    {
        return OE_RST_OUTPORTS_NUM;
    }

    modid = 0;
    if(ACL_DEBUG_SWITCH)
    { 
        printf("\r\n  %s %d outid=%d  lnkid=%d \n\r", __FUNCTION__,__LINE__,outid,lnkid);    
        printf("\r\n  %s %d slot=%d  modid=%d im_pbmp.pbits[0]=0x%x  im_pbmp.pbits[1]=0x%x \n\r", __FUNCTION__,__LINE__,slot,modid,im_pbmp.pbits[0],im_pbmp.pbits[1]);    
    }

    if ( (rv=_update_outid_to_lnkid(outid,lnkid)) != 0) 
    {
        printf("\r\n  %s %d rv=%d \n\r", __FUNCTION__,__LINE__,rv); 
        return rv;
    }

    cur_load_balance = load_balance;
    if ( (rv=im_trunk_add(0, lnkid, 1<<load_balance,im_pbmp, modid,g_port_sequence_num, g_port_sequence)) != 0) 
    {
        printf("\r\n  %s %d rv=%d \n\r", __FUNCTION__,__LINE__,rv); 
        return rv;
    }

    *index = lnkid;
    return rv;
}

int ow_ctc4300_trunk_add_weight(int outid, struct smp_t *smp, int type, int load_balance, int *index, int weight)
{
    return ow_ctc4300_trunk_add(outid, smp, type, load_balance, index);
}

int ow_ctc4300_trunk_del(int outid, struct smp_t *smp, int type)
{
    
    int rv = 0;
    int lnkid;
    int slot = 0;
    int modid = 0;
    opt_pbmp_t uport_pbmp;
    im_pbmp_t bmp;
    im_trunk_add_info_t trunk_info;
	
    if (type == OUTPORT_MC) {
        return 0;
    }

    _get_lnkid_by_outid(outid,&lnkid);
   
    memset(&uport_pbmp, 0, sizeof(opt_pbmp_t));
    _get_slot_module_ports_from_smp(smp, &slot, &uport_pbmp);

    if (IS_EMPTY_BMP(uport_pbmp)) {
        return 0;
    }
    
    memset(&bmp, 0, sizeof(im_pbmp_t));
    memcpy(&bmp, &uport_pbmp, sizeof(opt_pbmp_t));

   modid = 0;
   if(ACL_DEBUG_SWITCH)
   { 
  	 printf("\r\n  %s %d outid=%d  lnkid=%d \n\r", __FUNCTION__,__LINE__,outid,lnkid);    
	 printf("\r\n  %s %d slot=%d  modid=%d bmp.pbits[0]=0x%x  bmp.pbits[1]=0x%x \n\r", __FUNCTION__,__LINE__,slot,modid,bmp.pbits[0],bmp.pbits[1]);    
   }

    rv = im_trunk_remove(0, lnkid, bmp, modid);
    if(rv !=0)
    {
	printf("\r\n  %s %d rv=%d \n\r", __FUNCTION__,__LINE__,rv); 
	return rv;
    }

    im_trunk_get(0,lnkid,&trunk_info);

    printf("\r\n %s %d trunk_info.num_ports=%d \n\r",__FUNCTION__,__LINE__,trunk_info.num_ports);
    if(trunk_info.num_ports < 0)
    {
	linkagg_id[lnkid] = 0;
	_update_outid_to_lnkid(outid, -1);
    }
    return rv;
}

int ow_ctc4300_ud_set(int index, int type, int value)
{
    int rv = 0;

    rv = im_field_udf_set(0, index, type, value);
    return rv;
}

int ow_ctc4300_trunk_psc_set(int outid, int load_balance)
{
    int rv = OE_RST_OK;
    int rtag = 0;

    rtag = 1 << load_balance;
    cur_load_balance = load_balance;
    rv = im_trunk_psc_set(0, outid, rtag);
    return rv;
}

int ow_ctc4300_crossback_pbmp_get(opt_pbmp_t *slot_bmp, opt_pbmp_t *pbmp)
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
        printf("\r\n %s line%d port=%d\n", __func__, __LINE__, port);
        PORT_BMP_ADD((*pbmp), port);
    }    
    ITERATOR_BMP_END;

    return 0;
}

int stacking_backgroud_strem_init(void)
{
	im_fp_entry_t fp_v4;
       im_fp_action_t fp_action_v4;
	im_fp_entry_t fp_v6;
       im_fp_action_t fp_action_v6;
	int rc = 0;

	memset(&fp_v4, 0x00, sizeof(im_fp_entry_t));
	memset(&fp_v6, 0x00, sizeof(im_fp_entry_t));
	memset(&fp_action_v4, 0x00, sizeof(im_fp_action_t));
	memset(&fp_action_v6, 0x00, sizeof(im_fp_action_t));

	fp_v4.eid = 2047;
	fp_action_v4.eid =2047;
	fp_action_v4.param1 = 0x0;
	fp_action_v4.action = imFieldActionDrop;

	fp_v6.eid = 2048;
	fp_v6.key_mask |= KEY_MASK_SIPV6;
	fp_action_v6.eid =2048;
	fp_action_v6.param1 = 0x0;
	fp_action_v6.action = imFieldActionDrop;

       printf("\r\n %s %d fp_action_v4.action=%d fp_action_v6.action=%d \n\r",__FUNCTION__,__LINE__,fp_action_v4.action,fp_action_v6.action);
	rc = im_field_entry_add(0, &fp_v4);
	printf("\r\n %s %d rc=%d \n\r",__FUNCTION__,__LINE__,rc);
	if (rc != 0)
	{
		return OE_RST_SET_CAM_ERR;
	}

	rc = im_field_action_set(0, &fp_action_v4) ;
	printf("\r\n %s %d rc=%d \n\r",__FUNCTION__,__LINE__,rc);
	if (rc!= 0)
	{
		return OE_RST_SET_CAM_ERR;
	}

	rc = im_field_entry_install(0, fp_v4.eid);
	printf("\r\n %s %d rc=%d \n\r",__FUNCTION__,__LINE__,rc);
	if (rc != 0)
	{
		return OE_RST_SET_CAM_ERR;
	}


	rc = im_field_entry_add(0, &fp_v6);
	printf("\r\n %s %d rc=%d \n\r",__FUNCTION__,__LINE__,rc);
	if (rc != 0)
	{
		return OE_RST_SET_CAM_ERR;
	}

	rc = im_field_action_set(0, &fp_action_v6) ;
	printf("\r\n %s %d rc=%d \n\r",__FUNCTION__,__LINE__,rc);
	if (rc!= 0)
	{
		return OE_RST_SET_CAM_ERR;
	}

	rc = im_field_entry_install(0, fp_v6.eid);
	printf("\r\n %s %d rc=%d \n\r",__FUNCTION__,__LINE__,rc);
	if (rc != 0)
	{
		return OE_RST_SET_CAM_ERR;
	}
	return rc;
}


int ow_ctc4300_enable_backgroup_drop()
{
    int rv=0;
    printf("\r\n %s %d \n\r",__FUNCTION__,__LINE__);
    rv = stacking_backgroud_strem_init();
    return rv;
}


int ow_ctc4300_slot_load_balance_set(int all_load_balance, int slot_load_balance)
{
    int rtag;
    printf("%s line%d all_load_balance=0x%x,slot_load_balance=%d\n",__func__,__LINE__,all_load_balance, slot_load_balance);

    rtag = 1<<cur_load_balance;
    if(slot_load_balance)
    {
    	 rtag |= 1<< 10;
	 im_trunk_psc_set(0, 0, rtag);		
    }
    return OE_RST_OK; 
}


int _ctc4300_create_nexthop_on_slot(int rule_id, 
                              int slot,
                              int action, 
                              opt_pbmp_t* f_ports, 
                              opt_pbmp_t* r_ports, 
                              opt_pbmp_t* b_ports,
                              opt_pbmp_t* cross_back,
                              unsigned int* outports,
                              unsigned int num_outports)
{
    int rv=0;
    im_fp_entry_t fp_entry;
    im_fp_action_t fp_action;
    im_pbmp_t im_pbmp;
    int i=0;
    int nexthopid;

    memset(&fp_entry, 0x00, sizeof(im_fp_entry_t));
    memset(&fp_action, 0x00, sizeof(im_fp_action_t));

    if(slot != OeAp_Get_My_Slot_Id()) //Only work on target slot
    {
        return 0;
    }

    nexthopid = rule_id + NEXTHOPID_OFFSET;
    nexthop_id_record[rule_id] = 1;
    if(action==FW_MODE_DROP) // if drop, to BCM 
    {
        memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
        for(i=0;i<8;i++)
        {
            im_pbmp.pbits[i]=0xffffffff;
        }
        fp_action.action=imFieldActionDrop;
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
	 fp_action.param1=nexthopid;
        memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
    }
    
    //if HASH :
    // 1. if only one trunk , then redirect trunk with trunk number  CTC_HASH_CONDITION_1
    // 2. if more then one trunk, then egressmask                    CTC_HASH_CONDITION_1
    // 3. if cross_back and trunk both exist, then egressmask        CTC_HASH_CONDITION_1
    // 4. if only cross_back , then redirect pbmp                    CTC_HASH_CONDITION_2
    if(action==FW_MODE_HASH)    
    {
        int hash_condition=ctc4300_get_hash_condition(num_outports,cross_back);
        int i = 0;
	 fp_action.param1=nexthopid;
        if(ACL_DEBUG_SWITCH)
        {
            printf("[HASH-ACTION] hash condition:%d,outid=%d,g_outports=%p\n", hash_condition,outports[0],g_outports);
            printf("[HASH-ACTION] hash condition:%d, index=%d nexthopid=%d \n", hash_condition,g_outports[outports[0]].index,nexthopid);
        } 
        
	printf("\r\n %s %d hash_condition=%d \n\r",__FUNCTION__,__LINE__,hash_condition);
        switch(hash_condition)
        {
            case CTC_HASH_CONDITION_1:
		  for(i=0;i<num_outports;i++)
                {
                	fp_action.param0 |=1<<g_outports[outports[i]].index; 
		  }
		  
                fp_action.action = imFieldActionEgressMask;
                memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
                for (i =0; i< 8; i++) {
                    im_pbmp.pbits[i] = 0xffffffff;
                }
                _get_egress_mask_uport_pbmp(&im_pbmp, -1, cross_back,1);
                memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
                break;
            case CTC_HASH_CONDITION_2:
                fp_action.action = imFieldActionRedirectPbmp;
                memset(&im_pbmp, 0x00, sizeof(im_pbmp_t));
                _convert_crossback_to_pbmp(&im_pbmp, cross_back);
                memcpy(&fp_action.arg_pbmp, &im_pbmp, sizeof(im_pbmp_t));
                break;
            default:
                break;
        }
    }
	
    if(ACL_DEBUG_SWITCH)
    {
	printf("\r\n %s %d fp_action.action=%d \n\r",__FUNCTION__,__LINE__,fp_action.action);
    } 

    if (im_field_nexthop_create(0, &fp_action) != 0)  
    {
    	 printf("\r\n %s %d rv=%d\n\r",__FUNCTION__,__LINE__,rv);
        return OE_RST_SET_CAM_ERR;
    }

    return 0;
}


int ow_ctc4300_create_nexthop_on_slot(int rule_id, 
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
    
    printf("%s line%d\n", __func__,__LINE__);
    rv = _ctc4300_create_nexthop_on_slot(rule_id, slot,action, f_ports, r_ports, b_ports, 
		cross_back, outports,num_outports);
    return rv;
}

int ow_ctc4300_destroy_nexthop_on_slot(int rule_id)
{
    int rv = 0;
    
    printf("\r\n %s line%d  g_rule[%d].forward_action=%d  \n\r", __func__,__LINE__,rule_id,g_rule[rule_id].forward_action);

    if(g_rule[rule_id].forward_action != FW_MODE_DROP)
    {	
    	rv =  im_field_nexthop_destroy(0, rule_id+NEXTHOPID_OFFSET);
	nexthop_id_record[rule_id] = 0;
    }
    return rv;
}

int ow_ctc4300_get_port_info(int module, int port, struct port_sfp_t *port_sfp_p)
{
    return ow_pub_get_port_info(module,port,port_sfp_p);
}

int ow_ctc4300_printf_interface_sfp(struct board_sfp_t *board_sfp)
{
    return ow_pub_printf_interface_sfp(board_sfp);
}

int ow_ctc4300_set_interface_force_up(struct smp_t *smp, int onoff)
{
    int module, port;
    bcm_port_map_t *bpm;
    int slot = 0, unit = 0, bcm_port = 0;
    int rv = OE_RST_OK;

    slot = OeAp_Get_My_Slot_Id();

    for (module = MODULE_REAR; module < MODULE_REAR + MODULE_NUMBER_PER_BOARD; module++) {
        if ((smp->modules & (1 << module)) == 0)
            continue;

        bpm = OeAp_Get_Bcm_Portmap_By_Sm(slot, module);
        if(bpm == NULL)
            continue;

        for (port = 0; port < MAX_PORT_NUMBER; port++) {
            if (port >= bpm->port_number)
                continue;
            if (smp->ports & (1ull << port)) {
                if (OeAp_Get_Bcm_Unit_Port(bpm, module, port, &unit, &bcm_port) == 0) { 
                    if (onoff == 1)
                    {   
                        rv=im_port_link_force(unit,bcm_port,1,1);
                    }
                    else 
                        rv=im_port_link_force(unit,bcm_port,0,1);

                    if(rv != OE_RST_OK)
                        return rv;
                }    
            }    
        }    
    }    

    return OE_RST_OK;

}

int ow_ctc4300_soc_install(void)
{
    char buf[256];

    if (access("/appfs/exp_user_def/config.soc", F_OK) == 0)
    {
        sprintf(buf, "%s", "cat /appfs/exp_user_def/config.soc | /usr/local/ctc-user/ctccli 1>/dev/null 2>&1");
        system(buf);
    }

    return 0;
}

int ow_ctc4300_soc_uninstall(void)
{
    char buf[256];

    if (access("/appfs/exp_user_def/config_uninstall.soc", F_OK) == 0)
    {
        sprintf(buf, "%s", "cat /appfs/exp_user_def/config_uninstall.soc | /usr/local/ctc-user/ctccli 1>/dev/null 2>&1");
        system(buf);
    }

    return 0;
}

#endif

