#include <bf_rt/bf_rt_info.hpp>
#include <bf_rt/bf_rt_init.hpp>
#include <bf_rt/bf_rt_common.h>
#include <bf_rt/bf_rt_table_key.hpp>
#include <bf_rt/bf_rt_table_data.hpp>
#include <bf_rt/bf_rt_table.hpp>
#include <bf_rt/proto/bf_rt_server.h>

#include <getopt.h>
#include "tna_exact_match.h"
#include <arpa/inet.h>
//#include "copy_to_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <bf_switchd/bf_switchd.h>
#ifdef __cplusplus
}
#endif

/***********************************************************************************
 * This sample cpp application code is based on the P4 program
 *tna_exact_match.p4
 * Please refer to the P4 program and the generated bf-rt.json for information
 *on
 * the tables contained in the P4 program, and the associated key and data
 *fields.
 **********************************************************************************/

namespace bfrt {
namespace examples {
namespace tna_exact_match {

// Structure definition to represent the key of the ipRoute table
struct IpRouteKey {
  uint32_t ipDstAddr;
  uint16_t ingress_port;
};

// Structure definition to represent the data of the ipRoute table for action
// "route"
struct IpRoute_routeData {
  uint64_t srcMac;
  uint64_t dstMac;
  uint16_t dst_port;
};

// Structure definition to represent the data of the ipRoute table for action
// "nat"
struct IpRoute_natData {
  uint32_t srcAddr;
  uint32_t dstAddr;
  uint16_t dst_port;
};

// Structure definition tp represent the data of the ipRoute table
struct IpRouteData {
  union {
    IpRoute_routeData route_data;
    IpRoute_natData nat_data;
  } data;
  // Based on the action_id, contents of the enum are interpreted
  bf_rt_id_t action_id;
};

// Structure definition to represent the key of the forward table
struct ForwardKey {
  uint32_t ipDstAddr;
};

// Structure definition tp represent the data of the forward table
struct ForwardData { 
  uint16_t hit_data; 
  // Based on the action_id, contents of the enum are interpreted
  bf_rt_id_t action_id;
};

namespace {
// Key field ids, table data field ids, action ids, Table object required for
// interacting with the table
const bfrt::BfRtInfo *bfrtInfo = nullptr;
const bfrt::BfRtTable *ipRouteTable = nullptr;
std::shared_ptr<bfrt::BfRtSession> session;

std::unique_ptr<bfrt::BfRtTableKey> bfrtTableKey;
std::unique_ptr<bfrt::BfRtTableData> bfrtTableData;

// Key field ids
bf_rt_id_t ipRoute_ip_dst_field_id = 0;
bf_rt_id_t ipRoute_ingress_port_field_id = 0;

// Action Ids
bf_rt_id_t ipRoute_route_action_id = 0;
bf_rt_id_t ipRoute_nat_action_id = 0;

// Data field Ids for route action
bf_rt_id_t ipRoute_route_action_src_mac_field_id = 0;
bf_rt_id_t ipRoute_route_action_dst_mac_field_id = 0;
bf_rt_id_t ipRoute_route_action_port_field_id = 0;

// Data field ids for nat action
bf_rt_id_t ipRoute_nat_action_ip_src_field_id = 0;
bf_rt_id_t ipRoute_nat_action_ip_dst_field_id = 0;
bf_rt_id_t ipRoute_nat_action_port_field_id = 0;

//router counter ids
bf_rt_id_t ipRoute_router_action_counter_spec_bytes_field_id = 0;
bf_rt_id_t ipRoute_router_action_counter_spec_pkts_field_id = 0;

#define ALL_PIPES 0xffff
bf_rt_target_t dev_tgt;

const bfrt::BfRtTable *forwardTable = nullptr;
std::unique_ptr<bfrt::BfRtTableKey> bfrtTableKey_;
std::unique_ptr<bfrt::BfRtTableData> bfrtTableData_;
bf_rt_id_t forward_ip_dst_field_id = 0;
bf_rt_id_t forward_hit_action_id = 0; 
bf_rt_id_t forward_hit_action_port_field_id = 0; 
bf_rt_id_t forward_copy_to_cpu_action_id = 0;

const bfrt::BfRtTable *state_statsTable = nullptr;
std::unique_ptr<bfrt::BfRtTableKey> bfrtTableKey1;
std::unique_ptr<bfrt::BfRtTableData> bfrtTableData1;
bf_rt_id_t state_stats_counter_index_field_id = 0;
bf_rt_id_t state_stats_counter_spec_pkts_field_id = 0;
}  // anonymous namespace 

// This function does the initial setUp of getting bfrtInfo object associated
// with the P4 program from which all other required objects are obtained
void setUp() {
  dev_tgt.dev_id = 0;
  dev_tgt.pipe_id = ALL_PIPES;
  // Get devMgr singleton instance
  auto &devMgr = bfrt::BfRtDevMgr::getInstance();

  // Get bfrtInfo object from dev_id and p4 program name
  auto bf_status =
      devMgr.bfRtInfoGet(dev_tgt.dev_id, "tna_exact_match", &bfrtInfo);
  // Check for status
  assert(bf_status == BF_SUCCESS);

  // Create a session object
  session = bfrt::BfRtSession::sessionCreate();
}

// This function does the initial set up of getting key field-ids, action-ids
// and data field ids associated with the ipRoute table. This is done once
// during init time.
void tableSetUp() {
  // Get table object from name
  auto bf_status = bfrtInfo->bfrtTableFromNameGet("SwitchIngress.ipRoute", &ipRouteTable);
  assert(bf_status == BF_SUCCESS);

  bf_status = bfrtInfo->bfrtTableFromNameGet("SwitchIngress.forward", &forwardTable);
  assert(bf_status == BF_SUCCESS);

  // Get action Ids for route and nat actions
  bf_status = ipRouteTable->actionIdGet("SwitchIngress.route", &ipRoute_route_action_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->actionIdGet("SwitchIngress.nat", &ipRoute_nat_action_id);
  assert(bf_status == BF_SUCCESS);

  // Get action Ids for hit actions
  bf_status = forwardTable->actionIdGet("SwitchIngress.hit", &forward_hit_action_id);
  assert(bf_status == BF_SUCCESS); 

  bf_status = forwardTable->actionIdGet("SwitchIngress.copy_to_cpu", &forward_copy_to_cpu_action_id);
  assert(bf_status == BF_SUCCESS); 
  
  // Get field-ids for key field and data fields
  bf_status = ipRouteTable->keyFieldIdGet("hdr.ipv4.dst_addr", &ipRoute_ip_dst_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->keyFieldIdGet("ig_intr_md.ingress_port", &ipRoute_ingress_port_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = forwardTable->keyFieldIdGet("hdr.ethernet.dst_addr", &forward_ip_dst_field_id);
  assert(bf_status == BF_SUCCESS);
  /***********************************************************************
   * DATA FIELD ID GET FOR "route" ACTION
   **********************************************************************/
  bf_status = ipRouteTable->dataFieldIdGet("srcMac", ipRoute_route_action_id, &ipRoute_route_action_src_mac_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataFieldIdGet("dstMac", ipRoute_route_action_id, &ipRoute_route_action_dst_mac_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataFieldIdGet("dst_port", ipRoute_route_action_id, &ipRoute_route_action_port_field_id);
  assert(bf_status == BF_SUCCESS);

  /***********************************************************************
   * DATA FIELD ID GET FOR "nat" ACTION
   **********************************************************************/
  bf_status = ipRouteTable->dataFieldIdGet("srcAddr", ipRoute_nat_action_id, &ipRoute_nat_action_ip_src_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataFieldIdGet("dstAddr", ipRoute_nat_action_id, &ipRoute_nat_action_ip_dst_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataFieldIdGet("dst_port", ipRoute_nat_action_id, &ipRoute_nat_action_port_field_id);
  assert(bf_status == BF_SUCCESS);

  /***********************************************************************
   * DATA FIELD ID GET FOR "hit" ACTION
   **********************************************************************/
  bf_status = forwardTable->dataFieldIdGet("port", forward_hit_action_id, &forward_hit_action_port_field_id);
  assert(bf_status == BF_SUCCESS); 

  /***********************************************************************
   * DATA FIELD ID GET FOR COUNTER
   **********************************************************************/
  #if 0
  bf_status = ipRouteTable->dataFieldIdGet("$COUNTER_SPEC_BYTES", ipRoute_route_action_id, &ipRoute_router_action_counter_spec_bytes_field_id);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataFieldIdGet("$COUNTER_SPEC_PKTS", ipRoute_route_action_id, &ipRoute_router_action_counter_spec_pkts_field_id);
  assert(bf_status == BF_SUCCESS);
  //printf("%d %d %d\n",ipRoute_router_action_counter_spec_bytes_field_id,ipRoute_router_action_counter_spec_pkts_field_id,ipRoute_route_action_id);
  #endif

  // Allocate key and data once, and use reset across different uses
  bf_status = ipRouteTable->keyAllocate(&bfrtTableKey);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataAllocate(&bfrtTableData);
  assert(bf_status == BF_SUCCESS);

  bf_status = forwardTable->keyAllocate(&bfrtTableKey_);
  assert(bf_status == BF_SUCCESS);

  bf_status = forwardTable->dataAllocate(&bfrtTableData_);
  assert(bf_status == BF_SUCCESS);

  bf_status = bfrtInfo->bfrtTableFromNameGet("SwitchIngress.state_stats", &state_statsTable);
  assert(bf_status == BF_SUCCESS);
  bf_status = state_statsTable->keyFieldIdGet("$COUNTER_INDEX", &state_stats_counter_index_field_id);
  assert(bf_status == BF_SUCCESS);
  bf_status = state_statsTable->dataFieldIdGet("$COUNTER_SPEC_PKTS", &state_stats_counter_spec_pkts_field_id);
  assert(bf_status == BF_SUCCESS); 
  //printf("%d %d\n",state_stats_counter_index_field_id,state_stats_counter_spec_pkts_field_id);
  bf_status = state_statsTable->keyAllocate(&bfrtTableKey1);
  assert(bf_status == BF_SUCCESS);
  bf_status = state_statsTable->dataAllocate(&bfrtTableData1);
  assert(bf_status == BF_SUCCESS);
}

/*******************************************************************************
 * Utility functions associated with "ipRoute" table in the P4 program.
 ******************************************************************************/

// This function sets the passed in ip_dst and vrf value into the key object
// passed using the setValue methods on the key object
void ipRoute_key_setup(const IpRouteKey &ipRoute_key, bfrt::BfRtTableKey *table_key) {
  // Set value into the key object. Key type is "EXACT"
  auto bf_status = table_key->setValue(ipRoute_ip_dst_field_id, static_cast<uint64_t>(ipRoute_key.ipDstAddr));
  assert(bf_status == BF_SUCCESS);

  bf_status = table_key->setValue(ipRoute_ingress_port_field_id, static_cast<uint64_t>(ipRoute_key.ingress_port));
  assert(bf_status == BF_SUCCESS);
  return;
}

void forward_key_setup(const ForwardKey &forward_key, bfrt::BfRtTableKey *table_key) {
  // Set value into the key object. Key type is "EXACT"
  auto bf_status = table_key->setValue(forward_ip_dst_field_id, static_cast<uint64_t>(forward_key.ipDstAddr));
  assert(bf_status == BF_SUCCESS);
  return;
}

void state_stats_key_setup(const uint32_t &state_stats_key, bfrt::BfRtTableKey *table_key) {
  // Set value into the key object. Key type is "EXACT"
  auto bf_status = table_key->setValue(state_stats_counter_index_field_id, static_cast<uint64_t>(state_stats_key));
  assert(bf_status == BF_SUCCESS);
  return;
}

// This function sets the passed in "route" action data  into the
// data object associated with the ipRoute table
void ipRoute_data_setup_for_route(const IpRoute_routeData &ipRoute_data, bfrt::BfRtTableData *table_data) {
  // Set value into the data object
  auto bf_status = table_data->setValue(ipRoute_route_action_src_mac_field_id, ipRoute_data.srcMac);
  assert(bf_status == BF_SUCCESS);

  bf_status = table_data->setValue(ipRoute_route_action_dst_mac_field_id, ipRoute_data.dstMac);
  assert(bf_status == BF_SUCCESS);

  bf_status = table_data->setValue(ipRoute_route_action_port_field_id, static_cast<uint64_t>(ipRoute_data.dst_port));
  assert(bf_status == BF_SUCCESS);
  return;
}

// This functiona sets the passed in "nat" acton data into the
// data object associated with the ipRoute table and "nat" action within the
// ipRoute table
void ipRoute_data_setup_for_nat(const IpRoute_natData &ipRoute_data, bfrt::BfRtTableData *table_data) {
  // Set value into the data object
  auto bf_status = table_data->setValue(ipRoute_nat_action_ip_src_field_id, static_cast<uint64_t>(ipRoute_data.srcAddr));
  assert(bf_status == BF_SUCCESS);

  bf_status = table_data->setValue(ipRoute_nat_action_ip_dst_field_id, static_cast<uint64_t>(ipRoute_data.dstAddr));
  assert(bf_status == BF_SUCCESS);

  bf_status = table_data->setValue(ipRoute_nat_action_port_field_id, static_cast<uint64_t>(ipRoute_data.dst_port));
  assert(bf_status == BF_SUCCESS);
  return;
}

void forward_data_setup_for_hit(const uint16_t port, bfrt::BfRtTableData *table_data) {
  // Set value into the data object
  auto bf_status = table_data->setValue(forward_hit_action_port_field_id, static_cast<uint64_t>(port));
  assert(bf_status == BF_SUCCESS);
  return;
}

// This function adds or modifies an entry in the ipRoute table with "route"
// action. The workflow is similar for either table entry add or modify
void ipRoute_entry_add_modify_with_route(const IpRouteKey &ipRoute_key, const IpRoute_routeData &ipRoute_data, const bool &add) {
  // Adding a match entry with below mac Addr to be forwarded to the below port
  // Reset key and data before use
  ipRouteTable->keyReset(bfrtTableKey.get());
  ipRouteTable->dataReset(ipRoute_route_action_id, bfrtTableData.get());

  // Fill in the Key and Data object
  ipRoute_key_setup(ipRoute_key, bfrtTableKey.get());
  ipRoute_data_setup_for_route(ipRoute_data, bfrtTableData.get());

  // Call table entry add API, if the request is for an add, else call modify
  bf_status_t status = BF_SUCCESS;
  if (add) {
    status = ipRouteTable->tableEntryAdd(*session, dev_tgt, *bfrtTableKey, *bfrtTableData);
  } else {
    status = ipRouteTable->tableEntryMod(*session, dev_tgt, *bfrtTableKey, *bfrtTableData);
  }
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();
}

// This function adds or modifies an entry in the ipRoute table with "nat"
// action. The workflow is similar for either table entry add or modify
void ipRoute_entry_add_modify_with_nat(const IpRouteKey &ipRoute_key, const IpRoute_natData &ipRoute_data, const bool &add) {
  // Reset key and data before use
  ipRouteTable->keyReset(bfrtTableKey.get());
  ipRouteTable->dataReset(ipRoute_nat_action_id, bfrtTableData.get());

  ipRoute_key_setup(ipRoute_key, bfrtTableKey.get());
  ipRoute_data_setup_for_nat(ipRoute_data, bfrtTableData.get());

  // Call table entry add API, if the request is for an add, else call modify
  bf_status_t status = BF_SUCCESS;
  if (add) {
    status = ipRouteTable->tableEntryAdd(*session, dev_tgt, *bfrtTableKey, *bfrtTableData);
  } else {
    status = ipRouteTable->tableEntryMod(*session, dev_tgt, *bfrtTableKey, *bfrtTableData);
  }
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();
  return;
}

void forward_entry_add_modify_with_hit(const ForwardKey &forward_key, const uint16_t port, const bool &add) {
  // Adding a match entry with below mac Addr to be forwarded to the below port
  // Reset key and data before use
  forwardTable->keyReset(bfrtTableKey_.get());
  forwardTable->dataReset(forward_hit_action_id, bfrtTableData_.get());

  // Fill in the Key and Data object
  forward_key_setup(forward_key, bfrtTableKey_.get());
  forward_data_setup_for_hit(port, bfrtTableData_.get());

  // Call table entry add API, if the request is for an add, else call modify
  bf_status_t status = BF_SUCCESS;
  if (add) {
    status = forwardTable->tableEntryAdd(*session, dev_tgt, *bfrtTableKey_, *bfrtTableData_);
  } else {
    status = forwardTable->tableEntryMod(*session, dev_tgt, *bfrtTableKey_, *bfrtTableData_);
  }
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();
}

void forward_entry_add_modify_with_copy_to_cpu(const ForwardKey &forward_key, const bool &add) {
  // Adding a match entry with below mac Addr to be forwarded to the below port
  // Reset key and data before use
  forwardTable->keyReset(bfrtTableKey_.get());
  forwardTable->dataReset(forward_copy_to_cpu_action_id, bfrtTableData_.get());

  // Fill in the Key and Data object
  forward_key_setup(forward_key, bfrtTableKey_.get());

  // Call table entry add API, if the request is for an add, else call modify
  bf_status_t status = BF_SUCCESS;
  if (add) {
    status = forwardTable->tableEntryAdd(*session, dev_tgt, *bfrtTableKey_, *bfrtTableData_);
  } else {
    status = forwardTable->tableEntryMod(*session, dev_tgt, *bfrtTableKey_, *bfrtTableData_);
  }
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();
}

// This function process the entry obtained by a get call for a "route" action
// and populates the IpRoute_routeData structure
void ipRoute_process_route_entry_get(const bfrt::BfRtTableData &data, IpRoute_routeData *route_data) {
  bf_status_t status = BF_SUCCESS;

  status = data.getValue(ipRoute_route_action_src_mac_field_id, &route_data->srcMac);
  assert(status == BF_SUCCESS);

  status = data.getValue(ipRoute_route_action_dst_mac_field_id, &route_data->dstMac);
  assert(status == BF_SUCCESS);

  uint64_t port;
  status = data.getValue(ipRoute_route_action_port_field_id, &port);
  route_data->dst_port = static_cast<uint16_t>(port >> 48);
  assert(status == BF_SUCCESS);

  uint64_t counter_spec_bytes;
  status = data.getValue(ipRoute_router_action_counter_spec_bytes_field_id, &counter_spec_bytes);
  assert(status == BF_SUCCESS);

  uint64_t counter_spec_pkts;
  status = data.getValue(ipRoute_router_action_counter_spec_pkts_field_id, &counter_spec_pkts);
  assert(status == BF_SUCCESS);

  printf("in router counter_spec_bytes: %lu counter_spec_pkts: %lu\n",counter_spec_bytes,counter_spec_pkts);
  printf("route_data->srcMac: %lu route_data->dstMac: %lu route_data->dst_port: %u\n",route_data->srcMac,route_data->dstMac,route_data->dst_port);
  return;
}

// This function process the entry obtained by a get call for a "nat" action
// and populates the IpRoute_natData structure
void ipRoute_process_nat_entry_get(const bfrt::BfRtTableData &data, IpRoute_natData *nat_data) {
  bf_status_t status = BF_SUCCESS;

  uint64_t srcAddr;
  status = data.getValue(ipRoute_nat_action_ip_src_field_id, &srcAddr);
  assert(status == BF_SUCCESS);
  nat_data->srcAddr = static_cast<uint32_t>(srcAddr >> 32);

  uint64_t dstAddr;
  status = data.getValue(ipRoute_nat_action_ip_dst_field_id, &dstAddr);
  assert(status == BF_SUCCESS);
  nat_data->dstAddr = static_cast<uint32_t>(dstAddr >> 32);

  uint64_t dst_port;
  status = data.getValue(ipRoute_nat_action_port_field_id, &dst_port);
  assert(status == BF_SUCCESS);
  nat_data->dst_port = static_cast<uint16_t>(dst_port >> 48);

  uint64_t counter_spec_bytes;
  status = data.getValue(ipRoute_router_action_counter_spec_bytes_field_id, &counter_spec_bytes);
  assert(status == BF_SUCCESS);

  uint64_t counter_spec_pkts;
  status = data.getValue(ipRoute_router_action_counter_spec_pkts_field_id, &counter_spec_pkts);
  assert(status == BF_SUCCESS);

  printf("in nat counter_spec_bytes: %lu counter_spec_pkts: %lu\n",counter_spec_bytes,counter_spec_pkts);
  printf("nat_data->srcAddr: %u nat_data->dstAddr: %u nat_data->dst_port: %u\n",nat_data->srcAddr,nat_data->dstAddr,nat_data->dst_port);
  return;
}

void forward_process_hit_entry_get(const bfrt::BfRtTableData &data, uint16_t *port) {
  bf_status_t status = BF_SUCCESS;
  uint64_t port_;
  status = data.getValue(forward_hit_action_port_field_id, &port_);
  *port = static_cast<uint16_t>(port_ >> 48);
  assert(status == BF_SUCCESS);

  printf("port: %u\n",*port);
  return;
}

void state_stats_process_counter_get(const bfrt::BfRtTableData &data, uint64_t *counter) {
  bf_status_t status = BF_SUCCESS;
  uint64_t counter_;
  status = data.getValue(state_stats_counter_spec_pkts_field_id, &counter_);
  *counter = counter_;
  assert(status == BF_SUCCESS);

  printf("counter: %lu\n",*counter);
  return;
}

// This function processes the entry obtained by a get call. Based on the action
// id the data object is intepreted.
void ipRoute_process_entry_get(const bfrt::BfRtTableData &data, IpRouteData *ipRoute_data) {
  // First get actionId, then based on that, fill in appropriate fields
  bf_status_t bf_status;
  bf_rt_id_t action_id;

  bf_status = data.actionIdGet(&action_id);
  assert(bf_status == BF_SUCCESS);

  if (action_id == ipRoute_route_action_id) {
    ipRoute_process_route_entry_get(data, &ipRoute_data->data.route_data);
  } else if (action_id == ipRoute_nat_action_id) {
    ipRoute_process_nat_entry_get(data, &ipRoute_data->data.nat_data);
  }
  return;
}

void forward_process_entry_get(const bfrt::BfRtTableData &data, ForwardData *forward_data) {
  // First get actionId, then based on that, fill in appropriate fields
  bf_status_t bf_status;
  bf_rt_id_t action_id;

  bf_status = data.actionIdGet(&action_id);
  assert(bf_status == BF_SUCCESS);

  //printf("action_id,forward_hit_action_id, forward_copy_to_cpu_action_id: %d %d %d \n",action_id,forward_hit_action_id,forward_copy_to_cpu_action_id);

  if (action_id == forward_hit_action_id) {
    forward_process_hit_entry_get(data, &forward_data->hit_data);
  } else if (action_id == forward_copy_to_cpu_action_id) {
    printf("do copy_to_cpu action\n");
  } else {
    //forward_process_miss_entry_get(data, &forward_data->data.miss_data);
    printf("do default action,miss action\n");
  }
  return;
}

// This function reads an entry specified by the ipRoute_key, and fills in the
// passedin IpRoute object
void ipRoute_entry_get(const IpRouteKey &ipRoute_key, IpRouteData *data) {
  // Reset key and data before use
  ipRouteTable->keyReset(bfrtTableKey.get());
  // Data reset is done without action-id, since the action-id is filled in by
  // the get function
  ipRouteTable->dataReset(bfrtTableData.get());

  ipRoute_key_setup(ipRoute_key, bfrtTableKey.get());

  bf_status_t status = BF_SUCCESS;
  // Entry get from hardware with the flag set to read from hardware
  auto flag = bfrt::BfRtTable::BfRtTableGetFlag::GET_FROM_HW;
  status = ipRouteTable->tableEntryGet(*session, dev_tgt, *bfrtTableKey, flag, bfrtTableData.get());
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();

  ipRoute_process_entry_get(*bfrtTableData, data);

  return;
}

void forward_entry_get(const ForwardKey &forward_key, ForwardData *data) {
  // Reset key and data before use
  forwardTable->keyReset(bfrtTableKey_.get());
  // Data reset is done without action-id, since the action-id is filled in by
  // the get function
  forwardTable->dataReset(bfrtTableData_.get());

  forward_key_setup(forward_key, bfrtTableKey_.get());

  bf_status_t status = BF_SUCCESS;
  // Entry get from hardware with the flag set to read from hardware
  auto flag = bfrt::BfRtTable::BfRtTableGetFlag::GET_FROM_HW;
  status = forwardTable->tableEntryGet(*session, dev_tgt, *bfrtTableKey_, flag, bfrtTableData_.get());
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();

  forward_process_entry_get(*bfrtTableData_, data);

  return;
}

void state_stats_counter_get(const uint32_t &state_stats_key, uint64_t *data) {
  // Reset key and data before use
  state_statsTable->keyReset(bfrtTableKey1.get());
  // Data reset is done without action-id, since the action-id is filled in by
  // the get function
  state_statsTable->dataReset(bfrtTableData1.get());

  state_stats_key_setup(state_stats_key, bfrtTableKey1.get());

  bf_status_t status = BF_SUCCESS;
  // Entry get from hardware with the flag set to read from hardware
  auto flag = bfrt::BfRtTable::BfRtTableGetFlag::GET_FROM_HW;
  status = state_statsTable->tableEntryGet(*session, dev_tgt, *bfrtTableKey1, flag, bfrtTableData1.get());
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();

  state_stats_process_counter_get(*bfrtTableData1, data);

  return;
}


// This function deletes an entry specified by the ipRoute_key
void ipRoute_entry_delete(const IpRouteKey &ipRoute_key) {
  // Reset key before use
  ipRouteTable->keyReset(bfrtTableKey.get());

  ipRoute_key_setup(ipRoute_key, bfrtTableKey.get());

  auto status = ipRouteTable->tableEntryDel(*session, dev_tgt, *bfrtTableKey);
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();
  return;
}

void forward_entry_delete(const ForwardKey &forward_key) {
  // Reset key before use
  forwardTable->keyReset(bfrtTableKey_.get());

  forward_key_setup(forward_key, bfrtTableKey_.get());

  auto status = forwardTable->tableEntryDel(*session, dev_tgt, *bfrtTableKey_);
  assert(status == BF_SUCCESS);
  session->sessionCompleteOperations();
  return;
}

// Function to iterate over all the entries in the table
void table_iterate() {
  // Table iteration involves the following
  //    1. Use the getFirst API to get the first entry
  //    2. Use the tableUsageGet API to get the number of entries currently in
  //    the table.
  //    3. Use the number of entries returned in step 2 and pass it as a
  //    parameter to getNext_n (as n) to get all the remaining entries
  std::unique_ptr<BfRtTableKey> first_key;
  std::unique_ptr<BfRtTableData> first_data;

  auto bf_status = ipRouteTable->keyAllocate(&first_key);
  assert(bf_status == BF_SUCCESS);

  bf_status = ipRouteTable->dataAllocate(&first_data);
  assert(bf_status == BF_SUCCESS);

  auto flag = bfrt::BfRtTable::BfRtTableGetFlag::GET_FROM_HW;

  bf_status = ipRouteTable->tableEntryGetFirst(*session, dev_tgt, flag, first_key.get(), first_data.get());
  assert(bf_status == BF_SUCCESS);
  session->sessionCompleteOperations();

  // Process the first entry
  IpRouteData route_data;
  ipRoute_process_entry_get(*first_data, &route_data);

  // Get the usage of table
  uint32_t entry_count = 0;
  bf_status = ipRouteTable->tableUsageGet(*session, dev_tgt, flag, &entry_count);
  assert(bf_status == BF_SUCCESS);

  if (entry_count == 1) {
    return;
  }

  BfRtTable::keyDataPairs key_data_pairs;
  std::vector<std::unique_ptr<BfRtTableKey>> keys(entry_count - 1);
  std::vector<std::unique_ptr<BfRtTableData>> data(entry_count - 1);

  for (unsigned i = 0; i < entry_count - 1; ++i) {
    bf_status = ipRouteTable->keyAllocate(&keys[i]);
    assert(bf_status == BF_SUCCESS);

    bf_status = ipRouteTable->dataAllocate(&data[i]);
    assert(bf_status == BF_SUCCESS);

    key_data_pairs.push_back(std::make_pair(keys[i].get(), data[i].get()));
  }

  // Get next N
  uint32_t num_returned = 0;
  bf_status = ipRouteTable->tableEntryGetNext_n(*session, dev_tgt, *first_key.get(), entry_count - 1, flag, &key_data_pairs, &num_returned);
  assert(bf_status == BF_SUCCESS);
  assert(num_returned == entry_count - 1);
  session->sessionCompleteOperations();

  // Process the rest of the entries
  for (unsigned i = 0; i < entry_count - 1; ++i) {
    ipRoute_process_entry_get(*data[i], &route_data);
    // Do any required processing with the obtained data and key
  }
  return;
}

void forward_table_iterate() {
  // Table iteration involves the following
  //    1. Use the getFirst API to get the first entry
  //    2. Use the tableUsageGet API to get the number of entries currently in
  //    the table.
  //    3. Use the number of entries returned in step 2 and pass it as a
  //    parameter to getNext_n (as n) to get all the remaining entries
  std::unique_ptr<BfRtTableKey> first_key;
  std::unique_ptr<BfRtTableData> first_data;

  auto bf_status = forwardTable->keyAllocate(&first_key);
  assert(bf_status == BF_SUCCESS);

  bf_status = forwardTable->dataAllocate(&first_data);
  assert(bf_status == BF_SUCCESS);

  auto flag = bfrt::BfRtTable::BfRtTableGetFlag::GET_FROM_HW;

  bf_status = forwardTable->tableEntryGetFirst(*session, dev_tgt, flag, first_key.get(), first_data.get());
  assert(bf_status == BF_SUCCESS);
  session->sessionCompleteOperations();

  // Process the first entry
  ForwardData route_data;
  forward_process_entry_get(*first_data, &route_data);

  // Get the usage of table
  uint32_t entry_count = 0;
  bf_status = forwardTable->tableUsageGet(*session, dev_tgt, flag, &entry_count);
  assert(bf_status == BF_SUCCESS);

  if (entry_count == 1) {
    return;
  }

  BfRtTable::keyDataPairs key_data_pairs;
  std::vector<std::unique_ptr<BfRtTableKey>> keys(entry_count - 1);
  std::vector<std::unique_ptr<BfRtTableData>> data(entry_count - 1);

  for (unsigned i = 0; i < entry_count - 1; ++i) {
    bf_status = forwardTable->keyAllocate(&keys[i]);
    assert(bf_status == BF_SUCCESS);

    bf_status = forwardTable->dataAllocate(&data[i]);
    assert(bf_status == BF_SUCCESS);

    key_data_pairs.push_back(std::make_pair(keys[i].get(), data[i].get()));
  }

  // Get next N
  uint32_t num_returned = 0;
  bf_status = forwardTable->tableEntryGetNext_n(*session, dev_tgt, *first_key.get(), entry_count - 1, flag, &key_data_pairs, &num_returned);
  assert(bf_status == BF_SUCCESS);
  assert(num_returned == entry_count - 1);
  session->sessionCompleteOperations();

  // Process the rest of the entries
  for (unsigned i = 0; i < entry_count - 1; ++i) {
    forward_process_entry_get(*data[i], &route_data);
    // Do any required processing with the obtained data and key
  }
  return;
}
}  // tna_exact_match
}  // examples
}  // bfrt

#if 0
static void parse_options(bf_switchd_context_t *switchd_ctx,
                          int argc,
                          char **argv) {
  int option_index = 0;
  enum opts {
    OPT_INSTALLDIR = 1,
    OPT_CONFFILE,
  };
  static struct option options[] = {
      {"help", no_argument, 0, 'h'},
      {"install-dir", required_argument, 0, OPT_INSTALLDIR},
      {"conf-file", required_argument, 0, OPT_CONFFILE}};

  while (1) {
    int c = getopt_long(argc, argv, "h", options, &option_index);

    if (c == -1) {
      break;
    }
    switch (c) {
      case OPT_INSTALLDIR:
        switchd_ctx->install_dir = strdup(optarg);
        printf("Install Dir: %s\n", switchd_ctx->install_dir);
        break;
      case OPT_CONFFILE:
        switchd_ctx->conf_file = strdup(optarg);
        printf("Conf-file : %s\n", switchd_ctx->conf_file);
        break;
      case 'h':
      case '?':
        printf("tna_exact_match \n");
        printf(
            "Usage : tna_exact_match --install-dir <path to where the SDE is "
            "installed> --conf-file <full path to the conf file "
            "(tna_exact_match.conf)\n");
        exit(c == 'h' ? 0 : 1);
        break;
      default:
        printf("Invalid option\n");
        exit(0);
        break;
    }
  }
  if (switchd_ctx->install_dir == NULL) {
    printf("ERROR : --install-dir must be specified\n");
    exit(0);
  }

  if (switchd_ctx->conf_file == NULL) {
    printf("ERROR : --conf-file must be specified\n");
    exit(0);
  }
}


int main(int argc, char **argv) {
  bf_switchd_context_t *switchd_ctx;
  if ((switchd_ctx = (bf_switchd_context_t *)calloc(
           1, sizeof(bf_switchd_context_t))) == NULL) {
    printf("Cannot Allocate switchd context\n");
    exit(1);
  }
  parse_options(switchd_ctx, argc, argv);
  switchd_ctx->running_in_background = true;
  bf_status_t status = bf_switchd_lib_init(switchd_ctx);

  // Do initial set up
  bfrt::examples::tna_exact_match::setUp();
  // Do table level set up
  bfrt::examples::tna_exact_match::tableSetUp();
  bfrt::examples::tna_exact_match::table_iterate();
  
  return status;
}

#endif

int ipRoute_entry_add_or_modify_with_route(uint32_t ipDstAddr, uint16_t ingress_port, uint64_t srcMac, uint64_t dstMac, uint16_t dst_port, bool opflag)
{
    const bfrt::examples::tna_exact_match::IpRouteKey &ipRoute_key = {ipDstAddr, ingress_port};
    const bfrt::examples::tna_exact_match::IpRoute_routeData &ipRoute_routedata = {srcMac, dstMac, dst_port};
    const bool &add = opflag;

    //printf("%u %u %lu %lu %d \n",ipRoute_key.ipDstAddr,ipRoute_key.vrf,ipRoute_routedata.srcMac,ipRoute_routedata.dstMac,ipRoute_routedata.dst_port);
    bfrt::examples::tna_exact_match::ipRoute_entry_add_modify_with_route(ipRoute_key, ipRoute_routedata, add);

    return 0;
}

int ipRoute_entry_add_or_modify_with_nat(uint32_t ipDstAddr, uint16_t ingress_port, uint32_t srcAddr, uint32_t dstAddr, uint16_t dst_port, bool opflag)
{
    const bfrt::examples::tna_exact_match::IpRouteKey &ipRoute_key = {ipDstAddr, ingress_port};
    const bfrt::examples::tna_exact_match::IpRoute_natData &ipRoute_natdata = {srcAddr, dstAddr, dst_port};
    const bool &add = opflag;

    #if 0
    char buf[4] = {0};
	char buf1[8] = {0};
	int i;
    buf[0] = (ipRoute_natdata.srcAddr & 0xFF000000)%0x00FFFFFF;
	buf[1] = (ipRoute_natdata.srcAddr & 0x00FF0000)%0x0000FFFF;
	buf[2] = (ipRoute_natdata.srcAddr & 0x0000FF00)%0x000000FF;
	buf[3] = ipRoute_natdata.srcAddr & 0x000000FF;
	for(i=0;i<4;i++)
		printf("0x%x ",buf[i]);
	printf("%u\n",ipRoute_natdata.srcAddr);

	buf1[0] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0xFF00000000000000)%0x00FFFFFFFFFFFFFF;
	buf1[1] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x00FF000000000000)%0x0000FFFFFFFFFFFF;
	buf1[2] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x0000FF0000000000)%0x000000FFFFFFFFFF;
	buf1[3] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x000000FF00000000)%0x00000000FFFFFFFF;
	buf1[4] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x00000000FF000000)%0x0000000000FFFFFF;
	buf1[5] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x0000000000FF0000)%0x000000000000FFFF;
	buf1[6] = (static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x000000000000FF00)%0x00000000000000FF;
	buf1[7] = static_cast<uint64_t>(ipRoute_natdata.srcAddr) & 0x00000000000000FF;
	for(i=0;i<8;i++)
		printf("0x%x ",buf1[i]);
	printf("%lu\n",static_cast<uint64_t>(ipRoute_natdata.srcAddr));
    printf("%u %u %u %u %d \n",ipRoute_key.ipDstAddr,ipRoute_key.vrf,ipRoute_natdata.srcAddr,ipRoute_natdata.dstAddr,ipRoute_natdata.dst_port);
    #endif
    bfrt::examples::tna_exact_match::ipRoute_entry_add_modify_with_nat(ipRoute_key, ipRoute_natdata, add);

