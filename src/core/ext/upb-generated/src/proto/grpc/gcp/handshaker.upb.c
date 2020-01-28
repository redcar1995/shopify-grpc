/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     src/proto/grpc/gcp/handshaker.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "src/proto/grpc/gcp/handshaker.upb.h"
#include "src/proto/grpc/gcp/transport_security_common.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout_field grpc_gcp_Endpoint__fields[3] = {
  {1, UPB_SIZE(12, 16), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 5, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 14, 1},
};

const upb_msglayout grpc_gcp_Endpoint_msginit = {
  NULL,
  &grpc_gcp_Endpoint__fields[0],
  UPB_SIZE(24, 32), 3, false,
};

static const upb_msglayout *const grpc_gcp_Identity_submsgs[1] = {
  &grpc_gcp_Identity_AttributesEntry_msginit,
};

static const upb_msglayout_field grpc_gcp_Identity__fields[3] = {
  {1, UPB_SIZE(4, 8), UPB_SIZE(-13, -25), 0, 9, 1},
  {2, UPB_SIZE(4, 8), UPB_SIZE(-13, -25), 0, 9, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout grpc_gcp_Identity_msginit = {
  &grpc_gcp_Identity_submsgs[0],
  &grpc_gcp_Identity__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout_field grpc_gcp_Identity_AttributesEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout grpc_gcp_Identity_AttributesEntry_msginit = {
  NULL,
  &grpc_gcp_Identity_AttributesEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const grpc_gcp_StartClientHandshakeReq_submsgs[5] = {
  &grpc_gcp_Endpoint_msginit,
  &grpc_gcp_Identity_msginit,
  &grpc_gcp_RpcProtocolVersions_msginit,
};

static const upb_msglayout_field grpc_gcp_StartClientHandshakeReq__fields[10] = {
  {1, UPB_SIZE(0, 0), 0, 0, 14, 1},
  {2, UPB_SIZE(36, 64), 0, 0, 9, 3},
  {3, UPB_SIZE(40, 72), 0, 0, 9, 3},
  {4, UPB_SIZE(44, 80), 0, 1, 11, 3},
  {5, UPB_SIZE(20, 32), 0, 1, 11, 1},
  {6, UPB_SIZE(24, 40), 0, 0, 11, 1},
  {7, UPB_SIZE(28, 48), 0, 0, 11, 1},
  {8, UPB_SIZE(12, 16), 0, 0, 9, 1},
  {9, UPB_SIZE(32, 56), 0, 2, 11, 1},
  {10, UPB_SIZE(8, 8), 0, 0, 13, 1},
};

const upb_msglayout grpc_gcp_StartClientHandshakeReq_msginit = {
  &grpc_gcp_StartClientHandshakeReq_submsgs[0],
  &grpc_gcp_StartClientHandshakeReq__fields[0],
  UPB_SIZE(48, 96), 10, false,
};

static const upb_msglayout *const grpc_gcp_ServerHandshakeParameters_submsgs[1] = {
  &grpc_gcp_Identity_msginit,
};

static const upb_msglayout_field grpc_gcp_ServerHandshakeParameters__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 3},
  {2, UPB_SIZE(4, 8), 0, 0, 11, 3},
};

const upb_msglayout grpc_gcp_ServerHandshakeParameters_msginit = {
  &grpc_gcp_ServerHandshakeParameters_submsgs[0],
  &grpc_gcp_ServerHandshakeParameters__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const grpc_gcp_StartServerHandshakeReq_submsgs[4] = {
  &grpc_gcp_Endpoint_msginit,
  &grpc_gcp_RpcProtocolVersions_msginit,
  &grpc_gcp_StartServerHandshakeReq_HandshakeParametersEntry_msginit,
};

static const upb_msglayout_field grpc_gcp_StartServerHandshakeReq__fields[7] = {
  {1, UPB_SIZE(24, 48), 0, 0, 9, 3},
  {2, UPB_SIZE(28, 56), 0, 2, 11, 3},
  {3, UPB_SIZE(4, 8), 0, 0, 12, 1},
  {4, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {5, UPB_SIZE(16, 32), 0, 0, 11, 1},
  {6, UPB_SIZE(20, 40), 0, 1, 11, 1},
  {7, UPB_SIZE(0, 0), 0, 0, 13, 1},
};

const upb_msglayout grpc_gcp_StartServerHandshakeReq_msginit = {
  &grpc_gcp_StartServerHandshakeReq_submsgs[0],
  &grpc_gcp_StartServerHandshakeReq__fields[0],
  UPB_SIZE(32, 64), 7, false,
};

static const upb_msglayout *const grpc_gcp_StartServerHandshakeReq_HandshakeParametersEntry_submsgs[1] = {
  &grpc_gcp_ServerHandshakeParameters_msginit,
};

static const upb_msglayout_field grpc_gcp_StartServerHandshakeReq_HandshakeParametersEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 5, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 11, 1},
};

const upb_msglayout grpc_gcp_StartServerHandshakeReq_HandshakeParametersEntry_msginit = {
  &grpc_gcp_StartServerHandshakeReq_HandshakeParametersEntry_submsgs[0],
  &grpc_gcp_StartServerHandshakeReq_HandshakeParametersEntry__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout_field grpc_gcp_NextHandshakeMessageReq__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 12, 1},
};

const upb_msglayout grpc_gcp_NextHandshakeMessageReq_msginit = {
  NULL,
  &grpc_gcp_NextHandshakeMessageReq__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

static const upb_msglayout *const grpc_gcp_HandshakerReq_submsgs[3] = {
  &grpc_gcp_NextHandshakeMessageReq_msginit,
  &grpc_gcp_StartClientHandshakeReq_msginit,
  &grpc_gcp_StartServerHandshakeReq_msginit,
};

static const upb_msglayout_field grpc_gcp_HandshakerReq__fields[3] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 2, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
};

const upb_msglayout grpc_gcp_HandshakerReq_msginit = {
  &grpc_gcp_HandshakerReq_submsgs[0],
  &grpc_gcp_HandshakerReq__fields[0],
  UPB_SIZE(8, 16), 3, false,
};

static const upb_msglayout *const grpc_gcp_HandshakerResult_submsgs[3] = {
  &grpc_gcp_Identity_msginit,
  &grpc_gcp_RpcProtocolVersions_msginit,
};

static const upb_msglayout_field grpc_gcp_HandshakerResult__fields[8] = {
  {1, UPB_SIZE(8, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(16, 24), 0, 0, 9, 1},
  {3, UPB_SIZE(24, 40), 0, 0, 12, 1},
  {4, UPB_SIZE(32, 56), 0, 0, 11, 1},
  {5, UPB_SIZE(36, 64), 0, 0, 11, 1},
  {6, UPB_SIZE(4, 4), 0, 0, 8, 1},
  {7, UPB_SIZE(40, 72), 0, 1, 11, 1},
  {8, UPB_SIZE(0, 0), 0, 0, 13, 1},
};

const upb_msglayout grpc_gcp_HandshakerResult_msginit = {
  &grpc_gcp_HandshakerResult_submsgs[0],
  &grpc_gcp_HandshakerResult__fields[0],
  UPB_SIZE(48, 80), 8, false,
};

static const upb_msglayout_field grpc_gcp_HandshakerStatus__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 13, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 9, 1},
};

const upb_msglayout grpc_gcp_HandshakerStatus_msginit = {
  NULL,
  &grpc_gcp_HandshakerStatus__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const grpc_gcp_HandshakerResp_submsgs[2] = {
  &grpc_gcp_HandshakerResult_msginit,
  &grpc_gcp_HandshakerStatus_msginit,
};

static const upb_msglayout_field grpc_gcp_HandshakerResp__fields[4] = {
  {1, UPB_SIZE(4, 8), 0, 0, 12, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 13, 1},
  {3, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {4, UPB_SIZE(16, 32), 0, 1, 11, 1},
};

const upb_msglayout grpc_gcp_HandshakerResp_msginit = {
  &grpc_gcp_HandshakerResp_submsgs[0],
  &grpc_gcp_HandshakerResp__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

#include "upb/port_undef.inc"

