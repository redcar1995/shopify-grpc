/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/admin/v3/certs.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_ADMIN_V3_CERTS_PROTO_UPB_H_
#define ENVOY_ADMIN_V3_CERTS_PROTO_UPB_H_

#include "upb/msg_internal.h"
#include "upb/decode.h"
#include "upb/decode_fast.h"
#include "upb/encode.h"

#include "upb/port_def.inc"

#ifdef __cplusplus
extern "C" {
#endif

struct envoy_admin_v3_Certificates;
struct envoy_admin_v3_Certificate;
struct envoy_admin_v3_CertificateDetails;
struct envoy_admin_v3_CertificateDetails_OcspDetails;
struct envoy_admin_v3_SubjectAlternateName;
typedef struct envoy_admin_v3_Certificates envoy_admin_v3_Certificates;
typedef struct envoy_admin_v3_Certificate envoy_admin_v3_Certificate;
typedef struct envoy_admin_v3_CertificateDetails envoy_admin_v3_CertificateDetails;
typedef struct envoy_admin_v3_CertificateDetails_OcspDetails envoy_admin_v3_CertificateDetails_OcspDetails;
typedef struct envoy_admin_v3_SubjectAlternateName envoy_admin_v3_SubjectAlternateName;
extern const upb_msglayout envoy_admin_v3_Certificates_msginit;
extern const upb_msglayout envoy_admin_v3_Certificate_msginit;
extern const upb_msglayout envoy_admin_v3_CertificateDetails_msginit;
extern const upb_msglayout envoy_admin_v3_CertificateDetails_OcspDetails_msginit;
extern const upb_msglayout envoy_admin_v3_SubjectAlternateName_msginit;
struct google_protobuf_Timestamp;
extern const upb_msglayout google_protobuf_Timestamp_msginit;


/* envoy.admin.v3.Certificates */

UPB_INLINE envoy_admin_v3_Certificates *envoy_admin_v3_Certificates_new(upb_arena *arena) {
  return (envoy_admin_v3_Certificates *)_upb_msg_new(&envoy_admin_v3_Certificates_msginit, arena);
}
UPB_INLINE envoy_admin_v3_Certificates *envoy_admin_v3_Certificates_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_admin_v3_Certificates *ret = envoy_admin_v3_Certificates_new(arena);
  if (!ret) return NULL;
  if (!upb_decode(buf, size, ret, &envoy_admin_v3_Certificates_msginit, arena)) return NULL;
  return ret;
}
UPB_INLINE envoy_admin_v3_Certificates *envoy_admin_v3_Certificates_parse_ex(const char *buf, size_t size,
                           const upb_extreg *extreg, int options,
                           upb_arena *arena) {
  envoy_admin_v3_Certificates *ret = envoy_admin_v3_Certificates_new(arena);
  if (!ret) return NULL;
  if (!_upb_decode(buf, size, ret, &envoy_admin_v3_Certificates_msginit, extreg, options, arena)) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char *envoy_admin_v3_Certificates_serialize(const envoy_admin_v3_Certificates *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_admin_v3_Certificates_msginit, arena, len);
}

UPB_INLINE bool envoy_admin_v3_Certificates_has_certificates(const envoy_admin_v3_Certificates *msg) { return _upb_has_submsg_nohasbit(msg, UPB_SIZE(0, 0)); }
UPB_INLINE const envoy_admin_v3_Certificate* const* envoy_admin_v3_Certificates_certificates(const envoy_admin_v3_Certificates *msg, size_t *len) { return (const envoy_admin_v3_Certificate* const*)_upb_array_accessor(msg, UPB_SIZE(0, 0), len); }

