/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/service/load_stats/v3/lrs.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/service/load_stats/v3/lrs.upb.h"
#include "envoy/config/core/v3/base.upb.h"
#include "envoy/config/endpoint/v3/load_report.upb.h"
#include "google/protobuf/duration.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_service_load_stats_v3_LoadStatsRequest_submsgs[2] = {
  &envoy_config_core_v3_Node_msginit,
  &envoy_config_endpoint_v3_ClusterStats_msginit,
};

static const upb_msglayout_field envoy_service_load_stats_v3_LoadStatsRequest__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 3},
};

const upb_msglayout envoy_service_load_stats_v3_LoadStatsRequest_msginit = {
  &envoy_service_load_stats_v3_LoadStatsRequest_submsgs[0],
  &envoy_service_load_stats_v3_LoadStatsRequest__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_service_load_stats_v3_LoadStatsResponse_submsgs[1] = {
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_service_load_stats_v3_LoadStatsResponse__fields[4] = {
  {1, UPB_SIZE(8, 16), 0, 0, 9, 3},
  {2, UPB_SIZE(4, 8), 0, 0, 11, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 8, 1},
  {4, UPB_SIZE(1, 1), 0, 0, 8, 1},
};

const upb_msglayout envoy_service_load_stats_v3_LoadStatsResponse_msginit = {
  &envoy_service_load_stats_v3_LoadStatsResponse_submsgs[0],
  &envoy_service_load_stats_v3_LoadStatsResponse__fields[0],
  UPB_SIZE(12, 24), 4, false,
};

#include "upb/port_undef.inc"

