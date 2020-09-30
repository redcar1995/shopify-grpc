/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/endpoint/v3/endpoint.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include "upb/def.h"
#include "envoy/config/endpoint/v3/endpoint.upbdefs.h"

extern upb_def_init envoy_config_endpoint_v3_endpoint_components_proto_upbdefinit;
extern upb_def_init envoy_type_v3_percent_proto_upbdefinit;
extern upb_def_init google_api_annotations_proto_upbdefinit;
extern upb_def_init google_protobuf_duration_proto_upbdefinit;
extern upb_def_init google_protobuf_wrappers_proto_upbdefinit;
extern upb_def_init udpa_annotations_status_proto_upbdefinit;
extern upb_def_init udpa_annotations_versioning_proto_upbdefinit;
extern upb_def_init validate_validate_proto_upbdefinit;
extern const upb_msglayout envoy_config_endpoint_v3_ClusterLoadAssignment_msginit;
extern const upb_msglayout envoy_config_endpoint_v3_ClusterLoadAssignment_Policy_msginit;
extern const upb_msglayout envoy_config_endpoint_v3_ClusterLoadAssignment_Policy_DropOverload_msginit;
extern const upb_msglayout envoy_config_endpoint_v3_ClusterLoadAssignment_NamedEndpointsEntry_msginit;

static const upb_msglayout *layouts[4] = {
  &envoy_config_endpoint_v3_ClusterLoadAssignment_msginit,
  &envoy_config_endpoint_v3_ClusterLoadAssignment_Policy_msginit,
  &envoy_config_endpoint_v3_ClusterLoadAssignment_Policy_DropOverload_msginit,
  &envoy_config_endpoint_v3_ClusterLoadAssignment_NamedEndpointsEntry_msginit,
};