UPB_INLINE envoy_admin_v3_Certificate** envoy_admin_v3_Certificates_mutable_certificates(envoy_admin_v3_Certificates *msg, size_t *len) {
  return (envoy_admin_v3_Certificate**)_upb_array_mutable_accessor(msg, UPB_SIZE(0, 0), len);
}
UPB_INLINE envoy_admin_v3_Certificate** envoy_admin_v3_Certificates_resize_certificates(envoy_admin_v3_Certificates *msg, size_t len, upb_arena *arena) {
  return (envoy_admin_v3_Certificate**)_upb_array_resize_accessor2(msg, UPB_SIZE(0, 0), len, UPB_SIZE(2, 3), arena);
}
UPB_INLINE struct envoy_admin_v3_Certificate* envoy_admin_v3_Certificates_add_certificates(envoy_admin_v3_Certificates *msg, upb_arena *arena) {
  struct envoy_admin_v3_Certificate* sub = (struct envoy_admin_v3_Certificate*)_upb_msg_new(&envoy_admin_v3_Certificate_msginit, arena);
  bool ok = _upb_array_append_accessor2(
      msg, UPB_SIZE(0, 0), UPB_SIZE(2, 3), &sub, arena);
  if (!ok) return NULL;
  return sub;
}

/* envoy.admin.v3.Certificate */

UPB_INLINE envoy_admin_v3_Certificate *envoy_admin_v3_Certificate_new(upb_arena *arena) {
  return (envoy_admin_v3_Certificate *)_upb_msg_new(&envoy_admin_v3_Certificate_msginit, arena);
}
UPB_INLINE envoy_admin_v3_Certificate *envoy_admin_v3_Certificate_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_admin_v3_Certificate *ret = envoy_admin_v3_Certificate_new(arena);
  if (!ret) return NULL;
  if (!upb_decode(buf, size, ret, &envoy_admin_v3_Certificate_msginit, arena)) return NULL;
  return ret;
}
UPB_INLINE envoy_admin_v3_Certificate *envoy_admin_v3_Certificate_parse_ex(const char *buf, size_t size,
                           const upb_extreg *extreg, int options,
                           upb_arena *arena) {
  envoy_admin_v3_Certificate *ret = envoy_admin_v3_Certificate_new(arena);
  if (!ret) return NULL;
  if (!_upb_decode(buf, size, ret, &envoy_admin_v3_Certificate_msginit, extreg, options, arena)) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char *envoy_admin_v3_Certificate_serialize(const envoy_admin_v3_Certificate *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_admin_v3_Certificate_msginit, arena, len);
}

UPB_INLINE bool envoy_admin_v3_Certificate_has_ca_cert(const envoy_admin_v3_Certificate *msg) { return _upb_has_submsg_nohasbit(msg, UPB_SIZE(0, 0)); }
UPB_INLINE const envoy_admin_v3_CertificateDetails* const* envoy_admin_v3_Certificate_ca_cert(const envoy_admin_v3_Certificate *msg, size_t *len) { return (const envoy_admin_v3_CertificateDetails* const*)_upb_array_accessor(msg, UPB_SIZE(0, 0), len); }
UPB_INLINE bool envoy_admin_v3_Certificate_has_cert_chain(const envoy_admin_v3_Certificate *msg) { return _upb_has_submsg_nohasbit(msg, UPB_SIZE(4, 8)); }
UPB_INLINE const envoy_admin_v3_CertificateDetails* const* envoy_admin_v3_Certificate_cert_chain(const envoy_admin_v3_Certificate *msg, size_t *len) { return (const envoy_admin_v3_CertificateDetails* const*)_upb_array_accessor(msg, UPB_SIZE(4, 8), len); }

