/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/endpoint/load_report.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/endpoint/load_report.upb.h"
#include "envoy/api/v2/core/address.upb.h"
#include "envoy/api/v2/core/base.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/struct.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_endpoint_UpstreamLocalityStats_submsgs[3] = {
  &envoy_api_v2_core_Locality_msginit,
  &envoy_api_v2_endpoint_EndpointLoadMetricStats_msginit,
  &envoy_api_v2_endpoint_UpstreamEndpointStats_msginit,
};

static const upb_msglayout_field envoy_api_v2_endpoint_UpstreamLocalityStats__fields[8] = {
  {1, UPB_SIZE(36, 40), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 4, 1},
  {3, UPB_SIZE(8, 8), 0, 0, 4, 1},
  {4, UPB_SIZE(16, 16), 0, 0, 4, 1},
  {5, UPB_SIZE(40, 48), 0, 1, 11, 3},
  {6, UPB_SIZE(32, 32), 0, 0, 13, 1},
  {7, UPB_SIZE(44, 56), 0, 2, 11, 3},
  {8, UPB_SIZE(24, 24), 0, 0, 4, 1},
};

const upb_msglayout envoy_api_v2_endpoint_UpstreamLocalityStats_msginit = {
  &envoy_api_v2_endpoint_UpstreamLocalityStats_submsgs[0],
  &envoy_api_v2_endpoint_UpstreamLocalityStats__fields[0],
  UPB_SIZE(48, 64), 8, false,
};

static const upb_msglayout *const envoy_api_v2_endpoint_UpstreamEndpointStats_submsgs[3] = {
  &envoy_api_v2_core_Address_msginit,
  &envoy_api_v2_endpoint_EndpointLoadMetricStats_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_endpoint_UpstreamEndpointStats__fields[7] = {
  {1, UPB_SIZE(32, 32), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 4, 1},
  {3, UPB_SIZE(8, 8), 0, 0, 4, 1},
  {4, UPB_SIZE(16, 16), 0, 0, 4, 1},
  {5, UPB_SIZE(40, 48), 0, 1, 11, 3},
  {6, UPB_SIZE(36, 40), 0, 2, 11, 1},
  {7, UPB_SIZE(24, 24), 0, 0, 4, 1},
};

const upb_msglayout envoy_api_v2_endpoint_UpstreamEndpointStats_msginit = {
  &envoy_api_v2_endpoint_UpstreamEndpointStats_submsgs[0],
  &envoy_api_v2_endpoint_UpstreamEndpointStats__fields[0],
  UPB_SIZE(48, 56), 7, false,
};

static const upb_msglayout_field envoy_api_v2_endpoint_EndpointLoadMetricStats__fields[3] = {
  {1, UPB_SIZE(16, 16), 0, 0, 9, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 4, 1},
  {3, UPB_SIZE(8, 8), 0, 0, 1, 1},
};

const upb_msglayout envoy_api_v2_endpoint_EndpointLoadMetricStats_msginit = {
  NULL,
  &envoy_api_v2_endpoint_EndpointLoadMetricStats__fields[0],
  UPB_SIZE(24, 32), 3, false,
};

static const upb_msglayout *const envoy_api_v2_endpoint_ClusterStats_submsgs[3] = {
  &envoy_api_v2_endpoint_ClusterStats_DroppedRequests_msginit,
  &envoy_api_v2_endpoint_UpstreamLocalityStats_msginit,
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_api_v2_endpoint_ClusterStats__fields[6] = {
  {1, UPB_SIZE(8, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(28, 48), 0, 1, 11, 3},
  {3, UPB_SIZE(0, 0), 0, 0, 4, 1},
  {4, UPB_SIZE(24, 40), 0, 2, 11, 1},
  {5, UPB_SIZE(32, 56), 0, 0, 11, 3},
  {6, UPB_SIZE(16, 24), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_endpoint_ClusterStats_msginit = {
  &envoy_api_v2_endpoint_ClusterStats_submsgs[0],
  &envoy_api_v2_endpoint_ClusterStats__fields[0],
  UPB_SIZE(40, 64), 6, false,
};

static const upb_msglayout_field envoy_api_v2_endpoint_ClusterStats_DroppedRequests__fields[2] = {
  {1, UPB_SIZE(8, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 4, 1},
};

const upb_msglayout envoy_api_v2_endpoint_ClusterStats_DroppedRequests_msginit = {
  NULL,
  &envoy_api_v2_endpoint_ClusterStats_DroppedRequests__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

#include "upb/port_undef.inc"

