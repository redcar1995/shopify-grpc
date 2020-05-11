/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/api/v2/core/base.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/api/v2/core/base.upb.h"
#include "envoy/api/v2/core/address.upb.h"
#include "envoy/api/v2/core/backoff.upb.h"
#include "envoy/api/v2/core/http_uri.upb.h"
#include "envoy/type/percent.upb.h"
#include "envoy/type/semantic_version.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/struct.upb.h"
#include "google/protobuf/wrappers.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "udpa/annotations/status.upb.h"
#include "validate/validate.upb.h"
#include "envoy/api/v2/core/socket_option.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout_field envoy_api_v2_core_Locality__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(16, 32), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_Locality_msginit = {
  NULL,
  &envoy_api_v2_core_Locality__fields[0],
  UPB_SIZE(24, 48), 3, false,
};

static const upb_msglayout *const envoy_api_v2_core_BuildVersion_submsgs[2] = {
  &envoy_type_SemanticVersion_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_BuildVersion__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_core_BuildVersion_msginit = {
  &envoy_api_v2_core_BuildVersion_submsgs[0],
  &envoy_api_v2_core_BuildVersion__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_Extension_submsgs[1] = {
  &envoy_api_v2_core_BuildVersion_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Extension__fields[5] = {
  {1, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(12, 24), 0, 0, 9, 1},
  {3, UPB_SIZE(20, 40), 0, 0, 9, 1},
  {4, UPB_SIZE(28, 56), 0, 0, 11, 1},
  {5, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout envoy_api_v2_core_Extension_msginit = {
  &envoy_api_v2_core_Extension_submsgs[0],
  &envoy_api_v2_core_Extension__fields[0],
  UPB_SIZE(32, 64), 5, false,
};

static const upb_msglayout *const envoy_api_v2_core_Node_submsgs[5] = {
  &envoy_api_v2_core_Address_msginit,
  &envoy_api_v2_core_BuildVersion_msginit,
  &envoy_api_v2_core_Extension_msginit,
  &envoy_api_v2_core_Locality_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Node__fields[11] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(32, 64), 0, 4, 11, 1},
  {4, UPB_SIZE(36, 72), 0, 3, 11, 1},
  {5, UPB_SIZE(16, 32), 0, 0, 9, 1},
  {6, UPB_SIZE(24, 48), 0, 0, 9, 1},
  {7, UPB_SIZE(52, 104), UPB_SIZE(-61, -121), 0, 9, 1},
  {8, UPB_SIZE(52, 104), UPB_SIZE(-61, -121), 1, 11, 1},
  {9, UPB_SIZE(40, 80), 0, 2, 11, 3},
  {10, UPB_SIZE(44, 88), 0, 0, 9, 3},
  {11, UPB_SIZE(48, 96), 0, 0, 11, 3},
};

const upb_msglayout envoy_api_v2_core_Node_msginit = {
  &envoy_api_v2_core_Node_submsgs[0],
  &envoy_api_v2_core_Node__fields[0],
  UPB_SIZE(64, 128), 11, false,
};

static const upb_msglayout *const envoy_api_v2_core_Metadata_submsgs[1] = {
  &envoy_api_v2_core_Metadata_FilterMetadataEntry_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Metadata__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 4},
};

const upb_msglayout envoy_api_v2_core_Metadata_msginit = {
  &envoy_api_v2_core_Metadata_submsgs[0],
  &envoy_api_v2_core_Metadata__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_api_v2_core_Metadata_FilterMetadataEntry_submsgs[1] = {
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_Metadata_FilterMetadataEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_Metadata_FilterMetadataEntry_msginit = {
  &envoy_api_v2_core_Metadata_FilterMetadataEntry_submsgs[0],
  &envoy_api_v2_core_Metadata_FilterMetadataEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_api_v2_core_RuntimeUInt32__fields[2] = {
  {2, UPB_SIZE(0, 0), 0, 0, 13, 1},
  {3, UPB_SIZE(4, 8), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_RuntimeUInt32_msginit = {
  NULL,
  &envoy_api_v2_core_RuntimeUInt32__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_api_v2_core_RuntimeDouble__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 1, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_RuntimeDouble_msginit = {
  NULL,
  &envoy_api_v2_core_RuntimeDouble__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_RuntimeFeatureFlag_submsgs[1] = {
  &google_protobuf_BoolValue_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_RuntimeFeatureFlag__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_RuntimeFeatureFlag_msginit = {
  &envoy_api_v2_core_RuntimeFeatureFlag_submsgs[0],
  &envoy_api_v2_core_RuntimeFeatureFlag__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_api_v2_core_HeaderValue__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_HeaderValue_msginit = {
  NULL,
  &envoy_api_v2_core_HeaderValue__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_HeaderValueOption_submsgs[2] = {
  &envoy_api_v2_core_HeaderValue_msginit,
  &google_protobuf_BoolValue_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HeaderValueOption__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_core_HeaderValueOption_msginit = {
  &envoy_api_v2_core_HeaderValueOption_submsgs[0],
  &envoy_api_v2_core_HeaderValueOption__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_HeaderMap_submsgs[1] = {
  &envoy_api_v2_core_HeaderValue_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_HeaderMap__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_api_v2_core_HeaderMap_msginit = {
  &envoy_api_v2_core_HeaderMap_submsgs[0],
  &envoy_api_v2_core_HeaderMap__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout_field envoy_api_v2_core_DataSource__fields[3] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 12, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_DataSource_msginit = {
  NULL,
  &envoy_api_v2_core_DataSource__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_api_v2_core_RetryPolicy_submsgs[2] = {
  &envoy_api_v2_core_BackoffStrategy_msginit,
  &google_protobuf_UInt32Value_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_RetryPolicy__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_core_RetryPolicy_msginit = {
  &envoy_api_v2_core_RetryPolicy_submsgs[0],
  &envoy_api_v2_core_RetryPolicy__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_RemoteDataSource_submsgs[2] = {
  &envoy_api_v2_core_HttpUri_msginit,
  &envoy_api_v2_core_RetryPolicy_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_RemoteDataSource__fields[3] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {3, UPB_SIZE(12, 24), 0, 1, 11, 1},
};

const upb_msglayout envoy_api_v2_core_RemoteDataSource_msginit = {
  &envoy_api_v2_core_RemoteDataSource_submsgs[0],
  &envoy_api_v2_core_RemoteDataSource__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_api_v2_core_AsyncDataSource_submsgs[2] = {
  &envoy_api_v2_core_DataSource_msginit,
  &envoy_api_v2_core_RemoteDataSource_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_AsyncDataSource__fields[2] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
};

const upb_msglayout envoy_api_v2_core_AsyncDataSource_msginit = {
  &envoy_api_v2_core_AsyncDataSource_submsgs[0],
  &envoy_api_v2_core_AsyncDataSource__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_api_v2_core_TransportSocket_submsgs[2] = {
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_TransportSocket__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 1, 11, 1},
  {3, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_api_v2_core_TransportSocket_msginit = {
  &envoy_api_v2_core_TransportSocket_submsgs[0],
  &envoy_api_v2_core_TransportSocket__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_api_v2_core_RuntimeFractionalPercent_submsgs[1] = {
  &envoy_type_FractionalPercent_msginit,
};

static const upb_msglayout_field envoy_api_v2_core_RuntimeFractionalPercent__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_RuntimeFractionalPercent_msginit = {
  &envoy_api_v2_core_RuntimeFractionalPercent_submsgs[0],
  &envoy_api_v2_core_RuntimeFractionalPercent__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field envoy_api_v2_core_ControlPlane__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout envoy_api_v2_core_ControlPlane_msginit = {
  NULL,
  &envoy_api_v2_core_ControlPlane__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

#include "upb/port_undef.inc"

