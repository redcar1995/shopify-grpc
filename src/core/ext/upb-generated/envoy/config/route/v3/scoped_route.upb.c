/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/route/v3/scoped_route.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/config/route/v3/scoped_route.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_config_route_v3_ScopedRouteConfiguration_submsgs[1] = {
  &envoy_config_route_v3_ScopedRouteConfiguration_Key_msginit,
};

static const upb_msglayout_field envoy_config_route_v3_ScopedRouteConfiguration__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(16, 32), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_route_v3_ScopedRouteConfiguration_msginit = {
  &envoy_config_route_v3_ScopedRouteConfiguration_submsgs[0],
  &envoy_config_route_v3_ScopedRouteConfiguration__fields[0],
  UPB_SIZE(24, 48), 3, false,
};

static const upb_msglayout *const envoy_config_route_v3_ScopedRouteConfiguration_Key_submsgs[1] = {
  &envoy_config_route_v3_ScopedRouteConfiguration_Key_Fragment_msginit,
};

static const upb_msglayout_field envoy_config_route_v3_ScopedRouteConfiguration_Key__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_route_v3_ScopedRouteConfiguration_Key_msginit = {
  &envoy_config_route_v3_ScopedRouteConfiguration_Key_submsgs[0],
  &envoy_config_route_v3_ScopedRouteConfiguration_Key__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout_field envoy_config_route_v3_ScopedRouteConfiguration_Key_Fragment__fields[1] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
};

const upb_msglayout envoy_config_route_v3_ScopedRouteConfiguration_Key_Fragment_msginit = {
  NULL,
  &envoy_config_route_v3_ScopedRouteConfiguration_Key_Fragment__fields[0],
  UPB_SIZE(16, 32), 1, false,
};

#include "upb/port_undef.inc"

