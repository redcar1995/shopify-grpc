/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/listener/udp_listener_config.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_API_V2_LISTENER_UDP_LISTENER_CONFIG_PROTO_UPB_H_
#define ENVOY_API_V2_LISTENER_UDP_LISTENER_CONFIG_PROTO_UPB_H_

#include "upb/msg.h"
#include "upb/decode.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_api_v2_listener_UdpListenerConfig;
struct envoy_api_v2_listener_ActiveRawUdpListenerConfig;
typedef struct envoy_api_v2_listener_UdpListenerConfig envoy_api_v2_listener_UdpListenerConfig;
typedef struct envoy_api_v2_listener_ActiveRawUdpListenerConfig envoy_api_v2_listener_ActiveRawUdpListenerConfig;
extern const upb_msglayout envoy_api_v2_listener_UdpListenerConfig_msginit;
extern const upb_msglayout envoy_api_v2_listener_ActiveRawUdpListenerConfig_msginit;
struct google_protobuf_Any;
struct google_protobuf_Struct;
extern const upb_msglayout google_protobuf_Any_msginit;
extern const upb_msglayout google_protobuf_Struct_msginit;


/* envoy.api.v2.listener.UdpListenerConfig */

UPB_INLINE envoy_api_v2_listener_UdpListenerConfig *envoy_api_v2_listener_UdpListenerConfig_new(upb_arena *arena) {
  return (envoy_api_v2_listener_UdpListenerConfig *)_upb_msg_new(&envoy_api_v2_listener_UdpListenerConfig_msginit, arena);
}
UPB_INLINE envoy_api_v2_listener_UdpListenerConfig *envoy_api_v2_listener_UdpListenerConfig_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_listener_UdpListenerConfig *ret = envoy_api_v2_listener_UdpListenerConfig_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_listener_UdpListenerConfig_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_listener_UdpListenerConfig_serialize(const envoy_api_v2_listener_UdpListenerConfig *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_listener_UdpListenerConfig_msginit, arena, len);
}

typedef enum {
  envoy_api_v2_listener_UdpListenerConfig_config_type_config = 2,
  envoy_api_v2_listener_UdpListenerConfig_config_type_typed_config = 3,
  envoy_api_v2_listener_UdpListenerConfig_config_type_NOT_SET = 0
} envoy_api_v2_listener_UdpListenerConfig_config_type_oneofcases;
UPB_INLINE envoy_api_v2_listener_UdpListenerConfig_config_type_oneofcases envoy_api_v2_listener_UdpListenerConfig_config_type_case(const envoy_api_v2_listener_UdpListenerConfig* msg) { return (envoy_api_v2_listener_UdpListenerConfig_config_type_oneofcases)UPB_FIELD_AT(msg, int32_t, UPB_SIZE(12, 24)); }

UPB_INLINE upb_strview envoy_api_v2_listener_UdpListenerConfig_udp_listener_name(const envoy_api_v2_listener_UdpListenerConfig *msg) { return UPB_FIELD_AT(msg, upb_strview, UPB_SIZE(0, 0)); }
UPB_INLINE bool envoy_api_v2_listener_UdpListenerConfig_has_config(const envoy_api_v2_listener_UdpListenerConfig *msg) { return _upb_has_oneof_field(msg, UPB_SIZE(12, 24), 2); }
UPB_INLINE const struct google_protobuf_Struct* envoy_api_v2_listener_UdpListenerConfig_config(const envoy_api_v2_listener_UdpListenerConfig *msg) { return UPB_READ_ONEOF(msg, const struct google_protobuf_Struct*, UPB_SIZE(8, 16), UPB_SIZE(12, 24), 2, NULL); }
UPB_INLINE bool envoy_api_v2_listener_UdpListenerConfig_has_typed_config(const envoy_api_v2_listener_UdpListenerConfig *msg) { return _upb_has_oneof_field(msg, UPB_SIZE(12, 24), 3); }
UPB_INLINE const struct google_protobuf_Any* envoy_api_v2_listener_UdpListenerConfig_typed_config(const envoy_api_v2_listener_UdpListenerConfig *msg) { return UPB_READ_ONEOF(msg, const struct google_protobuf_Any*, UPB_SIZE(8, 16), UPB_SIZE(12, 24), 3, NULL); }

UPB_INLINE void envoy_api_v2_listener_UdpListenerConfig_set_udp_listener_name(envoy_api_v2_listener_UdpListenerConfig *msg, upb_strview value) {
  UPB_FIELD_AT(msg, upb_strview, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE void envoy_api_v2_listener_UdpListenerConfig_set_config(envoy_api_v2_listener_UdpListenerConfig *msg, struct google_protobuf_Struct* value) {
  UPB_WRITE_ONEOF(msg, struct google_protobuf_Struct*, UPB_SIZE(8, 16), value, UPB_SIZE(12, 24), 2);
}
UPB_INLINE struct google_protobuf_Struct* envoy_api_v2_listener_UdpListenerConfig_mutable_config(envoy_api_v2_listener_UdpListenerConfig *msg, upb_arena *arena) {
  struct google_protobuf_Struct* sub = (struct google_protobuf_Struct*)envoy_api_v2_listener_UdpListenerConfig_config(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Struct*)_upb_msg_new(&google_protobuf_Struct_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_listener_UdpListenerConfig_set_config(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_api_v2_listener_UdpListenerConfig_set_typed_config(envoy_api_v2_listener_UdpListenerConfig *msg, struct google_protobuf_Any* value) {
  UPB_WRITE_ONEOF(msg, struct google_protobuf_Any*, UPB_SIZE(8, 16), value, UPB_SIZE(12, 24), 3);
}
UPB_INLINE struct google_protobuf_Any* envoy_api_v2_listener_UdpListenerConfig_mutable_typed_config(envoy_api_v2_listener_UdpListenerConfig *msg, upb_arena *arena) {
  struct google_protobuf_Any* sub = (struct google_protobuf_Any*)envoy_api_v2_listener_UdpListenerConfig_typed_config(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Any*)_upb_msg_new(&google_protobuf_Any_msginit, arena);
    if (!sub) return NULL;
    envoy_api_v2_listener_UdpListenerConfig_set_typed_config(msg, sub);
  }
  return sub;
}

/* envoy.api.v2.listener.ActiveRawUdpListenerConfig */

UPB_INLINE envoy_api_v2_listener_ActiveRawUdpListenerConfig *envoy_api_v2_listener_ActiveRawUdpListenerConfig_new(upb_arena *arena) {
  return (envoy_api_v2_listener_ActiveRawUdpListenerConfig *)_upb_msg_new(&envoy_api_v2_listener_ActiveRawUdpListenerConfig_msginit, arena);
}
UPB_INLINE envoy_api_v2_listener_ActiveRawUdpListenerConfig *envoy_api_v2_listener_ActiveRawUdpListenerConfig_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_api_v2_listener_ActiveRawUdpListenerConfig *ret = envoy_api_v2_listener_ActiveRawUdpListenerConfig_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_api_v2_listener_ActiveRawUdpListenerConfig_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_api_v2_listener_ActiveRawUdpListenerConfig_serialize(const envoy_api_v2_listener_ActiveRawUdpListenerConfig *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_api_v2_listener_ActiveRawUdpListenerConfig_msginit, arena, len);
}



#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_API_V2_LISTENER_UDP_LISTENER_CONFIG_PROTO_UPB_H_ */
