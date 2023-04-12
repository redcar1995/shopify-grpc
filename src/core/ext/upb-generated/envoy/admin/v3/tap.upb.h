/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/admin/v3/tap.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_ADMIN_V3_TAP_PROTO_UPB_H_
#define ENVOY_ADMIN_V3_TAP_PROTO_UPB_H_

#include "upb/collections/array_internal.h"
#include "upb/collections/map_gencode_util.h"
#include "upb/message/accessors.h"
#include "upb/message/internal.h"
#include "upb/mini_table/enum_internal.h"
#include "upb/wire/decode.h"
#include "upb/wire/decode_fast.h"
#include "upb/wire/encode.h"

// Must be last. 
#include "upb/port/def.inc"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct envoy_admin_v3_TapRequest envoy_admin_v3_TapRequest;
extern const upb_MiniTable envoy_admin_v3_TapRequest_msg_init;
struct envoy_config_tap_v3_TapConfig;
extern const upb_MiniTable envoy_config_tap_v3_TapConfig_msg_init;



/* envoy.admin.v3.TapRequest */

UPB_INLINE envoy_admin_v3_TapRequest* envoy_admin_v3_TapRequest_new(upb_Arena* arena) {
  return (envoy_admin_v3_TapRequest*)_upb_Message_New(&envoy_admin_v3_TapRequest_msg_init, arena);
}
UPB_INLINE envoy_admin_v3_TapRequest* envoy_admin_v3_TapRequest_parse(const char* buf, size_t size, upb_Arena* arena) {
  envoy_admin_v3_TapRequest* ret = envoy_admin_v3_TapRequest_new(arena);
  if (!ret) return NULL;
  if (upb_Decode(buf, size, ret, &envoy_admin_v3_TapRequest_msg_init, NULL, 0, arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
UPB_INLINE envoy_admin_v3_TapRequest* envoy_admin_v3_TapRequest_parse_ex(const char* buf, size_t size,
                           const upb_ExtensionRegistry* extreg,
                           int options, upb_Arena* arena) {
  envoy_admin_v3_TapRequest* ret = envoy_admin_v3_TapRequest_new(arena);
  if (!ret) return NULL;
  if (upb_Decode(buf, size, ret, &envoy_admin_v3_TapRequest_msg_init, extreg, options, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char* envoy_admin_v3_TapRequest_serialize(const envoy_admin_v3_TapRequest* msg, upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)upb_Encode(msg, &envoy_admin_v3_TapRequest_msg_init, 0, arena, &ptr, len);
  return ptr;
}
UPB_INLINE char* envoy_admin_v3_TapRequest_serialize_ex(const envoy_admin_v3_TapRequest* msg, int options,
                                 upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)upb_Encode(msg, &envoy_admin_v3_TapRequest_msg_init, options, arena, &ptr, len);
  return ptr;
}
UPB_INLINE void envoy_admin_v3_TapRequest_clear_config_id(envoy_admin_v3_TapRequest* msg) {
  const upb_MiniTableField field = {1, 8, 0, kUpb_NoSub, 9, kUpb_FieldMode_Scalar | (kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _upb_Message_ClearNonExtensionField(msg, &field);
}
UPB_INLINE upb_StringView envoy_admin_v3_TapRequest_config_id(const envoy_admin_v3_TapRequest* msg) {
  upb_StringView default_val = upb_StringView_FromString("");
  upb_StringView ret;
  const upb_MiniTableField field = {1, 8, 0, kUpb_NoSub, 9, kUpb_FieldMode_Scalar | (kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _upb_Message_GetNonExtensionField(msg, &field, &default_val, &ret);
  return ret;
}
UPB_INLINE void envoy_admin_v3_TapRequest_clear_tap_config(envoy_admin_v3_TapRequest* msg) {
  const upb_MiniTableField field = {2, UPB_SIZE(4, 24), 1, 0, 11, kUpb_FieldMode_Scalar | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  _upb_Message_ClearNonExtensionField(msg, &field);
}
UPB_INLINE const struct envoy_config_tap_v3_TapConfig* envoy_admin_v3_TapRequest_tap_config(const envoy_admin_v3_TapRequest* msg) {
  const struct envoy_config_tap_v3_TapConfig* default_val = NULL;
  const struct envoy_config_tap_v3_TapConfig* ret;
  const upb_MiniTableField field = {2, UPB_SIZE(4, 24), 1, 0, 11, kUpb_FieldMode_Scalar | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  _upb_Message_GetNonExtensionField(msg, &field, &default_val, &ret);
  return ret;
}
UPB_INLINE bool envoy_admin_v3_TapRequest_has_tap_config(const envoy_admin_v3_TapRequest* msg) {
  const upb_MiniTableField field = {2, UPB_SIZE(4, 24), 1, 0, 11, kUpb_FieldMode_Scalar | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return _upb_Message_HasNonExtensionField(msg, &field);
}

UPB_INLINE void envoy_admin_v3_TapRequest_set_config_id(envoy_admin_v3_TapRequest *msg, upb_StringView value) {
  const upb_MiniTableField field = {1, 8, 0, kUpb_NoSub, 9, kUpb_FieldMode_Scalar | (kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _upb_Message_SetNonExtensionField(msg, &field, &value);
}
UPB_INLINE void envoy_admin_v3_TapRequest_set_tap_config(envoy_admin_v3_TapRequest *msg, struct envoy_config_tap_v3_TapConfig* value) {
  const upb_MiniTableField field = {2, UPB_SIZE(4, 24), 1, 0, 11, kUpb_FieldMode_Scalar | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  _upb_Message_SetNonExtensionField(msg, &field, &value);
}
UPB_INLINE struct envoy_config_tap_v3_TapConfig* envoy_admin_v3_TapRequest_mutable_tap_config(envoy_admin_v3_TapRequest* msg, upb_Arena* arena) {
  struct envoy_config_tap_v3_TapConfig* sub = (struct envoy_config_tap_v3_TapConfig*)envoy_admin_v3_TapRequest_tap_config(msg);
  if (sub == NULL) {
    sub = (struct envoy_config_tap_v3_TapConfig*)_upb_Message_New(&envoy_config_tap_v3_TapConfig_msg_init, arena);
    if (sub) envoy_admin_v3_TapRequest_set_tap_config(msg, sub);
  }
  return sub;
}

extern const upb_MiniTableFile envoy_admin_v3_tap_proto_upb_file_layout;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port/undef.inc"

#endif  /* ENVOY_ADMIN_V3_TAP_PROTO_UPB_H_ */
