/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/core/v3/substitution_format_string.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_CONFIG_CORE_V3_SUBSTITUTION_FORMAT_STRING_PROTO_UPB_H_
#define ENVOY_CONFIG_CORE_V3_SUBSTITUTION_FORMAT_STRING_PROTO_UPB_H_

#include "upb/msg.h"
#include "upb/decode.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_config_core_v3_SubstitutionFormatString;
typedef struct envoy_config_core_v3_SubstitutionFormatString envoy_config_core_v3_SubstitutionFormatString;
extern const upb_msglayout envoy_config_core_v3_SubstitutionFormatString_msginit;
struct google_protobuf_Struct;
extern const upb_msglayout google_protobuf_Struct_msginit;


/* envoy.config.core.v3.SubstitutionFormatString */

UPB_INLINE envoy_config_core_v3_SubstitutionFormatString *envoy_config_core_v3_SubstitutionFormatString_new(upb_arena *arena) {
  return (envoy_config_core_v3_SubstitutionFormatString *)_upb_msg_new(&envoy_config_core_v3_SubstitutionFormatString_msginit, arena);
}
UPB_INLINE envoy_config_core_v3_SubstitutionFormatString *envoy_config_core_v3_SubstitutionFormatString_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_config_core_v3_SubstitutionFormatString *ret = envoy_config_core_v3_SubstitutionFormatString_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_config_core_v3_SubstitutionFormatString_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_config_core_v3_SubstitutionFormatString_serialize(const envoy_config_core_v3_SubstitutionFormatString *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_config_core_v3_SubstitutionFormatString_msginit, arena, len);
}

typedef enum {
  envoy_config_core_v3_SubstitutionFormatString_format_text_format = 1,
  envoy_config_core_v3_SubstitutionFormatString_format_json_format = 2,
  envoy_config_core_v3_SubstitutionFormatString_format_NOT_SET = 0
} envoy_config_core_v3_SubstitutionFormatString_format_oneofcases;
UPB_INLINE envoy_config_core_v3_SubstitutionFormatString_format_oneofcases envoy_config_core_v3_SubstitutionFormatString_format_case(const envoy_config_core_v3_SubstitutionFormatString* msg) { return (envoy_config_core_v3_SubstitutionFormatString_format_oneofcases)*UPB_PTR_AT(msg, UPB_SIZE(20, 40), int32_t); }

UPB_INLINE bool envoy_config_core_v3_SubstitutionFormatString_has_text_format(const envoy_config_core_v3_SubstitutionFormatString *msg) { return _upb_getoneofcase(msg, UPB_SIZE(20, 40)) == 1; }
UPB_INLINE upb_strview envoy_config_core_v3_SubstitutionFormatString_text_format(const envoy_config_core_v3_SubstitutionFormatString *msg) { return UPB_READ_ONEOF(msg, upb_strview, UPB_SIZE(12, 24), UPB_SIZE(20, 40), 1, upb_strview_make("", strlen(""))); }
UPB_INLINE bool envoy_config_core_v3_SubstitutionFormatString_has_json_format(const envoy_config_core_v3_SubstitutionFormatString *msg) { return _upb_getoneofcase(msg, UPB_SIZE(20, 40)) == 2; }
UPB_INLINE const struct google_protobuf_Struct* envoy_config_core_v3_SubstitutionFormatString_json_format(const envoy_config_core_v3_SubstitutionFormatString *msg) { return UPB_READ_ONEOF(msg, const struct google_protobuf_Struct*, UPB_SIZE(12, 24), UPB_SIZE(20, 40), 2, NULL); }
UPB_INLINE bool envoy_config_core_v3_SubstitutionFormatString_omit_empty_values(const envoy_config_core_v3_SubstitutionFormatString *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(0, 0), bool); }
UPB_INLINE upb_strview envoy_config_core_v3_SubstitutionFormatString_content_type(const envoy_config_core_v3_SubstitutionFormatString *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(4, 8), upb_strview); }

UPB_INLINE void envoy_config_core_v3_SubstitutionFormatString_set_text_format(envoy_config_core_v3_SubstitutionFormatString *msg, upb_strview value) {
  UPB_WRITE_ONEOF(msg, upb_strview, UPB_SIZE(12, 24), value, UPB_SIZE(20, 40), 1);
}
UPB_INLINE void envoy_config_core_v3_SubstitutionFormatString_set_json_format(envoy_config_core_v3_SubstitutionFormatString *msg, struct google_protobuf_Struct* value) {
  UPB_WRITE_ONEOF(msg, struct google_protobuf_Struct*, UPB_SIZE(12, 24), value, UPB_SIZE(20, 40), 2);
}
UPB_INLINE struct google_protobuf_Struct* envoy_config_core_v3_SubstitutionFormatString_mutable_json_format(envoy_config_core_v3_SubstitutionFormatString *msg, upb_arena *arena) {
  struct google_protobuf_Struct* sub = (struct google_protobuf_Struct*)envoy_config_core_v3_SubstitutionFormatString_json_format(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Struct*)_upb_msg_new(&google_protobuf_Struct_msginit, arena);
    if (!sub) return NULL;
    envoy_config_core_v3_SubstitutionFormatString_set_json_format(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_config_core_v3_SubstitutionFormatString_set_omit_empty_values(envoy_config_core_v3_SubstitutionFormatString *msg, bool value) {
  *UPB_PTR_AT(msg, UPB_SIZE(0, 0), bool) = value;
}
UPB_INLINE void envoy_config_core_v3_SubstitutionFormatString_set_content_type(envoy_config_core_v3_SubstitutionFormatString *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(4, 8), upb_strview) = value;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_CONFIG_CORE_V3_SUBSTITUTION_FORMAT_STRING_PROTO_UPB_H_ */
