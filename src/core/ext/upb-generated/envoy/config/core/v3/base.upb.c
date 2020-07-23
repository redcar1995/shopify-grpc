/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/core/v3/base.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/config/core/v3/base.upb.h"
#include "envoy/config/core/v3/address.upb.h"
#include "envoy/config/core/v3/backoff.upb.h"
#include "envoy/config/core/v3/http_uri.upb.h"
#include "envoy/type/v3/percent.upb.h"
#include "envoy/type/v3/semantic_version.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/struct.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout_field envoy_config_core_v3_Locality__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(16, 32), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_Locality_msginit = {
  NULL,
  &envoy_config_core_v3_Locality__fields[0],
  UPB_SIZE(24, 48), 3, false,
};

static const upb_msglayout *const envoy_config_core_v3_BuildVersion_submsgs[2] = {
  &envoy_type_v3_SemanticVersion_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_BuildVersion__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_config_core_v3_BuildVersion_msginit = {
  &envoy_config_core_v3_BuildVersion_submsgs[0],
  &envoy_config_core_v3_BuildVersion__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_Extension_submsgs[1] = {
  &envoy_config_core_v3_BuildVersion_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_Extension__fields[5] = {
  {1, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(12, 24), 0, 0, 9, 1},
  {3, UPB_SIZE(20, 40), 0, 0, 9, 1},
  {4, UPB_SIZE(28, 56), 0, 0, 11, 1},
  {5, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout envoy_config_core_v3_Extension_msginit = {
  &envoy_config_core_v3_Extension_submsgs[0],
  &envoy_config_core_v3_Extension__fields[0],
  UPB_SIZE(32, 64), 5, false,
};

static const upb_msglayout *const envoy_config_core_v3_Node_submsgs[5] = {
  &envoy_config_core_v3_Address_msginit,
  &envoy_config_core_v3_BuildVersion_msginit,
  &envoy_config_core_v3_Extension_msginit,
  &envoy_config_core_v3_Locality_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_Node__fields[10] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(24, 48), 0, 4, 11, 1},
  {4, UPB_SIZE(28, 56), 0, 3, 11, 1},
  {6, UPB_SIZE(16, 32), 0, 0, 9, 1},
  {7, UPB_SIZE(44, 88), UPB_SIZE(-53, -105), 0, 9, 1},
  {8, UPB_SIZE(44, 88), UPB_SIZE(-53, -105), 1, 11, 1},
  {9, UPB_SIZE(32, 64), 0, 2, 11, 3},
  {10, UPB_SIZE(36, 72), 0, 0, 9, 3},
  {11, UPB_SIZE(40, 80), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_core_v3_Node_msginit = {
  &envoy_config_core_v3_Node_submsgs[0],
  &envoy_config_core_v3_Node__fields[0],
  UPB_SIZE(56, 112), 10, false,
};

static const upb_msglayout *const envoy_config_core_v3_Metadata_submsgs[1] = {
  &envoy_config_core_v3_Metadata_FilterMetadataEntry_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_Metadata__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, _UPB_LABEL_MAP},
};

const upb_msglayout envoy_config_core_v3_Metadata_msginit = {
  &envoy_config_core_v3_Metadata_submsgs[0],
  &envoy_config_core_v3_Metadata__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_core_v3_Metadata_FilterMetadataEntry_submsgs[1] = {
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_Metadata_FilterMetadataEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_core_v3_Metadata_FilterMetadataEntry_msginit = {
  &envoy_config_core_v3_Metadata_FilterMetadataEntry_submsgs[0],
  &envoy_config_core_v3_Metadata_FilterMetadataEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_config_core_v3_RuntimeUInt32__fields[2] = {
  {2, UPB_SIZE(0, 0), 0, 0, 13, 1},
  {3, UPB_SIZE(4, 8), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_RuntimeUInt32_msginit = {
  NULL,
  &envoy_config_core_v3_RuntimeUInt32__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_config_core_v3_RuntimeDouble__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 1, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_RuntimeDouble_msginit = {
  NULL,
  &envoy_config_core_v3_RuntimeDouble__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_RuntimeFeatureFlag_submsgs[1] = {
  &google_protobuf_BoolValue_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_RuntimeFeatureFlag__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_RuntimeFeatureFlag_msginit = {
  &envoy_config_core_v3_RuntimeFeatureFlag_submsgs[0],
  &envoy_config_core_v3_RuntimeFeatureFlag__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_config_core_v3_HeaderValue__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_HeaderValue_msginit = {
  NULL,
  &envoy_config_core_v3_HeaderValue__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_HeaderValueOption_submsgs[2] = {
  &envoy_config_core_v3_HeaderValue_msginit,
  &google_protobuf_BoolValue_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_HeaderValueOption__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_config_core_v3_HeaderValueOption_msginit = {
  &envoy_config_core_v3_HeaderValueOption_submsgs[0],
  &envoy_config_core_v3_HeaderValueOption__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_HeaderMap_submsgs[1] = {
  &envoy_config_core_v3_HeaderValue_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_HeaderMap__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_core_v3_HeaderMap_msginit = {
  &envoy_config_core_v3_HeaderMap_submsgs[0],
  &envoy_config_core_v3_HeaderMap__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout_field envoy_config_core_v3_DataSource__fields[3] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 12, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_DataSource_msginit = {
  NULL,
  &envoy_config_core_v3_DataSource__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_config_core_v3_RetryPolicy_submsgs[2] = {
  &envoy_config_core_v3_BackoffStrategy_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_RetryPolicy__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_config_core_v3_RetryPolicy_msginit = {
  &envoy_config_core_v3_RetryPolicy_submsgs[0],
  &envoy_config_core_v3_RetryPolicy__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_RemoteDataSource_submsgs[2] = {
  &envoy_config_core_v3_HttpUri_msginit,
  &envoy_config_core_v3_RetryPolicy_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_RemoteDataSource__fields[3] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {3, UPB_SIZE(12, 24), 0, 1, 11, 1},
};

const upb_msglayout envoy_config_core_v3_RemoteDataSource_msginit = {
  &envoy_config_core_v3_RemoteDataSource_submsgs[0],
  &envoy_config_core_v3_RemoteDataSource__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_config_core_v3_AsyncDataSource_submsgs[2] = {
  &envoy_config_core_v3_DataSource_msginit,
  &envoy_config_core_v3_RemoteDataSource_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_AsyncDataSource__fields[2] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
};

const upb_msglayout envoy_config_core_v3_AsyncDataSource_msginit = {
  &envoy_config_core_v3_AsyncDataSource_submsgs[0],
  &envoy_config_core_v3_AsyncDataSource__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_TransportSocket_submsgs[1] = {
  &google_protobuf_Any_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_TransportSocket__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {3, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_config_core_v3_TransportSocket_msginit = {
  &envoy_config_core_v3_TransportSocket_submsgs[0],
  &envoy_config_core_v3_TransportSocket__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_config_core_v3_RuntimeFractionalPercent_submsgs[1] = {
  &envoy_type_v3_FractionalPercent_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_RuntimeFractionalPercent__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_RuntimeFractionalPercent_msginit = {
  &envoy_config_core_v3_RuntimeFractionalPercent_submsgs[0],
  &envoy_config_core_v3_RuntimeFractionalPercent__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_config_core_v3_ControlPlane__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_config_core_v3_ControlPlane_msginit = {
  NULL,
  &envoy_config_core_v3_ControlPlane__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

#include "upb/port_undef.inc"