UPB_INLINE envoy_admin_v3_CertificateDetails** envoy_admin_v3_Certificate_mutable_ca_cert(envoy_admin_v3_Certificate *msg, size_t *len) {
  return (envoy_admin_v3_CertificateDetails**)_upb_array_mutable_accessor(msg, UPB_SIZE(0, 0), len);
}
UPB_INLINE envoy_admin_v3_CertificateDetails** envoy_admin_v3_Certificate_resize_ca_cert(envoy_admin_v3_Certificate *msg, size_t len, upb_arena *arena) {
  return (envoy_admin_v3_CertificateDetails**)_upb_array_resize_accessor2(msg, UPB_SIZE(0, 0), len, UPB_SIZE(2, 3), arena);
}
UPB_INLINE struct envoy_admin_v3_CertificateDetails* envoy_admin_v3_Certificate_add_ca_cert(envoy_admin_v3_Certificate *msg, upb_arena *arena) {
  struct envoy_admin_v3_CertificateDetails* sub = (struct envoy_admin_v3_CertificateDetails*)_upb_msg_new(&envoy_admin_v3_CertificateDetails_msginit, arena);
  bool ok = _upb_array_append_accessor2(
      msg, UPB_SIZE(0, 0), UPB_SIZE(2, 3), &sub, arena);
  if (!ok) return NULL;
  return sub;
}
UPB_INLINE envoy_admin_v3_CertificateDetails** envoy_admin_v3_Certificate_mutable_cert_chain(envoy_admin_v3_Certificate *msg, size_t *len) {
  return (envoy_admin_v3_CertificateDetails**)_upb_array_mutable_accessor(msg, UPB_SIZE(4, 8), len);
}
UPB_INLINE envoy_admin_v3_CertificateDetails** envoy_admin_v3_Certificate_resize_cert_chain(envoy_admin_v3_Certificate *msg, size_t len, upb_arena *arena) {
  return (envoy_admin_v3_CertificateDetails**)_upb_array_resize_accessor2(msg, UPB_SIZE(4, 8), len, UPB_SIZE(2, 3), arena);
}
UPB_INLINE struct envoy_admin_v3_CertificateDetails* envoy_admin_v3_Certificate_add_cert_chain(envoy_admin_v3_Certificate *msg, upb_arena *arena) {
  struct envoy_admin_v3_CertificateDetails* sub = (struct envoy_admin_v3_CertificateDetails*)_upb_msg_new(&envoy_admin_v3_CertificateDetails_msginit, arena);
  bool ok = _upb_array_append_accessor2(
      msg, UPB_SIZE(4, 8), UPB_SIZE(2, 3), &sub, arena);
  if (!ok) return NULL;
  return sub;
}

/* envoy.admin.v3.CertificateDetails */

UPB_INLINE envoy_admin_v3_CertificateDetails *envoy_admin_v3_CertificateDetails_new(upb_arena *arena) {
  return (envoy_admin_v3_CertificateDetails *)_upb_msg_new(&envoy_admin_v3_CertificateDetails_msginit, arena);
}
UPB_INLINE envoy_admin_v3_CertificateDetails *envoy_admin_v3_CertificateDetails_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_admin_v3_CertificateDetails *ret = envoy_admin_v3_CertificateDetails_new(arena);
  if (!ret) return NULL;
  if (!upb_decode(buf, size, ret, &envoy_admin_v3_CertificateDetails_msginit, arena)) return NULL;
  return ret;
}
UPB_INLINE envoy_admin_v3_CertificateDetails *envoy_admin_v3_CertificateDetails_parse_ex(const char *buf, size_t size,
                           const upb_extreg *extreg, int options,
                           upb_arena *arena) {
  envoy_admin_v3_CertificateDetails *ret = envoy_admin_v3_CertificateDetails_new(arena);
  if (!ret) return NULL;
  if (!_upb_decode(buf, size, ret, &envoy_admin_v3_CertificateDetails_msginit, extreg, options, arena)) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char *envoy_admin_v3_CertificateDetails_serialize(const envoy_admin_v3_CertificateDetails *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_admin_v3_CertificateDetails_msginit, arena, len);
}

