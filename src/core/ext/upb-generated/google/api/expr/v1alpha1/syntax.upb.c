/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     google/api/expr/v1alpha1/syntax.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "google/api/expr/v1alpha1/syntax.upb.h"
#include "google/protobuf/duration.upb.h"
#include "google/protobuf/struct.upb.h"
#include "google/protobuf/timestamp.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const google_api_expr_v1alpha1_ParsedExpr_submsgs[2] = {
  &google_api_expr_v1alpha1_Expr_msginit,
  &google_api_expr_v1alpha1_SourceInfo_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_ParsedExpr__fields[2] = {
  {2, UPB_SIZE(0, 0), 0, 0, 11, 1},
  {3, UPB_SIZE(4, 8), 0, 1, 11, 1},
};

const upb_msglayout google_api_expr_v1alpha1_ParsedExpr_msginit = {
  &google_api_expr_v1alpha1_ParsedExpr_submsgs[0],
  &google_api_expr_v1alpha1_ParsedExpr__fields[0],
  UPB_SIZE(8, 16), 2, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_submsgs[7] = {
  &google_api_expr_v1alpha1_Constant_msginit,
  &google_api_expr_v1alpha1_Expr_Call_msginit,
  &google_api_expr_v1alpha1_Expr_Comprehension_msginit,
  &google_api_expr_v1alpha1_Expr_CreateList_msginit,
  &google_api_expr_v1alpha1_Expr_CreateStruct_msginit,
  &google_api_expr_v1alpha1_Expr_Ident_msginit,
  &google_api_expr_v1alpha1_Expr_Select_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr__fields[8] = {
  {2, UPB_SIZE(0, 0), 0, 0, 3, 1},
  {3, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 0, 11, 1},
  {4, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 5, 11, 1},
  {5, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 6, 11, 1},
  {6, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 1, 11, 1},
  {7, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 3, 11, 1},
  {8, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 4, 11, 1},
  {9, UPB_SIZE(8, 8), UPB_SIZE(-13, -17), 2, 11, 1},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_msginit = {
  &google_api_expr_v1alpha1_Expr_submsgs[0],
  &google_api_expr_v1alpha1_Expr__fields[0],
  UPB_SIZE(16, 24), 8, false,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_Ident__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_Ident_msginit = {
  NULL,
  &google_api_expr_v1alpha1_Expr_Ident__fields[0],
  UPB_SIZE(8, 16), 1, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_Select_submsgs[1] = {
  &google_api_expr_v1alpha1_Expr_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_Select__fields[3] = {
  {1, UPB_SIZE(12, 24), 0, 0, 11, 1},
  {2, UPB_SIZE(4, 8), 0, 0, 9, 1},
  {3, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_Select_msginit = {
  &google_api_expr_v1alpha1_Expr_Select_submsgs[0],
  &google_api_expr_v1alpha1_Expr_Select__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_Call_submsgs[2] = {
  &google_api_expr_v1alpha1_Expr_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_Call__fields[3] = {
  {1, UPB_SIZE(8, 16), 0, 0, 11, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {3, UPB_SIZE(12, 24), 0, 0, 11, 3},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_Call_msginit = {
  &google_api_expr_v1alpha1_Expr_Call_submsgs[0],
  &google_api_expr_v1alpha1_Expr_Call__fields[0],
  UPB_SIZE(16, 32), 3, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_CreateList_submsgs[1] = {
  &google_api_expr_v1alpha1_Expr_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_CreateList__fields[1] = {
  {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_CreateList_msginit = {
  &google_api_expr_v1alpha1_Expr_CreateList_submsgs[0],
  &google_api_expr_v1alpha1_Expr_CreateList__fields[0],
  UPB_SIZE(4, 8), 1, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_CreateStruct_submsgs[1] = {
  &google_api_expr_v1alpha1_Expr_CreateStruct_Entry_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_CreateStruct__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 11, 3},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_CreateStruct_msginit = {
  &google_api_expr_v1alpha1_Expr_CreateStruct_submsgs[0],
  &google_api_expr_v1alpha1_Expr_CreateStruct__fields[0],
  UPB_SIZE(16, 32), 2, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_CreateStruct_Entry_submsgs[2] = {
  &google_api_expr_v1alpha1_Expr_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_CreateStruct_Entry__fields[4] = {
  {1, UPB_SIZE(0, 0), 0, 0, 3, 1},
  {2, UPB_SIZE(12, 16), UPB_SIZE(-21, -33), 0, 9, 1},
  {3, UPB_SIZE(12, 16), UPB_SIZE(-21, -33), 0, 11, 1},
  {4, UPB_SIZE(8, 8), 0, 0, 11, 1},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_CreateStruct_Entry_msginit = {
  &google_api_expr_v1alpha1_Expr_CreateStruct_Entry_submsgs[0],
  &google_api_expr_v1alpha1_Expr_CreateStruct_Entry__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Expr_Comprehension_submsgs[5] = {
  &google_api_expr_v1alpha1_Expr_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Expr_Comprehension__fields[7] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(16, 32), 0, 0, 11, 1},
  {3, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {4, UPB_SIZE(20, 40), 0, 0, 11, 1},
  {5, UPB_SIZE(24, 48), 0, 0, 11, 1},
  {6, UPB_SIZE(28, 56), 0, 0, 11, 1},
  {7, UPB_SIZE(32, 64), 0, 0, 11, 1},
};

const upb_msglayout google_api_expr_v1alpha1_Expr_Comprehension_msginit = {
  &google_api_expr_v1alpha1_Expr_Comprehension_submsgs[0],
  &google_api_expr_v1alpha1_Expr_Comprehension__fields[0],
  UPB_SIZE(40, 80), 7, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_Constant_submsgs[2] = {
  &google_protobuf_Duration_msginit,
  &google_protobuf_Timestamp_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_Constant__fields[9] = {
  {1, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 14, 1},
  {2, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 8, 1},
  {3, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 3, 1},
  {4, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 4, 1},
  {5, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 1, 1},
  {6, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 9, 1},
  {7, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 12, 1},
  {8, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 0, 11, 1},
  {9, UPB_SIZE(0, 0), UPB_SIZE(-9, -17), 1, 11, 1},
};

const upb_msglayout google_api_expr_v1alpha1_Constant_msginit = {
  &google_api_expr_v1alpha1_Constant_submsgs[0],
  &google_api_expr_v1alpha1_Constant__fields[0],
  UPB_SIZE(16, 32), 9, false,
};

static const upb_msglayout *const google_api_expr_v1alpha1_SourceInfo_submsgs[1] = {
  &google_api_expr_v1alpha1_SourceInfo_PositionsEntry_msginit,
};

static const upb_msglayout_field google_api_expr_v1alpha1_SourceInfo__fields[4] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {3, UPB_SIZE(16, 32), 0, 0, 5, 3},
  {4, UPB_SIZE(20, 40), 0, 0, 11, 3},
};

const upb_msglayout google_api_expr_v1alpha1_SourceInfo_msginit = {
  &google_api_expr_v1alpha1_SourceInfo_submsgs[0],
  &google_api_expr_v1alpha1_SourceInfo__fields[0],
  UPB_SIZE(24, 48), 4, false,
};

static const upb_msglayout_field google_api_expr_v1alpha1_SourceInfo_PositionsEntry__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 3, 1},
  {2, UPB_SIZE(8, 8), 0, 0, 5, 1},
};

const upb_msglayout google_api_expr_v1alpha1_SourceInfo_PositionsEntry_msginit = {
  NULL,
  &google_api_expr_v1alpha1_SourceInfo_PositionsEntry__fields[0],
  UPB_SIZE(16, 16), 2, false,
};

static const upb_msglayout_field google_api_expr_v1alpha1_SourcePosition__fields[4] = {
  {1, UPB_SIZE(12, 16), 0, 0, 9, 1},
  {2, UPB_SIZE(0, 0), 0, 0, 5, 1},
  {3, UPB_SIZE(4, 4), 0, 0, 5, 1},
  {4, UPB_SIZE(8, 8), 0, 0, 5, 1},
};

const upb_msglayout google_api_expr_v1alpha1_SourcePosition_msginit = {
  NULL,
  &google_api_expr_v1alpha1_SourcePosition__fields[0],
  UPB_SIZE(24, 32), 4, false,
};

#include "upb/port_undef.inc"

