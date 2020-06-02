/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/core/grpc_service.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/core/grpc_service.upb.h"
#include "envoy/api/v2/core/base.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/empty.upb.h"
#include "google/protobuf/struct.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/sensitive.upb.h"
#include "udpa/annotations/status.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_core_GrpcService_submsgs[4] = {
  &envoy_api_v2_core_GrpcService_EnvoyGrpc_msginit,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_msginit,
  &envoy_api_v2_core_HeaderValue_msginit,
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService__fields[4] = {
  {1, UPB_SIZE(8, 16), UPB_SIZE(-12, -24), 0, 11, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-12, -24), 1, 11, 1},
  {3, UPB_SIZE(0, 0), 0, 3, 11, 1},
  {5, UPB_SIZE(4, 8), 0, 2, 11, 3},
};

const upb_msglayout envoy_api_v2_core_GrpcService_msginit = {
  &envoy_api_v2_core_GrpcService_submsgs[0],
  &envoy_api_v2_core_GrpcService__fields[0],
  UPB_SIZE(16, 32), 4, false,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_EnvoyGrpc__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_EnvoyGrpc_msginit = {
  NULL,
  &envoy_api_v2_core_GrpcService_EnvoyGrpc__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

static const upb_msglayout *const envoy_api_v2_core_GrpcService_GoogleGrpc_submsgs[3] = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_msginit,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_ChannelCredentials_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc__fields[6] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(24, 48), 0, 1, 11, 1},
  {3, UPB_SIZE(32, 64), 0, 0, 11, 3},
  {4, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {5, UPB_SIZE(16, 32), 0, 0, 9, 1},
  {6, UPB_SIZE(28, 56), 0, 2, 11, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_msginit = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_submsgs[0],
  &envoy_api_v2_core_GrpcService_GoogleGrpc__fields[0],
  UPB_SIZE(40, 80), 6, false,
};

static const upb_msglayout *const envoy_api_v2_core_GrpcService_GoogleGrpc_SslCredentials_submsgs[3] = {
  &envoy_api_v2_core_DataSource_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_SslCredentials__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 11, 1},
  {3, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_SslCredentials_msginit = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_SslCredentials_submsgs[0],
  &envoy_api_v2_core_GrpcService_GoogleGrpc_SslCredentials__fields[0],
  UPB_SIZE(12, 24), 3, false,
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_GoogleLocalCredentials_msginit = {
  NULL,
  NULL,
  UPB_SIZE(0, 0), 0, false,
};

static const upb_msglayout *const envoy_api_v2_core_GrpcService_GoogleGrpc_ChannelCredentials_submsgs[3] = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_GoogleLocalCredentials_msginit,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_SslCredentials_msginit,
  &google_protobuf_Empty_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_ChannelCredentials__fields[3] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-4, -8), 1, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-4, -8), 2, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-4, -8), 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_ChannelCredentials_msginit = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_ChannelCredentials_submsgs[0],
  &envoy_api_v2_core_GrpcService_GoogleGrpc_ChannelCredentials__fields[0],
  UPB_SIZE(8, 16), 3, false,
};

static const upb_msglayout *const envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_submsgs[5] = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_GoogleIAMCredentials_msginit,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_MetadataCredentialsFromPlugin_msginit,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_ServiceAccountJWTAccessCredentials_msginit,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_StsService_msginit,
  &google_protobuf_Empty_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials__fields[7] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 0, 9, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 4, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 0, 9, 1},
  {4, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 2, 11, 1},
  {5, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 0, 11, 1},
  {6, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 1, 11, 1},
  {7, UPB_SIZE(0, 0), UPB_SIZE(-8, -16), 3, 11, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_msginit = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_submsgs[0],
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials__fields[0],
  UPB_SIZE(16, 32), 7, false,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_ServiceAccountJWTAccessCredentials__fields[2] = {
  {1, UPB_SIZE(8, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 4, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_ServiceAccountJWTAccessCredentials_msginit = {
  NULL,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_ServiceAccountJWTAccessCredentials__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_GoogleIAMCredentials__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_GoogleIAMCredentials_msginit = {
  NULL,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_GoogleIAMCredentials__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_MetadataCredentialsFromPlugin_submsgs[2] = {
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_MetadataCredentialsFromPlugin__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-12, -24), 1, 11, 1},
  {3, UPB_SIZE(8, 16), UPB_SIZE(-12, -24), 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_MetadataCredentialsFromPlugin_msginit = {
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_MetadataCredentialsFromPlugin_submsgs[0],
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_MetadataCredentialsFromPlugin__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout_field envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_StsService__fields[9] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(16, 32), 0, 0, 9, 1},
  {4, UPB_SIZE(24, 48), 0, 0, 9, 1},
  {5, UPB_SIZE(32, 64), 0, 0, 9, 1},
  {6, UPB_SIZE(40, 80), 0, 0, 9, 1},
  {7, UPB_SIZE(48, 96), 0, 0, 9, 1},
  {8, UPB_SIZE(56, 112), 0, 0, 9, 1},
  {9, UPB_SIZE(64, 128), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_StsService_msginit = {
  NULL,
  &envoy_api_v2_core_GrpcService_GoogleGrpc_CallCredentials_StsService__fields[0],
  UPB_SIZE(72, 144), 9, false,
};

#include "upb/port_undef.inc"

