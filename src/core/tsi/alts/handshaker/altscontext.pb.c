/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.7-dev */

#include "src/core/tsi/alts/handshaker/altscontext.pb.h"
/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t grpc_gcp_AltsContext_fields[7] = {
    PB_FIELD(  1, STRING  , OPTIONAL, CALLBACK, FIRST, grpc_gcp_AltsContext, application_protocol, application_protocol, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, CALLBACK, OTHER, grpc_gcp_AltsContext, record_protocol, application_protocol, 0),
    PB_FIELD(  3, UENUM   , OPTIONAL, STATIC  , OTHER, grpc_gcp_AltsContext, security_level, record_protocol, 0),
    PB_FIELD(  4, STRING  , OPTIONAL, CALLBACK, OTHER, grpc_gcp_AltsContext, peer_service_account, security_level, 0),
    PB_FIELD(  5, STRING  , OPTIONAL, CALLBACK, OTHER, grpc_gcp_AltsContext, local_service_account, peer_service_account, 0),
    PB_FIELD(  6, MESSAGE , OPTIONAL, STATIC  , OTHER, grpc_gcp_AltsContext, peer_rpc_versions, local_service_account, &grpc_gcp_RpcProtocolVersions_fields),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(grpc_gcp_AltsContext, peer_rpc_versions) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_grpc_gcp_AltsContext)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(grpc_gcp_AltsContext, peer_rpc_versions) < 256), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_grpc_gcp_AltsContext)
#endif


/* @@protoc_insertion_point(eof) */