UPB_INLINE upb_strview envoy_admin_v3_CertificateDetails_path(const envoy_admin_v3_CertificateDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(16, 16), upb_strview); }
UPB_INLINE upb_strview envoy_admin_v3_CertificateDetails_serial_number(const envoy_admin_v3_CertificateDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(24, 32), upb_strview); }
UPB_INLINE bool envoy_admin_v3_CertificateDetails_has_subject_alt_names(const envoy_admin_v3_CertificateDetails *msg) { return _upb_has_submsg_nohasbit(msg, UPB_SIZE(44, 72)); }
UPB_INLINE const envoy_admin_v3_SubjectAlternateName* const* envoy_admin_v3_CertificateDetails_subject_alt_names(const envoy_admin_v3_CertificateDetails *msg, size_t *len) { return (const envoy_admin_v3_SubjectAlternateName* const*)_upb_array_accessor(msg, UPB_SIZE(44, 72), len); }
UPB_INLINE uint64_t envoy_admin_v3_CertificateDetails_days_until_expiration(const envoy_admin_v3_CertificateDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(8, 8), uint64_t); }
UPB_INLINE bool envoy_admin_v3_CertificateDetails_has_valid_from(const envoy_admin_v3_CertificateDetails *msg) { return _upb_hasbit(msg, 1); }
UPB_INLINE const struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_valid_from(const envoy_admin_v3_CertificateDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(32, 48), const struct google_protobuf_Timestamp*); }
UPB_INLINE bool envoy_admin_v3_CertificateDetails_has_expiration_time(const envoy_admin_v3_CertificateDetails *msg) { return _upb_hasbit(msg, 2); }
UPB_INLINE const struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_expiration_time(const envoy_admin_v3_CertificateDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(36, 56), const struct google_protobuf_Timestamp*); }
UPB_INLINE bool envoy_admin_v3_CertificateDetails_has_ocsp_details(const envoy_admin_v3_CertificateDetails *msg) { return _upb_hasbit(msg, 3); }
UPB_INLINE const envoy_admin_v3_CertificateDetails_OcspDetails* envoy_admin_v3_CertificateDetails_ocsp_details(const envoy_admin_v3_CertificateDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(40, 64), const envoy_admin_v3_CertificateDetails_OcspDetails*); }

