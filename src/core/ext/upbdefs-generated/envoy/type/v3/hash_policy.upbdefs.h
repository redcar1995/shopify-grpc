/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/type/v3/hash_policy.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_TYPE_V3_HASH_POLICY_PROTO_UPBDEFS_H_
#define ENVOY_TYPE_V3_HASH_POLICY_PROTO_UPBDEFS_H_

#include "upb/reflection/def.h"
#include "upb/reflection/def_pool_internal.h"
#include "upb/port/def.inc"
#ifdef __cplusplus
extern "C" {
#endif

#include "upb/reflection/def.h"

#include "upb/port/def.inc"

extern _upb_DefPool_Init envoy_type_v3_hash_policy_proto_upbdefinit;

UPB_INLINE const upb_MessageDef *envoy_type_v3_HashPolicy_getmsgdef(upb_DefPool *s) {
  _upb_DefPool_LoadDefInit(s, &envoy_type_v3_hash_policy_proto_upbdefinit);
  return upb_DefPool_FindMessageByName(s, "envoy.type.v3.HashPolicy");
}

UPB_INLINE const upb_MessageDef *envoy_type_v3_HashPolicy_SourceIp_getmsgdef(upb_DefPool *s) {
  _upb_DefPool_LoadDefInit(s, &envoy_type_v3_hash_policy_proto_upbdefinit);
  return upb_DefPool_FindMessageByName(s, "envoy.type.v3.HashPolicy.SourceIp");
}

UPB_INLINE const upb_MessageDef *envoy_type_v3_HashPolicy_FilterState_getmsgdef(upb_DefPool *s) {
  _upb_DefPool_LoadDefInit(s, &envoy_type_v3_hash_policy_proto_upbdefinit);
  return upb_DefPool_FindMessageByName(s, "envoy.type.v3.HashPolicy.FilterState");
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port/undef.inc"

#endif  /* ENVOY_TYPE_V3_HASH_POLICY_PROTO_UPBDEFS_H_ */
