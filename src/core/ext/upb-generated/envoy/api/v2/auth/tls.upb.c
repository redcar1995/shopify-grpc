/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/auth/tls.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/auth/tls.upb.h"
#include "envoy/api/v2/auth/common.upb.h"
#include "envoy/api/v2/auth/secret.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_auth_UpstreamTlsContext_submsgs[2] = {
  &envoy_api_v2_auth_CommonTlsContext_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_auth_UpstreamTlsContext__fields[4] = {
  {1, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 8, 1},
  {4, UPB_SIZE(16, 32), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_auth_UpstreamTlsContext_msginit = {
  &envoy_api_v2_auth_UpstreamTlsContext_submsgs[0],
  &envoy_api_v2_auth_UpstreamTlsContext__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

static const upb_msglayout *const envoy_api_v2_auth_DownstreamTlsContext_submsgs[6] = {
  &envoy_api_v2_auth_CommonTlsContext_msginit,
  &envoy_api_v2_auth_SdsSecretConfig_msginit,
  &envoy_api_v2_auth_TlsSessionTicketKeys_msginit,
  &google_protobuf_BoolValue_msginit,
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_api_v2_auth_DownstreamTlsContext__fields[7] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 3, 11, 1},
  {3, UPB_SIZE(8, 16), 0, 3, 11, 1},
  {4, UPB_SIZE(16, 32), UPB_SIZE(-20, -40), 2, 11, 1},
  {5, UPB_SIZE(16, 32), UPB_SIZE(-20, -40), 1, 11, 1},
  {6, UPB_SIZE(12, 24), 0, 4, 11, 1},
  {7, UPB_SIZE(16, 32), UPB_SIZE(-20, -40), 0, 8, 1},
};

const upb_msglayout envoy_api_v2_auth_DownstreamTlsContext_msginit = {
  &envoy_api_v2_auth_DownstreamTlsContext_submsgs[0],
  &envoy_api_v2_auth_DownstreamTlsContext__fields[0],
  UPB_SIZE(24, 48), 7, false,
};

static const upb_msglayout *const envoy_api_v2_auth_CommonTlsContext_submsgs[6] = {
  &envoy_api_v2_auth_CertificateValidationContext_msginit,
  &envoy_api_v2_auth_CommonTlsContext_CombinedCertificateValidationContext_msginit,
  &envoy_api_v2_auth_SdsSecretConfig_msginit,
  &envoy_api_v2_auth_TlsCertificate_msginit,
  &envoy_api_v2_auth_TlsParameters_msginit,
};

static const upb_msglayout_field envoy_api_v2_auth_CommonTlsContext__fields[7] = {
  {1, UPB_SIZE(0, 0), 0, 4, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 3, 11, 3},
  {3, UPB_SIZE(16, 32), UPB_SIZE(-20, -40), 0, 11, 1},
  {4, UPB_SIZE(8, 16), 0, 0, 9, 3},
  {6, UPB_SIZE(12, 24), 0, 2, 11, 3},
  {7, UPB_SIZE(16, 32), UPB_SIZE(-20, -40), 2, 11, 1},
  {8, UPB_SIZE(16, 32), UPB_SIZE(-20, -40), 1, 11, 1},
};

const upb_msglayout envoy_api_v2_auth_CommonTlsContext_msginit = {
  &envoy_api_v2_auth_CommonTlsContext_submsgs[0],
  &envoy_api_v2_auth_CommonTlsContext__fields[0],
  UPB_SIZE(24, 48), 7, false,
};

static const upb_msglayout *const envoy_api_v2_auth_CommonTlsContext_CombinedCertificateValidationContext_submsgs[2] = {
  &envoy_api_v2_auth_CertificateValidationContext_msginit,
  &envoy_api_v2_auth_SdsSecretConfig_msginit,
};

static const upb_msglayout_field envoy_api_v2_auth_CommonTlsContext_CombinedCertificateValidationContext__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_auth_CommonTlsContext_CombinedCertificateValidationContext_msginit = {
  &envoy_api_v2_auth_CommonTlsContext_CombinedCertificateValidationContext_submsgs[0],
  &envoy_api_v2_auth_CommonTlsContext_CombinedCertificateValidationContext__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

#include "upb/port_undef.inc"