UPB_INLINE void envoy_admin_v3_CertificateDetails_set_path(envoy_admin_v3_CertificateDetails *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(16, 16), upb_strview) = value;
}
UPB_INLINE void envoy_admin_v3_CertificateDetails_set_serial_number(envoy_admin_v3_CertificateDetails *msg, upb_strview value) {
  *UPB_PTR_AT(msg, UPB_SIZE(24, 32), upb_strview) = value;
}
UPB_INLINE envoy_admin_v3_SubjectAlternateName** envoy_admin_v3_CertificateDetails_mutable_subject_alt_names(envoy_admin_v3_CertificateDetails *msg, size_t *len) {
  return (envoy_admin_v3_SubjectAlternateName**)_upb_array_mutable_accessor(msg, UPB_SIZE(44, 72), len);
}
UPB_INLINE envoy_admin_v3_SubjectAlternateName** envoy_admin_v3_CertificateDetails_resize_subject_alt_names(envoy_admin_v3_CertificateDetails *msg, size_t len, upb_arena *arena) {
  return (envoy_admin_v3_SubjectAlternateName**)_upb_array_resize_accessor2(msg, UPB_SIZE(44, 72), len, UPB_SIZE(2, 3), arena);
}
UPB_INLINE struct envoy_admin_v3_SubjectAlternateName* envoy_admin_v3_CertificateDetails_add_subject_alt_names(envoy_admin_v3_CertificateDetails *msg, upb_arena *arena) {
  struct envoy_admin_v3_SubjectAlternateName* sub = (struct envoy_admin_v3_SubjectAlternateName*)_upb_msg_new(&envoy_admin_v3_SubjectAlternateName_msginit, arena);
  bool ok = _upb_array_append_accessor2(
      msg, UPB_SIZE(44, 72), UPB_SIZE(2, 3), &sub, arena);
  if (!ok) return NULL;
  return sub;
}
UPB_INLINE void envoy_admin_v3_CertificateDetails_set_days_until_expiration(envoy_admin_v3_CertificateDetails *msg, uint64_t value) {
  *UPB_PTR_AT(msg, UPB_SIZE(8, 8), uint64_t) = value;
}
UPB_INLINE void envoy_admin_v3_CertificateDetails_set_valid_from(envoy_admin_v3_CertificateDetails *msg, struct google_protobuf_Timestamp* value) {
  _upb_sethas(msg, 1);
  *UPB_PTR_AT(msg, UPB_SIZE(32, 48), struct google_protobuf_Timestamp*) = value;
}
UPB_INLINE struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_mutable_valid_from(envoy_admin_v3_CertificateDetails *msg, upb_arena *arena) {
  struct google_protobuf_Timestamp* sub = (struct google_protobuf_Timestamp*)envoy_admin_v3_CertificateDetails_valid_from(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Timestamp*)_upb_msg_new(&google_protobuf_Timestamp_msginit, arena);
    if (!sub) return NULL;
    envoy_admin_v3_CertificateDetails_set_valid_from(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_admin_v3_CertificateDetails_set_expiration_time(envoy_admin_v3_CertificateDetails *msg, struct google_protobuf_Timestamp* value) {
  _upb_sethas(msg, 2);
  *UPB_PTR_AT(msg, UPB_SIZE(36, 56), struct google_protobuf_Timestamp*) = value;
}
UPB_INLINE struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_mutable_expiration_time(envoy_admin_v3_CertificateDetails *msg, upb_arena *arena) {
  struct google_protobuf_Timestamp* sub = (struct google_protobuf_Timestamp*)envoy_admin_v3_CertificateDetails_expiration_time(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Timestamp*)_upb_msg_new(&google_protobuf_Timestamp_msginit, arena);
    if (!sub) return NULL;
    envoy_admin_v3_CertificateDetails_set_expiration_time(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_admin_v3_CertificateDetails_set_ocsp_details(envoy_admin_v3_CertificateDetails *msg, envoy_admin_v3_CertificateDetails_OcspDetails* value) {
  _upb_sethas(msg, 3);
  *UPB_PTR_AT(msg, UPB_SIZE(40, 64), envoy_admin_v3_CertificateDetails_OcspDetails*) = value;
}
UPB_INLINE struct envoy_admin_v3_CertificateDetails_OcspDetails* envoy_admin_v3_CertificateDetails_mutable_ocsp_details(envoy_admin_v3_CertificateDetails *msg, upb_arena *arena) {
  struct envoy_admin_v3_CertificateDetails_OcspDetails* sub = (struct envoy_admin_v3_CertificateDetails_OcspDetails*)envoy_admin_v3_CertificateDetails_ocsp_details(msg);
  if (sub == NULL) {
    sub = (struct envoy_admin_v3_CertificateDetails_OcspDetails*)_upb_msg_new(&envoy_admin_v3_CertificateDetails_OcspDetails_msginit, arena);
    if (!sub) return NULL;
    envoy_admin_v3_CertificateDetails_set_ocsp_details(msg, sub);
  }
  return sub;
}

/* envoy.admin.v3.CertificateDetails.OcspDetails */

UPB_INLINE envoy_admin_v3_CertificateDetails_OcspDetails *envoy_admin_v3_CertificateDetails_OcspDetails_new(upb_arena *arena) {
  return (envoy_admin_v3_CertificateDetails_OcspDetails *)_upb_msg_new(&envoy_admin_v3_CertificateDetails_OcspDetails_msginit, arena);
}
UPB_INLINE envoy_admin_v3_CertificateDetails_OcspDetails *envoy_admin_v3_CertificateDetails_OcspDetails_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_admin_v3_CertificateDetails_OcspDetails *ret = envoy_admin_v3_CertificateDetails_OcspDetails_new(arena);
  if (!ret) return NULL;
  if (!upb_decode(buf, size, ret, &envoy_admin_v3_CertificateDetails_OcspDetails_msginit, arena)) return NULL;
  return ret;
}
UPB_INLINE envoy_admin_v3_CertificateDetails_OcspDetails *envoy_admin_v3_CertificateDetails_OcspDetails_parse_ex(const char *buf, size_t size,
                           const upb_extreg *extreg, int options,
                           upb_arena *arena) {
  envoy_admin_v3_CertificateDetails_OcspDetails *ret = envoy_admin_v3_CertificateDetails_OcspDetails_new(arena);
  if (!ret) return NULL;
  if (!_upb_decode(buf, size, ret, &envoy_admin_v3_CertificateDetails_OcspDetails_msginit, extreg, options, arena)) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char *envoy_admin_v3_CertificateDetails_OcspDetails_serialize(const envoy_admin_v3_CertificateDetails_OcspDetails *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_admin_v3_CertificateDetails_OcspDetails_msginit, arena, len);
}

UPB_INLINE bool envoy_admin_v3_CertificateDetails_OcspDetails_has_valid_from(const envoy_admin_v3_CertificateDetails_OcspDetails *msg) { return _upb_hasbit(msg, 1); }
UPB_INLINE const struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_OcspDetails_valid_from(const envoy_admin_v3_CertificateDetails_OcspDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(4, 8), const struct google_protobuf_Timestamp*); }
UPB_INLINE bool envoy_admin_v3_CertificateDetails_OcspDetails_has_expiration(const envoy_admin_v3_CertificateDetails_OcspDetails *msg) { return _upb_hasbit(msg, 2); }
UPB_INLINE const struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_OcspDetails_expiration(const envoy_admin_v3_CertificateDetails_OcspDetails *msg) { return *UPB_PTR_AT(msg, UPB_SIZE(8, 16), const struct google_protobuf_Timestamp*); }

UPB_INLINE void envoy_admin_v3_CertificateDetails_OcspDetails_set_valid_from(envoy_admin_v3_CertificateDetails_OcspDetails *msg, struct google_protobuf_Timestamp* value) {
  _upb_sethas(msg, 1);
  *UPB_PTR_AT(msg, UPB_SIZE(4, 8), struct google_protobuf_Timestamp*) = value;
}
UPB_INLINE struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_OcspDetails_mutable_valid_from(envoy_admin_v3_CertificateDetails_OcspDetails *msg, upb_arena *arena) {
  struct google_protobuf_Timestamp* sub = (struct google_protobuf_Timestamp*)envoy_admin_v3_CertificateDetails_OcspDetails_valid_from(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Timestamp*)_upb_msg_new(&google_protobuf_Timestamp_msginit, arena);
    if (!sub) return NULL;
    envoy_admin_v3_CertificateDetails_OcspDetails_set_valid_from(msg, sub);
  }
  return sub;
}
UPB_INLINE void envoy_admin_v3_CertificateDetails_OcspDetails_set_expiration(envoy_admin_v3_CertificateDetails_OcspDetails *msg, struct google_protobuf_Timestamp* value) {
  _upb_sethas(msg, 2);
  *UPB_PTR_AT(msg, UPB_SIZE(8, 16), struct google_protobuf_Timestamp*) = value;
}
UPB_INLINE struct google_protobuf_Timestamp* envoy_admin_v3_CertificateDetails_OcspDetails_mutable_expiration(envoy_admin_v3_CertificateDetails_OcspDetails *msg, upb_arena *arena) {
  struct google_protobuf_Timestamp* sub = (struct google_protobuf_Timestamp*)envoy_admin_v3_CertificateDetails_OcspDetails_expiration(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_Timestamp*)_upb_msg_new(&google_protobuf_Timestamp_msginit, arena);
    if (!sub) return NULL;
    envoy_admin_v3_CertificateDetails_OcspDetails_set_expiration(msg, sub);
  }
  return sub;
}

