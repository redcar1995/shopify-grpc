/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/listener/listener_components.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/listener/listener_components.upb.h"
#include "envoy/api/v2/auth/tls.upb.h"
#include "envoy/api/v2/core/address.upb.h"
#include "envoy/api/v2/core/base.upb.h"
#include "envoy/type/range.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/struct.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_api_v2_listener_Filter_submsgs[2] = {
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_listener_Filter__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 1, 11, 1},
  {4, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_api_v2_listener_Filter_msginit = {
  &envoy_api_v2_listener_Filter_submsgs[0],
  &envoy_api_v2_listener_Filter__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_api_v2_listener_FilterChainMatch_submsgs[4] = {
  &envoy_api_v2_core_CidrRange_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_listener_FilterChainMatch__fields[10] = {
  {3, UPB_SIZE(32, 56), 0, 0, 11, 3},
  {4, UPB_SIZE(8, 8), 0, 0, 9, 1},
  {5, UPB_SIZE(24, 40), 0, 1, 11, 1},
  {6, UPB_SIZE(36, 64), 0, 0, 11, 3},
  {7, UPB_SIZE(40, 72), 0, 0, 13, _UPB_LABEL_PACKED},
  {8, UPB_SIZE(28, 48), 0, 1, 11, 1},
  {9, UPB_SIZE(16, 24), 0, 0, 9, 1},
  {10, UPB_SIZE(44, 80), 0, 0, 9, 3},
  {11, UPB_SIZE(48, 88), 0, 0, 9, 3},
  {12, UPB_SIZE(0, 0), 0, 0, 14, 1},
};

const upb_msglayout envoy_api_v2_listener_FilterChainMatch_msginit = {
  &envoy_api_v2_listener_FilterChainMatch_submsgs[0],
  &envoy_api_v2_listener_FilterChainMatch__fields[0],
  UPB_SIZE(56, 96), 10, false,
};

static const upb_msglayout *const envoy_api_v2_listener_FilterChain_submsgs[6] = {
  &envoy_api_v2_auth_DownstreamTlsContext_msginit,
  &envoy_api_v2_core_Metadata_msginit,
  &envoy_api_v2_core_TransportSocket_msginit,
  &envoy_api_v2_listener_Filter_msginit,
  &envoy_api_v2_listener_FilterChainMatch_msginit,
  &google_protobuf_BoolValue_msginit,
};

static const upb_msglayout_field envoy_api_v2_listener_FilterChain__fields[7] = {
  {1, UPB_SIZE(8, 16), 0, 4, 11, 1},
  {2, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {3, UPB_SIZE(28, 56), 0, 3, 11, 3},
  {4, UPB_SIZE(16, 32), 0, 5, 11, 1},
  {5, UPB_SIZE(20, 40), 0, 1, 11, 1},
  {6, UPB_SIZE(24, 48), 0, 2, 11, 1},
  {7, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_listener_FilterChain_msginit = {
  &envoy_api_v2_listener_FilterChain_submsgs[0],
  &envoy_api_v2_listener_FilterChain__fields[0],
  UPB_SIZE(32, 64), 7, false,
};

static const upb_msglayout *const envoy_api_v2_listener_ListenerFilterChainMatchPredicate_submsgs[4] = {
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_msginit,
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_MatchSet_msginit,
  &envoy_type_Int32Range_msginit,
};

static const upb_msglayout_field envoy_api_v2_listener_ListenerFilterChainMatchPredicate__fields[5] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {4, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 8, 1},
  {5, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 2, 11, 1},
};

const upb_msglayout envoy_api_v2_listener_ListenerFilterChainMatchPredicate_msginit = {
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_submsgs[0],
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate__fields[0],
  UPB_SIZE(8, 16), 5, false,
};

static const upb_msglayout *const envoy_api_v2_listener_ListenerFilterChainMatchPredicate_MatchSet_submsgs[1] = {
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_msginit,
};

static const upb_msglayout_field envoy_api_v2_listener_ListenerFilterChainMatchPredicate_MatchSet__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_api_v2_listener_ListenerFilterChainMatchPredicate_MatchSet_msginit = {
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_MatchSet_submsgs[0],
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_MatchSet__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_api_v2_listener_ListenerFilter_submsgs[3] = {
  &envoy_api_v2_listener_ListenerFilterChainMatchPredicate_msginit,
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_listener_ListenerFilter__fields[4] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(12, 24), UPB_SIZE(-17, -33), 2, 11, 1},
  {3, UPB_SIZE(12, 24), UPB_SIZE(-17, -33), 1, 11, 1},
  {4, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_listener_ListenerFilter_msginit = {
  &envoy_api_v2_listener_ListenerFilter_submsgs[0],
  &envoy_api_v2_listener_ListenerFilter__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

#include "upb/port_undef.inc"

