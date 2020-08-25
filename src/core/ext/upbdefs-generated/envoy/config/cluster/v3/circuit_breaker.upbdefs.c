/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/cluster/v3/circuit_breaker.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include "upb/def.h"
#include "envoy/config/cluster/v3/circuit_breaker.upbdefs.h"

extern upb_def_init envoy_config_core_v3_base_proto_upbdefinit;
extern upb_def_init envoy_type_v3_percent_proto_upbdefinit;
extern upb_def_init google_protobuf_wrappers_proto_upbdefinit;
extern upb_def_init udpa_annotations_status_proto_upbdefinit;
extern upb_def_init udpa_annotations_versioning_proto_upbdefinit;
extern upb_def_init validate_validate_proto_upbdefinit;
extern const upb_msglayout envoy_config_cluster_v3_CircuitBreakers_msginit;
extern const upb_msglayout envoy_config_cluster_v3_CircuitBreakers_Thresholds_msginit;
extern const upb_msglayout envoy_config_cluster_v3_CircuitBreakers_Thresholds_RetryBudget_msginit;

static const upb_msglayout *layouts[3] = {
  &envoy_config_cluster_v3_CircuitBreakers_msginit,
  &envoy_config_cluster_v3_CircuitBreakers_Thresholds_msginit,
  &envoy_config_cluster_v3_CircuitBreakers_Thresholds_RetryBudget_msginit,
};