/* envoy.admin.v3.SubjectAlternateName */

UPB_INLINE envoy_admin_v3_SubjectAlternateName *envoy_admin_v3_SubjectAlternateName_new(upb_arena *arena) {
  return (envoy_admin_v3_SubjectAlternateName *)_upb_msg_new(&envoy_admin_v3_SubjectAlternateName_msginit, arena);
}
UPB_INLINE envoy_admin_v3_SubjectAlternateName *envoy_admin_v3_SubjectAlternateName_parse(const char *buf, size_t size,
                        upb_arena *arena) {
  envoy_admin_v3_SubjectAlternateName *ret = envoy_admin_v3_SubjectAlternateName_new(arena);
  if (!ret) return NULL;
  if (!upb_decode(buf, size, ret, &envoy_admin_v3_SubjectAlternateName_msginit, arena)) return NULL;
  return ret;
}
UPB_INLINE envoy_admin_v3_SubjectAlternateName *envoy_admin_v3_SubjectAlternateName_parse_ex(const char *buf, size_t size,
                           const upb_extreg *extreg, int options,
                           upb_arena *arena) {
  envoy_admin_v3_SubjectAlternateName *ret = envoy_admin_v3_SubjectAlternateName_new(arena);
  if (!ret) return NULL;
  if (!_upb_decode(buf, size, ret, &envoy_admin_v3_SubjectAlternateName_msginit, extreg, options, arena)) {
    return NULL;
  }
  return ret;
}
UPB_INLINE char *envoy_admin_v3_SubjectAlternateName_serialize(const envoy_admin_v3_SubjectAlternateName *msg, upb_arena *arena, size_t *len) {
  return upb_encode(msg, &envoy_admin_v3_SubjectAlternateName_msginit, arena, len);
}

