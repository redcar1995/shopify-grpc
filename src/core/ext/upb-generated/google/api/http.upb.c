/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     google/api/http.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include <stddef.h>
#include "upb/msg.h"
#include "google/api/http.upb.h"

#include "upb/port_def.inc"

static const upb_msglayout *const google_api_Http_submsgs[1] = {
  &google_api_HttpRule_msginit,
};

static const upb_msglayout_field google_api_Http__fields[2] = {
  {1, UPB_SIZE(4, 8), 0, 0, 11, 3},
  {2, UPB_SIZE(0, 0), 0, 0, 8, 1},
};

const upb_msglayout google_api_Http_msginit = {
  &google_api_Http_submsgs[0],
  &google_api_Http__fields[0],
  UPB_SIZE(8, 16), 2, false, 255,
};

static const upb_msglayout *const google_api_HttpRule_submsgs[2] = {
  &google_api_CustomHttpPattern_msginit,
  &google_api_HttpRule_msginit,
};

static const upb_msglayout_field google_api_HttpRule__fields[10] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(28, 56), UPB_SIZE(-37, -73), 0, 9, 1},
  {3, UPB_SIZE(28, 56), UPB_SIZE(-37, -73), 0, 9, 1},
  {4, UPB_SIZE(28, 56), UPB_SIZE(-37, -73), 0, 9, 1},
  {5, UPB_SIZE(28, 56), UPB_SIZE(-37, -73), 0, 9, 1},
  {6, UPB_SIZE(28, 56), UPB_SIZE(-37, -73), 0, 9, 1},
  {7, UPB_SIZE(8, 16), 0, 0, 9, 1},
  {8, UPB_SIZE(28, 56), UPB_SIZE(-37, -73), 0, 11, 1},
  {11, UPB_SIZE(24, 48), 0, 1, 11, 3},
  {12, UPB_SIZE(16, 32), 0, 0, 9, 1},
};

const upb_msglayout google_api_HttpRule_msginit = {
  &google_api_HttpRule_submsgs[0],
  &google_api_HttpRule__fields[0],
  UPB_SIZE(40, 80), 10, false, 255,
};

static const upb_msglayout_field google_api_CustomHttpPattern__fields[2] = {
  {1, UPB_SIZE(0, 0), 0, 0, 9, 1},
  {2, UPB_SIZE(8, 16), 0, 0, 9, 1},
};

const upb_msglayout google_api_CustomHttpPattern_msginit = {
  NULL,
  &google_api_CustomHttpPattern__fields[0],
  UPB_SIZE(16, 32), 2, false, 255,
};

#include "upb/port_undef.inc"