static const char descriptor[1364] = {'\n', '-', 'e', 'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'c', 'l', 'u', 's', 't', 'e', 'r', '/', 'v', '3', 
'/', 'c', 'i', 'r', 'c', 'u', 'i', 't', '_', 'b', 'r', 'e', 'a', 'k', 'e', 'r', '.', 'p', 'r', 'o', 't', 'o', '\022', '\027', 'e', 
'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'l', 'u', 's', 't', 'e', 'r', '.', 'v', '3', '\032', '\037', 'e', 
'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'c', 'o', 'r', 'e', '/', 'v', '3', '/', 'b', 'a', 's', 'e', '.', 
'p', 'r', 'o', 't', 'o', '\032', '\033', 'e', 'n', 'v', 'o', 'y', '/', 't', 'y', 'p', 'e', '/', 'v', '3', '/', 'p', 'e', 'r', 'c', 
'e', 'n', 't', '.', 'p', 'r', 'o', 't', 'o', '\032', '\036', 'g', 'o', 'o', 'g', 'l', 'e', '/', 'p', 'r', 'o', 't', 'o', 'b', 'u', 
'f', '/', 'w', 'r', 'a', 'p', 'p', 'e', 'r', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', '\035', 'u', 'd', 'p', 'a', '/', 'a', 'n', 
'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 's', 't', 'a', 't', 'u', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', '!', 'u', 
'd', 'p', 'a', '/', 'a', 'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 'v', 'e', 'r', 's', 'i', 'o', 'n', 'i', 'n', 
'g', '.', 'p', 'r', 'o', 't', 'o', '\032', '\027', 'v', 'a', 'l', 'i', 'd', 'a', 't', 'e', '/', 'v', 'a', 'l', 'i', 'd', 'a', 't', 
'e', '.', 'p', 'r', 'o', 't', 'o', '\"', '\200', '\010', '\n', '\017', 'C', 'i', 'r', 'c', 'u', 'i', 't', 'B', 'r', 'e', 'a', 'k', 'e', 
'r', 's', '\022', 'S', '\n', '\n', 't', 'h', 'r', 'e', 's', 'h', 'o', 'l', 'd', 's', '\030', '\001', ' ', '\003', '(', '\013', '2', '3', '.', 
'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'l', 'u', 's', 't', 'e', 'r', '.', 'v', '3', '.', 'C', 
'i', 'r', 'c', 'u', 'i', 't', 'B', 'r', 'e', 'a', 'k', 'e', 'r', 's', '.', 'T', 'h', 'r', 'e', 's', 'h', 'o', 'l', 'd', 's', 
'R', '\n', 't', 'h', 'r', 'e', 's', 'h', 'o', 'l', 'd', 's', '\032', '\352', '\006', '\n', '\n', 'T', 'h', 'r', 'e', 's', 'h', 'o', 'l', 
'd', 's', '\022', 'K', '\n', '\010', 'p', 'r', 'i', 'o', 'r', 'i', 't', 'y', '\030', '\001', ' ', '\001', '(', '\016', '2', '%', '.', 'e', 'n', 
'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'R', 'o', 'u', 't', 'i', 'n', 
'g', 'P', 'r', 'i', 'o', 'r', 'i', 't', 'y', 'B', '\010', '\372', 'B', '\005', '\202', '\001', '\002', '\020', '\001', 'R', '\010', 'p', 'r', 'i', 'o', 
'r', 'i', 't', 'y', '\022', 'E', '\n', '\017', 'm', 'a', 'x', '_', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', 's', '\030', '\002', 
' ', '\001', '(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 'I', 
'n', 't', '3', '2', 'V', 'a', 'l', 'u', 'e', 'R', '\016', 'm', 'a', 'x', 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', 's', 
'\022', 'N', '\n', '\024', 'm', 'a', 'x', '_', 'p', 'e', 'n', 'd', 'i', 'n', 'g', '_', 'r', 'e', 'q', 'u', 'e', 's', 't', 's', '\030', 
'\003', ' ', '\001', '(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 
'I', 'n', 't', '3', '2', 'V', 'a', 'l', 'u', 'e', 'R', '\022', 'm', 'a', 'x', 'P', 'e', 'n', 'd', 'i', 'n', 'g', 'R', 'e', 'q', 
'u', 'e', 's', 't', 's', '\022', '?', '\n', '\014', 'm', 'a', 'x', '_', 'r', 'e', 'q', 'u', 'e', 's', 't', 's', '\030', '\004', ' ', '\001', 
'(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 'I', 'n', 't', 
'3', '2', 'V', 'a', 'l', 'u', 'e', 'R', '\013', 'm', 'a', 'x', 'R', 'e', 'q', 'u', 'e', 's', 't', 's', '\022', '=', '\n', '\013', 'm', 
'a', 'x', '_', 'r', 'e', 't', 'r', 'i', 'e', 's', '\030', '\005', ' ', '\001', '(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', 
'.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 'I', 'n', 't', '3', '2', 'V', 'a', 'l', 'u', 'e', 'R', '\n', 'm', 'a', 
'x', 'R', 'e', 't', 'r', 'i', 'e', 's', '\022', 'b', '\n', '\014', 'r', 'e', 't', 'r', 'y', '_', 'b', 'u', 'd', 'g', 'e', 't', '\030', 
'\010', ' ', '\001', '(', '\013', '2', '?', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'l', 'u', 's', 
't', 'e', 'r', '.', 'v', '3', '.', 'C', 'i', 'r', 'c', 'u', 'i', 't', 'B', 'r', 'e', 'a', 'k', 'e', 'r', 's', '.', 'T', 'h', 
'r', 'e', 's', 'h', 'o', 'l', 'd', 's', '.', 'R', 'e', 't', 'r', 'y', 'B', 'u', 'd', 'g', 'e', 't', 'R', '\013', 'r', 'e', 't', 
'r', 'y', 'B', 'u', 'd', 'g', 'e', 't', '\022', '\'', '\n', '\017', 't', 'r', 'a', 'c', 'k', '_', 'r', 'e', 'm', 'a', 'i', 'n', 'i', 
'n', 'g', '\030', '\006', ' ', '\001', '(', '\010', 'R', '\016', 't', 'r', 'a', 'c', 'k', 'R', 'e', 'm', 'a', 'i', 'n', 'i', 'n', 'g', '\022', 
'N', '\n', '\024', 'm', 'a', 'x', '_', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', '_', 'p', 'o', 'o', 'l', 's', '\030', '\007', 
' ', '\001', '(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 'I', 
'n', 't', '3', '2', 'V', 'a', 'l', 'u', 'e', 'R', '\022', 'm', 'a', 'x', 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', 'P', 
'o', 'o', 'l', 's', '\032', '\342', '\001', '\n', '\013', 'R', 'e', 't', 'r', 'y', 'B', 'u', 'd', 'g', 'e', 't', '\022', '=', '\n', '\016', 'b', 
'u', 'd', 'g', 'e', 't', '_', 'p', 'e', 'r', 'c', 'e', 'n', 't', '\030', '\001', ' ', '\001', '(', '\013', '2', '\026', '.', 'e', 'n', 'v', 
'o', 'y', '.', 't', 'y', 'p', 'e', '.', 'v', '3', '.', 'P', 'e', 'r', 'c', 'e', 'n', 't', 'R', '\r', 'b', 'u', 'd', 'g', 'e', 
't', 'P', 'e', 'r', 'c', 'e', 'n', 't', '\022', 'P', '\n', '\025', 'm', 'i', 'n', '_', 'r', 'e', 't', 'r', 'y', '_', 'c', 'o', 'n', 
'c', 'u', 'r', 'r', 'e', 'n', 'c', 'y', '\030', '\002', ' ', '\001', '(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 
'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'U', 'I', 'n', 't', '3', '2', 'V', 'a', 'l', 'u', 'e', 'R', '\023', 'm', 'i', 'n', 'R', 
'e', 't', 'r', 'y', 'C', 'o', 'n', 'c', 'u', 'r', 'r', 'e', 'n', 'c', 'y', ':', 'B', '\232', '\305', '\210', '\036', '=', '\n', ';', 'e', 
'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'l', 'u', 's', 't', 'e', 'r', '.', 'C', 'i', 'r', 'c', 'u', 
'i', 't', 'B', 'r', 'e', 'a', 'k', 'e', 'r', 's', '.', 'T', 'h', 'r', 'e', 's', 'h', 'o', 'l', 'd', 's', '.', 'R', 'e', 't', 
'r', 'y', 'B', 'u', 'd', 'g', 'e', 't', ':', '6', '\232', '\305', '\210', '\036', '1', '\n', '/', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 
'i', '.', 'v', '2', '.', 'c', 'l', 'u', 's', 't', 'e', 'r', '.', 'C', 'i', 'r', 'c', 'u', 'i', 't', 'B', 'r', 'e', 'a', 'k', 
'e', 'r', 's', '.', 'T', 'h', 'r', 'e', 's', 'h', 'o', 'l', 'd', 's', ':', '+', '\232', '\305', '\210', '\036', '&', '\n', '$', 'e', 'n', 
'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'l', 'u', 's', 't', 'e', 'r', '.', 'C', 'i', 'r', 'c', 'u', 'i', 
't', 'B', 'r', 'e', 'a', 'k', 'e', 'r', 's', 'B', 'F', '\n', '%', 'i', 'o', '.', 'e', 'n', 'v', 'o', 'y', 'p', 'r', 'o', 'x', 
'y', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'l', 'u', 's', 't', 'e', 'r', '.', 'v', '3', 
'B', '\023', 'C', 'i', 'r', 'c', 'u', 'i', 't', 'B', 'r', 'e', 'a', 'k', 'e', 'r', 'P', 'r', 'o', 't', 'o', 'P', '\001', '\272', '\200', 
'\310', '\321', '\006', '\002', '\020', '\002', 'b', '\006', 'p', 'r', 'o', 't', 'o', '3', 
};

static upb_def_init *deps[7] = {
  &envoy_config_core_v3_base_proto_upbdefinit,
  &envoy_type_v3_percent_proto_upbdefinit,
  &google_protobuf_wrappers_proto_upbdefinit,
  &udpa_annotations_status_proto_upbdefinit,
  &udpa_annotations_versioning_proto_upbdefinit,
  &validate_validate_proto_upbdefinit,
  NULL
};

upb_def_init envoy_config_cluster_v3_circuit_breaker_proto_upbdefinit = {
  deps,
  layouts,
  "envoy/config/cluster/v3/circuit_breaker.proto",
  UPB_STRVIEW_INIT(descriptor, 1364)
};
