/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     udpa/data/orca/v1/orca_load_report.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "udpa/data/orca/v1/orca_load_report.upb.h"
#include "validate/validate.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const udpa_data_orca_v1_OrcaLoadReport_submsgs[2] = {
  &udpa_data_orca_v1_OrcaLoadReport_RequestCostEntry_msginit,
  &udpa_data_orca_v1_OrcaLoadReport_UtilizationEntry_msginit,
};

static const upb_msglayout_field udpa_data_orca_v1_OrcaLoadReport__fields[5] = {
  {1, UPB_SIZE(0, 0), 0, 0, 1, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 1, 1},
  {3, UPB_SIZE(16, 16), 0, 0, 4, 1},
  {4, UPB_SIZE(24, 24), 0, 0, 11, 4},
  {5, UPB_SIZE(28, 32), 0, 1, 11, 4},
};

const upb_msglayout udpa_data_orca_v1_OrcaLoadReport_msginit = {
  &udpa_data_orca_v1_OrcaLoadReport_submsgs[0],
  &udpa_data_orca_v1_OrcaLoadReport__fields[0],
  UPB_SIZE(32, 40), 5, false,
};

static const upb_msglayout_field udpa_data_orca_v1_OrcaLoadReport_RequestCostEntry__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 1, 1},
};

const upb_msglayout udpa_data_orca_v1_OrcaLoadReport_RequestCostEntry_msginit = {
  NULL,
  &udpa_data_orca_v1_OrcaLoadReport_RequestCostEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout_field udpa_data_orca_v1_OrcaLoadReport_UtilizationEntry__fields[2] = {
  {1, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 1, 1},
};

const upb_msglayout udpa_data_orca_v1_OrcaLoadReport_UtilizationEntry_msginit = {
  NULL,
  &udpa_data_orca_v1_OrcaLoadReport_UtilizationEntry__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

#include "upb/port_undef.inc"

