/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/endpoint/endpoint.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_API_V2_ENDPOINT_ENDPOINT_PROTO_UPB_H_
#define ENVOY_API_V2_ENDPOINT_ENDPOINT_PROTO_UPB_H_

#include "upb/msg.h"
#include "upb/decode.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_api_v2_endpoint_Endpoint;
struct envoy_api_v2_endpoint_Endpoint_HealthCheckConfig;
struct envoy_api_v2_endpoint_LbEndpoint;
struct envoy_api_v2_endpoint_LocalityLbEndpoints;
typedef struct envoy_api_v2_endpoint_Endpoint envoy_api_v2_endpoint_Endpoint;
typedef struct envoy_api_v2_endpoint_Endpoint_HealthCheckConfig envoy_api_v2_endpoint_Endpoint_HealthCheckConfig;
typedef struct envoy_api_v2_endpoint_LbEndpoint envoy_api_v2_endpoint_LbEndpoint;
typedef struct envoy_api_v2_endpoint_LocalityLbEndpoints envoy_api_v2_endpoint_LocalityLbEndpoints;
extern const upb_msglayout envoy_api_v2_endpoint_Endpoint_msginit;
extern const upb_msglayout envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_msginit;
extern const upb_msglayout envoy_api_v2_endpoint_LbEndpoint_msginit;
extern const upb_msglayout envoy_api_v2_endpoint_LocalityLbEndpoints_msginit;
struct envoy_api_v2_core_Address;
struct envoy_api_v2_core_Locality;
struct envoy_api_v2_core_Metadata;
struct google_protobuf_UInt32Value;
extern const upb_msglayout envoy_api_v2_core_Address_msginit;
extern const upb_msglayout envoy_api_v2_core_Locality_msginit;
extern const upb_msglayout envoy_api_v2_core_Metadata_msginit;
extern const upb_msglayout google_protobuf_UInt32Value_msginit;


/* envoy.api.v2.endpoint.Endpoint */

UPB_INLINE envoy_api_v2_endpoint_Endpoint *envoy_api_v2_endpoint_Endpoint_new(upb_arena *arena) {
  return (envoy_api_v2_endpoint_Endpoint *)_upb_msg_new(&envoy_api_v2_endpoint_Endpoint_msginit, arena);
}
UPB_INLINE envoy_api_v2_endpoint_Endpoint *envoy_api_v2_endpoint_Endpoint_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_endpoint_Endpoint *ret = envoy_api_v2_endpoint_Endpoint_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_endpoint_Endpoint_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_endpoint_Endpoint_serialize(const envoy_api_v2_endpoint_Endpoint *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_endpoint_Endpoint_msginit, arena, len);
}

UPB_INLINE const struct envoy_api_v2_core_Address* envoy_api_v2_endpoint_Endpoint_address(const envoy_api_v2_endpoint_Endpoint *msg) { return UPB_FIELD_AT(msg, const struct envoy_api_v2_core_Address*, UPB_SIZE(0, 0)); }
UPB_INLINE const envoy_api_v2_endpoint_Endpoint_HealthCheckConfig* envoy_api_v2_endpoint_Endpoint_health_check_config(const envoy_api_v2_endpoint_Endpoint *msg) { return UPB_FIELD_AT(msg, const envoy_api_v2_endpoint_Endpoint_HealthCheckConfig*, UPB_SIZE(4, 8)); }

