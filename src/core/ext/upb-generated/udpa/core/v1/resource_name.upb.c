/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     udpa/core/v1/resource_name.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "udpa/core/v1/resource_name.upb.h"
#include "udpa/annotations/status.upb.h"
#include "udpa/core/v1/context_params.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const udpa_core_v1_ResourceName_submsgs[1] = {
  &udpa_core_v1_ContextParams_msginit,
};

static const upb_msglayout_field udpa_core_v1_ResourceName__fields[4] = {
  {1, UPB_SIZE(24, 48), 0, 0, 9, 3},
  {2, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {3, UPB_SIZE(12, 24), 0, 0, 9, 1},
  {4, UPB_SIZE(20, 40), 1, 0, 11, 1},
};

const upb_msglayout udpa_core_v1_ResourceName_msginit = {
  &udpa_core_v1_ResourceName_submsgs[0],
  &udpa_core_v1_ResourceName__fields[0],
  UPB_SIZE(32, 64), 4, false, 255,
};

#include "upb/port_undef.inc"

