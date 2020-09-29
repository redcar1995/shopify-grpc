/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/extensions/transport_sockets/tls/v3/tls.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/extensions/transport_sockets/tls/v3/tls.upb.h"
#include "envoy/config/core/v3/extension.upb.h"
#include "envoy/extensions/transport_sockets/tls/v3/common.upb.h"
#include "envoy/extensions/transport_sockets/tls/v3/secret.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_extensions_transport_sockets_tls_v3_UpstreamTlsContext_submsgs[2] = {
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_extensions_transport_sockets_tls_v3_UpstreamTlsContext__fields[4] = {
  {1, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 8, 1},
  {4, UPB_SIZE(16, 32), 0, 1, 11, 1},
};

const upb_msglayout envoy_extensions_transport_sockets_tls_v3_UpstreamTlsContext_msginit = {
  &envoy_extensions_transport_sockets_tls_v3_UpstreamTlsContext_submsgs[0],
  &envoy_extensions_transport_sockets_tls_v3_UpstreamTlsContext__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

static const upb_msglayout *const envoy_extensions_transport_sockets_tls_v3_DownstreamTlsContext_submsgs[6] = {
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_msginit,
  &envoy_extensions_transport_sockets_tls_v3_SdsSecretConfig_msginit,
  &envoy_extensions_transport_sockets_tls_v3_TlsSessionTicketKeys_msginit,
  &google_protobuf_BoolValue_msginit,
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_extensions_transport_sockets_tls_v3_DownstreamTlsContext__fields[8] = {
  {1, UPB_SIZE(8, 8), 0, 0, 11, 1},
  {2, UPB_SIZE(12, 16), 0, 3, 11, 1},
  {3, UPB_SIZE(16, 24), 0, 3, 11, 1},
  {4, UPB_SIZE(24, 40), UPB_SIZE(-29, -49), 2, 11, 1},
  {5, UPB_SIZE(24, 40), UPB_SIZE(-29, -49), 1, 11, 1},
  {6, UPB_SIZE(20, 32), 0, 4, 11, 1},
  {7, UPB_SIZE(24, 40), UPB_SIZE(-29, -49), 0, 8, 1},
  {8, UPB_SIZE(0, 0), 0, 0, 14, 1},
};

const upb_msglayout envoy_extensions_transport_sockets_tls_v3_DownstreamTlsContext_msginit = {
  &envoy_extensions_transport_sockets_tls_v3_DownstreamTlsContext_submsgs[0],
  &envoy_extensions_transport_sockets_tls_v3_DownstreamTlsContext__fields[0],
  UPB_SIZE(32, 56), 8, false,
};

static const upb_msglayout *const envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_submsgs[11] = {
  &envoy_config_core_v3_TypedExtensionConfig_msginit,
  &envoy_extensions_transport_sockets_tls_v3_CertificateValidationContext_msginit,
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider_msginit,
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProviderInstance_msginit,
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CombinedCertificateValidationContext_msginit,
  &envoy_extensions_transport_sockets_tls_v3_SdsSecretConfig_msginit,
  &envoy_extensions_transport_sockets_tls_v3_TlsCertificate_msginit,
  &envoy_extensions_transport_sockets_tls_v3_TlsParameters_msginit,
};

static const upb_msglayout_field envoy_extensions_transport_sockets_tls_v3_CommonTlsContext__fields[12] = {
  {1, UPB_SIZE(0, 0), 0, 7, 11, 1},
  {2, UPB_SIZE(16, 32), 0, 6, 11, 3},
  {3, UPB_SIZE(28, 56), UPB_SIZE(-33, -65), 1, 11, 1},
  {4, UPB_SIZE(20, 40), 0, 0, 9, 3},
  {6, UPB_SIZE(24, 48), 0, 5, 11, 3},
  {7, UPB_SIZE(28, 56), UPB_SIZE(-33, -65), 5, 11, 1},
  {8, UPB_SIZE(28, 56), UPB_SIZE(-33, -65), 4, 11, 1},
  {9, UPB_SIZE(4, 8), 0, 2, 11, 1},
  {10, UPB_SIZE(28, 56), UPB_SIZE(-33, -65), 2, 11, 1},
  {11, UPB_SIZE(8, 16), 0, 3, 11, 1},
  {12, UPB_SIZE(28, 56), UPB_SIZE(-33, -65), 3, 11, 1},
  {13, UPB_SIZE(12, 24), 0, 0, 11, 1},
};

const upb_msglayout envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_msginit = {
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_submsgs[0],
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext__fields[0],
  UPB_SIZE(36, 72), 12, false,
};

static const upb_msglayout *const envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider_submsgs[1] = {
  &envoy_config_core_v3_TypedExtensionConfig_msginit,
};

static const upb_msglayout_field envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider_msginit = {
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider_submsgs[0],
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProviderInstance__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProviderInstance_msginit = {
  NULL,
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProviderInstance__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CombinedCertificateValidationContext_submsgs[4] = {
  &envoy_extensions_transport_sockets_tls_v3_CertificateValidationContext_msginit,
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProvider_msginit,
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CertificateProviderInstance_msginit,
  &envoy_extensions_transport_sockets_tls_v3_SdsSecretConfig_msginit,
};

static const upb_msglayout_field envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CombinedCertificateValidationContext__fields[4] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 3, 11, 1},
  {3, UPB_SIZE(8, 16), 0, 1, 11, 1},
  {4, UPB_SIZE(12, 24), 0, 2, 11, 1},
};

const upb_msglayout envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CombinedCertificateValidationContext_msginit = {
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CombinedCertificateValidationContext_submsgs[0],
  &envoy_extensions_transport_sockets_tls_v3_CommonTlsContext_CombinedCertificateValidationContext__fields[0],
  UPB_SIZE(16, 32), 4, false,
};

#include "upb/port_undef.inc"

