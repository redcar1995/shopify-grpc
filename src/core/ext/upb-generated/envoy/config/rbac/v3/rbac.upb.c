/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/rbac/v3/rbac.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/config/rbac/v3/rbac.upb.h"
#include "envoy/config/core/v3/address.upb.h"
#include "envoy/config/route/v3/route_components.upb.h"
#include "envoy/type/matcher/v3/metadata.upb.h"
#include "envoy/type/matcher/v3/path.upb.h"
#include "envoy/type/matcher/v3/string.upb.h"
#include "google/api/expr/v1alpha1/checked.upb.h"
#include "google/api/expr/v1alpha1/syntax.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_config_rbac_v3_RBAC_submsgs[1] = {
  &envoy_config_rbac_v3_RBAC_PoliciesEntry_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_RBAC__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 14, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 11, _UPB_LABEL_MAP},
};

const upb_msglayout envoy_config_rbac_v3_RBAC_msginit = {
  &envoy_config_rbac_v3_RBAC_submsgs[0],
  &envoy_config_rbac_v3_RBAC__fields[0],
  UPB_SIZE(16, 16), 2, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_RBAC_PoliciesEntry_submsgs[1] = {
  &envoy_config_rbac_v3_Policy_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_RBAC_PoliciesEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_rbac_v3_RBAC_PoliciesEntry_msginit = {
  &envoy_config_rbac_v3_RBAC_PoliciesEntry_submsgs[0],
  &envoy_config_rbac_v3_RBAC_PoliciesEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_Policy_submsgs[4] = {
  &envoy_config_rbac_v3_Permission_msginit,
  &envoy_config_rbac_v3_Principal_msginit,
  &google_api_expr_v1alpha1_CheckedExpr_msginit,
  &google_api_expr_v1alpha1_Expr_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_Policy__fields[4] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 3},
  {2, UPB_SIZE(12, 24), 0, 1, 11, 3},
  {3, UPB_SIZE(0, 0), 0, 3, 11, 1},
  {4, UPB_SIZE(4, 8), 0, 2, 11, 1},
};

const upb_msglayout envoy_config_rbac_v3_Policy_msginit = {
  &envoy_config_rbac_v3_Policy_submsgs[0],
  &envoy_config_rbac_v3_Policy__fields[0],
  UPB_SIZE(16, 32), 4, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_Permission_submsgs[8] = {
  &envoy_config_core_v3_CidrRange_msginit,
  &envoy_config_rbac_v3_Permission_msginit,
  &envoy_config_rbac_v3_Permission_Set_msginit,
  &envoy_config_route_v3_HeaderMatcher_msginit,
  &envoy_type_matcher_v3_MetadataMatcher_msginit,
  &envoy_type_matcher_v3_PathMatcher_msginit,
  &envoy_type_matcher_v3_StringMatcher_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_Permission__fields[10] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 2, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 2, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 8, 1},
  {4, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 3, 11, 1},
  {5, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {6, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 13, 1},
  {7, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 4, 11, 1},
  {8, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
  {9, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 6, 11, 1},
  {10, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 5, 11, 1},
};

const upb_msglayout envoy_config_rbac_v3_Permission_msginit = {
  &envoy_config_rbac_v3_Permission_submsgs[0],
  &envoy_config_rbac_v3_Permission__fields[0],
  UPB_SIZE(8, 16), 10, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_Permission_Set_submsgs[1] = {
  &envoy_config_rbac_v3_Permission_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_Permission_Set__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_rbac_v3_Permission_Set_msginit = {
  &envoy_config_rbac_v3_Permission_Set_submsgs[0],
  &envoy_config_rbac_v3_Permission_Set__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_Principal_submsgs[10] = {
  &envoy_config_core_v3_CidrRange_msginit,
  &envoy_config_rbac_v3_Principal_msginit,
  &envoy_config_rbac_v3_Principal_Authenticated_msginit,
  &envoy_config_rbac_v3_Principal_Set_msginit,
  &envoy_config_route_v3_HeaderMatcher_msginit,
  &envoy_type_matcher_v3_MetadataMatcher_msginit,
  &envoy_type_matcher_v3_PathMatcher_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_Principal__fields[11] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 3, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 3, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 8, 1},
  {4, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 2, 11, 1},
  {5, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {6, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 4, 11, 1},
  {7, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 5, 11, 1},
  {8, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
  {9, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 6, 11, 1},
  {10, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {11, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
};

const upb_msglayout envoy_config_rbac_v3_Principal_msginit = {
  &envoy_config_rbac_v3_Principal_submsgs[0],
  &envoy_config_rbac_v3_Principal__fields[0],
  UPB_SIZE(8, 16), 11, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_Principal_Set_submsgs[1] = {
  &envoy_config_rbac_v3_Principal_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_Principal_Set__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_rbac_v3_Principal_Set_msginit = {
  &envoy_config_rbac_v3_Principal_Set_submsgs[0],
  &envoy_config_rbac_v3_Principal_Set__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_rbac_v3_Principal_Authenticated_submsgs[1] = {
  &envoy_type_matcher_v3_StringMatcher_msginit,
};

static const upb_msglayout_field envoy_config_rbac_v3_Principal_Authenticated__fields[1] = {
  {2, UPB_SIZE(0, 0), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_rbac_v3_Principal_Authenticated_msginit = {
  &envoy_config_rbac_v3_Principal_Authenticated_submsgs[0],
  &envoy_config_rbac_v3_Principal_Authenticated__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

#include "upb/port_undef.inc"

