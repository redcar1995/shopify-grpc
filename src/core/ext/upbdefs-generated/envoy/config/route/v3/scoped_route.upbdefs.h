/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/route/v3/scoped_route.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_CONFIG_ROUTE_V3_SCOPED_ROUTE_PROTO_UPBDEFS_H_
#define ENVOY_CONFIG_ROUTE_V3_SCOPED_ROUTE_PROTO_UPBDEFS_H_

#include "upb/def.h"
#include "upb/port_def.inc"
#ifdef __cplusplus
extern "C" {
#endif

#include "upb/def.h"

#include "upb/port_def.inc"

extern upb_def_init envoy_config_route_v3_scoped_route_proto_upbdefinit;

UPB_INLINE const upb_msgdef *envoy_config_route_v3_ScopedRouteConfiguration_getmsgdef(upb_symtab *s) {
  _upb_symtab_loaddefinit(s, &envoy_config_route_v3_scoped_route_proto_upbdefinit);
  return upb_symtab_lookupmsg(s, "envoy.config.route.v3.ScopedRouteConfiguration");
}

UPB_INLINE const upb_msgdef *envoy_config_route_v3_ScopedRouteConfiguration_Key_getmsgdef(upb_symtab *s) {
  _upb_symtab_loaddefinit(s, &envoy_config_route_v3_scoped_route_proto_upbdefinit);
  return upb_symtab_lookupmsg(s, "envoy.config.route.v3.ScopedRouteConfiguration.Key");
}

UPB_INLINE const upb_msgdef *envoy_config_route_v3_ScopedRouteConfiguration_Key_Fragment_getmsgdef(upb_symtab *s) {
  _upb_symtab_loaddefinit(s, &envoy_config_route_v3_scoped_route_proto_upbdefinit);
  return upb_symtab_lookupmsg(s, "envoy.config.route.v3.ScopedRouteConfiguration.Key.Fragment");
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_CONFIG_ROUTE_V3_SCOPED_ROUTE_PROTO_UPBDEFS_H_ */
