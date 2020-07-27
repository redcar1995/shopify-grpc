/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/trace/v3/http_tracer.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/config/trace/v3/http_tracer.upb.h"
#include "google/protobuf/any.upb.h"
#include "google/protobuf/struct.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_config_trace_v3_Tracing_submsgs[1] = {
  &envoy_config_trace_v3_Tracing_Http_msginit,
};

static const upb_msglayout_field envoy_config_trace_v3_Tracing__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 1},
};

const upb_msglayout envoy_config_trace_v3_Tracing_msginit = {
  &envoy_config_trace_v3_Tracing_submsgs[0],
  &envoy_config_trace_v3_Tracing__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const envoy_config_trace_v3_Tracing_Http_submsgs[1] = {
  &google_protobuf_Any_msginit,
};

static const upb_msglayout_field envoy_config_trace_v3_Tracing_Http__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {3, UPB_SIZE(8, 16), UPB_SIZE(-13, -25), 0, 11, 1},
};

const upb_msglayout envoy_config_trace_v3_Tracing_Http_msginit = {
  &envoy_config_trace_v3_Tracing_Http_submsgs[0],
  &envoy_config_trace_v3_Tracing_Http__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

#include "upb/port_undef.inc"

