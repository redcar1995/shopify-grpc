/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/cluster/circuit_breaker.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_API_V2_CLUSTER_CIRCUIT_BREAKER_PROTO_UPB_H_
#define ENVOY_API_V2_CLUSTER_CIRCUIT_BREAKER_PROTO_UPB_H_

#include "upb/msg.h"
#include "upb/decode.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_api_v2_cluster_CircuitBreakers;
struct envoy_api_v2_cluster_CircuitBreakers_Thresholds;
typedef struct envoy_api_v2_cluster_CircuitBreakers envoy_api_v2_cluster_CircuitBreakers;
typedef struct envoy_api_v2_cluster_CircuitBreakers_Thresholds envoy_api_v2_cluster_CircuitBreakers_Thresholds;
extern const upb_msglayout envoy_api_v2_cluster_CircuitBreakers_msginit;
extern const upb_msglayout envoy_api_v2_cluster_CircuitBreakers_Thresholds_msginit;
struct google_protobuf_UInt32Value;
extern const upb_msglayout google_protobuf_UInt32Value_msginit;


/* envoy.api.v2.cluster.CircuitBreakers */

UPB_INLINE envoy_api_v2_cluster_CircuitBreakers *envoy_api_v2_cluster_CircuitBreakers_new(upb_arena *arena) {
  return (envoy_api_v2_cluster_CircuitBreakers *)_upb_msg_new(&envoy_api_v2_cluster_CircuitBreakers_msginit, arena);
}
UPB_INLINE envoy_api_v2_cluster_CircuitBreakers *envoy_api_v2_cluster_CircuitBreakers_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_cluster_CircuitBreakers *ret = envoy_api_v2_cluster_CircuitBreakers_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_cluster_CircuitBreakers_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_cluster_CircuitBreakers_serialize(const envoy_api_v2_cluster_CircuitBreakers *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_cluster_CircuitBreakers_msginit, arena, len);
}

UPB_INLINE const envoy_api_v2_cluster_CircuitBreakers_Thresholds* const* envoy_api_v2_cluster_CircuitBreakers_thresholds(const envoy_api_v2_cluster_CircuitBreakers *msg, size_t *len) { return (const envoy_api_v2_cluster_CircuitBreakers_Thresholds* const*)_upb_array_accessor(msg, UPB_SIZE(0, 0), len); }

UPB_INLINE envoy_api_v2_cluster_CircuitBreakers_Thresholds** envoy_api_v2_cluster_CircuitBreakers_mutable_thresholds(envoy_api_v2_cluster_CircuitBreakers *msg, size_t *len) {
  return (envoy_api_v2_cluster_CircuitBreakers_Thresholds**)_upb_array_mutable_accessor(msg, UPB_SIZE(0, 0), len);
}
UPB_INLINE envoy_api_v2_cluster_CircuitBreakers_Thresholds** envoy_api_v2_cluster_CircuitBreakers_resize_thresholds(envoy_api_v2_cluster_CircuitBreakers *msg, size_t len, upb_arena *arena) {
  return (envoy_api_v2_cluster_CircuitBreakers_Thresholds**)_upb_array_resize_accessor(msg, UPB_SIZE(0, 0), len, UPB_TYPE_MESSAGE, arena);
}
UPB_INLINE struct envoy_api_v2_cluster_CircuitBreakers_Thresholds* envoy_api_v2_cluster_CircuitBreakers_add_thresholds(envoy_api_v2_cluster_CircuitBreakers *msg, upb_arena *arena) {
  struct envoy_api_v2_cluster_CircuitBreakers_Thresholds* sub = (struct envoy_api_v2_cluster_CircuitBreakers_Thresholds*)_upb_msg_new(&envoy_api_v2_cluster_CircuitBreakers_Thresholds_msginit, arena);
  bool ok = _upb_array_append_accessor(
      msg, UPB_SIZE(0, 0), UPB_SIZE(4, 8), UPB_TYPE_MESSAGE, &sub, arena);
  if (!ok) return NULL;
  return sub;
}

/* envoy.api.v2.cluster.CircuitBreakers.Thresholds */

UPB_INLINE envoy_api_v2_cluster_CircuitBreakers_Thresholds *envoy_api_v2_cluster_CircuitBreakers_Thresholds_new(upb_arena *arena) {
  return (envoy_api_v2_cluster_CircuitBreakers_Thresholds *)_upb_msg_new(&envoy_api_v2_cluster_CircuitBreakers_Thresholds_msginit, arena);
}
UPB_INLINE envoy_api_v2_cluster_CircuitBreakers_Thresholds *envoy_api_v2_cluster_CircuitBreakers_Thresholds_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_cluster_CircuitBreakers_Thresholds *ret = envoy_api_v2_cluster_CircuitBreakers_Thresholds_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_cluster_CircuitBreakers_Thresholds_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_cluster_CircuitBreakers_Thresholds_serialize(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_cluster_CircuitBreakers_Thresholds_msginit, arena, len);
}

UPB_INLINE int32_t envoy_api_v2_cluster_CircuitBreakers_Thresholds_priority(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, int32_t, UPB_SIZE(0, 0)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_connections(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(12, 16)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_pending_requests(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(16, 24)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_requests(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(20, 32)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_retries(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(24, 40)); }
UPB_INLINE bool envoy_api_v2_cluster_CircuitBreakers_Thresholds_track_remaining(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, bool, UPB_SIZE(8, 8)); }
UPB_INLINE const struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_connection_pools(const envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg) { return UPB_FIELD_AT(msg, const struct google_protobuf_UInt32Value*, UPB_SIZE(28, 48)); }

UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_priority(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, int32_t value) {
  UPB_FIELD_AT(msg, int32_t, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_connections(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(12, 16)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_mutable_max_connections(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_connections(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_connections(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_pending_requests(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(16, 24)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_mutable_max_pending_requests(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_pending_requests(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_pending_requests(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_requests(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(20, 32)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_mutable_max_requests(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_requests(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_requests(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_retries(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(24, 40)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_mutable_max_retries(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_retries(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_retries(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_track_remaining(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, bool value) {
  UPB_FIELD_AT(msg, bool, UPB_SIZE(8, 8)) = value;
}
UPB_INLINE void envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_connection_pools(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, struct google_protobuf_UInt32Value* value) {
  UPB_FIELD_AT(msg, struct google_protobuf_UInt32Value*, UPB_SIZE(28, 48)) = value;
}
UPB_INLINE struct google_protobuf_UInt32Value* envoy_api_v2_cluster_CircuitBreakers_Thresholds_mutable_max_connection_pools(envoy_api_v2_cluster_CircuitBreakers_Thresholds *msg, upb_arena *arena) {
  struct google_protobuf_UInt32Value* sub = (struct google_protobuf_UInt32Value*)envoy_api_v2_cluster_CircuitBreakers_Thresholds_max_connection_pools(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_UInt32Value*)_upb_msg_new(&google_protobuf_UInt32Value_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_cluster_CircuitBreakers_Thresholds_set_max_connection_pools(msg, sub);
  }
  return sub;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_API_V2_CLUSTER_CIRCUIT_BREAKER_PROTO_UPB_H_ */
