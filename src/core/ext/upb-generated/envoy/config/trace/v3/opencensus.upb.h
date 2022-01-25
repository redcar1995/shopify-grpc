/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/trace/v3/opencensus.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_CONFIG_TRACE_V3_OPENCENSUS_PROTO_UPB_H_
#define ENVOY_CONFIG_TRACE_V3_OPENCENSUS_PROTO_UPB_H_

#include "upb/msg_internal.h"
#include "upb/decode.h"
#include "upb/decode_fast.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_config_trace_v3_OpenCensusConfig;
typedef struct envoy_config_trace_v3_OpenCensusConfig envoy_config_trace_v3_OpenCensusConfig;
extern const upb_msglayout envoy_config_trace_v3_OpenCensusConfig_msginit;
struct envoy_config_core_v3_GrpcService;
struct opencensus_proto_trace_v1_TraceConfig;
extern const upb_msglayout envoy_config_core_v3_GrpcService_msginit;
extern const upb_msglayout opencensus_proto_trace_v1_TraceConfig_msginit;

typedef enum {
  envoy_config_trace_v3_OpenCensusConfig_NONE = 0,
  envoy_config_trace_v3_OpenCensusConfig_TRACE_CONTEXT = 1,
  envoy_config_trace_v3_OpenCensusConfig_GRPC_TRACE_BIN = 2,
  envoy_config_trace_v3_OpenCensusConfig_CLOUD_TRACE_CONTEXT = 3,
  envoy_config_trace_v3_OpenCensusConfig_B3 = 4
} envoy_config_trace_v3_OpenCensusConfig_TraceContext;


/* envoy.config.trace.v3.OpenCensusConfig */

UPB_INLINE envoy_config_trace_v3_OpenCensusConfig *envoy_config_trace_v3_OpenCensusConfig_new(upb_arena *arena) {
  return (envoy_config_trace_v3_OpenCensusConfig *)_upb_msg_new(&envoy_config_trace_v3_OpenCensusConfig_msginit, arena);
}
UPB_INLINE envoy_config_trace_v3_OpenCensusConfig *envoy_config_trace_v3_OpenCensusConfig_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_config_trace_v3_OpenCensusConfig *ret = envoy_config_trace_v3_OpenCensusConfig_new(arena);
  if (!ret) return NULL;
  if (!upb_decode(buf, size, ret, &envoy_config_trace_v3_OpenCensusConfig_msginit, arena)) return NULL;
  return ret;
}
UPB_INLINE envoy_config_trace_v3_OpenCensusConfig *envoy_config_trace_v3_OpenCensusConfig_parse_ex(const char *buf, size_t size,
                           const upb_extreg *extreg, int options,
                           upb_arena *arena) {
  envoy_config_trace_v3_OpenCensusConfig *ret = envoy_config_trace_v3_OpenCensusConfig_new(arena);
  if (!ret) return NULL;
  if (!_upb_decode(buf, size, ret, &envoy_config_trace_v3_OpenCensusConfig_msginit, extreg, options, arena)) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char *envoy_config_trace_v3_OpenCensusConfig_serialize(const envoy_config_trace_v3_OpenCensusConfig *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_config_trace_v3_OpenCensusConfig_msginit, arena, len);
}

UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_has_trace_config(const envoy_config_trace_v3_OpenCensusConfig *msg) { return _upb_hasbit(msg, 1); }
UPB_INLINE const struct opencensus_proto_trace_v1_TraceConfig* envoy_config_trace_v3_OpenCensusConfig_trace_config(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(40, 72), const struct opencensus_proto_trace_v1_TraceConfig*); }
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_stdout_exporter_enabled(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(1, 1), bool); }
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_stackdriver_exporter_enabled(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(2, 2), bool); }
UPB_INLINE upb_strview envoy_config_trace_v3_OpenCensusConfig_stackdriver_project_id(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(8, 8), upb_strview); }
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_zipkin_exporter_enabled(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(3, 3), bool); }
UPB_INLINE upb_strview envoy_config_trace_v3_OpenCensusConfig_zipkin_url(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(16, 24), upb_strview); }
UPB_INLINE int32_t const* envoy_config_trace_v3_OpenCensusConfig_incoming_trace_context(const envoy_config_trace_v3_OpenCensusConfig *msg, size_t *len) { return (int32_t const*)_upb_array_accessor(msg, UPB_SIZE(52, 96), len); }
UPB_INLINE int32_t const* envoy_config_trace_v3_OpenCensusConfig_outgoing_trace_context(const envoy_config_trace_v3_OpenCensusConfig *msg, size_t *len) { return (int32_t const*)_upb_array_accessor(msg, UPB_SIZE(56, 104), len); }
UPB_INLINE upb_strview envoy_config_trace_v3_OpenCensusConfig_stackdriver_address(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(24, 40), upb_strview); }
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_ocagent_exporter_enabled(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(4, 4), bool); }
UPB_INLINE upb_strview envoy_config_trace_v3_OpenCensusConfig_ocagent_address(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(32, 56), upb_strview); }
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_has_stackdriver_grpc_service(const envoy_config_trace_v3_OpenCensusConfig *msg) { return _upb_hasbit(msg, 2); }
UPB_INLINE const struct envoy_config_core_v3_GrpcService* envoy_config_trace_v3_OpenCensusConfig_stackdriver_grpc_service(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(44, 80), const struct envoy_config_core_v3_GrpcService*); }
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_has_ocagent_grpc_service(const envoy_config_trace_v3_OpenCensusConfig *msg) { return _upb_hasbit(msg, 3); }
UPB_INLINE const struct envoy_config_core_v3_GrpcService* envoy_config_trace_v3_OpenCensusConfig_ocagent_grpc_service(const envoy_config_trace_v3_OpenCensusConfig *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(48, 88), const struct envoy_config_core_v3_GrpcService*); }

UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_trace_config(envoy_config_trace_v3_OpenCensusConfig *msg, struct opencensus_proto_trace_v1_TraceConfig* value) {
  _upb_sethas(msg, 1);
  *UPB_PTR_AT(msg, UPB_SIZE(40, 72), struct opencensus_proto_trace_v1_TraceConfig*) = value;
}
UPB_INLINE struct opencensus_proto_trace_v1_TraceConfig* envoy_config_trace_v3_OpenCensusConfig_mutable_trace_config(envoy_config_trace_v3_OpenCensusConfig *msg, upb_arena *arena) {
  struct opencensus_proto_trace_v1_TraceConfig* sub = (struct opencensus_proto_trace_v1_TraceConfig*)envoy_config_trace_v3_OpenCensusConfig_trace_config(msg);
  if (sub == NULL) {
    sub = (struct opencensus_proto_trace_v1_TraceConfig*)_upb_msg_new(&opencensus_proto_trace_v1_TraceConfig_msginit, arena);
    if (!sub) return NULL;
    envoy_config_trace_v3_OpenCensusConfig_set_trace_config(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_stdout_exporter_enabled(envoy_config_trace_v3_OpenCensusConfig *msg, bool value) {
  *UPB_PTR_AT(msg, UPB_SIZE(1, 1), bool) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_stackdriver_exporter_enabled(envoy_config_trace_v3_OpenCensusConfig *msg, bool value) {
  *UPB_PTR_AT(msg, UPB_SIZE(2, 2), bool) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_stackdriver_project_id(envoy_config_trace_v3_OpenCensusConfig *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(8, 8), upb_strview) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_zipkin_exporter_enabled(envoy_config_trace_v3_OpenCensusConfig *msg, bool value) {
  *UPB_PTR_AT(msg, UPB_SIZE(3, 3), bool) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_zipkin_url(envoy_config_trace_v3_OpenCensusConfig *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(16, 24), upb_strview) = value;
}
UPB_INLINE int32_t* envoy_config_trace_v3_OpenCensusConfig_mutable_incoming_trace_context(envoy_config_trace_v3_OpenCensusConfig *msg, size_t *len) {
  return (int32_t*)_upb_array_mutable_accessor(msg, UPB_SIZE(52, 96), len);
}
UPB_INLINE int32_t* envoy_config_trace_v3_OpenCensusConfig_resize_incoming_trace_context(envoy_config_trace_v3_OpenCensusConfig *msg, size_t len, upb_arena *arena) {
  return (int32_t*)_upb_array_resize_accessor2(msg, UPB_SIZE(52, 96), len, 2, arena);
}
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_add_incoming_trace_context(envoy_config_trace_v3_OpenCensusConfig *msg, int32_t val, upb_arena *arena) {
  return _upb_array_append_accessor2(msg, UPB_SIZE(52, 96), 2, &val,
      arena);
}
UPB_INLINE int32_t* envoy_config_trace_v3_OpenCensusConfig_mutable_outgoing_trace_context(envoy_config_trace_v3_OpenCensusConfig *msg, size_t *len) {
  return (int32_t*)_upb_array_mutable_accessor(msg, UPB_SIZE(56, 104), len);
}
UPB_INLINE int32_t* envoy_config_trace_v3_OpenCensusConfig_resize_outgoing_trace_context(envoy_config_trace_v3_OpenCensusConfig *msg, size_t len, upb_arena *arena) {
  return (int32_t*)_upb_array_resize_accessor2(msg, UPB_SIZE(56, 104), len, 2, arena);
}
UPB_INLINE bool envoy_config_trace_v3_OpenCensusConfig_add_outgoing_trace_context(envoy_config_trace_v3_OpenCensusConfig *msg, int32_t val, upb_arena *arena) {
  return _upb_array_append_accessor2(msg, UPB_SIZE(56, 104), 2, &val,
      arena);
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_stackdriver_address(envoy_config_trace_v3_OpenCensusConfig *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(24, 40), upb_strview) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_ocagent_exporter_enabled(envoy_config_trace_v3_OpenCensusConfig *msg, bool value) {
  *UPB_PTR_AT(msg, UPB_SIZE(4, 4), bool) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_ocagent_address(envoy_config_trace_v3_OpenCensusConfig *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(32, 56), upb_strview) = value;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_stackdriver_grpc_service(envoy_config_trace_v3_OpenCensusConfig *msg, struct envoy_config_core_v3_GrpcService* value) {
  _upb_sethas(msg, 2);
  *UPB_PTR_AT(msg, UPB_SIZE(44, 80), struct envoy_config_core_v3_GrpcService*) = value;
}
UPB_INLINE struct envoy_config_core_v3_GrpcService* envoy_config_trace_v3_OpenCensusConfig_mutable_stackdriver_grpc_service(envoy_config_trace_v3_OpenCensusConfig *msg, upb_arena *arena) {
  struct envoy_config_core_v3_GrpcService* sub = (struct envoy_config_core_v3_GrpcService*)envoy_config_trace_v3_OpenCensusConfig_stackdriver_grpc_service(msg);
  if (sub == NULL) {
    sub = (struct envoy_config_core_v3_GrpcService*)_upb_msg_new(&envoy_config_core_v3_GrpcService_msginit, arena);
    if (!sub) return NULL;
    envoy_config_trace_v3_OpenCensusConfig_set_stackdriver_grpc_service(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_config_trace_v3_OpenCensusConfig_set_ocagent_grpc_service(envoy_config_trace_v3_OpenCensusConfig *msg, struct envoy_config_core_v3_GrpcService* value) {
  _upb_sethas(msg, 3);
  *UPB_PTR_AT(msg, UPB_SIZE(48, 88), struct envoy_config_core_v3_GrpcService*) = value;
}
UPB_INLINE struct envoy_config_core_v3_GrpcService* envoy_config_trace_v3_OpenCensusConfig_mutable_ocagent_grpc_service(envoy_config_trace_v3_OpenCensusConfig *msg, upb_arena *arena) {
  struct envoy_config_core_v3_GrpcService* sub = (struct envoy_config_core_v3_GrpcService*)envoy_config_trace_v3_OpenCensusConfig_ocagent_grpc_service(msg);
  if (sub == NULL) {
    sub = (struct envoy_config_core_v3_GrpcService*)_upb_msg_new(&envoy_config_core_v3_GrpcService_msginit, arena);
    if (!sub) return NULL;
    envoy_config_trace_v3_OpenCensusConfig_set_ocagent_grpc_service(msg, sub);
  }
  return sub;
}

extern const upb_msglayout_file envoy_config_trace_v3_opencensus_proto_upb_file_layout;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_CONFIG_TRACE_V3_OPENCENSUS_PROTO_UPB_H_ */
