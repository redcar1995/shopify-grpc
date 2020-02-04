/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/type/range.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_TYPE_RANGE_PROTO_UPB_H_
#define ENVOY_TYPE_RANGE_PROTO_UPB_H_

#include "upb/msg.h"
#include "upb/decode.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_type_Int64Range;
struct envoy_type_DoubleRange;
typedef struct envoy_type_Int64Range envoy_type_Int64Range;
typedef struct envoy_type_DoubleRange envoy_type_DoubleRange;
extern const upb_msglayout envoy_type_Int64Range_msginit;
extern const upb_msglayout envoy_type_DoubleRange_msginit;


/* envoy.type.Int64Range */

UPB_INLINE envoy_type_Int64Range *envoy_type_Int64Range_new(upb_arena *arena) {
  return (envoy_type_Int64Range *)_upb_msg_new(&envoy_type_Int64Range_msginit, arena);
}
UPB_INLINE envoy_type_Int64Range *envoy_type_Int64Range_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_type_Int64Range *ret = envoy_type_Int64Range_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_type_Int64Range_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_type_Int64Range_serialize(const envoy_type_Int64Range *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_type_Int64Range_msginit, arena, len);
}

UPB_INLINE int64_t envoy_type_Int64Range_start(const envoy_type_Int64Range *msg) { return UPB_FIELD_AT(msg, int64_t, UPB_SIZE(0, 0)); }
UPB_INLINE int64_t envoy_type_Int64Range_end(const envoy_type_Int64Range *msg) { return UPB_FIELD_AT(msg, int64_t, UPB_SIZE(8, 8)); }

UPB_INLINE void envoy_type_Int64Range_set_start(envoy_type_Int64Range *msg, int64_t value) {
  UPB_FIELD_AT(msg, int64_t, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE void envoy_type_Int64Range_set_end(envoy_type_Int64Range *msg, int64_t value) {
  UPB_FIELD_AT(msg, int64_t, UPB_SIZE(8, 8)) = value;
}

/* envoy.type.DoubleRange */

UPB_INLINE envoy_type_DoubleRange *envoy_type_DoubleRange_new(upb_arena *arena) {
  return (envoy_type_DoubleRange *)_upb_msg_new(&envoy_type_DoubleRange_msginit, arena);
}
UPB_INLINE envoy_type_DoubleRange *envoy_type_DoubleRange_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_type_DoubleRange *ret = envoy_type_DoubleRange_new(arena);
  return (ret && upb_decode(buf, size, ret, &envoy_type_DoubleRange_msginit, arena)) ? ret : NULL;
}
UPB_INLINE char *envoy_type_DoubleRange_serialize(const envoy_type_DoubleRange *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_type_DoubleRange_msginit, arena, len);
}

UPB_INLINE double envoy_type_DoubleRange_start(const envoy_type_DoubleRange *msg) { return UPB_FIELD_AT(msg, double, UPB_SIZE(0, 0)); }
UPB_INLINE double envoy_type_DoubleRange_end(const envoy_type_DoubleRange *msg) { return UPB_FIELD_AT(msg, double, UPB_SIZE(8, 8)); }

UPB_INLINE void envoy_type_DoubleRange_set_start(envoy_type_DoubleRange *msg, double value) {
  UPB_FIELD_AT(msg, double, UPB_SIZE(0, 0)) = value;
}
UPB_INLINE void envoy_type_DoubleRange_set_end(envoy_type_DoubleRange *msg, double value) {
  UPB_FIELD_AT(msg, double, UPB_SIZE(8, 8)) = value;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_TYPE_RANGE_PROTO_UPB_H_ */
