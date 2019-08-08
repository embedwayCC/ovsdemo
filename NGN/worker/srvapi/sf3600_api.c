#ifdef SW_BCM_5684X
#include "ow_api.h"
#include "sf3600_api.h"

ow_api_t ow_sf3600_api = {
    ow_sf3600_dev_open,
    ow_sf3600_dev_close,
    ow_sf3600_init,
    ow_sf3600_bcm_init,
    ow_sf3600_sequence_init,
    ow_sf3600_pvlan_set,
    ow_sf3600_vlan_add,
    ow_sf3600_vlan_delete,
    ow_sf3600_create_vlan_range,
    ow_sf3600_destroy_vlan_range,
    ow_sf3600_enable_acl_on_slot,
    ow_sf3600_disable_acl_on_slot,
    ow_sf3600_trunk_add, 
    ow_sf3600_trunk_del,
    ow_sf3600_ud_set,
    ow_sf3600_crossback_pbmp_get,
    ow_sf3600_egress_fp_set,
    ow_sf3600_egress_fp_remove,
    ow_sf3600_trunk_psc_set,
    ow_sf3600_enable_backgroup_drop,
    ow_sf3600_slot_load_balance_set,
    ow_sf3600_create_nexthop_on_slot,
    ow_sf3600_destroy_nexthop_on_slot,
    ow_sf3600_get_port_info,
    ow_sf3600_printf_interface_sfp,
    ow_sf3600_set_interface_force_up,
    ow_sf3600_soc_install,
    ow_sf3600_soc_uninstall,
    ow_sf3600_trunk_add_weight, 
};
#endif

