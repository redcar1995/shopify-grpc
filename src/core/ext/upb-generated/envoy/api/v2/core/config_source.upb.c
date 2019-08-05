/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/core/config_source.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/core/config_source.upb.h"
#include "envoy/api/v2/core/grpc_service.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "validate/validate.upb.h"
#include "gogoproto/gogo.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_core_ApiConfigSource_submsgs[4] = {
  &envoy_api_v2_core_GrpcService_msginit,
  &envoy_api_v2_core_RateLimitSettings_msginit,
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_ApiConfigSource__fields[6] = {
  {1, UPB_SIZE(0, 0), 0, 0, 14, 1},
  {2, UPB_SIZE(20, 32), 0, 0, 9, 3},
  {3, UPB_SIZE(8, 8), 0, 2, 11, 1},
  {4, UPB_SIZE(24, 40), 0, 0, 11, 3},
  {5, UPB_SIZE(12, 16), 0, 2, 11, 1},
  {6, UPB_SIZE(16, 24), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_core_ApiConfigSource_msginit = {
  &envoy_api_v2_core_ApiConfigSource_submsgs[0],
  &envoy_api_v2_core_ApiConfigSource__fields[0],
  UPB_SIZE(32, 48), 6, false,
};

const upb_msglayout envoy_api_v2_core_AggregatedConfigSource_msginit = {
  NULL,
  NULL,
  UPB_SIZE(0, 0), 0, false,
};

static const upb_msglayout *const envoy_api_v2_core_RateLimitSettings_submsgs[2] = {
  &google_protobuf_DoubleValue_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_RateLimitSettings__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 1, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_RateLimitSettings_msginit = {
  &envoy_api_v2_core_RateLimitSettings_submsgs[0],
  &envoy_api_v2_core_RateLimitSettings__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_ConfigSource_submsgs[3] = {
  &envoy_api_v2_core_AggregatedConfigSource_msginit,
  &envoy_api_v2_core_ApiConfigSource_msginit,
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_ConfigSource__fields[4] = {
  {1, UPB_SIZE(4, 8), UPB_SIZE(-13, -25), 0, 9, 1},
  {2, UPB_SIZE(4, 8), UPB_SIZE(-13, -25), 1, 11, 1},
  {3, UPB_SIZE(4, 8), UPB_SIZE(-13, -25), 0, 11, 1},
  {4, UPB_SIZE(0, 0), 0, 2, 11, 1},
};

const upb_msglayout envoy_api_v2_core_ConfigSource_msginit = {
  &envoy_api_v2_core_ConfigSource_submsgs[0],
  &envoy_api_v2_core_ConfigSource__fields[0],
  UPB_SIZE(16, 32), 4, false,
};

#include "upb/port_undef.inc"

