#ifdef SW_CTC8096
#include "ow_api.h"
#include "ctc4300_api.h"

ow_api_t ow_ctc4300_api = {
    ow_ctc4300_dev_open,
    ow_ctc4300_dev_close,
    ow_ctc4300_init,
    ow_ctc4300_ctc_init,
    ow_ctc4300_sequence_init,
    ow_ctc4300_pvlan_set,
    ow_ctc4300_vlan_add,
    ow_ctc4300_vlan_delete, 
    ow_ctc4300_create_vlan_range,
    ow_ctc4300_destroy_vlan_range,
    ow_ctc4300_enable_acl_on_slot,
    ow_ctc4300_disable_acl_on_slot,
    ow_ctc4300_trunk_add,
    ow_ctc4300_trunk_del,
    ow_ctc4300_ud_set,
    ow_ctc4300_crossback_pbmp_get,
    NULL,
    NULL,
    ow_ctc4300_trunk_psc_set,
    ow_ctc4300_enable_backgroup_drop,
    ow_ctc4300_slot_load_balance_set,
    ow_ctc4300_create_nexthop_on_slot,
    ow_ctc4300_destroy_nexthop_on_slot,
    ow_ctc4300_get_port_info,
    ow_ctc4300_printf_interface_sfp,
    ow_ctc4300_set_interface_force_up,
    ow_ctc4300_soc_install,
    ow_ctc4300_soc_uninstall,
    ow_ctc4300_trunk_add_weight,
};
#endif

