/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/core/health_check.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/core/health_check.upb.h"
#include "envoy/api/v2/core/base.upb.h"
#include "envoy/type/http.upb.h"
#include "envoy/type/range.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/struct.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_core_HealthCheck_submsgs[16] = {
  &envoy_api_v2_core_HealthCheck_CustomHealthCheck_msginit,
  &envoy_api_v2_core_HealthCheck_GrpcHealthCheck_msginit,
  &envoy_api_v2_core_HealthCheck_HttpHealthCheck_msginit,
  &envoy_api_v2_core_HealthCheck_TcpHealthCheck_msginit,
  &google_protobuf_BoolValue_msginit,
  &google_protobuf_Duration_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck__fields[19] = {
  {1, UPB_SIZE(16, 24), 0, 5, 11, 1},
  {2, UPB_SIZE(20, 32), 0, 5, 11, 1},
  {3, UPB_SIZE(24, 40), 0, 5, 11, 1},
  {4, UPB_SIZE(28, 48), 0, 6, 11, 1},
  {5, UPB_SIZE(32, 56), 0, 6, 11, 1},
  {6, UPB_SIZE(36, 64), 0, 6, 11, 1},
  {7, UPB_SIZE(40, 72), 0, 4, 11, 1},
  {8, UPB_SIZE(64, 120), UPB_SIZE(-69, -129), 2, 11, 1},
  {9, UPB_SIZE(64, 120), UPB_SIZE(-69, -129), 3, 11, 1},
  {11, UPB_SIZE(64, 120), UPB_SIZE(-69, -129), 1, 11, 1},
  {12, UPB_SIZE(44, 80), 0, 5, 11, 1},
  {13, UPB_SIZE(64, 120), UPB_SIZE(-69, -129), 0, 11, 1},
  {14, UPB_SIZE(48, 88), 0, 5, 11, 1},
  {15, UPB_SIZE(52, 96), 0, 5, 11, 1},
  {16, UPB_SIZE(56, 104), 0, 5, 11, 1},
  {17, UPB_SIZE(8, 8), 0, 0, 9, 1},
  {18, UPB_SIZE(0, 0), 0, 0, 13, 1},
  {19, UPB_SIZE(4, 4), 0, 0, 8, 1},
  {20, UPB_SIZE(60, 112), 0, 5, 11, 1},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_msginit = {
  &envoy_api_v2_core_HealthCheck_submsgs[0],
  &envoy_api_v2_core_HealthCheck__fields[0],
  UPB_SIZE(72, 144), 19, false,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck_Payload__fields[2] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 12, 1},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_Payload_msginit = {
  NULL,
  &envoy_api_v2_core_HealthCheck_Payload__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_HealthCheck_HttpHealthCheck_submsgs[4] = {
  &envoy_api_v2_core_HeaderValueOption_msginit,
  &envoy_api_v2_core_HealthCheck_Payload_msginit,
  &envoy_type_Int64Range_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck_HttpHealthCheck__fields[10] = {
  {1, UPB_SIZE(12, 16), 0, 0, 9, 1},
  {2, UPB_SIZE(20, 32), 0, 0, 9, 1},
  {3, UPB_SIZE(36, 64), 0, 1, 11, 1},
  {4, UPB_SIZE(40, 72), 0, 1, 11, 1},
  {5, UPB_SIZE(28, 48), 0, 0, 9, 1},
  {6, UPB_SIZE(44, 80), 0, 0, 11, 3},
  {7, UPB_SIZE(8, 8), 0, 0, 8, 1},
  {8, UPB_SIZE(48, 88), 0, 0, 9, 3},
  {9, UPB_SIZE(52, 96), 0, 2, 11, 3},
  {10, UPB_SIZE(0, 0), 0, 0, 14, 1},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_HttpHealthCheck_msginit = {
  &envoy_api_v2_core_HealthCheck_HttpHealthCheck_submsgs[0],
  &envoy_api_v2_core_HealthCheck_HttpHealthCheck__fields[0],
  UPB_SIZE(56, 112), 10, false,
};

static const upb_msglayout *const envoy_api_v2_core_HealthCheck_TcpHealthCheck_submsgs[2] = {
  &envoy_api_v2_core_HealthCheck_Payload_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck_TcpHealthCheck__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 11, 3},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_TcpHealthCheck_msginit = {
  &envoy_api_v2_core_HealthCheck_TcpHealthCheck_submsgs[0],
  &envoy_api_v2_core_HealthCheck_TcpHealthCheck__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck_RedisHealthCheck__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_RedisHealthCheck_msginit = {
  NULL,
  &envoy_api_v2_core_HealthCheck_RedisHealthCheck__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck_GrpcHealthCheck__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_GrpcHealthCheck_msginit = {
  NULL,
  &envoy_api_v2_core_HealthCheck_GrpcHealthCheck__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_HealthCheck_CustomHealthCheck_submsgs[2] = {
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HealthCheck_CustomHealthCheck__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 1, 11, 1},
  {3, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_HealthCheck_CustomHealthCheck_msginit = {
  &envoy_api_v2_core_HealthCheck_CustomHealthCheck_submsgs[0],
  &envoy_api_v2_core_HealthCheck_CustomHealthCheck__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

#include "upb/port_undef.inc"

