/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/core/v3/backoff.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "envoy/config/core/v3/backoff.upb.h"
#include "google/protobuf/duration.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/annotations/versioning.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const envoy_config_core_v3_BackoffStrategy_submsgs[1] = {
  &google_protobuf_Duration_msginit,
};

static const upb_msglayout_field envoy_config_core_v3_BackoffStrategy__fields[2] = {
  {1, UPB_SIZE(4, 8), 1, 0, 11, 1},
  {2, UPB_SIZE(8, 16), 2, 0, 11, 1},
};

const upb_msglayout envoy_config_core_v3_BackoffStrategy_msginit = {
  &envoy_config_core_v3_BackoffStrategy_submsgs[0],
  &envoy_config_core_v3_BackoffStrategy__fields[0],
  UPB_SIZE(16, 24), 2, false, 255,
};

#include "upb/port_undef.inc"

