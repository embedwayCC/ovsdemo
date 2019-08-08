#ifdef SW_BCM_56334
#include "ow_api.h"
#include "ac1200_api.h"

ow_api_t ow_ac1200_api = {
    NULL,
    NULL,
    NULL,
    ow_ac1200_bcm_init,
    ow_ac1200_sequence_init,
    ow_ac1200_pvlan_set,
    ow_ac1200_vlan_add,
    ow_ac1200_vlan_delete,
    NULL,
    NULL,
    NULL,
    NULL,
    ow_ac1200_trunk_add,
    ow_ac1200_trunk_del,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
#endif