UPB_INLINE void envoy_api_v2_endpoint_Endpoint_set_address(envoy_api_v2_endpoint_Endpoint *msg, struct envoy_api_v2_core_Address* value) {
  UPB_FIELD_AT(msg, struct envoy_api_v2_core_Address*, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE struct envoy_api_v2_core_Address* envoy_api_v2_endpoint_Endpoint_mutable_address(envoy_api_v2_endpoint_Endpoint *msg, upb_arena *arena) {
  struct envoy_api_v2_core_Address* sub = (struct envoy_api_v2_core_Address*)envoy_api_v2_endpoint_Endpoint_address(msg);
  if (sub == NULL) {
    sub = (struct envoy_api_v2_core_Address*)_upb_msg_new(&envoy_api_v2_core_Address_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_Endpoint_set_address(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_endpoint_Endpoint_set_health_check_config(envoy_api_v2_endpoint_Endpoint *msg, envoy_api_v2_endpoint_Endpoint_HealthCheckConfig* value) {
  UPB_FIELD_AT(msg, envoy_api_v2_endpoint_Endpoint_HealthCheckConfig*, UPB_SIZE(4, 8)) = value;
}
UPB_INLINE struct envoy_api_v2_endpoint_Endpoint_HealthCheckConfig* envoy_api_v2_endpoint_Endpoint_mutable_health_check_config(envoy_api_v2_endpoint_Endpoint *msg, upb_arena *arena) {
  struct envoy_api_v2_endpoint_Endpoint_HealthCheckConfig* sub = (struct envoy_api_v2_endpoint_Endpoint_HealthCheckConfig*)envoy_api_v2_endpoint_Endpoint_health_check_config(msg);
  if (sub == NULL) {
    sub = (struct envoy_api_v2_endpoint_Endpoint_HealthCheckConfig*)_upb_msg_new(&envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_Endpoint_set_health_check_config(msg, sub);
  }
  return sub;
}

/* envoy.api.v2.endpoint.Endpoint.HealthCheckConfig */

UPB_INLINE envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_new(upb_arena *arena) {
  return (envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *)_upb_msg_new(&envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_msginit, arena);
}
UPB_INLINE envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *ret = envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_serialize(const envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_msginit, arena, len);
}

UPB_INLINE uint32_t envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_port_value(const envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *msg) { return UPB_FIELD_AT(msg, uint32_t, UPB_SIZE(0, 0)); }

UPB_INLINE void envoy_api_v2_endpoint_Endpoint_HealthCheckConfig_set_port_value(envoy_api_v2_endpoint_Endpoint_HealthCheckConfig *msg, uint32_t value) {
  UPB_FIELD_AT(msg, uint32_t, UPB_SIZE(0, 0)) = value;
}

/* envoy.api.v2.endpoint.LbEndpoint */

UPB_INLINE envoy_api_v2_endpoint_LbEndpoint *envoy_api_v2_endpoint_LbEndpoint_new(upb_arena *arena) {
  return (envoy_api_v2_endpoint_LbEndpoint *)_upb_msg_new(&envoy_api_v2_endpoint_LbEndpoint_msginit, arena);
}
UPB_INLINE envoy_api_v2_endpoint_LbEndpoint *envoy_api_v2_endpoint_LbEndpoint_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_endpoint_LbEndpoint *ret = envoy_api_v2_endpoint_LbEndpoint_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_endpoint_LbEndpoint_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_endpoint_LbEndpoint_serialize(const envoy_api_v2_endpoint_LbEndpoint *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_endpoint_LbEndpoint_msginit, arena, len);
}

typedef enum {
  envoy_api_v2_endpoint_LbEndpoint_host_identifier_endpoint = 1,
  envoy_api_v2_endpoint_LbEndpoint_host_identifier_endpoint_name = 5,
  envoy_api_v2_endpoint_LbEndpoint_host_identifier_NOT_SET = 0
} envoy_api_v2_endpoint_LbEndpoint_host_identifier_oneofcases;
UPB_INLINE envoy_api_v2_endpoint_LbEndpoint_host_identifier_oneofcases envoy_api_v2_endpoint_LbEndpoint_host_identifier_case(const envoy_api_v2_endpoint_LbEndpoint* msg) { return (envoy_api_v2_endpoint_LbEndpoint_host_identifier_oneofcases)UPB_FIELD_AT(msg, int32_t, UPB_SIZE(24, 40)); }

UPB_INLINE bool envoy_api_v2_endpoint_LbEndpoint_has_endpoint(const envoy_api_v2_endpoint_LbEndpoint *msg) { return _upb_has_oneof_field(msg, UPB_SIZE(24, 40), 1); }
UPB_INLINE const envoy_api_v2_endpoint_Endpoint* envoy_api_v2_endpoint_LbEndpoint_endpoint(const envoy_api_v2_endpoint_LbEndpoint *msg) { return UPB_READ_ONEOF(msg, const envoy_api_v2_endpoint_Endpoint*, UPB_SIZE(16, 24), UPB_SIZE(24, 40), 1, NULL); }
UPB_INLINE int32_t envoy_api_v2_endpoint_LbEndpoint_health_status(const envoy_api_v2_endpoint_LbEndpoint *msg) { return UPB_FIELD_AT(msg, int32_t, UPB_SIZE(0, 0)); }
UPB_INLINE const struct envoy_api_v2_core_Metadata* envoy_api_v2_endpoint_LbEndpoint_metadata(const envoy_api_v2_endpoint_LbEndpoint *msg) { return UPB_FIELD_AT(msg, const struct envoy_api_v2_core_Metadata*, UPB_SIZE(8, 8)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_endpoint_LbEndpoint_load_balancing_weight(const envoy_api_v2_endpoint_LbEndpoint *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(12, 16)); }
UPB_INLINE bool envoy_api_v2_endpoint_LbEndpoint_has_endpoint_name(const envoy_api_v2_endpoint_LbEndpoint *msg) { return _upb_has_oneof_field(msg, UPB_SIZE(24, 40), 5); }
UPB_INLINE upb_strview envoy_api_v2_endpoint_LbEndpoint_endpoint_name(const envoy_api_v2_endpoint_LbEndpoint *msg) { return UPB_READ_ONEOF(msg, upb_strview, UPB_SIZE(16, 24), UPB_SIZE(24, 40), 5, upb_strview_make("", strlen(""))); }

UPB_INLINE void envoy_api_v2_endpoint_LbEndpoint_set_endpoint(envoy_api_v2_endpoint_LbEndpoint *msg, envoy_api_v2_endpoint_Endpoint* value) {
  UPB_WRITE_ONEOF(msg, envoy_api_v2_endpoint_Endpoint*, UPB_SIZE(16, 24), value, UPB_SIZE(24, 40), 1);
}
UPB_INLINE struct envoy_api_v2_endpoint_Endpoint* envoy_api_v2_endpoint_LbEndpoint_mutable_endpoint(envoy_api_v2_endpoint_LbEndpoint *msg, upb_arena *arena) {
  struct envoy_api_v2_endpoint_Endpoint* sub = (struct envoy_api_v2_endpoint_Endpoint*)envoy_api_v2_endpoint_LbEndpoint_endpoint(msg);
  if (sub == NULL) {
    sub = (struct envoy_api_v2_endpoint_Endpoint*)_upb_msg_new(&envoy_api_v2_endpoint_Endpoint_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_LbEndpoint_set_endpoint(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_endpoint_LbEndpoint_set_health_status(envoy_api_v2_endpoint_LbEndpoint *msg, int32_t value) {
  UPB_FIELD_AT(msg, int32_t, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE void envoy_api_v2_endpoint_LbEndpoint_set_metadata(envoy_api_v2_endpoint_LbEndpoint *msg, struct envoy_api_v2_core_Metadata* value) {
  UPB_FIELD_AT(msg, struct envoy_api_v2_core_Metadata*, UPB_SIZE(8, 8)) = value;
}
UPB_INLINE struct envoy_api_v2_core_Metadata* envoy_api_v2_endpoint_LbEndpoint_mutable_metadata(envoy_api_v2_endpoint_LbEndpoint *msg, upb_arena *arena) {
  struct envoy_api_v2_core_Metadata* sub = (struct envoy_api_v2_core_Metadata*)envoy_api_v2_endpoint_LbEndpoint_metadata(msg);
  if (sub == NULL) {
    sub = (struct envoy_api_v2_core_Metadata*)_upb_msg_new(&envoy_api_v2_core_Metadata_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_LbEndpoint_set_metadata(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_endpoint_LbEndpoint_set_load_balancing_weight(envoy_api_v2_endpoint_LbEndpoint *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(12, 16)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_endpoint_LbEndpoint_mutable_load_balancing_weight(envoy_api_v2_endpoint_LbEndpoint *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_endpoint_LbEndpoint_load_balancing_weight(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_LbEndpoint_set_load_balancing_weight(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_endpoint_LbEndpoint_set_endpoint_name(envoy_api_v2_endpoint_LbEndpoint *msg, upb_strview value) {
  UPB_WRITE_ONEOF(msg, upb_strview, UPB_SIZE(16, 24), value, UPB_SIZE(24, 40), 5);
}

/* envoy.api.v2.endpoint.LocalityLbEndpoints */

UPB_INLINE envoy_api_v2_endpoint_LocalityLbEndpoints *envoy_api_v2_endpoint_LocalityLbEndpoints_new(upb_arena *arena) {
  return (envoy_api_v2_endpoint_LocalityLbEndpoints *)_upb_msg_new(&envoy_api_v2_endpoint_LocalityLbEndpoints_msginit, arena);
}
UPB_INLINE envoy_api_v2_endpoint_LocalityLbEndpoints *envoy_api_v2_endpoint_LocalityLbEndpoints_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_endpoint_LocalityLbEndpoints *ret = envoy_api_v2_endpoint_LocalityLbEndpoints_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_endpoint_LocalityLbEndpoints_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_endpoint_LocalityLbEndpoints_serialize(const envoy_api_v2_endpoint_LocalityLbEndpoints *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_endpoint_LocalityLbEndpoints_msginit, arena, len);
}

UPB_INLINE const struct envoy_api_v2_core_Locality* envoy_api_v2_endpoint_LocalityLbEndpoints_locality(const envoy_api_v2_endpoint_LocalityLbEndpoints *msg) { return UPB_FIELD_AT(msg, const struct envoy_api_v2_core_Locality*, UPB_SIZE(4, 8)); }
UPB_INLINE const envoy_api_v2_endpoint_LbEndpoint* const* envoy_api_v2_endpoint_LocalityLbEndpoints_lb_endpoints(const envoy_api_v2_endpoint_LocalityLbEndpoints *msg, size_t *len) { return (const envoy_api_v2_endpoint_LbEndpoint* const*)_upb_array_accessor(msg, UPB_SIZE(16, 32), len); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_endpoint_LocalityLbEndpoints_load_balancing_weight(const envoy_api_v2_endpoint_LocalityLbEndpoints *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(8, 16)); }
UPB_INLINE uint32_t envoy_api_v2_endpoint_LocalityLbEndpoints_priority(const envoy_api_v2_endpoint_LocalityLbEndpoints *msg) { return UPB_FIELD_AT(msg, uint32_t, UPB_SIZE(0, 0)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_endpoint_LocalityLbEndpoints_proximity(const envoy_api_v2_endpoint_LocalityLbEndpoints *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(12, 24)); }

UPB_INLINE void envoy_api_v2_endpoint_LocalityLbEndpoints_set_locality(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, struct envoy_api_v2_core_Locality* value) {
  UPB_FIELD_AT(msg, struct envoy_api_v2_core_Locality*, UPB_SIZE(4, 8)) = value;
}
UPB_INLINE struct envoy_api_v2_core_Locality* envoy_api_v2_endpoint_LocalityLbEndpoints_mutable_locality(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, upb_arena *arena) {
  struct envoy_api_v2_core_Locality* sub = (struct envoy_api_v2_core_Locality*)envoy_api_v2_endpoint_LocalityLbEndpoints_locality(msg);
  if (sub == NULL) {
    sub = (struct envoy_api_v2_core_Locality*)_upb_msg_new(&envoy_api_v2_core_Locality_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_LocalityLbEndpoints_set_locality(msg, sub);
  }
  return sub;
}
UPB_INLINE envoy_api_v2_endpoint_LbEndpoint** envoy_api_v2_endpoint_LocalityLbEndpoints_mutable_lb_endpoints(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, size_t *len) {
  return (envoy_api_v2_endpoint_LbEndpoint**)_upb_array_mutable_accessor(msg, UPB_SIZE(16, 32), len);
}
UPB_INLINE envoy_api_v2_endpoint_LbEndpoint** envoy_api_v2_endpoint_LocalityLbEndpoints_resize_lb_endpoints(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, size_t len, upb_arena *arena) {
  return (envoy_api_v2_endpoint_LbEndpoint**)_upb_array_resize_accessor(msg, UPB_SIZE(16, 32), len, UPB_TYPE_MESSAGE, arena);
}
UPB_INLINE struct envoy_api_v2_endpoint_LbEndpoint* envoy_api_v2_endpoint_LocalityLbEndpoints_add_lb_endpoints(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, upb_arena *arena) {
  struct envoy_api_v2_endpoint_LbEndpoint* sub = (struct envoy_api_v2_endpoint_LbEndpoint*)_upb_msg_new(&envoy_api_v2_endpoint_LbEndpoint_msginit, arena);
  bool ok = _upb_array_append_accessor(
      msg, UPB_SIZE(16, 32), UPB_SIZE(4, 8), UPB_TYPE_MESSAGE, &sub, arena);
  if (!ok) return NULL;
  return sub;
}
UPB_INLINE void envoy_api_v2_endpoint_LocalityLbEndpoints_set_load_balancing_weight(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(8, 16)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_endpoint_LocalityLbEndpoints_mutable_load_balancing_weight(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_endpoint_LocalityLbEndpoints_load_balancing_weight(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_LocalityLbEndpoints_set_load_balancing_weight(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_endpoint_LocalityLbEndpoints_set_priority(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, uint32_t value) {
  UPB_FIELD_AT(msg, uint32_t, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE void envoy_api_v2_endpoint_LocalityLbEndpoints_set_proximity(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(12, 24)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_endpoint_LocalityLbEndpoints_mutable_proximity(envoy_api_v2_endpoint_LocalityLbEndpoints *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_endpoint_LocalityLbEndpoints_proximity(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_endpoint_LocalityLbEndpoints_set_proximity(msg, sub);
  }
  return sub;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_API_V2_ENDPOINT_ENDPOINT_PROTO_UPB_H_ */
