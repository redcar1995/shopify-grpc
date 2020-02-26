/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/filter/accesslog/v2/accesslog.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/config/filter/accesslog/v2/accesslog.upb.h"
#include "envoy/api/v2/core/base.upb.h"
#include "envoy/api/v2/route/route_components.upb.h"
#include "envoy/type/percent.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/struct.upb.h"
#include "udpa/annotations/migrate.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_config_filter_accesslog_v2_AccessLog_submsgs[3] = {
  &envoy_config_filter_accesslog_v2_AccessLogFilter_msginit,
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_AccessLog__fields[4] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {3, UPB_SIZE(12, 24), UPB_SIZE(-17, -33), 2, 11, 1},
  {4, UPB_SIZE(12, 24), UPB_SIZE(-17, -33), 1, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_AccessLog_msginit = {
  &envoy_config_filter_accesslog_v2_AccessLog_submsgs[0],
  &envoy_config_filter_accesslog_v2_AccessLog__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_AccessLogFilter_submsgs[11] = {
  &envoy_config_filter_accesslog_v2_AndFilter_msginit,
  &envoy_config_filter_accesslog_v2_DurationFilter_msginit,
  &envoy_config_filter_accesslog_v2_ExtensionFilter_msginit,
  &envoy_config_filter_accesslog_v2_GrpcStatusFilter_msginit,
  &envoy_config_filter_accesslog_v2_HeaderFilter_msginit,
  &envoy_config_filter_accesslog_v2_NotHealthCheckFilter_msginit,
  &envoy_config_filter_accesslog_v2_OrFilter_msginit,
  &envoy_config_filter_accesslog_v2_ResponseFlagFilter_msginit,
  &envoy_config_filter_accesslog_v2_RuntimeFilter_msginit,
  &envoy_config_filter_accesslog_v2_StatusCodeFilter_msginit,
  &envoy_config_filter_accesslog_v2_TraceableFilter_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_AccessLogFilter__fields[11] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 9, 11, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 1, 11, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 5, 11, 1},
  {4, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 10, 11, 1},
  {5, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 8, 11, 1},
  {6, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 0, 11, 1},
  {7, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 6, 11, 1},
  {8, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 4, 11, 1},
  {9, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 7, 11, 1},
  {10, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 3, 11, 1},
  {11, UPB_SIZE(0, 0), UPB_SIZE(-5, -9), 2, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_AccessLogFilter_msginit = {
  &envoy_config_filter_accesslog_v2_AccessLogFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_AccessLogFilter__fields[0],
  UPB_SIZE(8, 16), 11, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_ComparisonFilter_submsgs[1] = {
  &envoy_api_v2_core_RuntimeUInt32_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_ComparisonFilter__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 14, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_ComparisonFilter_msginit = {
  &envoy_config_filter_accesslog_v2_ComparisonFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_ComparisonFilter__fields[0],
  UPB_SIZE(16, 16), 2, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_StatusCodeFilter_submsgs[1] = {
  &envoy_config_filter_accesslog_v2_ComparisonFilter_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_StatusCodeFilter__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_StatusCodeFilter_msginit = {
  &envoy_config_filter_accesslog_v2_StatusCodeFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_StatusCodeFilter__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_DurationFilter_submsgs[1] = {
  &envoy_config_filter_accesslog_v2_ComparisonFilter_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_DurationFilter__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_DurationFilter_msginit = {
  &envoy_config_filter_accesslog_v2_DurationFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_DurationFilter__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

const upb_msglayout envoy_config_filter_accesslog_v2_NotHealthCheckFilter_msginit = {
  NULL,
  NULL,
  UPB_SIZE(0, 0), 0, false,
};

const upb_msglayout envoy_config_filter_accesslog_v2_TraceableFilter_msginit = {
  NULL,
  NULL,
  UPB_SIZE(0, 0), 0, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_RuntimeFilter_submsgs[1] = {
  &envoy_type_FractionalPercent_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_RuntimeFilter__fields[3] = {
  {1, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {2, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_RuntimeFilter_msginit = {
  &envoy_config_filter_accesslog_v2_RuntimeFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_RuntimeFilter__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_AndFilter_submsgs[1] = {
  &envoy_config_filter_accesslog_v2_AccessLogFilter_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_AndFilter__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_filter_accesslog_v2_AndFilter_msginit = {
  &envoy_config_filter_accesslog_v2_AndFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_AndFilter__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_OrFilter_submsgs[1] = {
  &envoy_config_filter_accesslog_v2_AccessLogFilter_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_OrFilter__fields[1] = {
  {2, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout envoy_config_filter_accesslog_v2_OrFilter_msginit = {
  &envoy_config_filter_accesslog_v2_OrFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_OrFilter__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_HeaderFilter_submsgs[1] = {
  &envoy_api_v2_route_HeaderMatcher_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_HeaderFilter__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_HeaderFilter_msginit = {
  &envoy_config_filter_accesslog_v2_HeaderFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_HeaderFilter__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_ResponseFlagFilter__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 3},
};

const upb_msglayout envoy_config_filter_accesslog_v2_ResponseFlagFilter_msginit = {
  NULL,
  &envoy_config_filter_accesslog_v2_ResponseFlagFilter__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_GrpcStatusFilter__fields[2] = {
  {1, UPB_SIZE(4, 8), 0, 0, 14, 3},
  {2, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_GrpcStatusFilter_msginit = {
  NULL,
  &envoy_config_filter_accesslog_v2_GrpcStatusFilter__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const envoy_config_filter_accesslog_v2_ExtensionFilter_submsgs[2] = {
  &google_protobuf_Any_msginit,
  &google_protobuf_Struct_msginit,
};

static const upb_msglayout_field envoy_config_filter_accesslog_v2_ExtensionFilter__fields[3] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 1, 11, 1},
  {3, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_config_filter_accesslog_v2_ExtensionFilter_msginit = {
  &envoy_config_filter_accesslog_v2_ExtensionFilter_submsgs[0],
  &envoy_config_filter_accesslog_v2_ExtensionFilter__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

#include "upb/port_undef.inc"

