/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/endpoint.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/endpoint.upb.h"
#include "envoy/api/v2/endpoint/endpoint_components.upb.h"
#include "envoy/type/percent.upb.h"
#include "google/api/annotations.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_ClusterLoadAssignment_submsgs[3] = {
  &envoy_api_v2_ClusterLoadAssignment_NamedEndpointsEntry_msginit,
  &envoy_api_v2_ClusterLoadAssignment_Policy_msginit,
  &envoy_api_v2_endpoint_LocalityLbEndpoints_msginit,
};

static const upb_msglayout_field envoy_api_v2_ClusterLoadAssignment__fields[4] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(12, 24), 0, 2, 11, 3},
  {4, UPB_SIZE(8, 16), 0, 1, 11, 1},
  {5, UPB_SIZE(16, 32), 0, 0, 11, 3},
};

const upb_msglayout envoy_api_v2_ClusterLoadAssignment_msginit = {
  &envoy_api_v2_ClusterLoadAssignment_submsgs[0],
  &envoy_api_v2_ClusterLoadAssignment__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

static const upb_msglayout *const envoy_api_v2_ClusterLoadAssignment_Policy_submsgs[3] = {
  &envoy_api_v2_ClusterLoadAssignment_Policy_DropOverload_msginit,
  &google_protobuf_Duration_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_ClusterLoadAssignment_Policy__fields[4] = {
  {2, UPB_SIZE(12, 24), 0, 0, 11, 3},
  {3, UPB_SIZE(4, 8), 0, 2, 11, 1},
  {4, UPB_SIZE(8, 16), 0, 1, 11, 1},
  {5, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout envoy_api_v2_ClusterLoadAssignment_Policy_msginit = {
  &envoy_api_v2_ClusterLoadAssignment_Policy_submsgs[0],
  &envoy_api_v2_ClusterLoadAssignment_Policy__fields[0],
  UPB_SIZE(16, 32), 4, false,
};

static const upb_msglayout *const envoy_api_v2_ClusterLoadAssignment_Policy_DropOverload_submsgs[1] = {
  &envoy_type_FractionalPercent_msginit,
};

static const upb_msglayout_field envoy_api_v2_ClusterLoadAssignment_Policy_DropOverload__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_ClusterLoadAssignment_Policy_DropOverload_msginit = {
  &envoy_api_v2_ClusterLoadAssignment_Policy_DropOverload_submsgs[0],
  &envoy_api_v2_ClusterLoadAssignment_Policy_DropOverload__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_api_v2_ClusterLoadAssignment_NamedEndpointsEntry_submsgs[1] = {
  &envoy_api_v2_endpoint_Endpoint_msginit,
};

static const upb_msglayout_field envoy_api_v2_ClusterLoadAssignment_NamedEndpointsEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_ClusterLoadAssignment_NamedEndpointsEntry_msginit = {
  &envoy_api_v2_ClusterLoadAssignment_NamedEndpointsEntry_submsgs[0],
  &envoy_api_v2_ClusterLoadAssignment_NamedEndpointsEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

#include "upb/port_undef.inc"

