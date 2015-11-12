/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.4-dev at Wed Nov 11 15:31:24 2015. */

#include "load_balancer.pb.h"

#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t grpc_lb_v0_Duration_fields[3] = {
    PB_FIELD(  1, INT64   , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_Duration, seconds, seconds, 0),
    PB_FIELD(  2, INT32   , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_Duration, nanos, seconds, 0),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_LoadBalanceRequest_fields[3] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_LoadBalanceRequest, initial_request, initial_request, &grpc_lb_v0_InitialLoadBalanceRequest_fields),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_LoadBalanceRequest, client_stats, initial_request, &grpc_lb_v0_ClientStats_fields),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_InitialLoadBalanceRequest_fields[2] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_InitialLoadBalanceRequest, name, name, 0),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_ClientStats_fields[4] = {
    PB_FIELD(  1, INT64   , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_ClientStats, total_requests, total_requests, 0),
    PB_FIELD(  2, INT64   , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_ClientStats, client_rpc_errors, total_requests, 0),
    PB_FIELD(  3, INT64   , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_ClientStats, dropped_requests, client_rpc_errors, 0),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_LoadBalanceResponse_fields[3] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_LoadBalanceResponse, initial_response, initial_response, &grpc_lb_v0_InitialLoadBalanceResponse_fields),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_LoadBalanceResponse, server_list, initial_response, &grpc_lb_v0_ServerList_fields),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_InitialLoadBalanceResponse_fields[4] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_InitialLoadBalanceResponse, client_config, client_config, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_InitialLoadBalanceResponse, load_balancer_delegate, client_config, 0),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_InitialLoadBalanceResponse, client_stats_report_interval, load_balancer_delegate, &grpc_lb_v0_Duration_fields),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_ServerList_fields[3] = {
    PB_FIELD(  1, MESSAGE , REPEATED, CALLBACK, FIRST, grpc_lb_v0_ServerList, servers, servers, &grpc_lb_v0_Server_fields),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_ServerList, expiration_interval, servers, &grpc_lb_v0_Duration_fields),
    PB_LAST_FIELD
};

const pb_field_t grpc_lb_v0_Server_fields[5] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, grpc_lb_v0_Server, ip_address, ip_address, 0),
    PB_FIELD(  2, INT32   , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_Server, port, ip_address, 0),
    PB_FIELD(  3, BYTES   , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_Server, load_balance_token, port, 0),
    PB_FIELD(  4, BOOL    , OPTIONAL, STATIC  , OTHER, grpc_lb_v0_Server, drop_request, load_balance_token, 0),
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
PB_STATIC_ASSERT((pb_membersize(grpc_lb_v0_LoadBalanceRequest, initial_request) < 65536 && pb_membersize(grpc_lb_v0_LoadBalanceRequest, client_stats) < 65536 && pb_membersize(grpc_lb_v0_LoadBalanceResponse, initial_response) < 65536 && pb_membersize(grpc_lb_v0_LoadBalanceResponse, server_list) < 65536 && pb_membersize(grpc_lb_v0_InitialLoadBalanceResponse, client_stats_report_interval) < 65536 && pb_membersize(grpc_lb_v0_ServerList, servers) < 65536 && pb_membersize(grpc_lb_v0_ServerList, expiration_interval) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_grpc_lb_v0_Duration_grpc_lb_v0_LoadBalanceRequest_grpc_lb_v0_InitialLoadBalanceRequest_grpc_lb_v0_ClientStats_grpc_lb_v0_LoadBalanceResponse_grpc_lb_v0_InitialLoadBalanceResponse_grpc_lb_v0_ServerList_grpc_lb_v0_Server)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(grpc_lb_v0_LoadBalanceRequest, initial_request) < 256 && pb_membersize(grpc_lb_v0_LoadBalanceRequest, client_stats) < 256 && pb_membersize(grpc_lb_v0_LoadBalanceResponse, initial_response) < 256 && pb_membersize(grpc_lb_v0_LoadBalanceResponse, server_list) < 256 && pb_membersize(grpc_lb_v0_InitialLoadBalanceResponse, client_stats_report_interval) < 256 && pb_membersize(grpc_lb_v0_ServerList, servers) < 256 && pb_membersize(grpc_lb_v0_ServerList, expiration_interval) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_grpc_lb_v0_Duration_grpc_lb_v0_LoadBalanceRequest_grpc_lb_v0_InitialLoadBalanceRequest_grpc_lb_v0_ClientStats_grpc_lb_v0_LoadBalanceResponse_grpc_lb_v0_InitialLoadBalanceResponse_grpc_lb_v0_ServerList_grpc_lb_v0_Server)
#endif


