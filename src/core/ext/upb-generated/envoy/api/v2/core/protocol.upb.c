/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/core/protocol.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/core/protocol.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

const upb_msglayout envoy_api_v2_core_TcpProtocolOptions_msginit = {
  NULL,
  NULL,
  UPB_SIZE(0, 0), 0, false,
};

static const upb_msglayout_field envoy_api_v2_core_UpstreamHttpProtocolOptions__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 8, 1},
  {2, UPB_SIZE(1, 1), 0, 0, 8, 1},
};

const upb_msglayout envoy_api_v2_core_UpstreamHttpProtocolOptions_msginit = {
  NULL,
  &envoy_api_v2_core_UpstreamHttpProtocolOptions__fields[0],
  UPB_SIZE(2, 2), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_HttpProtocolOptions_submsgs[3] = {
  &google_protobuf_Duration_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HttpProtocolOptions__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
  {3, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_HttpProtocolOptions_msginit = {
  &envoy_api_v2_core_HttpProtocolOptions_submsgs[0],
  &envoy_api_v2_core_HttpProtocolOptions__fields[0],
  UPB_SIZE(12, 24), 3, false,
};

static const upb_msglayout *const envoy_api_v2_core_Http1ProtocolOptions_submsgs[2] = {
  &envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat_msginit,
  &google_protobuf_BoolValue_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Http1ProtocolOptions__fields[5] = {
  {1, UPB_SIZE(12, 24), 0, 1, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 8, 1},
  {3, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {4, UPB_SIZE(16, 32), 0, 0, 11, 1},
  {5, UPB_SIZE(1, 1), 0, 0, 8, 1},
};

const upb_msglayout envoy_api_v2_core_Http1ProtocolOptions_msginit = {
  &envoy_api_v2_core_Http1ProtocolOptions_submsgs[0],
  &envoy_api_v2_core_Http1ProtocolOptions__fields[0],
  UPB_SIZE(24, 48), 5, false,
};

static const upb_msglayout *const envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat_submsgs[1] = {
  &envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat_ProperCaseWords_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat__fields[1] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat_msginit = {
  &envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat_submsgs[0],
  &envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

const upb_msglayout envoy_api_v2_core_Http1ProtocolOptions_HeaderKeyFormat_ProperCaseWords_msginit = {
  NULL,
  NULL,
  UPB_SIZE(0, 0), 0, false,
};

static const upb_msglayout *const envoy_api_v2_core_Http2ProtocolOptions_submsgs[9] = {
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Http2ProtocolOptions__fields[12] = {
  {1, UPB_SIZE(4, 8), 0, 0, 11, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {3, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {4, UPB_SIZE(16, 32), 0, 0, 11, 1},
  {5, UPB_SIZE(0, 0), 0, 0, 8, 1},
  {6, UPB_SIZE(1, 1), 0, 0, 8, 1},
  {7, UPB_SIZE(20, 40), 0, 0, 11, 1},
  {8, UPB_SIZE(24, 48), 0, 0, 11, 1},
  {9, UPB_SIZE(28, 56), 0, 0, 11, 1},
  {10, UPB_SIZE(32, 64), 0, 0, 11, 1},
  {11, UPB_SIZE(36, 72), 0, 0, 11, 1},
  {12, UPB_SIZE(2, 2), 0, 0, 8, 1},
};

const upb_msglayout envoy_api_v2_core_Http2ProtocolOptions_msginit = {
  &envoy_api_v2_core_Http2ProtocolOptions_submsgs[0],
  &envoy_api_v2_core_Http2ProtocolOptions__fields[0],
  UPB_SIZE(40, 80), 12, false,
};

static const upb_msglayout *const envoy_api_v2_core_GrpcProtocolOptions_submsgs[1] = {
  &envoy_api_v2_core_Http2ProtocolOptions_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcProtocolOptions__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcProtocolOptions_msginit = {
  &envoy_api_v2_core_GrpcProtocolOptions_submsgs[0],
  &envoy_api_v2_core_GrpcProtocolOptions__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

#include "upb/port_undef.inc"

