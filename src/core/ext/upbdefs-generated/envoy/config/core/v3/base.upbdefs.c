/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/core/v3/base.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include "upb/def.h"
#include "envoy/config/core/v3/base.upbdefs.h"
#include "envoy/config/core/v3/base.upb.h"

extern _upb_DefPool_Init envoy_config_core_v3_address_proto_upbdefinit;
extern _upb_DefPool_Init envoy_config_core_v3_backoff_proto_upbdefinit;
extern _upb_DefPool_Init envoy_config_core_v3_http_uri_proto_upbdefinit;
extern _upb_DefPool_Init envoy_type_v3_percent_proto_upbdefinit;
extern _upb_DefPool_Init envoy_type_v3_semantic_version_proto_upbdefinit;
extern _upb_DefPool_Init google_protobuf_any_proto_upbdefinit;
extern _upb_DefPool_Init google_protobuf_struct_proto_upbdefinit;
extern _upb_DefPool_Init google_protobuf_wrappers_proto_upbdefinit;
extern _upb_DefPool_Init xds_core_v3_context_params_proto_upbdefinit;
extern _upb_DefPool_Init envoy_annotations_deprecation_proto_upbdefinit;
extern _upb_DefPool_Init udpa_annotations_migrate_proto_upbdefinit;
extern _upb_DefPool_Init udpa_annotations_status_proto_upbdefinit;
extern _upb_DefPool_Init udpa_annotations_versioning_proto_upbdefinit;
extern _upb_DefPool_Init validate_validate_proto_upbdefinit;
static const char descriptor[5442] = {'\n', '\037', 'e', 'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'c', 'o', 'r', 'e', '/', 'v', '3', '/', 'b', 'a', 
's', 'e', '.', 'p', 'r', 'o', 't', 'o', '\022', '\024', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 
'r', 'e', '.', 'v', '3', '\032', '\"', 'e', 'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'c', 'o', 'r', 'e', '/', 
'v', '3', '/', 'a', 'd', 'd', 'r', 'e', 's', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', '\"', 'e', 'n', 'v', 'o', 'y', '/', 'c', 
'o', 'n', 'f', 'i', 'g', '/', 'c', 'o', 'r', 'e', '/', 'v', '3', '/', 'b', 'a', 'c', 'k', 'o', 'f', 'f', '.', 'p', 'r', 'o', 
't', 'o', '\032', '#', 'e', 'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'c', 'o', 'r', 'e', '/', 'v', '3', '/', 
'h', 't', 't', 'p', '_', 'u', 'r', 'i', '.', 'p', 'r', 'o', 't', 'o', '\032', '\033', 'e', 'n', 'v', 'o', 'y', '/', 't', 'y', 'p', 
'e', '/', 'v', '3', '/', 'p', 'e', 'r', 'c', 'e', 'n', 't', '.', 'p', 'r', 'o', 't', 'o', '\032', '$', 'e', 'n', 'v', 'o', 'y', 
'/', 't', 'y', 'p', 'e', '/', 'v', '3', '/', 's', 'e', 'm', 'a', 'n', 't', 'i', 'c', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', 
'.', 'p', 'r', 'o', 't', 'o', '\032', '\031', 'g', 'o', 'o', 'g', 'l', 'e', '/', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '/', 'a', 
'n', 'y', '.', 'p', 'r', 'o', 't', 'o', '\032', '\034', 'g', 'o', 'o', 'g', 'l', 'e', '/', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', 
'/', 's', 't', 'r', 'u', 'c', 't', '.', 'p', 'r', 'o', 't', 'o', '\032', '\036', 'g', 'o', 'o', 'g', 'l', 'e', '/', 'p', 'r', 'o', 
't', 'o', 'b', 'u', 'f', '/', 'w', 'r', 'a', 'p', 'p', 'e', 'r', 's', '.', 'p', 'r', 'o', 't', 'o', '\032', ' ', 'x', 'd', 's', 
'/', 'c', 'o', 'r', 'e', '/', 'v', '3', '/', 'c', 'o', 'n', 't', 'e', 'x', 't', '_', 'p', 'a', 'r', 'a', 'm', 's', '.', 'p', 
'r', 'o', 't', 'o', '\032', '#', 'e', 'n', 'v', 'o', 'y', '/', 'a', 'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 'd', 
'e', 'p', 'r', 'e', 'c', 'a', 't', 'i', 'o', 'n', '.', 'p', 'r', 'o', 't', 'o', '\032', '\036', 'u', 'd', 'p', 'a', '/', 'a', 'n', 
'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 'm', 'i', 'g', 'r', 'a', 't', 'e', '.', 'p', 'r', 'o', 't', 'o', '\032', '\035', 
'u', 'd', 'p', 'a', '/', 'a', 'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 's', 't', 'a', 't', 'u', 's', '.', 'p', 
'r', 'o', 't', 'o', '\032', '!', 'u', 'd', 'p', 'a', '/', 'a', 'n', 'n', 'o', 't', 'a', 't', 'i', 'o', 'n', 's', '/', 'v', 'e', 
'r', 's', 'i', 'o', 'n', 'i', 'n', 'g', '.', 'p', 'r', 'o', 't', 'o', '\032', '\027', 'v', 'a', 'l', 'i', 'd', 'a', 't', 'e', '/', 
'v', 'a', 'l', 'i', 'd', 'a', 't', 'e', '.', 'p', 'r', 'o', 't', 'o', '\"', 't', '\n', '\010', 'L', 'o', 'c', 'a', 'l', 'i', 't', 
'y', '\022', '\026', '\n', '\006', 'r', 'e', 'g', 'i', 'o', 'n', '\030', '\001', ' ', '\001', '(', '\t', 'R', '\006', 'r', 'e', 'g', 'i', 'o', 'n', 
'\022', '\022', '\n', '\004', 'z', 'o', 'n', 'e', '\030', '\002', ' ', '\001', '(', '\t', 'R', '\004', 'z', 'o', 'n', 'e', '\022', '\031', '\n', '\010', 's', 
'u', 'b', '_', 'z', 'o', 'n', 'e', '\030', '\003', ' ', '\001', '(', '\t', 'R', '\007', 's', 'u', 'b', 'Z', 'o', 'n', 'e', ':', '!', '\232', 
'\305', '\210', '\036', '\034', '\n', '\032', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'L', 
'o', 'c', 'a', 'l', 'i', 't', 'y', '\"', '\244', '\001', '\n', '\014', 'B', 'u', 'i', 'l', 'd', 'V', 'e', 'r', 's', 'i', 'o', 'n', '\022', 
'8', '\n', '\007', 'v', 'e', 'r', 's', 'i', 'o', 'n', '\030', '\001', ' ', '\001', '(', '\013', '2', '\036', '.', 'e', 'n', 'v', 'o', 'y', '.', 
't', 'y', 'p', 'e', '.', 'v', '3', '.', 'S', 'e', 'm', 'a', 'n', 't', 'i', 'c', 'V', 'e', 'r', 's', 'i', 'o', 'n', 'R', '\007', 
'v', 'e', 'r', 's', 'i', 'o', 'n', '\022', '3', '\n', '\010', 'm', 'e', 't', 'a', 'd', 'a', 't', 'a', '\030', '\002', ' ', '\001', '(', '\013', 
'2', '\027', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'S', 't', 'r', 'u', 'c', 't', 
'R', '\010', 'm', 'e', 't', 'a', 'd', 'a', 't', 'a', ':', '%', '\232', '\305', '\210', '\036', ' ', '\n', '\036', 'e', 'n', 'v', 'o', 'y', '.', 
'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'B', 'u', 'i', 'l', 'd', 'V', 'e', 'r', 's', 'i', 'o', 'n', '\"', 
'\214', '\002', '\n', '\t', 'E', 'x', 't', 'e', 'n', 's', 'i', 'o', 'n', '\022', '\022', '\n', '\004', 'n', 'a', 'm', 'e', '\030', '\001', ' ', '\001', 
'(', '\t', 'R', '\004', 'n', 'a', 'm', 'e', '\022', '\032', '\n', '\010', 'c', 'a', 't', 'e', 'g', 'o', 'r', 'y', '\030', '\002', ' ', '\001', '(', 
'\t', 'R', '\010', 'c', 'a', 't', 'e', 'g', 'o', 'r', 'y', '\022', '4', '\n', '\017', 't', 'y', 'p', 'e', '_', 'd', 'e', 's', 'c', 'r', 
'i', 'p', 't', 'o', 'r', '\030', '\003', ' ', '\001', '(', '\t', 'B', '\013', '\030', '\001', '\222', '\307', '\206', '\330', '\004', '\003', '3', '.', '0', 'R', 
'\016', 't', 'y', 'p', 'e', 'D', 'e', 's', 'c', 'r', 'i', 'p', 't', 'o', 'r', '\022', '<', '\n', '\007', 'v', 'e', 'r', 's', 'i', 'o', 
'n', '\030', '\004', ' ', '\001', '(', '\013', '2', '\"', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 
'r', 'e', '.', 'v', '3', '.', 'B', 'u', 'i', 'l', 'd', 'V', 'e', 'r', 's', 'i', 'o', 'n', 'R', '\007', 'v', 'e', 'r', 's', 'i', 
'o', 'n', '\022', '\032', '\n', '\010', 'd', 'i', 's', 'a', 'b', 'l', 'e', 'd', '\030', '\005', ' ', '\001', '(', '\010', 'R', '\010', 'd', 'i', 's', 
'a', 'b', 'l', 'e', 'd', '\022', '\033', '\n', '\t', 't', 'y', 'p', 'e', '_', 'u', 'r', 'l', 's', '\030', '\006', ' ', '\003', '(', '\t', 'R', 
'\010', 't', 'y', 'p', 'e', 'U', 'r', 'l', 's', ':', '\"', '\232', '\305', '\210', '\036', '\035', '\n', '\033', 'e', 'n', 'v', 'o', 'y', '.', 'a', 
'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'E', 'x', 't', 'e', 'n', 's', 'i', 'o', 'n', '\"', '\262', '\006', '\n', '\004', 
'N', 'o', 'd', 'e', '\022', '\016', '\n', '\002', 'i', 'd', '\030', '\001', ' ', '\001', '(', '\t', 'R', '\002', 'i', 'd', '\022', '\030', '\n', '\007', 'c', 
'l', 'u', 's', 't', 'e', 'r', '\030', '\002', ' ', '\001', '(', '\t', 'R', '\007', 'c', 'l', 'u', 's', 't', 'e', 'r', '\022', '3', '\n', '\010', 
'm', 'e', 't', 'a', 'd', 'a', 't', 'a', '\030', '\003', ' ', '\001', '(', '\013', '2', '\027', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 
'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'S', 't', 'r', 'u', 'c', 't', 'R', '\010', 'm', 'e', 't', 'a', 'd', 'a', 't', 'a', '\022', 
'`', '\n', '\022', 'd', 'y', 'n', 'a', 'm', 'i', 'c', '_', 'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', '\030', '\014', ' ', '\003', 
'(', '\013', '2', '1', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', 
'.', 'N', 'o', 'd', 'e', '.', 'D', 'y', 'n', 'a', 'm', 'i', 'c', 'P', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', 'E', 'n', 
't', 'r', 'y', 'R', '\021', 'd', 'y', 'n', 'a', 'm', 'i', 'c', 'P', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', '\022', ':', '\n', 
'\010', 'l', 'o', 'c', 'a', 'l', 'i', 't', 'y', '\030', '\004', ' ', '\001', '(', '\013', '2', '\036', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 
'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'L', 'o', 'c', 'a', 'l', 'i', 't', 'y', 'R', '\010', 'l', 
'o', 'c', 'a', 'l', 'i', 't', 'y', '\022', '&', '\n', '\017', 'u', 's', 'e', 'r', '_', 'a', 'g', 'e', 'n', 't', '_', 'n', 'a', 'm', 
'e', '\030', '\006', ' ', '\001', '(', '\t', 'R', '\r', 'u', 's', 'e', 'r', 'A', 'g', 'e', 'n', 't', 'N', 'a', 'm', 'e', '\022', '.', '\n', 
'\022', 'u', 's', 'e', 'r', '_', 'a', 'g', 'e', 'n', 't', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '\030', '\007', ' ', '\001', '(', '\t', 
'H', '\000', 'R', '\020', 'u', 's', 'e', 'r', 'A', 'g', 'e', 'n', 't', 'V', 'e', 'r', 's', 'i', 'o', 'n', '\022', ']', '\n', '\030', 'u', 
's', 'e', 'r', '_', 'a', 'g', 'e', 'n', 't', '_', 'b', 'u', 'i', 'l', 'd', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '\030', '\010', 
' ', '\001', '(', '\013', '2', '\"', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 
'v', '3', '.', 'B', 'u', 'i', 'l', 'd', 'V', 'e', 'r', 's', 'i', 'o', 'n', 'H', '\000', 'R', '\025', 'u', 's', 'e', 'r', 'A', 'g', 
'e', 'n', 't', 'B', 'u', 'i', 'l', 'd', 'V', 'e', 'r', 's', 'i', 'o', 'n', '\022', '?', '\n', '\n', 'e', 'x', 't', 'e', 'n', 's', 
'i', 'o', 'n', 's', '\030', '\t', ' ', '\003', '(', '\013', '2', '\037', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', 
'.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'E', 'x', 't', 'e', 'n', 's', 'i', 'o', 'n', 'R', '\n', 'e', 'x', 't', 'e', 'n', 
's', 'i', 'o', 'n', 's', '\022', '\'', '\n', '\017', 'c', 'l', 'i', 'e', 'n', 't', '_', 'f', 'e', 'a', 't', 'u', 'r', 'e', 's', '\030', 
'\n', ' ', '\003', '(', '\t', 'R', '\016', 'c', 'l', 'i', 'e', 'n', 't', 'F', 'e', 'a', 't', 'u', 'r', 'e', 's', '\022', '[', '\n', '\023', 
'l', 'i', 's', 't', 'e', 'n', 'i', 'n', 'g', '_', 'a', 'd', 'd', 'r', 'e', 's', 's', 'e', 's', '\030', '\013', ' ', '\003', '(', '\013', 
'2', '\035', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'A', 
'd', 'd', 'r', 'e', 's', 's', 'B', '\013', '\030', '\001', '\222', '\307', '\206', '\330', '\004', '\003', '3', '.', '0', 'R', '\022', 'l', 'i', 's', 't', 
'e', 'n', 'i', 'n', 'g', 'A', 'd', 'd', 'r', 'e', 's', 's', 'e', 's', '\032', '`', '\n', '\026', 'D', 'y', 'n', 'a', 'm', 'i', 'c', 
'P', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', 'E', 'n', 't', 'r', 'y', '\022', '\020', '\n', '\003', 'k', 'e', 'y', '\030', '\001', ' ', 
'\001', '(', '\t', 'R', '\003', 'k', 'e', 'y', '\022', '0', '\n', '\005', 'v', 'a', 'l', 'u', 'e', '\030', '\002', ' ', '\001', '(', '\013', '2', '\032', 
'.', 'x', 'd', 's', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'C', 'o', 'n', 't', 'e', 'x', 't', 'P', 'a', 'r', 'a', 'm', 
's', 'R', '\005', 'v', 'a', 'l', 'u', 'e', ':', '\002', '8', '\001', ':', '\035', '\232', '\305', '\210', '\036', '\030', '\n', '\026', 'e', 'n', 'v', 'o', 
'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'N', 'o', 'd', 'e', 'B', '\031', '\n', '\027', 'u', 's', 'e', 
'r', '_', 'a', 'g', 'e', 'n', 't', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', 't', 'y', 'p', 'e', 'J', '\004', '\010', '\005', '\020', 
'\006', 'R', '\r', 'b', 'u', 'i', 'l', 'd', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '\"', '\261', '\003', '\n', '\010', 'M', 'e', 't', 'a', 
'd', 'a', 't', 'a', '\022', '[', '\n', '\017', 'f', 'i', 'l', 't', 'e', 'r', '_', 'm', 'e', 't', 'a', 'd', 'a', 't', 'a', '\030', '\001', 
' ', '\003', '(', '\013', '2', '2', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 
'v', '3', '.', 'M', 'e', 't', 'a', 'd', 'a', 't', 'a', '.', 'F', 'i', 'l', 't', 'e', 'r', 'M', 'e', 't', 'a', 'd', 'a', 't', 
'a', 'E', 'n', 't', 'r', 'y', 'R', '\016', 'f', 'i', 'l', 't', 'e', 'r', 'M', 'e', 't', 'a', 'd', 'a', 't', 'a', '\022', 'k', '\n', 
'\025', 't', 'y', 'p', 'e', 'd', '_', 'f', 'i', 'l', 't', 'e', 'r', '_', 'm', 'e', 't', 'a', 'd', 'a', 't', 'a', '\030', '\002', ' ', 
'\003', '(', '\013', '2', '7', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', 
'3', '.', 'M', 'e', 't', 'a', 'd', 'a', 't', 'a', '.', 'T', 'y', 'p', 'e', 'd', 'F', 'i', 'l', 't', 'e', 'r', 'M', 'e', 't', 
'a', 'd', 'a', 't', 'a', 'E', 'n', 't', 'r', 'y', 'R', '\023', 't', 'y', 'p', 'e', 'd', 'F', 'i', 'l', 't', 'e', 'r', 'M', 'e', 
't', 'a', 'd', 'a', 't', 'a', '\032', 'Z', '\n', '\023', 'F', 'i', 'l', 't', 'e', 'r', 'M', 'e', 't', 'a', 'd', 'a', 't', 'a', 'E', 
'n', 't', 'r', 'y', '\022', '\020', '\n', '\003', 'k', 'e', 'y', '\030', '\001', ' ', '\001', '(', '\t', 'R', '\003', 'k', 'e', 'y', '\022', '-', '\n', 
'\005', 'v', 'a', 'l', 'u', 'e', '\030', '\002', ' ', '\001', '(', '\013', '2', '\027', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 
't', 'o', 'b', 'u', 'f', '.', 'S', 't', 'r', 'u', 'c', 't', 'R', '\005', 'v', 'a', 'l', 'u', 'e', ':', '\002', '8', '\001', '\032', '\\', 
'\n', '\030', 'T', 'y', 'p', 'e', 'd', 'F', 'i', 'l', 't', 'e', 'r', 'M', 'e', 't', 'a', 'd', 'a', 't', 'a', 'E', 'n', 't', 'r', 
'y', '\022', '\020', '\n', '\003', 'k', 'e', 'y', '\030', '\001', ' ', '\001', '(', '\t', 'R', '\003', 'k', 'e', 'y', '\022', '*', '\n', '\005', 'v', 'a', 
'l', 'u', 'e', '\030', '\002', ' ', '\001', '(', '\013', '2', '\024', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 
'u', 'f', '.', 'A', 'n', 'y', 'R', '\005', 'v', 'a', 'l', 'u', 'e', ':', '\002', '8', '\001', ':', '!', '\232', '\305', '\210', '\036', '\034', '\n', 
'\032', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'M', 'e', 't', 'a', 'd', 'a', 
't', 'a', '\"', '\206', '\001', '\n', '\r', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'U', 'I', 'n', 't', '3', '2', '\022', '#', '\n', '\r', 'd', 
'e', 'f', 'a', 'u', 'l', 't', '_', 'v', 'a', 'l', 'u', 'e', '\030', '\002', ' ', '\001', '(', '\r', 'R', '\014', 'd', 'e', 'f', 'a', 'u', 
'l', 't', 'V', 'a', 'l', 'u', 'e', '\022', '(', '\n', '\013', 'r', 'u', 'n', 't', 'i', 'm', 'e', '_', 'k', 'e', 'y', '\030', '\003', ' ', 
'\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\n', 'r', 'u', 'n', 't', 'i', 'm', 'e', 'K', 'e', 'y', ':', 
'&', '\232', '\305', '\210', '\036', '!', '\n', '\037', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', 
'.', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'U', 'I', 'n', 't', '3', '2', '\"', 'w', '\n', '\016', 'R', 'u', 'n', 't', 'i', 'm', 'e', 
'P', 'e', 'r', 'c', 'e', 'n', 't', '\022', ';', '\n', '\r', 'd', 'e', 'f', 'a', 'u', 'l', 't', '_', 'v', 'a', 'l', 'u', 'e', '\030', 
'\001', ' ', '\001', '(', '\013', '2', '\026', '.', 'e', 'n', 'v', 'o', 'y', '.', 't', 'y', 'p', 'e', '.', 'v', '3', '.', 'P', 'e', 'r', 
'c', 'e', 'n', 't', 'R', '\014', 'd', 'e', 'f', 'a', 'u', 'l', 't', 'V', 'a', 'l', 'u', 'e', '\022', '(', '\n', '\013', 'r', 'u', 'n', 
't', 'i', 'm', 'e', '_', 'k', 'e', 'y', '\030', '\002', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\n', 
'r', 'u', 'n', 't', 'i', 'm', 'e', 'K', 'e', 'y', '\"', '\206', '\001', '\n', '\r', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'D', 'o', 'u', 
'b', 'l', 'e', '\022', '#', '\n', '\r', 'd', 'e', 'f', 'a', 'u', 'l', 't', '_', 'v', 'a', 'l', 'u', 'e', '\030', '\001', ' ', '\001', '(', 
'\001', 'R', '\014', 'd', 'e', 'f', 'a', 'u', 'l', 't', 'V', 'a', 'l', 'u', 'e', '\022', '(', '\n', '\013', 'r', 'u', 'n', 't', 'i', 'm', 
'e', '_', 'k', 'e', 'y', '\030', '\002', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\n', 'r', 'u', 'n', 
't', 'i', 'm', 'e', 'K', 'e', 'y', ':', '&', '\232', '\305', '\210', '\036', '!', '\n', '\037', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', 
'.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'D', 'o', 'u', 'b', 'l', 'e', '\"', '\266', '\001', 
'\n', '\022', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'F', 'e', 'a', 't', 'u', 'r', 'e', 'F', 'l', 'a', 'g', '\022', 'I', '\n', '\r', 'd', 
'e', 'f', 'a', 'u', 'l', 't', '_', 'v', 'a', 'l', 'u', 'e', '\030', '\001', ' ', '\001', '(', '\013', '2', '\032', '.', 'g', 'o', 'o', 'g', 
'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'B', 'o', 'o', 'l', 'V', 'a', 'l', 'u', 'e', 'B', '\010', '\372', 'B', 
'\005', '\212', '\001', '\002', '\020', '\001', 'R', '\014', 'd', 'e', 'f', 'a', 'u', 'l', 't', 'V', 'a', 'l', 'u', 'e', '\022', '(', '\n', '\013', 'r', 
'u', 'n', 't', 'i', 'm', 'e', '_', 'k', 'e', 'y', '\030', '\002', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 
'R', '\n', 'r', 'u', 'n', 't', 'i', 'm', 'e', 'K', 'e', 'y', ':', '+', '\232', '\305', '\210', '\036', '&', '\n', '$', 'e', 'n', 'v', 'o', 
'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'F', 'e', 'a', 't', 
'u', 'r', 'e', 'F', 'l', 'a', 'g', '\"', 'A', '\n', '\016', 'Q', 'u', 'e', 'r', 'y', 'P', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 
'\022', '\031', '\n', '\003', 'k', 'e', 'y', '\030', '\001', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\003', 'k', 
'e', 'y', '\022', '\024', '\n', '\005', 'v', 'a', 'l', 'u', 'e', '\030', '\002', ' ', '\001', '(', '\t', 'R', '\005', 'v', 'a', 'l', 'u', 'e', '\"', 
'\177', '\n', '\013', 'H', 'e', 'a', 'd', 'e', 'r', 'V', 'a', 'l', 'u', 'e', '\022', '#', '\n', '\003', 'k', 'e', 'y', '\030', '\001', ' ', '\001', 
'(', '\t', 'B', '\021', '\372', 'B', '\016', 'r', '\014', '\020', '\001', '(', '\200', '\200', '\001', '\300', '\001', '\001', '\310', '\001', '\000', 'R', '\003', 'k', 'e', 
'y', '\022', '%', '\n', '\005', 'v', 'a', 'l', 'u', 'e', '\030', '\002', ' ', '\001', '(', '\t', 'B', '\017', '\372', 'B', '\014', 'r', '\n', '(', '\200', 
'\200', '\001', '\300', '\001', '\002', '\310', '\001', '\000', 'R', '\005', 'v', 'a', 'l', 'u', 'e', ':', '$', '\232', '\305', '\210', '\036', '\037', '\n', '\035', 'e', 
'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'H', 'e', 'a', 'd', 'e', 'r', 'V', 'a', 
'l', 'u', 'e', '\"', '\263', '\003', '\n', '\021', 'H', 'e', 'a', 'd', 'e', 'r', 'V', 'a', 'l', 'u', 'e', 'O', 'p', 't', 'i', 'o', 'n', 
'\022', 'C', '\n', '\006', 'h', 'e', 'a', 'd', 'e', 'r', '\030', '\001', ' ', '\001', '(', '\013', '2', '!', '.', 'e', 'n', 'v', 'o', 'y', '.', 
'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'H', 'e', 'a', 'd', 'e', 'r', 'V', 'a', 'l', 'u', 
'e', 'B', '\010', '\372', 'B', '\005', '\212', '\001', '\002', '\020', '\001', 'R', '\006', 'h', 'e', 'a', 'd', 'e', 'r', '\022', '2', '\n', '\006', 'a', 'p', 
'p', 'e', 'n', 'd', '\030', '\002', ' ', '\001', '(', '\013', '2', '\032', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 
'b', 'u', 'f', '.', 'B', 'o', 'o', 'l', 'V', 'a', 'l', 'u', 'e', 'R', '\006', 'a', 'p', 'p', 'e', 'n', 'd', '\022', 'i', '\n', '\r', 
'a', 'p', 'p', 'e', 'n', 'd', '_', 'a', 'c', 't', 'i', 'o', 'n', '\030', '\003', ' ', '\001', '(', '\016', '2', ':', '.', 'e', 'n', 'v', 
'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'H', 'e', 'a', 'd', 'e', 'r', 'V', 
'a', 'l', 'u', 'e', 'O', 'p', 't', 'i', 'o', 'n', '.', 'H', 'e', 'a', 'd', 'e', 'r', 'A', 'p', 'p', 'e', 'n', 'd', 'A', 'c', 
't', 'i', 'o', 'n', 'B', '\010', '\372', 'B', '\005', '\202', '\001', '\002', '\020', '\001', 'R', '\014', 'a', 'p', 'p', 'e', 'n', 'd', 'A', 'c', 't', 
'i', 'o', 'n', '\022', '(', '\n', '\020', 'k', 'e', 'e', 'p', '_', 'e', 'm', 'p', 't', 'y', '_', 'v', 'a', 'l', 'u', 'e', '\030', '\004', 
' ', '\001', '(', '\010', 'R', '\016', 'k', 'e', 'e', 'p', 'E', 'm', 'p', 't', 'y', 'V', 'a', 'l', 'u', 'e', '\"', 'd', '\n', '\022', 'H', 
'e', 'a', 'd', 'e', 'r', 'A', 'p', 'p', 'e', 'n', 'd', 'A', 'c', 't', 'i', 'o', 'n', '\022', '\033', '\n', '\027', 'A', 'P', 'P', 'E', 
'N', 'D', '_', 'I', 'F', '_', 'E', 'X', 'I', 'S', 'T', 'S', '_', 'O', 'R', '_', 'A', 'D', 'D', '\020', '\000', '\022', '\021', '\n', '\r', 
'A', 'D', 'D', '_', 'I', 'F', '_', 'A', 'B', 'S', 'E', 'N', 'T', '\020', '\001', '\022', '\036', '\n', '\032', 'O', 'V', 'E', 'R', 'W', 'R', 
'I', 'T', 'E', '_', 'I', 'F', '_', 'E', 'X', 'I', 'S', 'T', 'S', '_', 'O', 'R', '_', 'A', 'D', 'D', '\020', '\002', ':', '*', '\232', 
'\305', '\210', '\036', '%', '\n', '#', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'H', 
'e', 'a', 'd', 'e', 'r', 'V', 'a', 'l', 'u', 'e', 'O', 'p', 't', 'i', 'o', 'n', '\"', 'l', '\n', '\t', 'H', 'e', 'a', 'd', 'e', 
'r', 'M', 'a', 'p', '\022', ';', '\n', '\007', 'h', 'e', 'a', 'd', 'e', 'r', 's', '\030', '\001', ' ', '\003', '(', '\013', '2', '!', '.', 'e', 
'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'H', 'e', 'a', 'd', 'e', 
'r', 'V', 'a', 'l', 'u', 'e', 'R', '\007', 'h', 'e', 'a', 'd', 'e', 'r', 's', ':', '\"', '\232', '\305', '\210', '\036', '\035', '\n', '\033', 'e', 
'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'H', 'e', 'a', 'd', 'e', 'r', 'M', 'a', 
'p', '\"', '/', '\n', '\020', 'W', 'a', 't', 'c', 'h', 'e', 'd', 'D', 'i', 'r', 'e', 'c', 't', 'o', 'r', 'y', '\022', '\033', '\n', '\004', 
'p', 'a', 't', 'h', '\030', '\001', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\004', 'p', 'a', 't', 'h', 
'\"', '\364', '\001', '\n', '\n', 'D', 'a', 't', 'a', 'S', 'o', 'u', 'r', 'c', 'e', '\022', '%', '\n', '\010', 'f', 'i', 'l', 'e', 'n', 'a', 
'm', 'e', '\030', '\001', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'H', '\000', 'R', '\010', 'f', 'i', 'l', 'e', 
'n', 'a', 'm', 'e', '\022', '#', '\n', '\014', 'i', 'n', 'l', 'i', 'n', 'e', '_', 'b', 'y', 't', 'e', 's', '\030', '\002', ' ', '\001', '(', 
'\014', 'H', '\000', 'R', '\013', 'i', 'n', 'l', 'i', 'n', 'e', 'B', 'y', 't', 'e', 's', '\022', '%', '\n', '\r', 'i', 'n', 'l', 'i', 'n', 
'e', '_', 's', 't', 'r', 'i', 'n', 'g', '\030', '\003', ' ', '\001', '(', '\t', 'H', '\000', 'R', '\014', 'i', 'n', 'l', 'i', 'n', 'e', 'S', 
't', 'r', 'i', 'n', 'g', '\022', '<', '\n', '\024', 'e', 'n', 'v', 'i', 'r', 'o', 'n', 'm', 'e', 'n', 't', '_', 'v', 'a', 'r', 'i', 
'a', 'b', 'l', 'e', '\030', '\004', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'H', '\000', 'R', '\023', 'e', 'n', 
'v', 'i', 'r', 'o', 'n', 'm', 'e', 'n', 't', 'V', 'a', 'r', 'i', 'a', 'b', 'l', 'e', ':', '#', '\232', '\305', '\210', '\036', '\036', '\n', 
'\034', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'D', 'a', 't', 'a', 'S', 'o', 
'u', 'r', 'c', 'e', 'B', '\020', '\n', '\t', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'e', 'r', '\022', '\003', '\370', 'B', '\001', '\"', '\324', '\001', 
'\n', '\013', 'R', 'e', 't', 'r', 'y', 'P', 'o', 'l', 'i', 'c', 'y', '\022', 'K', '\n', '\016', 'r', 'e', 't', 'r', 'y', '_', 'b', 'a', 
'c', 'k', '_', 'o', 'f', 'f', '\030', '\001', ' ', '\001', '(', '\013', '2', '%', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 
'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'B', 'a', 'c', 'k', 'o', 'f', 'f', 'S', 't', 'r', 'a', 't', 'e', 'g', 
'y', 'R', '\014', 'r', 'e', 't', 'r', 'y', 'B', 'a', 'c', 'k', 'O', 'f', 'f', '\022', 'R', '\n', '\013', 'n', 'u', 'm', '_', 'r', 'e', 
't', 'r', 'i', 'e', 's', '\030', '\002', ' ', '\001', '(', '\013', '2', '\034', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 
'o', 'b', 'u', 'f', '.', 'U', 'I', 'n', 't', '3', '2', 'V', 'a', 'l', 'u', 'e', 'B', '\023', '\362', '\230', '\376', '\217', '\005', '\r', '\n', 
'\013', 'm', 'a', 'x', '_', 'r', 'e', 't', 'r', 'i', 'e', 's', 'R', '\n', 'n', 'u', 'm', 'R', 'e', 't', 'r', 'i', 'e', 's', ':', 
'$', '\232', '\305', '\210', '\036', '\037', '\n', '\035', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', 
'.', 'R', 'e', 't', 'r', 'y', 'P', 'o', 'l', 'i', 'c', 'y', '\"', '\350', '\001', '\n', '\020', 'R', 'e', 'm', 'o', 't', 'e', 'D', 'a', 
't', 'a', 'S', 'o', 'u', 'r', 'c', 'e', '\022', 'B', '\n', '\010', 'h', 't', 't', 'p', '_', 'u', 'r', 'i', '\030', '\001', ' ', '\001', '(', 
'\013', '2', '\035', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 
'H', 't', 't', 'p', 'U', 'r', 'i', 'B', '\010', '\372', 'B', '\005', '\212', '\001', '\002', '\020', '\001', 'R', '\007', 'h', 't', 't', 'p', 'U', 'r', 
'i', '\022', '\037', '\n', '\006', 's', 'h', 'a', '2', '5', '6', '\030', '\002', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', 
'\001', 'R', '\006', 's', 'h', 'a', '2', '5', '6', '\022', 'D', '\n', '\014', 'r', 'e', 't', 'r', 'y', '_', 'p', 'o', 'l', 'i', 'c', 'y', 
'\030', '\003', ' ', '\001', '(', '\013', '2', '!', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 
'e', '.', 'v', '3', '.', 'R', 'e', 't', 'r', 'y', 'P', 'o', 'l', 'i', 'c', 'y', 'R', '\013', 'r', 'e', 't', 'r', 'y', 'P', 'o', 
'l', 'i', 'c', 'y', ':', ')', '\232', '\305', '\210', '\036', '$', '\n', '\"', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', 
'.', 'c', 'o', 'r', 'e', '.', 'R', 'e', 'm', 'o', 't', 'e', 'D', 'a', 't', 'a', 'S', 'o', 'u', 'r', 'c', 'e', '\"', '\311', '\001', 
'\n', '\017', 'A', 's', 'y', 'n', 'c', 'D', 'a', 't', 'a', 'S', 'o', 'u', 'r', 'c', 'e', '\022', '8', '\n', '\005', 'l', 'o', 'c', 'a', 
'l', '\030', '\001', ' ', '\001', '(', '\013', '2', ' ', '.', 'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 
'r', 'e', '.', 'v', '3', '.', 'D', 'a', 't', 'a', 'S', 'o', 'u', 'r', 'c', 'e', 'H', '\000', 'R', '\005', 'l', 'o', 'c', 'a', 'l', 
'\022', '@', '\n', '\006', 'r', 'e', 'm', 'o', 't', 'e', '\030', '\002', ' ', '\001', '(', '\013', '2', '&', '.', 'e', 'n', 'v', 'o', 'y', '.', 
'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', '.', 'R', 'e', 'm', 'o', 't', 'e', 'D', 'a', 't', 'a', 
'S', 'o', 'u', 'r', 'c', 'e', 'H', '\000', 'R', '\006', 'r', 'e', 'm', 'o', 't', 'e', ':', '(', '\232', '\305', '\210', '\036', '#', '\n', '!', 
'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'A', 's', 'y', 'n', 'c', 'D', 'a', 
't', 'a', 'S', 'o', 'u', 'r', 'c', 'e', 'B', '\020', '\n', '\t', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'e', 'r', '\022', '\003', '\370', 'B', 
'\001', '\"', '\260', '\001', '\n', '\017', 'T', 'r', 'a', 'n', 's', 'p', 'o', 'r', 't', 'S', 'o', 'c', 'k', 'e', 't', '\022', '\033', '\n', '\004', 
'n', 'a', 'm', 'e', '\030', '\001', ' ', '\001', '(', '\t', 'B', '\007', '\372', 'B', '\004', 'r', '\002', '\020', '\001', 'R', '\004', 'n', 'a', 'm', 'e', 
'\022', '9', '\n', '\014', 't', 'y', 'p', 'e', 'd', '_', 'c', 'o', 'n', 'f', 'i', 'g', '\030', '\003', ' ', '\001', '(', '\013', '2', '\024', '.', 
'g', 'o', 'o', 'g', 'l', 'e', '.', 'p', 'r', 'o', 't', 'o', 'b', 'u', 'f', '.', 'A', 'n', 'y', 'H', '\000', 'R', '\013', 't', 'y', 
'p', 'e', 'd', 'C', 'o', 'n', 'f', 'i', 'g', ':', '(', '\232', '\305', '\210', '\036', '#', '\n', '!', 'e', 'n', 'v', 'o', 'y', '.', 'a', 
'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'T', 'r', 'a', 'n', 's', 'p', 'o', 'r', 't', 'S', 'o', 'c', 'k', 'e', 
't', 'B', '\r', '\n', '\013', 'c', 'o', 'n', 'f', 'i', 'g', '_', 't', 'y', 'p', 'e', 'J', '\004', '\010', '\002', '\020', '\003', 'R', '\006', 'c', 
'o', 'n', 'f', 'i', 'g', '\"', '\277', '\001', '\n', '\030', 'R', 'u', 'n', 't', 'i', 'm', 'e', 'F', 'r', 'a', 'c', 't', 'i', 'o', 'n', 
'a', 'l', 'P', 'e', 'r', 'c', 'e', 'n', 't', '\022', 'O', '\n', '\r', 'd', 'e', 'f', 'a', 'u', 'l', 't', '_', 'v', 'a', 'l', 'u', 
'e', '\030', '\001', ' ', '\001', '(', '\013', '2', ' ', '.', 'e', 'n', 'v', 'o', 'y', '.', 't', 'y', 'p', 'e', '.', 'v', '3', '.', 'F', 
'r', 'a', 'c', 't', 'i', 'o', 'n', 'a', 'l', 'P', 'e', 'r', 'c', 'e', 'n', 't', 'B', '\010', '\372', 'B', '\005', '\212', '\001', '\002', '\020', 
'\001', 'R', '\014', 'd', 'e', 'f', 'a', 'u', 'l', 't', 'V', 'a', 'l', 'u', 'e', '\022', '\037', '\n', '\013', 'r', 'u', 'n', 't', 'i', 'm', 
'e', '_', 'k', 'e', 'y', '\030', '\002', ' ', '\001', '(', '\t', 'R', '\n', 'r', 'u', 'n', 't', 'i', 'm', 'e', 'K', 'e', 'y', ':', '1', 
'\232', '\305', '\210', '\036', ',', '\n', '*', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 
'R', 'u', 'n', 't', 'i', 'm', 'e', 'F', 'r', 'a', 'c', 't', 'i', 'o', 'n', 'a', 'l', 'P', 'e', 'r', 'c', 'e', 'n', 't', '\"', 
'U', '\n', '\014', 'C', 'o', 'n', 't', 'r', 'o', 'l', 'P', 'l', 'a', 'n', 'e', '\022', '\036', '\n', '\n', 'i', 'd', 'e', 'n', 't', 'i', 
'f', 'i', 'e', 'r', '\030', '\001', ' ', '\001', '(', '\t', 'R', '\n', 'i', 'd', 'e', 'n', 't', 'i', 'f', 'i', 'e', 'r', ':', '%', '\232', 
'\305', '\210', '\036', ' ', '\n', '\036', 'e', 'n', 'v', 'o', 'y', '.', 'a', 'p', 'i', '.', 'v', '2', '.', 'c', 'o', 'r', 'e', '.', 'C', 
'o', 'n', 't', 'r', 'o', 'l', 'P', 'l', 'a', 'n', 'e', '*', '(', '\n', '\017', 'R', 'o', 'u', 't', 'i', 'n', 'g', 'P', 'r', 'i', 
'o', 'r', 'i', 't', 'y', '\022', '\013', '\n', '\007', 'D', 'E', 'F', 'A', 'U', 'L', 'T', '\020', '\000', '\022', '\010', '\n', '\004', 'H', 'I', 'G', 
'H', '\020', '\001', '*', '\211', '\001', '\n', '\r', 'R', 'e', 'q', 'u', 'e', 's', 't', 'M', 'e', 't', 'h', 'o', 'd', '\022', '\026', '\n', '\022', 
'M', 'E', 'T', 'H', 'O', 'D', '_', 'U', 'N', 'S', 'P', 'E', 'C', 'I', 'F', 'I', 'E', 'D', '\020', '\000', '\022', '\007', '\n', '\003', 'G', 
'E', 'T', '\020', '\001', '\022', '\010', '\n', '\004', 'H', 'E', 'A', 'D', '\020', '\002', '\022', '\010', '\n', '\004', 'P', 'O', 'S', 'T', '\020', '\003', '\022', 
'\007', '\n', '\003', 'P', 'U', 'T', '\020', '\004', '\022', '\n', '\n', '\006', 'D', 'E', 'L', 'E', 'T', 'E', '\020', '\005', '\022', '\013', '\n', '\007', 'C', 
'O', 'N', 'N', 'E', 'C', 'T', '\020', '\006', '\022', '\013', '\n', '\007', 'O', 'P', 'T', 'I', 'O', 'N', 'S', '\020', '\007', '\022', '\t', '\n', '\005', 
'T', 'R', 'A', 'C', 'E', '\020', '\010', '\022', '\t', '\n', '\005', 'P', 'A', 'T', 'C', 'H', '\020', '\t', '*', '>', '\n', '\020', 'T', 'r', 'a', 
'f', 'f', 'i', 'c', 'D', 'i', 'r', 'e', 'c', 't', 'i', 'o', 'n', '\022', '\017', '\n', '\013', 'U', 'N', 'S', 'P', 'E', 'C', 'I', 'F', 
'I', 'E', 'D', '\020', '\000', '\022', '\013', '\n', '\007', 'I', 'N', 'B', 'O', 'U', 'N', 'D', '\020', '\001', '\022', '\014', '\n', '\010', 'O', 'U', 'T', 
'B', 'O', 'U', 'N', 'D', '\020', '\002', 'B', '}', '\n', '\"', 'i', 'o', '.', 'e', 'n', 'v', 'o', 'y', 'p', 'r', 'o', 'x', 'y', '.', 
'e', 'n', 'v', 'o', 'y', '.', 'c', 'o', 'n', 'f', 'i', 'g', '.', 'c', 'o', 'r', 'e', '.', 'v', '3', 'B', '\t', 'B', 'a', 's', 
'e', 'P', 'r', 'o', 't', 'o', 'P', '\001', 'Z', 'B', 'g', 'i', 't', 'h', 'u', 'b', '.', 'c', 'o', 'm', '/', 'e', 'n', 'v', 'o', 
'y', 'p', 'r', 'o', 'x', 'y', '/', 'g', 'o', '-', 'c', 'o', 'n', 't', 'r', 'o', 'l', '-', 'p', 'l', 'a', 'n', 'e', '/', 'e', 
'n', 'v', 'o', 'y', '/', 'c', 'o', 'n', 'f', 'i', 'g', '/', 'c', 'o', 'r', 'e', '/', 'v', '3', ';', 'c', 'o', 'r', 'e', 'v', 
'3', '\272', '\200', '\310', '\321', '\006', '\002', '\020', '\002', 'b', '\006', 'p', 'r', 'o', 't', 'o', '3', 
};

static _upb_DefPool_Init *deps[15] = {
  &envoy_config_core_v3_address_proto_upbdefinit,
  &envoy_config_core_v3_backoff_proto_upbdefinit,
  &envoy_config_core_v3_http_uri_proto_upbdefinit,
  &envoy_type_v3_percent_proto_upbdefinit,
  &envoy_type_v3_semantic_version_proto_upbdefinit,
  &google_protobuf_any_proto_upbdefinit,
  &google_protobuf_struct_proto_upbdefinit,
  &google_protobuf_wrappers_proto_upbdefinit,
  &xds_core_v3_context_params_proto_upbdefinit,
  &envoy_annotations_deprecation_proto_upbdefinit,
  &udpa_annotations_migrate_proto_upbdefinit,
  &udpa_annotations_status_proto_upbdefinit,
  &udpa_annotations_versioning_proto_upbdefinit,
  &validate_validate_proto_upbdefinit,
  NULL
};

_upb_DefPool_Init envoy_config_core_v3_base_proto_upbdefinit = {
  deps,
  &envoy_config_core_v3_base_proto_upb_file_layout,
  "envoy/config/core/v3/base.proto",
  UPB_STRINGVIEW_INIT(descriptor, 5442)
};
