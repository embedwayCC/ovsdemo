#ifdef SW_BCM_56960
#include "ow_api.h"
#include "sf4800_api.h"

ow_api_t ow_sf4800_api = {
    ow_sf4800_dev_open,
    ow_sf4800_dev_close,
    ow_sf4800_init,
    ow_sf4800_bcm_init,
    ow_sf4800_sequence_init,
    ow_sf4800_pvlan_set,
    ow_sf4800_vlan_add,
    ow_sf4800_vlan_delete,   
    ow_sf4800_create_vlan_range,
    ow_sf4800_destroy_vlan_range,
    ow_sf4800_enable_acl_on_slot,
    ow_sf4800_disable_acl_on_slot,
    ow_sf4800_trunk_add,
    ow_sf4800_trunk_del,
    ow_sf4800_ud_set,
    ow_sf4800_crossback_pbmp_get,
    ow_sf4800_egress_fp_set,
    ow_sf4800_egress_fp_remove,
    ow_sf4800_trunk_psc_set,
    ow_sf4800_enable_backgroup_drop,
    ow_sf4800_slot_load_balance_set,
    ow_sf4800_create_nexthop_on_slot,
    ow_sf4800_destroy_nexthop_on_slot,
    ow_sf4800_get_port_info,
    ow_sf4800_printf_interface_sfp, 
    ow_sf4800_set_interface_force_up,
    ow_sf4800_soc_install,
    ow_sf4800_soc_uninstall,
    ow_sf4800_trunk_add_weight,
};
#endif