    return 0;
}

int ipRoute_entry_get(uint32_t ipDstAddr, uint16_t ingress_port)
{
    const bfrt::examples::tna_exact_match::IpRouteKey &ipRoute_key = {ipDstAddr, ingress_port};
	bfrt::examples::tna_exact_match::IpRouteData data;

    bfrt::examples::tna_exact_match::ipRoute_entry_get(ipRoute_key, &data);

    return 0;
}

int ipRoute_entry_delete(uint32_t ipDstAddr, uint16_t ingress_port)
{
    const bfrt::examples::tna_exact_match::IpRouteKey &ipRoute_key = {ipDstAddr, ingress_port};

    bfrt::examples::tna_exact_match::ipRoute_entry_delete(ipRoute_key);

    return 0;
}

int table_traversal()
{
    bfrt::examples::tna_exact_match::table_iterate();

	return 0;
}

int forward_entry_add_or_modify_with_hit(uint32_t ipDstAddr, uint16_t port, bool opflag)
{
	const bfrt::examples::tna_exact_match::ForwardKey &forward_key = {ipDstAddr};
    const uint16_t &port_= port;
    const bool &add = opflag;

    bfrt::examples::tna_exact_match::forward_entry_add_modify_with_hit(forward_key, port_, add);
	
    return 0;
}

int forward_entry_add_or_modify_with_copy_to_cpu(uint32_t ipDstAddr, bool opflag)
{
	const bfrt::examples::tna_exact_match::ForwardKey &forward_key = {ipDstAddr};
    const bool &add = opflag;

    bfrt::examples::tna_exact_match::forward_entry_add_modify_with_copy_to_cpu(forward_key, add);
	
    return 0;
}

int forward_entry_get(uint32_t ipDstAddr)
{
    const bfrt::examples::tna_exact_match::ForwardKey &forward_key = {ipDstAddr};
	bfrt::examples::tna_exact_match::ForwardData data;

    bfrt::examples::tna_exact_match::forward_entry_get(forward_key, &data);

    return 0;
}

int forward_entry_delete(uint32_t ipDstAddr)
{
    const bfrt::examples::tna_exact_match::ForwardKey &forward_key = {ipDstAddr};

    bfrt::examples::tna_exact_match::forward_entry_delete(forward_key);

    return 0;
}

int forward_table_traversal()
{
    bfrt::examples::tna_exact_match::forward_table_iterate();

	return 0;
}

int state_stats_counter_get(uint32_t port)
{
	const uint32_t &key = port;
	uint64_t counter;

    bfrt::examples::tna_exact_match::state_stats_counter_get(key, &counter);

    return 0;
}

int config(char *install_dir, char *conf_file)
{
    bf_switchd_context_t *switchd_ctx;
    if((switchd_ctx = (bf_switchd_context_t *)calloc(1, sizeof(bf_switchd_context_t))) == NULL)
    {
        printf("Cannot Allocate switchd context\n");
        exit(1);
    }
    switchd_ctx->install_dir = install_dir;
    switchd_ctx->conf_file = conf_file;
    switchd_ctx->running_in_background = true;
    bf_status_t status = bf_switchd_lib_init(switchd_ctx);

    // Do initial set up
    bfrt::examples::tna_exact_match::setUp();
    // Do table level set up
    bfrt::examples::tna_exact_match::tableSetUp();

    return status;
}

int bfrt_test()
{
    printf("bfrt_test setup \n");
    //bfrt::examples::tna_exact_match::setUp();	  
    //bfrt::examples::tna_exact_match::table_iterate();
    return 0;
}