typedef enum {
  envoy_admin_v3_SubjectAlternateName_name_dns = 1,
  envoy_admin_v3_SubjectAlternateName_name_uri = 2,
  envoy_admin_v3_SubjectAlternateName_name_ip_address = 3,
  envoy_admin_v3_SubjectAlternateName_name_NOT_SET = 0
} envoy_admin_v3_SubjectAlternateName_name_oneofcases;
UPB_INLINE envoy_admin_v3_SubjectAlternateName_name_oneofcases envoy_admin_v3_SubjectAlternateName_name_case(const envoy_admin_v3_SubjectAlternateName* msg) { return (envoy_admin_v3_SubjectAlternateName_name_oneofcases)*UPB_PTR_AT(msg, UPB_SIZE(8, 16), int32_t); }

UPB_INLINE bool envoy_admin_v3_SubjectAlternateName_has_dns(const envoy_admin_v3_SubjectAlternateName *msg) { return _upb_getoneofcase(msg, UPB_SIZE(8, 16)) == 1; }
UPB_INLINE upb_strview envoy_admin_v3_SubjectAlternateName_dns(const envoy_admin_v3_SubjectAlternateName *msg) { return UPB_READ_ONEOF(msg, upb_strview, UPB_SIZE(0, 0), UPB_SIZE(8, 16), 1, upb_strview_make("", strlen(""))); }
UPB_INLINE bool envoy_admin_v3_SubjectAlternateName_has_uri(const envoy_admin_v3_SubjectAlternateName *msg) { return _upb_getoneofcase(msg, UPB_SIZE(8, 16)) == 2; }
UPB_INLINE upb_strview envoy_admin_v3_SubjectAlternateName_uri(const envoy_admin_v3_SubjectAlternateName *msg) { return UPB_READ_ONEOF(msg, upb_strview, UPB_SIZE(0, 0), UPB_SIZE(8, 16), 2, upb_strview_make("", strlen(""))); }
UPB_INLINE bool envoy_admin_v3_SubjectAlternateName_has_ip_address(const envoy_admin_v3_SubjectAlternateName *msg) { return _upb_getoneofcase(msg, UPB_SIZE(8, 16)) == 3; }
UPB_INLINE upb_strview envoy_admin_v3_SubjectAlternateName_ip_address(const envoy_admin_v3_SubjectAlternateName *msg) { return UPB_READ_ONEOF(msg, upb_strview, UPB_SIZE(0, 0), UPB_SIZE(8, 16), 3, upb_strview_make("", strlen(""))); }

UPB_INLINE void envoy_admin_v3_SubjectAlternateName_set_dns(envoy_admin_v3_SubjectAlternateName *msg, upb_strview value) {
  UPB_WRITE_ONEOF(msg, upb_strview, UPB_SIZE(0, 0), value, UPB_SIZE(8, 16), 1);
}
UPB_INLINE void envoy_admin_v3_SubjectAlternateName_set_uri(envoy_admin_v3_SubjectAlternateName *msg, upb_strview value) {
  UPB_WRITE_ONEOF(msg, upb_strview, UPB_SIZE(0, 0), value, UPB_SIZE(8, 16), 2);
}
UPB_INLINE void envoy_admin_v3_SubjectAlternateName_set_ip_address(envoy_admin_v3_SubjectAlternateName *msg, upb_strview value) {
  UPB_WRITE_ONEOF(msg, upb_strview, UPB_SIZE(0, 0), value, UPB_SIZE(8, 16), 3);
}

extern const upb_msglayout_file envoy_admin_v3_certs_proto_upb_file_layout;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port_undef.inc"

#endif  /* ENVOY_ADMIN_V3_CERTS_PROTO_UPB_H_ */