static const char descriptor[1473] = {'\n', '\'', 'e', 'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '/', 'v', 
'3', '/', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '.', 'p', 'r', 'o', 't', 'o', '\022', '\030', 'e', 'n', 'v', 'o', 'y', '.', 'c', 
'o', 'n', 'f', 'i', 'g', '.', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '.', 'v', '3', '\032', '2', 'e', 'n', 'v', 'o', 'y', '/', 
'c', 'o', 'n', 'f', 'i', 'g', '/', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '/', 'v', '3', '/', 'e', 'n', 'd', 'p', 'o', 'i', 
'n', 't', '_', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', '\033', 'e', 'n', 'v', 'o', 
'y', '/', 't', 'y', 'p', 'e', '/', 'v', '3', '/', 'p', 'e', 'r', 'c', 'e', 'n', 't', '.', 'p', 'r', 'o', 't', 'o', '\032', '\034', 
'g', 'o', 'o', 'g', 'l', 'e', '/', 'a', 'p', 'i', '/', 'a', 'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '.', 'p', 'r', 
'o', 't', 'o', '\032', '\036', 'g', 'o', 'o', 'g', 'l', 'e', '/', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '/', 'd', 'u', 'r', 'a', 
't', 'i', 'o', 'n', '.', 'p', 'r', 'o', 't', 'o', '\032', '\036', 'g', 'o', 'o', 'g', 'l', 'e', '/', 'p', 'r', 'o', 't', 'o', 'b', 
'u', 'f', '/', 'w', 'r', 'a', 'p', 'p', 'e', 'r', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', '\035', 'u', 'd', 'p', 'a', '/', 'a', 
'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 's', 't', 'a', 't', 'u', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', '!', 
'u', 'd', 'p', 'a', '/', 'a', 'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 'v', 'e', 'r', 's', 'i', 'o', 'n', 'i', 
'n', 'g', '.', 'p', 'r', 'o', 't', 'o', '\032', '\027', 'v', 'a', 'l', 'i', 'd', 'a', 't', 'e', '/', 'v', 'a', 'l', 'i', 'd', 'a', 
't', 'e', '.', 'p', 'r', 'o', 't', 'o', '\"', '\246', '\010', '\n', '\025', 'C', 'l', 'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 
's', 's', 'i', 'g', 'n', 'm', 'e', 'n', 't', '\022', '*', '\n', '\014', 'c', 'l', 'u', 's', 't', 'e', 'r', '_', 'n', 'a', 'm', 'e', 
'\030', '\001', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\013', 'c', 'l', 'u', 's', 't', 'e', 'r', 'N', 
'a', 'm', 'e', '\022', 'K', '\n', '\t', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', 's', '\030', '\002', ' ', '\003', '(', '\013', '2', '-', '.', 
'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '.', 'v', '3', '.', 
'L', 'o', 'c', 'a', 'l', 'i', 't', 'y', 'L', 'b', 'E', 'n', 'd', 'p', 'o', 'i', 'n', 't', 's', 'R', '\t', 'e', 'n', 'd', 'p', 
'o', 'i', 'n', 't', 's', '\022', 'l', '\n', '\017', 'n', 'a', 'm', 'e', 'd', '_', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', 's', '\030', 
'\005', ' ', '\003', '(', '\013', '2', 'C', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'e', 'n', 'd', 'p', 
'o', 'i', 'n', 't', '.', 'v', '3', '.', 'C', 'l', 'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 's', 's', 'i', 'g', 'n', 
'm', 'e', 'n', 't', '.', 'N', 'a', 'm', 'e', 'd', 'E', 'n', 'd', 'p', 'o', 'i', 'n', 't', 's', 'E', 'n', 't', 'r', 'y', 'R', 
'\016', 'n', 'a', 'm', 'e', 'd', 'E', 'n', 'd', 'p', 'o', 'i', 'n', 't', 's', '\022', 'N', '\n', '\006', 'p', 'o', 'l', 'i', 'c', 'y', 
'\030', '\004', ' ', '\001', '(', '\013', '2', '6', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'e', 'n', 'd', 
'p', 'o', 'i', 'n', 't', '.', 'v', '3', '.', 'C', 'l', 'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 's', 's', 'i', 'g', 
'n', 'm', 'e', 'n', 't', '.', 'P', 'o', 'l', 'i', 'c', 'y', 'R', '\006', 'p', 'o', 'l', 'i', 'c', 'y', '\032', '\303', '\004', '\n', '\006', 
'P', 'o', 'l', 'i', 'c', 'y', '\022', 'j', '\n', '\016', 'd', 'r', 'o', 'p', '_', 'o', 'v', 'e', 'r', 'l', 'o', 'a', 'd', 's', '\030', 
'\002', ' ', '\003', '(', '\013', '2', 'C', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'e', 'n', 'd', 'p', 
'o', 'i', 'n', 't', '.', 'v', '3', '.', 'C', 'l', 'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 's', 's', 'i', 'g', 'n', 
'm', 'e', 'n', 't', '.', 'P', 'o', 'l', 'i', 'c', 'y', '.', 'D', 'r', 'o', 'p', 'O', 'v', 'e', 'r', 'l', 'o', 'a', 'd', 'R', 
'\r', 'd', 'r', 'o', 'p', 'O', 'v', 'e', 'r', 'l', 'o', 'a', 'd', 's', '\022', '^', '\n', '\027', 'o', 'v', 'e', 'r', 'p', 'r', 'o', 
'v', 'i', 's', 'i', 'o', 'n', 'i', 'n', 'g', '_', 'f', 'a', 'c', 't', 'o', 'r', '\030', '\003', ' ', '\001', '(', '\013', '2', '\034', '.', 
'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 'I', 'n', 't', '3', '2', 'V', 'a', 'l', 
'u', 'e', 'B', '\007', '\372', 'B', '\004', '*', '\002', ' ', '\000', 'R', '\026', 'o', 'v', 'e', 'r', 'p', 'r', 'o', 'v', 'i', 's', 'i', 'o', 
'n', 'i', 'n', 'g', 'F', 'a', 'c', 't', 'o', 'r', '\022', 'U', '\n', '\024', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '_', 's', 't', 
'a', 'l', 'e', '_', 'a', 'f', 't', 'e', 'r', '\030', '\004', ' ', '\001', '(', '\013', '2', '\031', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 
'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'D', 'u', 'r', 'a', 't', 'i', 'o', 'n', 'B', '\010', '\372', 'B', '\005', '\252', '\001', '\002', 
'*', '\000', 'R', '\022', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', 'S', 't', 'a', 'l', 'e', 'A', 'f', 't', 'e', 'r', '\032', '\275', '\001', 
'\n', '\014', 'D', 'r', 'o', 'p', 'O', 'v', 'e', 'r', 'l', 'o', 'a', 'd', '\022', '#', '\n', '\010', 'c', 'a', 't', 'e', 'g', 'o', 'r', 
'y', '\030', '\001', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\010', 'c', 'a', 't', 'e', 'g', 'o', 'r', 
'y', '\022', 'I', '\n', '\017', 'd', 'r', 'o', 'p', '_', 'p', 'e', 'r', 'c', 'e', 'n', 't', 'a', 'g', 'e', '\030', '\002', ' ', '\001', '(', 
'\013', '2', ' ', '.', 'e', 'n', 'v', 'o', 'y', '.', 't', 'y', 'p', 'e', '.', 'v', '3', '.', 'F', 'r', 'a', 'c', 't', 'i', 'o', 
'n', 'a', 'l', 'P', 'e', 'r', 'c', 'e', 'n', 't', 'R', '\016', 'd', 'r', 'o', 'p', 'P', 'e', 'r', 'c', 'e', 'n', 't', 'a', 'g', 
'e', ':', '=', '\232', '\305', '\210', '\036', '8', '\n', '6', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'C', 'l', 
'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 's', 's', 'i', 'g', 'n', 'm', 'e', 'n', 't', '.', 'P', 'o', 'l', 'i', 'c', 
'y', '.', 'D', 'r', 'o', 'p', 'O', 'v', 'e', 'r', 'l', 'o', 'a', 'd', ':', '0', '\232', '\305', '\210', '\036', '+', '\n', ')', 'e', 'n', 
'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'C', 'l', 'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 's', 's', 
'i', 'g', 'n', 'm', 'e', 'n', 't', '.', 'P', 'o', 'l', 'i', 'c', 'y', 'J', '\004', '\010', '\001', '\020', '\002', 'J', '\004', '\010', '\005', '\020', 
'\006', 'R', '\030', 'd', 'i', 's', 'a', 'b', 'l', 'e', '_', 'o', 'v', 'e', 'r', 'p', 'r', 'o', 'v', 'i', 's', 'i', 'o', 'n', 'i', 
'n', 'g', '\032', 'e', '\n', '\023', 'N', 'a', 'm', 'e', 'd', 'E', 'n', 'd', 'p', 'o', 'i', 'n', 't', 's', 'E', 'n', 't', 'r', 'y', 
'\022', '\020', '\n', '\003', 'k', 'e', 'y', '\030', '\001', ' ', '\001', '(', '\t', 'R', '\003', 'k', 'e', 'y', '\022', '8', '\n', '\005', 'v', 'a', 'l', 
'u', 'e', '\030', '\002', ' ', '\001', '(', '\013', '2', '\"', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'e', 
'n', 'd', 'p', 'o', 'i', 'n', 't', '.', 'v', '3', '.', 'E', 'n', 'd', 'p', 'o', 'i', 'n', 't', 'R', '\005', 'v', 'a', 'l', 'u', 
'e', ':', '\002', '8', '\001', ':', ')', '\232', '\305', '\210', '\036', '$', '\n', '\"', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', 
'2', '.', 'C', 'l', 'u', 's', 't', 'e', 'r', 'L', 'o', 'a', 'd', 'A', 's', 's', 'i', 'g', 'n', 'm', 'e', 'n', 't', 'B', 'A', 
'\n', '&', 'i', 'o', '.', 'e', 'n', 'v', 'o', 'y', 'p', 'r', 'o', 'x', 'y', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 
'f', 'i', 'g', '.', 'e', 'n', 'd', 'p', 'o', 'i', 'n', 't', '.', 'v', '3', 'B', '\r', 'E', 'n', 'd', 'p', 'o', 'i', 'n', 't', 
'P', 'r', 'o', 't', 'o', 'P', '\001', '\272', '\200', '\310', '\321', '\006', '\002', '\020', '\002', 'b', '\006', 'p', 'r', 'o', 't', 'o', '3', 
};

static upb_def_init *deps[9] = {
  &envoy_config_endpoint_v3_endpoint_components_proto_upbdefinit,
  &envoy_type_v3_percent_proto_upbdefinit,
  &google_api_annotations_proto_upbdefinit,
  &google_protobuf_duration_proto_upbdefinit,
  &google_protobuf_wrappers_proto_upbdefinit,
  &udpa_annotations_status_proto_upbdefinit,
  &udpa_annotations_versioning_proto_upbdefinit,
  &validate_validate_proto_upbdefinit,
  NULL
};

upb_def_init envoy_config_endpoint_v3_endpoint_proto_upbdefinit = {
  deps,
  layouts,
  "envoy/config/endpoint/v3/endpoint.proto",
  UPB_STRVIEW_INIT(descriptor, 1473)
};
