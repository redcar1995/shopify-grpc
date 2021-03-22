/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/service/status/v3/csds.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/service/status/v3/csds.upb.h"
#include "envoy/admin/v3/config_dump.upb.h"
#include "envoy/config/core/v3/base.upb.h"
#include "envoy/type/matcher/v3/node.upb.h"
#include "google/api/annotations.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_service_status_v3_ClientStatusRequest_submsgs[2] = {
  &envoy_config_core_v3_Node_msginit,
  &envoy_type_matcher_v3_NodeMatcher_msginit,
};

static const upb_msglayout_field envoy_service_status_v3_ClientStatusRequest__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 1, 11, 3},
  {2, UPB_SIZE(4, 8), 1, 0, 11, 1},
};

const upb_msglayout envoy_service_status_v3_ClientStatusRequest_msginit = {
  &envoy_service_status_v3_ClientStatusRequest_submsgs[0],
  &envoy_service_status_v3_ClientStatusRequest__fields[0],
  UPB_SIZE(16, 24), 2, false, 255,
};

static const upb_msglayout *const envoy_service_status_v3_PerXdsConfig_submsgs[5] = {
  &envoy_admin_v3_ClustersConfigDump_msginit,
  &envoy_admin_v3_EndpointsConfigDump_msginit,
  &envoy_admin_v3_ListenersConfigDump_msginit,
  &envoy_admin_v3_RoutesConfigDump_msginit,
  &envoy_admin_v3_ScopedRoutesConfigDump_msginit,
};

static const upb_msglayout_field envoy_service_status_v3_PerXdsConfig__fields[7] = {
  {1, UPB_SIZE(0, 0), 0, 0, 14, 1},
  {2, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 2, 11, 1},
  {3, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 0, 11, 1},
  {4, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 3, 11, 1},
  {5, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 4, 11, 1},
  {6, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 1, 11, 1},
  {7, UPB_SIZE(4, 4), 0, 0, 14, 1},
};

const upb_msglayout envoy_service_status_v3_PerXdsConfig_msginit = {
  &envoy_service_status_v3_PerXdsConfig_submsgs[0],
  &envoy_service_status_v3_PerXdsConfig__fields[0],
  UPB_SIZE(16, 24), 7, false, 255,
};

static const upb_msglayout *const envoy_service_status_v3_ClientConfig_submsgs[2] = {
  &envoy_config_core_v3_Node_msginit,
  &envoy_service_status_v3_PerXdsConfig_msginit,
};

static const upb_msglayout_field envoy_service_status_v3_ClientConfig__fields[2] = {
  {1, UPB_SIZE(4, 8), 1, 0, 11, 1},
  {2, UPB_SIZE(8, 16), 0, 1, 11, 3},
};

const upb_msglayout envoy_service_status_v3_ClientConfig_msginit = {
  &envoy_service_status_v3_ClientConfig_submsgs[0],
  &envoy_service_status_v3_ClientConfig__fields[0],
  UPB_SIZE(16, 24), 2, false, 255,
};

static const upb_msglayout *const envoy_service_status_v3_ClientStatusResponse_submsgs[1] = {
  &envoy_service_status_v3_ClientConfig_msginit,
};

static const upb_msglayout_field envoy_service_status_v3_ClientStatusResponse__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_service_status_v3_ClientStatusResponse_msginit = {
  &envoy_service_status_v3_ClientStatusResponse_submsgs[0],
  &envoy_service_status_v3_ClientStatusResponse__fields[0],
  UPB_SIZE(8, 8), 1, false, 255,
};

#include "upb/port_undef.inc"

