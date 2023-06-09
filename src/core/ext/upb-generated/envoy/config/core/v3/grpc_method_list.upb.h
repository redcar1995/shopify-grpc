/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/core/v3/grpc_method_list.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_CONFIG_CORE_V3_GRPC_METHOD_LIST_PROTO_UPB_H_
#define ENVOY_CONFIG_CORE_V3_GRPC_METHOD_LIST_PROTO_UPB_H_

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

typedef struct envoy_config_core_v3_GrpcMethodList envoy_config_core_v3_GrpcMethodList;
typedef struct envoy_config_core_v3_GrpcMethodList_Service envoy_config_core_v3_GrpcMethodList_Service;
extern const upb_MiniTable envoy_config_core_v3_GrpcMethodList_msg_init;
extern const upb_MiniTable envoy_config_core_v3_GrpcMethodList_Service_msg_init;



/* envoy.config.core.v3.GrpcMethodList */

UPB_INLINE envoy_config_core_v3_GrpcMethodList* envoy_config_core_v3_GrpcMethodList_new(upb_Arena* arena) {
  return (envoy_config_core_v3_GrpcMethodList*)_upb_Message_New(&envoy_config_core_v3_GrpcMethodList_msg_init, arena);
}
UPB_INLINE envoy_config_core_v3_GrpcMethodList* envoy_config_core_v3_GrpcMethodList_parse(const char* buf, size_t size, upb_Arena* arena) {
  envoy_config_core_v3_GrpcMethodList* ret = envoy_config_core_v3_GrpcMethodList_new(arena);
  if (!ret) return NULL;
  if (upb_Decode(buf, size, ret, &envoy_config_core_v3_GrpcMethodList_msg_init, NULL, 0, arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
UPB_INLINE envoy_config_core_v3_GrpcMethodList* envoy_config_core_v3_GrpcMethodList_parse_ex(const char* buf, size_t size,
                           const upb_ExtensionRegistry* extreg,
                           int options, upb_Arena* arena) {
  envoy_config_core_v3_GrpcMethodList* ret = envoy_config_core_v3_GrpcMethodList_new(arena);
  if (!ret) return NULL;
  if (upb_Decode(buf, size, ret, &envoy_config_core_v3_GrpcMethodList_msg_init, extreg, options, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char* envoy_config_core_v3_GrpcMethodList_serialize(const envoy_config_core_v3_GrpcMethodList* msg, upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)upb_Encode(msg, &envoy_config_core_v3_GrpcMethodList_msg_init, 0, arena, &ptr, len);
  return ptr;
}
UPB_INLINE char* envoy_config_core_v3_GrpcMethodList_serialize_ex(const envoy_config_core_v3_GrpcMethodList* msg, int options,
                                 upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)upb_Encode(msg, &envoy_config_core_v3_GrpcMethodList_msg_init, options, arena, &ptr, len);
  return ptr;
}
UPB_INLINE void envoy_config_core_v3_GrpcMethodList_clear_services(envoy_config_core_v3_GrpcMethodList* msg) {
  const upb_MiniTableField field = {1, 0, 0, 0, 11, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  _upb_Message_ClearNonExtensionField(msg, &field);
}
UPB_INLINE const envoy_config_core_v3_GrpcMethodList_Service* const* envoy_config_core_v3_GrpcMethodList_services(const envoy_config_core_v3_GrpcMethodList* msg, size_t* size) {
  const upb_MiniTableField field = {1, 0, 0, 0, 11, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const upb_Array* arr = upb_Message_GetArray(msg, &field);
  if (arr) {
    if (size) *size = arr->size;
    return (const envoy_config_core_v3_GrpcMethodList_Service* const*)_upb_array_constptr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
UPB_INLINE bool envoy_config_core_v3_GrpcMethodList_has_services(const envoy_config_core_v3_GrpcMethodList* msg) {
  size_t size;
  envoy_config_core_v3_GrpcMethodList_services(msg, &size);
  return size != 0;
}

UPB_INLINE envoy_config_core_v3_GrpcMethodList_Service** envoy_config_core_v3_GrpcMethodList_mutable_services(envoy_config_core_v3_GrpcMethodList* msg, size_t* size) {
  upb_MiniTableField field = {1, 0, 0, 0, 11, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  upb_Array* arr = upb_Message_GetMutableArray(msg, &field);
  if (arr) {
    if (size) *size = arr->size;
    return (envoy_config_core_v3_GrpcMethodList_Service**)_upb_array_ptr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
UPB_INLINE envoy_config_core_v3_GrpcMethodList_Service** envoy_config_core_v3_GrpcMethodList_resize_services(envoy_config_core_v3_GrpcMethodList* msg, size_t size, upb_Arena* arena) {
  upb_MiniTableField field = {1, 0, 0, 0, 11, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (envoy_config_core_v3_GrpcMethodList_Service**)upb_Message_ResizeArray(msg, &field, size, arena);
}
UPB_INLINE struct envoy_config_core_v3_GrpcMethodList_Service* envoy_config_core_v3_GrpcMethodList_add_services(envoy_config_core_v3_GrpcMethodList* msg, upb_Arena* arena) {
  upb_MiniTableField field = {1, 0, 0, 0, 11, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  upb_Array* arr = upb_Message_GetOrCreateMutableArray(msg, &field, arena);
  if (!arr || !_upb_Array_ResizeUninitialized(arr, arr->size + 1, arena)) {
    return NULL;
  }
  struct envoy_config_core_v3_GrpcMethodList_Service* sub = (struct envoy_config_core_v3_GrpcMethodList_Service*)_upb_Message_New(&envoy_config_core_v3_GrpcMethodList_Service_msg_init, arena);
  if (!arr || !sub) return NULL;
  _upb_Array_Set(arr, arr->size - 1, &sub, sizeof(sub));
  return sub;
}

/* envoy.config.core.v3.GrpcMethodList.Service */

UPB_INLINE envoy_config_core_v3_GrpcMethodList_Service* envoy_config_core_v3_GrpcMethodList_Service_new(upb_Arena* arena) {
  return (envoy_config_core_v3_GrpcMethodList_Service*)_upb_Message_New(&envoy_config_core_v3_GrpcMethodList_Service_msg_init, arena);
}
UPB_INLINE envoy_config_core_v3_GrpcMethodList_Service* envoy_config_core_v3_GrpcMethodList_Service_parse(const char* buf, size_t size, upb_Arena* arena) {
  envoy_config_core_v3_GrpcMethodList_Service* ret = envoy_config_core_v3_GrpcMethodList_Service_new(arena);
  if (!ret) return NULL;
  if (upb_Decode(buf, size, ret, &envoy_config_core_v3_GrpcMethodList_Service_msg_init, NULL, 0, arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
UPB_INLINE envoy_config_core_v3_GrpcMethodList_Service* envoy_config_core_v3_GrpcMethodList_Service_parse_ex(const char* buf, size_t size,
                           const upb_ExtensionRegistry* extreg,
                           int options, upb_Arena* arena) {
  envoy_config_core_v3_GrpcMethodList_Service* ret = envoy_config_core_v3_GrpcMethodList_Service_new(arena);
  if (!ret) return NULL;
  if (upb_Decode(buf, size, ret, &envoy_config_core_v3_GrpcMethodList_Service_msg_init, extreg, options, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char* envoy_config_core_v3_GrpcMethodList_Service_serialize(const envoy_config_core_v3_GrpcMethodList_Service* msg, upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)upb_Encode(msg, &envoy_config_core_v3_GrpcMethodList_Service_msg_init, 0, arena, &ptr, len);
  return ptr;
}
UPB_INLINE char* envoy_config_core_v3_GrpcMethodList_Service_serialize_ex(const envoy_config_core_v3_GrpcMethodList_Service* msg, int options,
                                 upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)upb_Encode(msg, &envoy_config_core_v3_GrpcMethodList_Service_msg_init, options, arena, &ptr, len);
  return ptr;
}
UPB_INLINE void envoy_config_core_v3_GrpcMethodList_Service_clear_name(envoy_config_core_v3_GrpcMethodList_Service* msg) {
  const upb_MiniTableField field = {1, UPB_SIZE(4, 0), 0, kUpb_NoSub, 9, kUpb_FieldMode_Scalar | (kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _upb_Message_ClearNonExtensionField(msg, &field);
}
UPB_INLINE upb_StringView envoy_config_core_v3_GrpcMethodList_Service_name(const envoy_config_core_v3_GrpcMethodList_Service* msg) {
  upb_StringView default_val = upb_StringView_FromString("");
  upb_StringView ret;
  const upb_MiniTableField field = {1, UPB_SIZE(4, 0), 0, kUpb_NoSub, 9, kUpb_FieldMode_Scalar | (kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _upb_Message_GetNonExtensionField(msg, &field, &default_val, &ret);
  return ret;
}
UPB_INLINE void envoy_config_core_v3_GrpcMethodList_Service_clear_method_names(envoy_config_core_v3_GrpcMethodList_Service* msg) {
  const upb_MiniTableField field = {2, UPB_SIZE(0, 16), 0, kUpb_NoSub, 9, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  _upb_Message_ClearNonExtensionField(msg, &field);
}
UPB_INLINE upb_StringView const* envoy_config_core_v3_GrpcMethodList_Service_method_names(const envoy_config_core_v3_GrpcMethodList_Service* msg, size_t* size) {
  const upb_MiniTableField field = {2, UPB_SIZE(0, 16), 0, kUpb_NoSub, 9, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const upb_Array* arr = upb_Message_GetArray(msg, &field);
  if (arr) {
    if (size) *size = arr->size;
    return (upb_StringView const*)_upb_array_constptr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
UPB_INLINE bool envoy_config_core_v3_GrpcMethodList_Service_has_method_names(const envoy_config_core_v3_GrpcMethodList_Service* msg) {
  size_t size;
  envoy_config_core_v3_GrpcMethodList_Service_method_names(msg, &size);
  return size != 0;
}

UPB_INLINE void envoy_config_core_v3_GrpcMethodList_Service_set_name(envoy_config_core_v3_GrpcMethodList_Service *msg, upb_StringView value) {
  const upb_MiniTableField field = {1, UPB_SIZE(4, 0), 0, kUpb_NoSub, 9, kUpb_FieldMode_Scalar | (kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _upb_Message_SetNonExtensionField(msg, &field, &value);
}
UPB_INLINE upb_StringView* envoy_config_core_v3_GrpcMethodList_Service_mutable_method_names(envoy_config_core_v3_GrpcMethodList_Service* msg, size_t* size) {
  upb_MiniTableField field = {2, UPB_SIZE(0, 16), 0, kUpb_NoSub, 9, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  upb_Array* arr = upb_Message_GetMutableArray(msg, &field);
  if (arr) {
    if (size) *size = arr->size;
    return (upb_StringView*)_upb_array_ptr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
UPB_INLINE upb_StringView* envoy_config_core_v3_GrpcMethodList_Service_resize_method_names(envoy_config_core_v3_GrpcMethodList_Service* msg, size_t size, upb_Arena* arena) {
  upb_MiniTableField field = {2, UPB_SIZE(0, 16), 0, kUpb_NoSub, 9, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (upb_StringView*)upb_Message_ResizeArray(msg, &field, size, arena);
}
UPB_INLINE bool envoy_config_core_v3_GrpcMethodList_Service_add_method_names(envoy_config_core_v3_GrpcMethodList_Service* msg, upb_StringView val, upb_Arena* arena) {
  upb_MiniTableField field = {2, UPB_SIZE(0, 16), 0, kUpb_NoSub, 9, kUpb_FieldMode_Array | (UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  upb_Array* arr = upb_Message_GetOrCreateMutableArray(msg, &field, arena);
  if (!arr || !_upb_Array_ResizeUninitialized(arr, arr->size + 1, arena)) {
    return false;
  }
  _upb_Array_Set(arr, arr->size - 1, &val, sizeof(val));
  return true;
}

extern const upb_MiniTableFile envoy_config_core_v3_grpc_method_list_proto_upb_file_layout;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port/undef.inc"

#endif  /* ENVOY_CONFIG_CORE_V3_GRPC_METHOD_LIST_PROTO_UPB_H_ */
