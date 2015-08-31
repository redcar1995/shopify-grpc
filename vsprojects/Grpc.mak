# Copyright 2015, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# NMake file to build secondary gRPC targets on Windows.
# Use grpc.sln to solution to build the gRPC libraries.

OUT_DIR=test_bin

CC=cl.exe /nologo
LINK=link.exe /nologo
LIBTOOL=lib.exe /nologo /nodefaultlib

REPO_ROOT=..
OPENSSL_INCLUDES = .\packages\\build\native\include
ZLIB_INCLUDES = .\packages\\build\native\include
INCLUDES=/I$(REPO_ROOT) /I$(REPO_ROOT)\include /I$(OPENSSL_INCLUDES) /I$(ZLIB_INCLUDES)

GFLAGS_INCLUDES = .\..\third_party\gflags\include
GTEST_INCLUDES = .\..\third_party\gtest\include
PROTOBUF_INCLUDES = .\..\third_party\protobuf\src
CXX_INCLUDES=/I$(GFLAGS_INCLUDES) /I$(GTEST_INCLUDES) /I$(PROTOBUF_INCLUDES)

#_SCL_SECURE_NO_WARNINGS supresses a ton of "potentially unsafe use of std lib" warnings
DEFINES=/D WIN32 /D _LIB /D _USE_32BIT_TIME_T /D _UNICODE /D UNICODE /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS

#important options: /TC vs. /TP: compile as C vs. compile as C++
CFLAGS=/c $(INCLUDES) /Z7 /W3 /WX- /sdl $(DEFINES) /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /TC /analyze-
CXXFLAGS=/c $(INCLUDES) $(CXX_INCLUDES) /Z7 /W3 /WX- /sdl $(DEFINES) /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /TP /analyze-

LFLAGS=/DEBUG /INCREMENTAL /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT /MACHINE:X86

OPENSSL_LIBS=.\packages\\build\native\lib\v120\Win32\Debug\static\ssleay32.lib .\packages\\build\native\lib\v120\Win32\Debug\static\libeay32.lib
WINSOCK_LIBS=ws2_32.lib
GENERAL_LIBS=advapi32.lib comdlg32.lib gdi32.lib kernel32.lib odbc32.lib odbccp32.lib ole32.lib oleaut32.lib shell32.lib user32.lib uuid.lib winspool.lib
ZLIB_LIBS=.\packages\\build\native\lib\v120\Win32\Debug\static\cdecl\zlib.lib
LIBS=$(OPENSSL_LIBS) $(ZLIB_LIBS) $(GENERAL_LIBS) $(WINSOCK_LIBS)

#shlwapi.lib provides PathMatchSpec() for gflags in windows
GFLAGS_LIBS=.\..\third_party\gflags\lib\Debug\gflags.lib shlwapi.lib
GTEST_LIBS=.\..\third_party\gtest\msvc\gtest\Debug\gtestd.lib
PROTOBUF_LIBS=.\..\third_party\protobuf\vsprojects\Debug\libprotobuf.lib
CXX_LIBS=$(GFLAGS_LIBS) $(GTEST_LIBS) $(PROTOBUF_LIBS)

all: buildtests

tools:

tools_c:

tools_cxx:

$(OUT_DIR):
	mkdir $(OUT_DIR)

build_libs: build_gpr build_gpr_test_util build_grpc build_grpc_test_util build_grpc_test_util_unsecure build_grpc_unsecure Debug\reconnect_server.lib build_grpc++ Debug\grpc++_test_config.lib Debug\grpc++_test_util.lib build_grpc++_unsecure Debug\interop_client_helper.lib Debug\interop_client_main.lib Debug\interop_server_helper.lib Debug\interop_server_main.lib Debug\qps.lib Debug\end2end_fixture_h2_compress.lib Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_fixture_h2_full.lib Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_fixture_h2_proxy.lib Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_fixture_h2_ssl.lib Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_test_call_creds.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_test_default_host.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_test_empty_batch.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_test_large_metadata.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_test_max_message_length.lib Debug\end2end_test_metadata.lib Debug\end2end_test_no_op.lib Debug\end2end_test_payload.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_test_registered_call.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_test_simple_request.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\bad_client_test.lib 
buildtests: buildtests_c buildtests_cxx

buildtests_c: alarm_heap_test.exe alarm_list_test.exe alarm_test.exe alpn_test.exe bin_encoder_test.exe chttp2_status_conversion_test.exe chttp2_stream_encoder_test.exe chttp2_stream_map_test.exe compression_test.exe endpoint_pair_test.exe fling_client.exe fling_server.exe gpr_cmdline_test.exe gpr_env_test.exe gpr_file_test.exe gpr_histogram_test.exe gpr_host_port_test.exe gpr_log_test.exe gpr_slice_buffer_test.exe gpr_slice_test.exe gpr_stack_lockfree_test.exe gpr_string_test.exe gpr_sync_test.exe gpr_thd_test.exe gpr_time_test.exe gpr_tls_test.exe gpr_useful_test.exe grpc_auth_context_test.exe grpc_base64_test.exe grpc_byte_buffer_reader_test.exe grpc_channel_args_test.exe grpc_channel_stack_test.exe grpc_completion_queue_test.exe grpc_credentials_test.exe grpc_json_token_test.exe grpc_jwt_verifier_test.exe grpc_security_connector_test.exe grpc_stream_op_test.exe hpack_parser_test.exe hpack_table_test.exe httpcli_format_request_test.exe httpcli_parser_test.exe json_rewrite.exe json_rewrite_test.exe json_test.exe lame_client_test.exe message_compress_test.exe multi_init_test.exe multiple_server_queues_test.exe murmur_hash_test.exe no_server_test.exe resolve_address_test.exe secure_endpoint_test.exe sockaddr_utils_test.exe time_averaged_stats_test.exe timeout_encoding_test.exe timers_test.exe transport_metadata_test.exe transport_security_test.exe uri_parser_test.exe h2_compress_bad_hostname_test.exe h2_compress_binary_metadata_test.exe h2_compress_call_creds_test.exe h2_compress_cancel_after_accept_test.exe h2_compress_cancel_after_client_done_test.exe h2_compress_cancel_after_invoke_test.exe h2_compress_cancel_before_invoke_test.exe h2_compress_cancel_in_a_vacuum_test.exe h2_compress_census_simple_request_test.exe h2_compress_channel_connectivity_test.exe h2_compress_compressed_payload_test.exe h2_compress_default_host_test.exe h2_compress_disappearing_server_test.exe h2_compress_empty_batch_test.exe h2_compress_graceful_server_shutdown_test.exe h2_compress_high_initial_seqno_test.exe h2_compress_invoke_large_request_test.exe h2_compress_large_metadata_test.exe h2_compress_max_concurrent_streams_test.exe h2_compress_max_message_length_test.exe h2_compress_metadata_test.exe h2_compress_no_op_test.exe h2_compress_payload_test.exe h2_compress_ping_pong_streaming_test.exe h2_compress_registered_call_test.exe h2_compress_request_with_flags_test.exe h2_compress_request_with_payload_test.exe h2_compress_server_finishes_request_test.exe h2_compress_shutdown_finishes_calls_test.exe h2_compress_shutdown_finishes_tags_test.exe h2_compress_simple_delayed_request_test.exe h2_compress_simple_request_test.exe h2_compress_trailing_metadata_test.exe h2_fakesec_bad_hostname_test.exe h2_fakesec_binary_metadata_test.exe h2_fakesec_call_creds_test.exe h2_fakesec_cancel_after_accept_test.exe h2_fakesec_cancel_after_client_done_test.exe h2_fakesec_cancel_after_invoke_test.exe h2_fakesec_cancel_before_invoke_test.exe h2_fakesec_cancel_in_a_vacuum_test.exe h2_fakesec_census_simple_request_test.exe h2_fakesec_channel_connectivity_test.exe h2_fakesec_compressed_payload_test.exe h2_fakesec_default_host_test.exe h2_fakesec_disappearing_server_test.exe h2_fakesec_empty_batch_test.exe h2_fakesec_graceful_server_shutdown_test.exe h2_fakesec_high_initial_seqno_test.exe h2_fakesec_invoke_large_request_test.exe h2_fakesec_large_metadata_test.exe h2_fakesec_max_concurrent_streams_test.exe h2_fakesec_max_message_length_test.exe h2_fakesec_metadata_test.exe h2_fakesec_no_op_test.exe h2_fakesec_payload_test.exe h2_fakesec_ping_pong_streaming_test.exe h2_fakesec_registered_call_test.exe h2_fakesec_request_with_flags_test.exe h2_fakesec_request_with_payload_test.exe h2_fakesec_server_finishes_request_test.exe h2_fakesec_shutdown_finishes_calls_test.exe h2_fakesec_shutdown_finishes_tags_test.exe h2_fakesec_simple_delayed_request_test.exe h2_fakesec_simple_request_test.exe h2_fakesec_trailing_metadata_test.exe h2_full_bad_hostname_test.exe h2_full_binary_metadata_test.exe h2_full_call_creds_test.exe h2_full_cancel_after_accept_test.exe h2_full_cancel_after_client_done_test.exe h2_full_cancel_after_invoke_test.exe h2_full_cancel_before_invoke_test.exe h2_full_cancel_in_a_vacuum_test.exe h2_full_census_simple_request_test.exe h2_full_channel_connectivity_test.exe h2_full_compressed_payload_test.exe h2_full_default_host_test.exe h2_full_disappearing_server_test.exe h2_full_empty_batch_test.exe h2_full_graceful_server_shutdown_test.exe h2_full_high_initial_seqno_test.exe h2_full_invoke_large_request_test.exe h2_full_large_metadata_test.exe h2_full_max_concurrent_streams_test.exe h2_full_max_message_length_test.exe h2_full_metadata_test.exe h2_full_no_op_test.exe h2_full_payload_test.exe h2_full_ping_pong_streaming_test.exe h2_full_registered_call_test.exe h2_full_request_with_flags_test.exe h2_full_request_with_payload_test.exe h2_full_server_finishes_request_test.exe h2_full_shutdown_finishes_calls_test.exe h2_full_shutdown_finishes_tags_test.exe h2_full_simple_delayed_request_test.exe h2_full_simple_request_test.exe h2_full_trailing_metadata_test.exe h2_oauth2_bad_hostname_test.exe h2_oauth2_binary_metadata_test.exe h2_oauth2_call_creds_test.exe h2_oauth2_cancel_after_accept_test.exe h2_oauth2_cancel_after_client_done_test.exe h2_oauth2_cancel_after_invoke_test.exe h2_oauth2_cancel_before_invoke_test.exe h2_oauth2_cancel_in_a_vacuum_test.exe h2_oauth2_census_simple_request_test.exe h2_oauth2_channel_connectivity_test.exe h2_oauth2_compressed_payload_test.exe h2_oauth2_default_host_test.exe h2_oauth2_disappearing_server_test.exe h2_oauth2_empty_batch_test.exe h2_oauth2_graceful_server_shutdown_test.exe h2_oauth2_high_initial_seqno_test.exe h2_oauth2_invoke_large_request_test.exe h2_oauth2_large_metadata_test.exe h2_oauth2_max_concurrent_streams_test.exe h2_oauth2_max_message_length_test.exe h2_oauth2_metadata_test.exe h2_oauth2_no_op_test.exe h2_oauth2_payload_test.exe h2_oauth2_ping_pong_streaming_test.exe h2_oauth2_registered_call_test.exe h2_oauth2_request_with_flags_test.exe h2_oauth2_request_with_payload_test.exe h2_oauth2_server_finishes_request_test.exe h2_oauth2_shutdown_finishes_calls_test.exe h2_oauth2_shutdown_finishes_tags_test.exe h2_oauth2_simple_delayed_request_test.exe h2_oauth2_simple_request_test.exe h2_oauth2_trailing_metadata_test.exe h2_proxy_bad_hostname_test.exe h2_proxy_binary_metadata_test.exe h2_proxy_call_creds_test.exe h2_proxy_cancel_after_accept_test.exe h2_proxy_cancel_after_client_done_test.exe h2_proxy_cancel_after_invoke_test.exe h2_proxy_cancel_before_invoke_test.exe h2_proxy_cancel_in_a_vacuum_test.exe h2_proxy_census_simple_request_test.exe h2_proxy_default_host_test.exe h2_proxy_disappearing_server_test.exe h2_proxy_empty_batch_test.exe h2_proxy_graceful_server_shutdown_test.exe h2_proxy_high_initial_seqno_test.exe h2_proxy_invoke_large_request_test.exe h2_proxy_large_metadata_test.exe h2_proxy_max_message_length_test.exe h2_proxy_metadata_test.exe h2_proxy_no_op_test.exe h2_proxy_payload_test.exe h2_proxy_ping_pong_streaming_test.exe h2_proxy_registered_call_test.exe h2_proxy_request_with_payload_test.exe h2_proxy_server_finishes_request_test.exe h2_proxy_shutdown_finishes_calls_test.exe h2_proxy_shutdown_finishes_tags_test.exe h2_proxy_simple_delayed_request_test.exe h2_proxy_simple_request_test.exe h2_proxy_trailing_metadata_test.exe h2_sockpair_bad_hostname_test.exe h2_sockpair_binary_metadata_test.exe h2_sockpair_call_creds_test.exe h2_sockpair_cancel_after_accept_test.exe h2_sockpair_cancel_after_client_done_test.exe h2_sockpair_cancel_after_invoke_test.exe h2_sockpair_cancel_before_invoke_test.exe h2_sockpair_cancel_in_a_vacuum_test.exe h2_sockpair_census_simple_request_test.exe h2_sockpair_compressed_payload_test.exe h2_sockpair_empty_batch_test.exe h2_sockpair_graceful_server_shutdown_test.exe h2_sockpair_high_initial_seqno_test.exe h2_sockpair_invoke_large_request_test.exe h2_sockpair_large_metadata_test.exe h2_sockpair_max_concurrent_streams_test.exe h2_sockpair_max_message_length_test.exe h2_sockpair_metadata_test.exe h2_sockpair_no_op_test.exe h2_sockpair_payload_test.exe h2_sockpair_ping_pong_streaming_test.exe h2_sockpair_registered_call_test.exe h2_sockpair_request_with_flags_test.exe h2_sockpair_request_with_payload_test.exe h2_sockpair_server_finishes_request_test.exe h2_sockpair_shutdown_finishes_calls_test.exe h2_sockpair_shutdown_finishes_tags_test.exe h2_sockpair_simple_request_test.exe h2_sockpair_trailing_metadata_test.exe h2_sockpair+trace_bad_hostname_test.exe h2_sockpair+trace_binary_metadata_test.exe h2_sockpair+trace_call_creds_test.exe h2_sockpair+trace_cancel_after_accept_test.exe h2_sockpair+trace_cancel_after_client_done_test.exe h2_sockpair+trace_cancel_after_invoke_test.exe h2_sockpair+trace_cancel_before_invoke_test.exe h2_sockpair+trace_cancel_in_a_vacuum_test.exe h2_sockpair+trace_census_simple_request_test.exe h2_sockpair+trace_compressed_payload_test.exe h2_sockpair+trace_empty_batch_test.exe h2_sockpair+trace_graceful_server_shutdown_test.exe h2_sockpair+trace_high_initial_seqno_test.exe h2_sockpair+trace_invoke_large_request_test.exe h2_sockpair+trace_large_metadata_test.exe h2_sockpair+trace_max_concurrent_streams_test.exe h2_sockpair+trace_max_message_length_test.exe h2_sockpair+trace_metadata_test.exe h2_sockpair+trace_no_op_test.exe h2_sockpair+trace_payload_test.exe h2_sockpair+trace_ping_pong_streaming_test.exe h2_sockpair+trace_registered_call_test.exe h2_sockpair+trace_request_with_flags_test.exe h2_sockpair+trace_request_with_payload_test.exe h2_sockpair+trace_server_finishes_request_test.exe h2_sockpair+trace_shutdown_finishes_calls_test.exe h2_sockpair+trace_shutdown_finishes_tags_test.exe h2_sockpair+trace_simple_request_test.exe h2_sockpair+trace_trailing_metadata_test.exe h2_sockpair_1byte_bad_hostname_test.exe h2_sockpair_1byte_binary_metadata_test.exe h2_sockpair_1byte_call_creds_test.exe h2_sockpair_1byte_cancel_after_accept_test.exe h2_sockpair_1byte_cancel_after_client_done_test.exe h2_sockpair_1byte_cancel_after_invoke_test.exe h2_sockpair_1byte_cancel_before_invoke_test.exe h2_sockpair_1byte_cancel_in_a_vacuum_test.exe h2_sockpair_1byte_census_simple_request_test.exe h2_sockpair_1byte_compressed_payload_test.exe h2_sockpair_1byte_empty_batch_test.exe h2_sockpair_1byte_graceful_server_shutdown_test.exe h2_sockpair_1byte_high_initial_seqno_test.exe h2_sockpair_1byte_invoke_large_request_test.exe h2_sockpair_1byte_large_metadata_test.exe h2_sockpair_1byte_max_concurrent_streams_test.exe h2_sockpair_1byte_max_message_length_test.exe h2_sockpair_1byte_metadata_test.exe h2_sockpair_1byte_no_op_test.exe h2_sockpair_1byte_payload_test.exe h2_sockpair_1byte_ping_pong_streaming_test.exe h2_sockpair_1byte_registered_call_test.exe h2_sockpair_1byte_request_with_flags_test.exe h2_sockpair_1byte_request_with_payload_test.exe h2_sockpair_1byte_server_finishes_request_test.exe h2_sockpair_1byte_shutdown_finishes_calls_test.exe h2_sockpair_1byte_shutdown_finishes_tags_test.exe h2_sockpair_1byte_simple_request_test.exe h2_sockpair_1byte_trailing_metadata_test.exe h2_ssl_bad_hostname_test.exe h2_ssl_binary_metadata_test.exe h2_ssl_call_creds_test.exe h2_ssl_cancel_after_accept_test.exe h2_ssl_cancel_after_client_done_test.exe h2_ssl_cancel_after_invoke_test.exe h2_ssl_cancel_before_invoke_test.exe h2_ssl_cancel_in_a_vacuum_test.exe h2_ssl_census_simple_request_test.exe h2_ssl_channel_connectivity_test.exe h2_ssl_compressed_payload_test.exe h2_ssl_default_host_test.exe h2_ssl_disappearing_server_test.exe h2_ssl_empty_batch_test.exe h2_ssl_graceful_server_shutdown_test.exe h2_ssl_high_initial_seqno_test.exe h2_ssl_invoke_large_request_test.exe h2_ssl_large_metadata_test.exe h2_ssl_max_concurrent_streams_test.exe h2_ssl_max_message_length_test.exe h2_ssl_metadata_test.exe h2_ssl_no_op_test.exe h2_ssl_payload_test.exe h2_ssl_ping_pong_streaming_test.exe h2_ssl_registered_call_test.exe h2_ssl_request_with_flags_test.exe h2_ssl_request_with_payload_test.exe h2_ssl_server_finishes_request_test.exe h2_ssl_shutdown_finishes_calls_test.exe h2_ssl_shutdown_finishes_tags_test.exe h2_ssl_simple_delayed_request_test.exe h2_ssl_simple_request_test.exe h2_ssl_trailing_metadata_test.exe h2_ssl_proxy_bad_hostname_test.exe h2_ssl_proxy_binary_metadata_test.exe h2_ssl_proxy_call_creds_test.exe h2_ssl_proxy_cancel_after_accept_test.exe h2_ssl_proxy_cancel_after_client_done_test.exe h2_ssl_proxy_cancel_after_invoke_test.exe h2_ssl_proxy_cancel_before_invoke_test.exe h2_ssl_proxy_cancel_in_a_vacuum_test.exe h2_ssl_proxy_census_simple_request_test.exe h2_ssl_proxy_default_host_test.exe h2_ssl_proxy_disappearing_server_test.exe h2_ssl_proxy_empty_batch_test.exe h2_ssl_proxy_graceful_server_shutdown_test.exe h2_ssl_proxy_high_initial_seqno_test.exe h2_ssl_proxy_invoke_large_request_test.exe h2_ssl_proxy_large_metadata_test.exe h2_ssl_proxy_max_message_length_test.exe h2_ssl_proxy_metadata_test.exe h2_ssl_proxy_no_op_test.exe h2_ssl_proxy_payload_test.exe h2_ssl_proxy_ping_pong_streaming_test.exe h2_ssl_proxy_registered_call_test.exe h2_ssl_proxy_request_with_payload_test.exe h2_ssl_proxy_server_finishes_request_test.exe h2_ssl_proxy_shutdown_finishes_calls_test.exe h2_ssl_proxy_shutdown_finishes_tags_test.exe h2_ssl_proxy_simple_delayed_request_test.exe h2_ssl_proxy_simple_request_test.exe h2_ssl_proxy_trailing_metadata_test.exe h2_compress_bad_hostname_nosec_test.exe h2_compress_binary_metadata_nosec_test.exe h2_compress_cancel_after_accept_nosec_test.exe h2_compress_cancel_after_client_done_nosec_test.exe h2_compress_cancel_after_invoke_nosec_test.exe h2_compress_cancel_before_invoke_nosec_test.exe h2_compress_cancel_in_a_vacuum_nosec_test.exe h2_compress_census_simple_request_nosec_test.exe h2_compress_channel_connectivity_nosec_test.exe h2_compress_compressed_payload_nosec_test.exe h2_compress_default_host_nosec_test.exe h2_compress_disappearing_server_nosec_test.exe h2_compress_empty_batch_nosec_test.exe h2_compress_graceful_server_shutdown_nosec_test.exe h2_compress_high_initial_seqno_nosec_test.exe h2_compress_invoke_large_request_nosec_test.exe h2_compress_large_metadata_nosec_test.exe h2_compress_max_concurrent_streams_nosec_test.exe h2_compress_max_message_length_nosec_test.exe h2_compress_metadata_nosec_test.exe h2_compress_no_op_nosec_test.exe h2_compress_payload_nosec_test.exe h2_compress_ping_pong_streaming_nosec_test.exe h2_compress_registered_call_nosec_test.exe h2_compress_request_with_flags_nosec_test.exe h2_compress_request_with_payload_nosec_test.exe h2_compress_server_finishes_request_nosec_test.exe h2_compress_shutdown_finishes_calls_nosec_test.exe h2_compress_shutdown_finishes_tags_nosec_test.exe h2_compress_simple_delayed_request_nosec_test.exe h2_compress_simple_request_nosec_test.exe h2_compress_trailing_metadata_nosec_test.exe h2_full_bad_hostname_nosec_test.exe h2_full_binary_metadata_nosec_test.exe h2_full_cancel_after_accept_nosec_test.exe h2_full_cancel_after_client_done_nosec_test.exe h2_full_cancel_after_invoke_nosec_test.exe h2_full_cancel_before_invoke_nosec_test.exe h2_full_cancel_in_a_vacuum_nosec_test.exe h2_full_census_simple_request_nosec_test.exe h2_full_channel_connectivity_nosec_test.exe h2_full_compressed_payload_nosec_test.exe h2_full_default_host_nosec_test.exe h2_full_disappearing_server_nosec_test.exe h2_full_empty_batch_nosec_test.exe h2_full_graceful_server_shutdown_nosec_test.exe h2_full_high_initial_seqno_nosec_test.exe h2_full_invoke_large_request_nosec_test.exe h2_full_large_metadata_nosec_test.exe h2_full_max_concurrent_streams_nosec_test.exe h2_full_max_message_length_nosec_test.exe h2_full_metadata_nosec_test.exe h2_full_no_op_nosec_test.exe h2_full_payload_nosec_test.exe h2_full_ping_pong_streaming_nosec_test.exe h2_full_registered_call_nosec_test.exe h2_full_request_with_flags_nosec_test.exe h2_full_request_with_payload_nosec_test.exe h2_full_server_finishes_request_nosec_test.exe h2_full_shutdown_finishes_calls_nosec_test.exe h2_full_shutdown_finishes_tags_nosec_test.exe h2_full_simple_delayed_request_nosec_test.exe h2_full_simple_request_nosec_test.exe h2_full_trailing_metadata_nosec_test.exe h2_proxy_bad_hostname_nosec_test.exe h2_proxy_binary_metadata_nosec_test.exe h2_proxy_cancel_after_accept_nosec_test.exe h2_proxy_cancel_after_client_done_nosec_test.exe h2_proxy_cancel_after_invoke_nosec_test.exe h2_proxy_cancel_before_invoke_nosec_test.exe h2_proxy_cancel_in_a_vacuum_nosec_test.exe h2_proxy_census_simple_request_nosec_test.exe h2_proxy_default_host_nosec_test.exe h2_proxy_disappearing_server_nosec_test.exe h2_proxy_empty_batch_nosec_test.exe h2_proxy_graceful_server_shutdown_nosec_test.exe h2_proxy_high_initial_seqno_nosec_test.exe h2_proxy_invoke_large_request_nosec_test.exe h2_proxy_large_metadata_nosec_test.exe h2_proxy_max_message_length_nosec_test.exe h2_proxy_metadata_nosec_test.exe h2_proxy_no_op_nosec_test.exe h2_proxy_payload_nosec_test.exe h2_proxy_ping_pong_streaming_nosec_test.exe h2_proxy_registered_call_nosec_test.exe h2_proxy_request_with_payload_nosec_test.exe h2_proxy_server_finishes_request_nosec_test.exe h2_proxy_shutdown_finishes_calls_nosec_test.exe h2_proxy_shutdown_finishes_tags_nosec_test.exe h2_proxy_simple_delayed_request_nosec_test.exe h2_proxy_simple_request_nosec_test.exe h2_proxy_trailing_metadata_nosec_test.exe h2_sockpair_bad_hostname_nosec_test.exe h2_sockpair_binary_metadata_nosec_test.exe h2_sockpair_cancel_after_accept_nosec_test.exe h2_sockpair_cancel_after_client_done_nosec_test.exe h2_sockpair_cancel_after_invoke_nosec_test.exe h2_sockpair_cancel_before_invoke_nosec_test.exe h2_sockpair_cancel_in_a_vacuum_nosec_test.exe h2_sockpair_census_simple_request_nosec_test.exe h2_sockpair_compressed_payload_nosec_test.exe h2_sockpair_empty_batch_nosec_test.exe h2_sockpair_graceful_server_shutdown_nosec_test.exe h2_sockpair_high_initial_seqno_nosec_test.exe h2_sockpair_invoke_large_request_nosec_test.exe h2_sockpair_large_metadata_nosec_test.exe h2_sockpair_max_concurrent_streams_nosec_test.exe h2_sockpair_max_message_length_nosec_test.exe h2_sockpair_metadata_nosec_test.exe h2_sockpair_no_op_nosec_test.exe h2_sockpair_payload_nosec_test.exe h2_sockpair_ping_pong_streaming_nosec_test.exe h2_sockpair_registered_call_nosec_test.exe h2_sockpair_request_with_flags_nosec_test.exe h2_sockpair_request_with_payload_nosec_test.exe h2_sockpair_server_finishes_request_nosec_test.exe h2_sockpair_shutdown_finishes_calls_nosec_test.exe h2_sockpair_shutdown_finishes_tags_nosec_test.exe h2_sockpair_simple_request_nosec_test.exe h2_sockpair_trailing_metadata_nosec_test.exe h2_sockpair+trace_bad_hostname_nosec_test.exe h2_sockpair+trace_binary_metadata_nosec_test.exe h2_sockpair+trace_cancel_after_accept_nosec_test.exe h2_sockpair+trace_cancel_after_client_done_nosec_test.exe h2_sockpair+trace_cancel_after_invoke_nosec_test.exe h2_sockpair+trace_cancel_before_invoke_nosec_test.exe h2_sockpair+trace_cancel_in_a_vacuum_nosec_test.exe h2_sockpair+trace_census_simple_request_nosec_test.exe h2_sockpair+trace_compressed_payload_nosec_test.exe h2_sockpair+trace_empty_batch_nosec_test.exe h2_sockpair+trace_graceful_server_shutdown_nosec_test.exe h2_sockpair+trace_high_initial_seqno_nosec_test.exe h2_sockpair+trace_invoke_large_request_nosec_test.exe h2_sockpair+trace_large_metadata_nosec_test.exe h2_sockpair+trace_max_concurrent_streams_nosec_test.exe h2_sockpair+trace_max_message_length_nosec_test.exe h2_sockpair+trace_metadata_nosec_test.exe h2_sockpair+trace_no_op_nosec_test.exe h2_sockpair+trace_payload_nosec_test.exe h2_sockpair+trace_ping_pong_streaming_nosec_test.exe h2_sockpair+trace_registered_call_nosec_test.exe h2_sockpair+trace_request_with_flags_nosec_test.exe h2_sockpair+trace_request_with_payload_nosec_test.exe h2_sockpair+trace_server_finishes_request_nosec_test.exe h2_sockpair+trace_shutdown_finishes_calls_nosec_test.exe h2_sockpair+trace_shutdown_finishes_tags_nosec_test.exe h2_sockpair+trace_simple_request_nosec_test.exe h2_sockpair+trace_trailing_metadata_nosec_test.exe h2_sockpair_1byte_bad_hostname_nosec_test.exe h2_sockpair_1byte_binary_metadata_nosec_test.exe h2_sockpair_1byte_cancel_after_accept_nosec_test.exe h2_sockpair_1byte_cancel_after_client_done_nosec_test.exe h2_sockpair_1byte_cancel_after_invoke_nosec_test.exe h2_sockpair_1byte_cancel_before_invoke_nosec_test.exe h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test.exe h2_sockpair_1byte_census_simple_request_nosec_test.exe h2_sockpair_1byte_compressed_payload_nosec_test.exe h2_sockpair_1byte_empty_batch_nosec_test.exe h2_sockpair_1byte_graceful_server_shutdown_nosec_test.exe h2_sockpair_1byte_high_initial_seqno_nosec_test.exe h2_sockpair_1byte_invoke_large_request_nosec_test.exe h2_sockpair_1byte_large_metadata_nosec_test.exe h2_sockpair_1byte_max_concurrent_streams_nosec_test.exe h2_sockpair_1byte_max_message_length_nosec_test.exe h2_sockpair_1byte_metadata_nosec_test.exe h2_sockpair_1byte_no_op_nosec_test.exe h2_sockpair_1byte_payload_nosec_test.exe h2_sockpair_1byte_ping_pong_streaming_nosec_test.exe h2_sockpair_1byte_registered_call_nosec_test.exe h2_sockpair_1byte_request_with_flags_nosec_test.exe h2_sockpair_1byte_request_with_payload_nosec_test.exe h2_sockpair_1byte_server_finishes_request_nosec_test.exe h2_sockpair_1byte_shutdown_finishes_calls_nosec_test.exe h2_sockpair_1byte_shutdown_finishes_tags_nosec_test.exe h2_sockpair_1byte_simple_request_nosec_test.exe h2_sockpair_1byte_trailing_metadata_nosec_test.exe connection_prefix_bad_client_test.exe initial_settings_frame_bad_client_test.exe 
	echo All C tests built.

buildtests_cxx: async_end2end_test.exe auth_property_iterator_test.exe channel_arguments_test.exe cli_call_test.exe client_crash_test_server.exe credentials_test.exe cxx_byte_buffer_test.exe cxx_slice_test.exe cxx_string_ref_test.exe cxx_time_test.exe end2end_test.exe generic_end2end_test.exe grpc_cli.exe mock_test.exe reconnect_interop_client.exe reconnect_interop_server.exe secure_auth_context_test.exe server_crash_test_client.exe shutdown_test.exe status_test.exe thread_stress_test.exe 
	echo All C++ tests built.


alarm_heap_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building alarm_heap_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\alarm_heap_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\alarm_heap_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\alarm_heap_test.obj 
alarm_heap_test: alarm_heap_test.exe
	echo Running alarm_heap_test
	$(OUT_DIR)\alarm_heap_test.exe

alarm_list_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building alarm_list_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\alarm_list_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\alarm_list_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\alarm_list_test.obj 
alarm_list_test: alarm_list_test.exe
	echo Running alarm_list_test
	$(OUT_DIR)\alarm_list_test.exe

alarm_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building alarm_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\alarm_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\alarm_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\alarm_test.obj 
alarm_test: alarm_test.exe
	echo Running alarm_test
	$(OUT_DIR)\alarm_test.exe

alpn_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building alpn_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\alpn_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\alpn_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\alpn_test.obj 
alpn_test: alpn_test.exe
	echo Running alpn_test
	$(OUT_DIR)\alpn_test.exe

bin_encoder_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building bin_encoder_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\bin_encoder_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\bin_encoder_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\bin_encoder_test.obj 
bin_encoder_test: bin_encoder_test.exe
	echo Running bin_encoder_test
	$(OUT_DIR)\bin_encoder_test.exe

chttp2_status_conversion_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building chttp2_status_conversion_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\status_conversion_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\chttp2_status_conversion_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\status_conversion_test.obj 
chttp2_status_conversion_test: chttp2_status_conversion_test.exe
	echo Running chttp2_status_conversion_test
	$(OUT_DIR)\chttp2_status_conversion_test.exe

chttp2_stream_encoder_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building chttp2_stream_encoder_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\stream_encoder_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\chttp2_stream_encoder_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\stream_encoder_test.obj 
chttp2_stream_encoder_test: chttp2_stream_encoder_test.exe
	echo Running chttp2_stream_encoder_test
	$(OUT_DIR)\chttp2_stream_encoder_test.exe

chttp2_stream_map_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building chttp2_stream_map_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\stream_map_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\chttp2_stream_map_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\stream_map_test.obj 
chttp2_stream_map_test: chttp2_stream_map_test.exe
	echo Running chttp2_stream_map_test
	$(OUT_DIR)\chttp2_stream_map_test.exe

compression_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building compression_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\compression\compression_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\compression_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\compression_test.obj 
compression_test: compression_test.exe
	echo Running compression_test
	$(OUT_DIR)\compression_test.exe

endpoint_pair_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building endpoint_pair_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\endpoint_pair_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\endpoint_pair_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\endpoint_pair_test.obj 
endpoint_pair_test: endpoint_pair_test.exe
	echo Running endpoint_pair_test
	$(OUT_DIR)\endpoint_pair_test.exe

fling_client.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building fling_client
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\fling\client.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\fling_client.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\client.obj 
fling_client: fling_client.exe
	echo Running fling_client
	$(OUT_DIR)\fling_client.exe

fling_server.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building fling_server
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\fling\server.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\fling_server.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\server.obj 
fling_server: fling_server.exe
	echo Running fling_server
	$(OUT_DIR)\fling_server.exe

gen_hpack_tables.exe: build_gpr build_grpc $(OUT_DIR)
	echo Building gen_hpack_tables
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\tools\codegen\core\gen_hpack_tables.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gen_hpack_tables.exe" Debug\gpr.lib Debug\grpc.lib $(LIBS) $(OUT_DIR)\gen_hpack_tables.obj 
gen_hpack_tables: gen_hpack_tables.exe
	echo Running gen_hpack_tables
	$(OUT_DIR)\gen_hpack_tables.exe

gen_legal_metadata_characters.exe: $(OUT_DIR)
	echo Building gen_legal_metadata_characters
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\tools\codegen\core\gen_legal_metadata_characters.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gen_legal_metadata_characters.exe" $(LIBS) $(OUT_DIR)\gen_legal_metadata_characters.obj 
gen_legal_metadata_characters: gen_legal_metadata_characters.exe
	echo Running gen_legal_metadata_characters
	$(OUT_DIR)\gen_legal_metadata_characters.exe

gpr_cmdline_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_cmdline_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\cmdline_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_cmdline_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\cmdline_test.obj 
gpr_cmdline_test: gpr_cmdline_test.exe
	echo Running gpr_cmdline_test
	$(OUT_DIR)\gpr_cmdline_test.exe

gpr_env_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_env_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\env_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_env_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\env_test.obj 
gpr_env_test: gpr_env_test.exe
	echo Running gpr_env_test
	$(OUT_DIR)\gpr_env_test.exe

gpr_file_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_file_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\file_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_file_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\file_test.obj 
gpr_file_test: gpr_file_test.exe
	echo Running gpr_file_test
	$(OUT_DIR)\gpr_file_test.exe

gpr_histogram_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_histogram_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\histogram_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_histogram_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\histogram_test.obj 
gpr_histogram_test: gpr_histogram_test.exe
	echo Running gpr_histogram_test
	$(OUT_DIR)\gpr_histogram_test.exe

gpr_host_port_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_host_port_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\host_port_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_host_port_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\host_port_test.obj 
gpr_host_port_test: gpr_host_port_test.exe
	echo Running gpr_host_port_test
	$(OUT_DIR)\gpr_host_port_test.exe

gpr_log_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_log_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\log_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_log_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\log_test.obj 
gpr_log_test: gpr_log_test.exe
	echo Running gpr_log_test
	$(OUT_DIR)\gpr_log_test.exe

gpr_slice_buffer_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_slice_buffer_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\slice_buffer_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_slice_buffer_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\slice_buffer_test.obj 
gpr_slice_buffer_test: gpr_slice_buffer_test.exe
	echo Running gpr_slice_buffer_test
	$(OUT_DIR)\gpr_slice_buffer_test.exe

gpr_slice_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_slice_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\slice_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_slice_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\slice_test.obj 
gpr_slice_test: gpr_slice_test.exe
	echo Running gpr_slice_test
	$(OUT_DIR)\gpr_slice_test.exe

gpr_stack_lockfree_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_stack_lockfree_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\stack_lockfree_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_stack_lockfree_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\stack_lockfree_test.obj 
gpr_stack_lockfree_test: gpr_stack_lockfree_test.exe
	echo Running gpr_stack_lockfree_test
	$(OUT_DIR)\gpr_stack_lockfree_test.exe

gpr_string_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_string_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\string_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_string_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\string_test.obj 
gpr_string_test: gpr_string_test.exe
	echo Running gpr_string_test
	$(OUT_DIR)\gpr_string_test.exe

gpr_sync_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_sync_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\sync_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_sync_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\sync_test.obj 
gpr_sync_test: gpr_sync_test.exe
	echo Running gpr_sync_test
	$(OUT_DIR)\gpr_sync_test.exe

gpr_thd_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_thd_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\thd_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_thd_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\thd_test.obj 
gpr_thd_test: gpr_thd_test.exe
	echo Running gpr_thd_test
	$(OUT_DIR)\gpr_thd_test.exe

gpr_time_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_time_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\time_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_time_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\time_test.obj 
gpr_time_test: gpr_time_test.exe
	echo Running gpr_time_test
	$(OUT_DIR)\gpr_time_test.exe

gpr_tls_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_tls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\tls_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_tls_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\tls_test.obj 
gpr_tls_test: gpr_tls_test.exe
	echo Running gpr_tls_test
	$(OUT_DIR)\gpr_tls_test.exe

gpr_useful_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building gpr_useful_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\useful_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\gpr_useful_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\useful_test.obj 
gpr_useful_test: gpr_useful_test.exe
	echo Running gpr_useful_test
	$(OUT_DIR)\gpr_useful_test.exe

grpc_auth_context_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_auth_context_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\auth_context_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_auth_context_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\auth_context_test.obj 
grpc_auth_context_test: grpc_auth_context_test.exe
	echo Running grpc_auth_context_test
	$(OUT_DIR)\grpc_auth_context_test.exe

grpc_base64_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_base64_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\base64_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_base64_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\base64_test.obj 
grpc_base64_test: grpc_base64_test.exe
	echo Running grpc_base64_test
	$(OUT_DIR)\grpc_base64_test.exe

grpc_byte_buffer_reader_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_byte_buffer_reader_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\surface\byte_buffer_reader_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_byte_buffer_reader_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\byte_buffer_reader_test.obj 
grpc_byte_buffer_reader_test: grpc_byte_buffer_reader_test.exe
	echo Running grpc_byte_buffer_reader_test
	$(OUT_DIR)\grpc_byte_buffer_reader_test.exe

grpc_channel_args_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_channel_args_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\channel\channel_args_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_channel_args_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\channel_args_test.obj 
grpc_channel_args_test: grpc_channel_args_test.exe
	echo Running grpc_channel_args_test
	$(OUT_DIR)\grpc_channel_args_test.exe

grpc_channel_stack_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_channel_stack_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\channel\channel_stack_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_channel_stack_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\channel_stack_test.obj 
grpc_channel_stack_test: grpc_channel_stack_test.exe
	echo Running grpc_channel_stack_test
	$(OUT_DIR)\grpc_channel_stack_test.exe

grpc_completion_queue_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_completion_queue_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\surface\completion_queue_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_completion_queue_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\completion_queue_test.obj 
grpc_completion_queue_test: grpc_completion_queue_test.exe
	echo Running grpc_completion_queue_test
	$(OUT_DIR)\grpc_completion_queue_test.exe

grpc_create_jwt.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_create_jwt
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\create_jwt.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_create_jwt.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\create_jwt.obj 
grpc_create_jwt: grpc_create_jwt.exe
	echo Running grpc_create_jwt
	$(OUT_DIR)\grpc_create_jwt.exe

grpc_credentials_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_credentials_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\credentials_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_credentials_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\credentials_test.obj 
grpc_credentials_test: grpc_credentials_test.exe
	echo Running grpc_credentials_test
	$(OUT_DIR)\grpc_credentials_test.exe

grpc_fetch_oauth2.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_fetch_oauth2
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\fetch_oauth2.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_fetch_oauth2.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\fetch_oauth2.obj 
grpc_fetch_oauth2: grpc_fetch_oauth2.exe
	echo Running grpc_fetch_oauth2
	$(OUT_DIR)\grpc_fetch_oauth2.exe

grpc_json_token_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_json_token_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\json_token_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_json_token_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\json_token_test.obj 
grpc_json_token_test: grpc_json_token_test.exe
	echo Running grpc_json_token_test
	$(OUT_DIR)\grpc_json_token_test.exe

grpc_jwt_verifier_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_jwt_verifier_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\jwt_verifier_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_jwt_verifier_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\jwt_verifier_test.obj 
grpc_jwt_verifier_test: grpc_jwt_verifier_test.exe
	echo Running grpc_jwt_verifier_test
	$(OUT_DIR)\grpc_jwt_verifier_test.exe

grpc_print_google_default_creds_token.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_print_google_default_creds_token
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\print_google_default_creds_token.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_print_google_default_creds_token.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\print_google_default_creds_token.obj 
grpc_print_google_default_creds_token: grpc_print_google_default_creds_token.exe
	echo Running grpc_print_google_default_creds_token
	$(OUT_DIR)\grpc_print_google_default_creds_token.exe

grpc_security_connector_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_security_connector_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\security_connector_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_security_connector_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\security_connector_test.obj 
grpc_security_connector_test: grpc_security_connector_test.exe
	echo Running grpc_security_connector_test
	$(OUT_DIR)\grpc_security_connector_test.exe

grpc_stream_op_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_stream_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\stream_op_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_stream_op_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\stream_op_test.obj 
grpc_stream_op_test: grpc_stream_op_test.exe
	echo Running grpc_stream_op_test
	$(OUT_DIR)\grpc_stream_op_test.exe

grpc_verify_jwt.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building grpc_verify_jwt
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\verify_jwt.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_verify_jwt.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\verify_jwt.obj 
grpc_verify_jwt: grpc_verify_jwt.exe
	echo Running grpc_verify_jwt
	$(OUT_DIR)\grpc_verify_jwt.exe

hpack_parser_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building hpack_parser_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\hpack_parser_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\hpack_parser_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\hpack_parser_test.obj 
hpack_parser_test: hpack_parser_test.exe
	echo Running hpack_parser_test
	$(OUT_DIR)\hpack_parser_test.exe

hpack_table_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building hpack_table_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\hpack_table_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\hpack_table_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\hpack_table_test.obj 
hpack_table_test: hpack_table_test.exe
	echo Running hpack_table_test
	$(OUT_DIR)\hpack_table_test.exe

httpcli_format_request_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building httpcli_format_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\httpcli\format_request_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\httpcli_format_request_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\format_request_test.obj 
httpcli_format_request_test: httpcli_format_request_test.exe
	echo Running httpcli_format_request_test
	$(OUT_DIR)\httpcli_format_request_test.exe

httpcli_parser_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building httpcli_parser_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\httpcli\parser_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\httpcli_parser_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\parser_test.obj 
httpcli_parser_test: httpcli_parser_test.exe
	echo Running httpcli_parser_test
	$(OUT_DIR)\httpcli_parser_test.exe

json_rewrite.exe: build_grpc build_gpr $(OUT_DIR)
	echo Building json_rewrite
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\json\json_rewrite.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\json_rewrite.exe" Debug\grpc.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\json_rewrite.obj 
json_rewrite: json_rewrite.exe
	echo Running json_rewrite
	$(OUT_DIR)\json_rewrite.exe

json_rewrite_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building json_rewrite_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\json\json_rewrite_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\json_rewrite_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\json_rewrite_test.obj 
json_rewrite_test: json_rewrite_test.exe
	echo Running json_rewrite_test
	$(OUT_DIR)\json_rewrite_test.exe

json_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building json_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\json\json_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\json_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\json_test.obj 
json_test: json_test.exe
	echo Running json_test
	$(OUT_DIR)\json_test.exe

lame_client_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building lame_client_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\surface\lame_client_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\lame_client_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\lame_client_test.obj 
lame_client_test: lame_client_test.exe
	echo Running lame_client_test
	$(OUT_DIR)\lame_client_test.exe

message_compress_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building message_compress_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\compression\message_compress_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\message_compress_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\message_compress_test.obj 
message_compress_test: message_compress_test.exe
	echo Running message_compress_test
	$(OUT_DIR)\message_compress_test.exe

multi_init_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building multi_init_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\surface\multi_init_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\multi_init_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\multi_init_test.obj 
multi_init_test: multi_init_test.exe
	echo Running multi_init_test
	$(OUT_DIR)\multi_init_test.exe

multiple_server_queues_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building multiple_server_queues_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\multiple_server_queues_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\multiple_server_queues_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\multiple_server_queues_test.obj 
multiple_server_queues_test: multiple_server_queues_test.exe
	echo Running multiple_server_queues_test
	$(OUT_DIR)\multiple_server_queues_test.exe

murmur_hash_test.exe: build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building murmur_hash_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\support\murmur_hash_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\murmur_hash_test.exe" Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\murmur_hash_test.obj 
murmur_hash_test: murmur_hash_test.exe
	echo Running murmur_hash_test
	$(OUT_DIR)\murmur_hash_test.exe

no_server_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building no_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\no_server_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\no_server_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\no_server_test.obj 
no_server_test: no_server_test.exe
	echo Running no_server_test
	$(OUT_DIR)\no_server_test.exe

resolve_address_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building resolve_address_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\resolve_address_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\resolve_address_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\resolve_address_test.obj 
resolve_address_test: resolve_address_test.exe
	echo Running resolve_address_test
	$(OUT_DIR)\resolve_address_test.exe

secure_endpoint_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building secure_endpoint_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\security\secure_endpoint_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\secure_endpoint_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\secure_endpoint_test.obj 
secure_endpoint_test: secure_endpoint_test.exe
	echo Running secure_endpoint_test
	$(OUT_DIR)\secure_endpoint_test.exe

sockaddr_utils_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building sockaddr_utils_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\sockaddr_utils_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\sockaddr_utils_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\sockaddr_utils_test.obj 
sockaddr_utils_test: sockaddr_utils_test.exe
	echo Running sockaddr_utils_test
	$(OUT_DIR)\sockaddr_utils_test.exe

time_averaged_stats_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building time_averaged_stats_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\iomgr\time_averaged_stats_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\time_averaged_stats_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\time_averaged_stats_test.obj 
time_averaged_stats_test: time_averaged_stats_test.exe
	echo Running time_averaged_stats_test
	$(OUT_DIR)\time_averaged_stats_test.exe

timeout_encoding_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building timeout_encoding_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\chttp2\timeout_encoding_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\timeout_encoding_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\timeout_encoding_test.obj 
timeout_encoding_test: timeout_encoding_test.exe
	echo Running timeout_encoding_test
	$(OUT_DIR)\timeout_encoding_test.exe

timers_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building timers_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\profiling\timers_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\timers_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\timers_test.obj 
timers_test: timers_test.exe
	echo Running timers_test
	$(OUT_DIR)\timers_test.exe

transport_metadata_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building transport_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\transport\metadata_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\transport_metadata_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\metadata_test.obj 
transport_metadata_test: transport_metadata_test.exe
	echo Running transport_metadata_test
	$(OUT_DIR)\transport_metadata_test.exe

transport_security_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building transport_security_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\tsi\transport_security_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\transport_security_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\transport_security_test.obj 
transport_security_test: transport_security_test.exe
	echo Running transport_security_test
	$(OUT_DIR)\transport_security_test.exe

uri_parser_test.exe: build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building uri_parser_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\client_config\uri_parser_test.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\uri_parser_test.exe" Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\uri_parser_test.obj 
uri_parser_test: uri_parser_test.exe
	echo Running uri_parser_test
	$(OUT_DIR)\uri_parser_test.exe

async_end2end_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building async_end2end_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\async_end2end_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\async_end2end_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\async_end2end_test.obj 
async_end2end_test: async_end2end_test.exe
	echo Running async_end2end_test
	$(OUT_DIR)\async_end2end_test.exe

auth_property_iterator_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building auth_property_iterator_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\common\auth_property_iterator_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\auth_property_iterator_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\auth_property_iterator_test.obj 
auth_property_iterator_test: auth_property_iterator_test.exe
	echo Running auth_property_iterator_test
	$(OUT_DIR)\auth_property_iterator_test.exe

channel_arguments_test.exe: build_grpc++ build_grpc build_gpr $(OUT_DIR)
	echo Building channel_arguments_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\client\channel_arguments_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\channel_arguments_test.exe" Debug\grpc++.lib Debug\grpc.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\channel_arguments_test.obj 
channel_arguments_test: channel_arguments_test.exe
	echo Running channel_arguments_test
	$(OUT_DIR)\channel_arguments_test.exe

cli_call_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building cli_call_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\cli_call_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\cli_call_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\cli_call_test.obj 
cli_call_test: cli_call_test.exe
	echo Running cli_call_test
	$(OUT_DIR)\cli_call_test.exe

client_crash_test_server.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building client_crash_test_server
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\client_crash_test_server.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\client_crash_test_server.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\client_crash_test_server.obj 
client_crash_test_server: client_crash_test_server.exe
	echo Running client_crash_test_server
	$(OUT_DIR)\client_crash_test_server.exe

credentials_test.exe: build_grpc++ build_grpc build_gpr $(OUT_DIR)
	echo Building credentials_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\client\credentials_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\credentials_test.exe" Debug\grpc++.lib Debug\grpc.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\credentials_test.obj 
credentials_test: credentials_test.exe
	echo Running credentials_test
	$(OUT_DIR)\credentials_test.exe

cxx_byte_buffer_test.exe: build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building cxx_byte_buffer_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\byte_buffer_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\cxx_byte_buffer_test.exe" Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\byte_buffer_test.obj 
cxx_byte_buffer_test: cxx_byte_buffer_test.exe
	echo Running cxx_byte_buffer_test
	$(OUT_DIR)\cxx_byte_buffer_test.exe

cxx_slice_test.exe: build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building cxx_slice_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\slice_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\cxx_slice_test.exe" Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\slice_test.obj 
cxx_slice_test: cxx_slice_test.exe
	echo Running cxx_slice_test
	$(OUT_DIR)\cxx_slice_test.exe

cxx_string_ref_test.exe: build_grpc++ $(OUT_DIR)
	echo Building cxx_string_ref_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\string_ref_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\cxx_string_ref_test.exe" Debug\grpc++.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\string_ref_test.obj 
cxx_string_ref_test: cxx_string_ref_test.exe
	echo Running cxx_string_ref_test
	$(OUT_DIR)\cxx_string_ref_test.exe

cxx_time_test.exe: build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building cxx_time_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\time_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\cxx_time_test.exe" Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\time_test.obj 
cxx_time_test: cxx_time_test.exe
	echo Running cxx_time_test
	$(OUT_DIR)\cxx_time_test.exe

end2end_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building end2end_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\end2end_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\end2end_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\end2end_test.obj 
end2end_test: end2end_test.exe
	echo Running end2end_test
	$(OUT_DIR)\end2end_test.exe

generic_end2end_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building generic_end2end_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\generic_end2end_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\generic_end2end_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\generic_end2end_test.obj 
generic_end2end_test: generic_end2end_test.exe
	echo Running generic_end2end_test
	$(OUT_DIR)\generic_end2end_test.exe

grpc_cli.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr Debug\grpc++_test_config.lib $(OUT_DIR)
	echo Building grpc_cli
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\grpc_cli.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\grpc_cli.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib Debug\grpc++_test_config.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\grpc_cli.obj 
grpc_cli: grpc_cli.exe
	echo Running grpc_cli
	$(OUT_DIR)\grpc_cli.exe

mock_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building mock_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\mock_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\mock_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\mock_test.obj 
mock_test: mock_test.exe
	echo Running mock_test
	$(OUT_DIR)\mock_test.exe

qps_driver.exe: Debug\qps.lib Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr Debug\grpc++_test_config.lib $(OUT_DIR)
	echo Building qps_driver
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\qps\qps_driver.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\qps_driver.exe" Debug\qps.lib Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib Debug\grpc++_test_config.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\qps_driver.obj 
qps_driver: qps_driver.exe
	echo Running qps_driver
	$(OUT_DIR)\qps_driver.exe

qps_worker.exe: Debug\qps.lib Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr Debug\grpc++_test_config.lib $(OUT_DIR)
	echo Building qps_worker
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\qps\worker.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\qps_worker.exe" Debug\qps.lib Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib Debug\grpc++_test_config.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\worker.obj 
qps_worker: qps_worker.exe
	echo Running qps_worker
	$(OUT_DIR)\qps_worker.exe

reconnect_interop_client.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr Debug\grpc++_test_config.lib $(OUT_DIR)
	echo Building reconnect_interop_client
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\interop\reconnect_interop_client.cc $(REPO_ROOT)\test\proto\empty.pb.cc $(REPO_ROOT)\test\proto\empty.grpc.pb.cc $(REPO_ROOT)\test\proto\messages.pb.cc $(REPO_ROOT)\test\proto\messages.grpc.pb.cc $(REPO_ROOT)\test\proto\test.pb.cc $(REPO_ROOT)\test\proto\test.grpc.pb.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\reconnect_interop_client.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib Debug\grpc++_test_config.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\reconnect_interop_client.obj $(OUT_DIR)\empty.pb.obj $(OUT_DIR)\empty.grpc.pb.obj $(OUT_DIR)\messages.pb.obj $(OUT_DIR)\messages.grpc.pb.obj $(OUT_DIR)\test.pb.obj $(OUT_DIR)\test.grpc.pb.obj 
reconnect_interop_client: reconnect_interop_client.exe
	echo Running reconnect_interop_client
	$(OUT_DIR)\reconnect_interop_client.exe

reconnect_interop_server.exe: Debug\reconnect_server.lib Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr Debug\grpc++_test_config.lib $(OUT_DIR)
	echo Building reconnect_interop_server
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\interop\reconnect_interop_server.cc $(REPO_ROOT)\test\proto\empty.pb.cc $(REPO_ROOT)\test\proto\empty.grpc.pb.cc $(REPO_ROOT)\test\proto\messages.pb.cc $(REPO_ROOT)\test\proto\messages.grpc.pb.cc $(REPO_ROOT)\test\proto\test.pb.cc $(REPO_ROOT)\test\proto\test.grpc.pb.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\reconnect_interop_server.exe" Debug\reconnect_server.lib Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib Debug\grpc++_test_config.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\reconnect_interop_server.obj $(OUT_DIR)\empty.pb.obj $(OUT_DIR)\empty.grpc.pb.obj $(OUT_DIR)\messages.pb.obj $(OUT_DIR)\messages.grpc.pb.obj $(OUT_DIR)\test.pb.obj $(OUT_DIR)\test.grpc.pb.obj 
reconnect_interop_server: reconnect_interop_server.exe
	echo Running reconnect_interop_server
	$(OUT_DIR)\reconnect_interop_server.exe

secure_auth_context_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building secure_auth_context_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\common\secure_auth_context_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\secure_auth_context_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\secure_auth_context_test.obj 
secure_auth_context_test: secure_auth_context_test.exe
	echo Running secure_auth_context_test
	$(OUT_DIR)\secure_auth_context_test.exe

server_crash_test_client.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building server_crash_test_client
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\server_crash_test_client.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\server_crash_test_client.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\server_crash_test_client.obj 
server_crash_test_client: server_crash_test_client.exe
	echo Running server_crash_test_client
	$(OUT_DIR)\server_crash_test_client.exe

shutdown_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building shutdown_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\shutdown_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\shutdown_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\shutdown_test.obj 
shutdown_test: shutdown_test.exe
	echo Running shutdown_test
	$(OUT_DIR)\shutdown_test.exe

status_test.exe: build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building status_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\status_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\status_test.exe" Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\status_test.obj 
status_test: status_test.exe
	echo Running status_test
	$(OUT_DIR)\status_test.exe

thread_stress_test.exe: Debug\grpc++_test_util.lib build_grpc_test_util build_grpc++ build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building thread_stress_test
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\end2end\thread_stress_test.cc 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\thread_stress_test.exe" Debug\grpc++_test_util.lib Debug\grpc_test_util.lib Debug\grpc++.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(CXX_LIBS) $(LIBS) $(OUT_DIR)\thread_stress_test.obj 
thread_stress_test: thread_stress_test.exe
	echo Running thread_stress_test
	$(OUT_DIR)\thread_stress_test.exe

h2_compress_bad_hostname_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_bad_hostname_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_bad_hostname_test: h2_compress_bad_hostname_test.exe
	echo Running h2_compress_bad_hostname_test
	$(OUT_DIR)\h2_compress_bad_hostname_test.exe

h2_compress_binary_metadata_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_binary_metadata_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_binary_metadata_test: h2_compress_binary_metadata_test.exe
	echo Running h2_compress_binary_metadata_test
	$(OUT_DIR)\h2_compress_binary_metadata_test.exe

h2_compress_call_creds_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_call_creds_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_call_creds_test: h2_compress_call_creds_test.exe
	echo Running h2_compress_call_creds_test
	$(OUT_DIR)\h2_compress_call_creds_test.exe

h2_compress_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_after_accept_test: h2_compress_cancel_after_accept_test.exe
	echo Running h2_compress_cancel_after_accept_test
	$(OUT_DIR)\h2_compress_cancel_after_accept_test.exe

h2_compress_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_after_client_done_test: h2_compress_cancel_after_client_done_test.exe
	echo Running h2_compress_cancel_after_client_done_test
	$(OUT_DIR)\h2_compress_cancel_after_client_done_test.exe

h2_compress_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_after_invoke_test: h2_compress_cancel_after_invoke_test.exe
	echo Running h2_compress_cancel_after_invoke_test
	$(OUT_DIR)\h2_compress_cancel_after_invoke_test.exe

h2_compress_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_before_invoke_test: h2_compress_cancel_before_invoke_test.exe
	echo Running h2_compress_cancel_before_invoke_test
	$(OUT_DIR)\h2_compress_cancel_before_invoke_test.exe

h2_compress_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_in_a_vacuum_test: h2_compress_cancel_in_a_vacuum_test.exe
	echo Running h2_compress_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_compress_cancel_in_a_vacuum_test.exe

h2_compress_census_simple_request_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_census_simple_request_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_census_simple_request_test: h2_compress_census_simple_request_test.exe
	echo Running h2_compress_census_simple_request_test
	$(OUT_DIR)\h2_compress_census_simple_request_test.exe

h2_compress_channel_connectivity_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_channel_connectivity_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_channel_connectivity_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_channel_connectivity_test: h2_compress_channel_connectivity_test.exe
	echo Running h2_compress_channel_connectivity_test
	$(OUT_DIR)\h2_compress_channel_connectivity_test.exe

h2_compress_compressed_payload_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_compressed_payload_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_compressed_payload_test: h2_compress_compressed_payload_test.exe
	echo Running h2_compress_compressed_payload_test
	$(OUT_DIR)\h2_compress_compressed_payload_test.exe

h2_compress_default_host_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_default_host_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_default_host_test: h2_compress_default_host_test.exe
	echo Running h2_compress_default_host_test
	$(OUT_DIR)\h2_compress_default_host_test.exe

h2_compress_disappearing_server_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_disappearing_server_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_disappearing_server_test: h2_compress_disappearing_server_test.exe
	echo Running h2_compress_disappearing_server_test
	$(OUT_DIR)\h2_compress_disappearing_server_test.exe

h2_compress_empty_batch_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_empty_batch_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_empty_batch_test: h2_compress_empty_batch_test.exe
	echo Running h2_compress_empty_batch_test
	$(OUT_DIR)\h2_compress_empty_batch_test.exe

h2_compress_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_graceful_server_shutdown_test: h2_compress_graceful_server_shutdown_test.exe
	echo Running h2_compress_graceful_server_shutdown_test
	$(OUT_DIR)\h2_compress_graceful_server_shutdown_test.exe

h2_compress_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_high_initial_seqno_test: h2_compress_high_initial_seqno_test.exe
	echo Running h2_compress_high_initial_seqno_test
	$(OUT_DIR)\h2_compress_high_initial_seqno_test.exe

h2_compress_invoke_large_request_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_invoke_large_request_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_invoke_large_request_test: h2_compress_invoke_large_request_test.exe
	echo Running h2_compress_invoke_large_request_test
	$(OUT_DIR)\h2_compress_invoke_large_request_test.exe

h2_compress_large_metadata_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_large_metadata_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_large_metadata_test: h2_compress_large_metadata_test.exe
	echo Running h2_compress_large_metadata_test
	$(OUT_DIR)\h2_compress_large_metadata_test.exe

h2_compress_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_max_concurrent_streams_test: h2_compress_max_concurrent_streams_test.exe
	echo Running h2_compress_max_concurrent_streams_test
	$(OUT_DIR)\h2_compress_max_concurrent_streams_test.exe

h2_compress_max_message_length_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_max_message_length_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_max_message_length_test: h2_compress_max_message_length_test.exe
	echo Running h2_compress_max_message_length_test
	$(OUT_DIR)\h2_compress_max_message_length_test.exe

h2_compress_metadata_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_metadata_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_metadata_test: h2_compress_metadata_test.exe
	echo Running h2_compress_metadata_test
	$(OUT_DIR)\h2_compress_metadata_test.exe

h2_compress_no_op_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_no_op_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_no_op_test: h2_compress_no_op_test.exe
	echo Running h2_compress_no_op_test
	$(OUT_DIR)\h2_compress_no_op_test.exe

h2_compress_payload_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_payload_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_payload_test: h2_compress_payload_test.exe
	echo Running h2_compress_payload_test
	$(OUT_DIR)\h2_compress_payload_test.exe

h2_compress_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_ping_pong_streaming_test: h2_compress_ping_pong_streaming_test.exe
	echo Running h2_compress_ping_pong_streaming_test
	$(OUT_DIR)\h2_compress_ping_pong_streaming_test.exe

h2_compress_registered_call_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_registered_call_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_registered_call_test: h2_compress_registered_call_test.exe
	echo Running h2_compress_registered_call_test
	$(OUT_DIR)\h2_compress_registered_call_test.exe

h2_compress_request_with_flags_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_request_with_flags_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_request_with_flags_test: h2_compress_request_with_flags_test.exe
	echo Running h2_compress_request_with_flags_test
	$(OUT_DIR)\h2_compress_request_with_flags_test.exe

h2_compress_request_with_payload_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_request_with_payload_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_request_with_payload_test: h2_compress_request_with_payload_test.exe
	echo Running h2_compress_request_with_payload_test
	$(OUT_DIR)\h2_compress_request_with_payload_test.exe

h2_compress_server_finishes_request_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_server_finishes_request_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_server_finishes_request_test: h2_compress_server_finishes_request_test.exe
	echo Running h2_compress_server_finishes_request_test
	$(OUT_DIR)\h2_compress_server_finishes_request_test.exe

h2_compress_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_shutdown_finishes_calls_test: h2_compress_shutdown_finishes_calls_test.exe
	echo Running h2_compress_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_compress_shutdown_finishes_calls_test.exe

h2_compress_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_shutdown_finishes_tags_test: h2_compress_shutdown_finishes_tags_test.exe
	echo Running h2_compress_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_compress_shutdown_finishes_tags_test.exe

h2_compress_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_simple_delayed_request_test: h2_compress_simple_delayed_request_test.exe
	echo Running h2_compress_simple_delayed_request_test
	$(OUT_DIR)\h2_compress_simple_delayed_request_test.exe

h2_compress_simple_request_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_simple_request_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_simple_request_test: h2_compress_simple_request_test.exe
	echo Running h2_compress_simple_request_test
	$(OUT_DIR)\h2_compress_simple_request_test.exe

h2_compress_trailing_metadata_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_trailing_metadata_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_trailing_metadata_test: h2_compress_trailing_metadata_test.exe
	echo Running h2_compress_trailing_metadata_test
	$(OUT_DIR)\h2_compress_trailing_metadata_test.exe

h2_fakesec_bad_hostname_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_bad_hostname_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_bad_hostname_test: h2_fakesec_bad_hostname_test.exe
	echo Running h2_fakesec_bad_hostname_test
	$(OUT_DIR)\h2_fakesec_bad_hostname_test.exe

h2_fakesec_binary_metadata_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_binary_metadata_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_binary_metadata_test: h2_fakesec_binary_metadata_test.exe
	echo Running h2_fakesec_binary_metadata_test
	$(OUT_DIR)\h2_fakesec_binary_metadata_test.exe

h2_fakesec_call_creds_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_call_creds_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_call_creds_test: h2_fakesec_call_creds_test.exe
	echo Running h2_fakesec_call_creds_test
	$(OUT_DIR)\h2_fakesec_call_creds_test.exe

h2_fakesec_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_cancel_after_accept_test: h2_fakesec_cancel_after_accept_test.exe
	echo Running h2_fakesec_cancel_after_accept_test
	$(OUT_DIR)\h2_fakesec_cancel_after_accept_test.exe

h2_fakesec_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_cancel_after_client_done_test: h2_fakesec_cancel_after_client_done_test.exe
	echo Running h2_fakesec_cancel_after_client_done_test
	$(OUT_DIR)\h2_fakesec_cancel_after_client_done_test.exe

h2_fakesec_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_cancel_after_invoke_test: h2_fakesec_cancel_after_invoke_test.exe
	echo Running h2_fakesec_cancel_after_invoke_test
	$(OUT_DIR)\h2_fakesec_cancel_after_invoke_test.exe

h2_fakesec_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_cancel_before_invoke_test: h2_fakesec_cancel_before_invoke_test.exe
	echo Running h2_fakesec_cancel_before_invoke_test
	$(OUT_DIR)\h2_fakesec_cancel_before_invoke_test.exe

h2_fakesec_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_cancel_in_a_vacuum_test: h2_fakesec_cancel_in_a_vacuum_test.exe
	echo Running h2_fakesec_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_fakesec_cancel_in_a_vacuum_test.exe

h2_fakesec_census_simple_request_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_census_simple_request_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_census_simple_request_test: h2_fakesec_census_simple_request_test.exe
	echo Running h2_fakesec_census_simple_request_test
	$(OUT_DIR)\h2_fakesec_census_simple_request_test.exe

h2_fakesec_channel_connectivity_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_channel_connectivity_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_channel_connectivity_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_channel_connectivity_test: h2_fakesec_channel_connectivity_test.exe
	echo Running h2_fakesec_channel_connectivity_test
	$(OUT_DIR)\h2_fakesec_channel_connectivity_test.exe

h2_fakesec_compressed_payload_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_compressed_payload_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_compressed_payload_test: h2_fakesec_compressed_payload_test.exe
	echo Running h2_fakesec_compressed_payload_test
	$(OUT_DIR)\h2_fakesec_compressed_payload_test.exe

h2_fakesec_default_host_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_default_host_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_default_host_test: h2_fakesec_default_host_test.exe
	echo Running h2_fakesec_default_host_test
	$(OUT_DIR)\h2_fakesec_default_host_test.exe

h2_fakesec_disappearing_server_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_disappearing_server_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_disappearing_server_test: h2_fakesec_disappearing_server_test.exe
	echo Running h2_fakesec_disappearing_server_test
	$(OUT_DIR)\h2_fakesec_disappearing_server_test.exe

h2_fakesec_empty_batch_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_empty_batch_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_empty_batch_test: h2_fakesec_empty_batch_test.exe
	echo Running h2_fakesec_empty_batch_test
	$(OUT_DIR)\h2_fakesec_empty_batch_test.exe

h2_fakesec_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_graceful_server_shutdown_test: h2_fakesec_graceful_server_shutdown_test.exe
	echo Running h2_fakesec_graceful_server_shutdown_test
	$(OUT_DIR)\h2_fakesec_graceful_server_shutdown_test.exe

h2_fakesec_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_high_initial_seqno_test: h2_fakesec_high_initial_seqno_test.exe
	echo Running h2_fakesec_high_initial_seqno_test
	$(OUT_DIR)\h2_fakesec_high_initial_seqno_test.exe

h2_fakesec_invoke_large_request_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_invoke_large_request_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_invoke_large_request_test: h2_fakesec_invoke_large_request_test.exe
	echo Running h2_fakesec_invoke_large_request_test
	$(OUT_DIR)\h2_fakesec_invoke_large_request_test.exe

h2_fakesec_large_metadata_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_large_metadata_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_large_metadata_test: h2_fakesec_large_metadata_test.exe
	echo Running h2_fakesec_large_metadata_test
	$(OUT_DIR)\h2_fakesec_large_metadata_test.exe

h2_fakesec_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_max_concurrent_streams_test: h2_fakesec_max_concurrent_streams_test.exe
	echo Running h2_fakesec_max_concurrent_streams_test
	$(OUT_DIR)\h2_fakesec_max_concurrent_streams_test.exe

h2_fakesec_max_message_length_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_max_message_length_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_max_message_length_test: h2_fakesec_max_message_length_test.exe
	echo Running h2_fakesec_max_message_length_test
	$(OUT_DIR)\h2_fakesec_max_message_length_test.exe

h2_fakesec_metadata_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_metadata_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_metadata_test: h2_fakesec_metadata_test.exe
	echo Running h2_fakesec_metadata_test
	$(OUT_DIR)\h2_fakesec_metadata_test.exe

h2_fakesec_no_op_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_no_op_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_no_op_test: h2_fakesec_no_op_test.exe
	echo Running h2_fakesec_no_op_test
	$(OUT_DIR)\h2_fakesec_no_op_test.exe

h2_fakesec_payload_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_payload_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_payload_test: h2_fakesec_payload_test.exe
	echo Running h2_fakesec_payload_test
	$(OUT_DIR)\h2_fakesec_payload_test.exe

h2_fakesec_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_ping_pong_streaming_test: h2_fakesec_ping_pong_streaming_test.exe
	echo Running h2_fakesec_ping_pong_streaming_test
	$(OUT_DIR)\h2_fakesec_ping_pong_streaming_test.exe

h2_fakesec_registered_call_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_registered_call_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_registered_call_test: h2_fakesec_registered_call_test.exe
	echo Running h2_fakesec_registered_call_test
	$(OUT_DIR)\h2_fakesec_registered_call_test.exe

h2_fakesec_request_with_flags_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_request_with_flags_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_request_with_flags_test: h2_fakesec_request_with_flags_test.exe
	echo Running h2_fakesec_request_with_flags_test
	$(OUT_DIR)\h2_fakesec_request_with_flags_test.exe

h2_fakesec_request_with_payload_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_request_with_payload_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_request_with_payload_test: h2_fakesec_request_with_payload_test.exe
	echo Running h2_fakesec_request_with_payload_test
	$(OUT_DIR)\h2_fakesec_request_with_payload_test.exe

h2_fakesec_server_finishes_request_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_server_finishes_request_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_server_finishes_request_test: h2_fakesec_server_finishes_request_test.exe
	echo Running h2_fakesec_server_finishes_request_test
	$(OUT_DIR)\h2_fakesec_server_finishes_request_test.exe

h2_fakesec_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_shutdown_finishes_calls_test: h2_fakesec_shutdown_finishes_calls_test.exe
	echo Running h2_fakesec_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_fakesec_shutdown_finishes_calls_test.exe

h2_fakesec_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_shutdown_finishes_tags_test: h2_fakesec_shutdown_finishes_tags_test.exe
	echo Running h2_fakesec_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_fakesec_shutdown_finishes_tags_test.exe

h2_fakesec_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_simple_delayed_request_test: h2_fakesec_simple_delayed_request_test.exe
	echo Running h2_fakesec_simple_delayed_request_test
	$(OUT_DIR)\h2_fakesec_simple_delayed_request_test.exe

h2_fakesec_simple_request_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_simple_request_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_simple_request_test: h2_fakesec_simple_request_test.exe
	echo Running h2_fakesec_simple_request_test
	$(OUT_DIR)\h2_fakesec_simple_request_test.exe

h2_fakesec_trailing_metadata_test.exe: Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_fakesec_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_fakesec_trailing_metadata_test.exe" Debug\end2end_fixture_h2_fakesec.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_fakesec_trailing_metadata_test: h2_fakesec_trailing_metadata_test.exe
	echo Running h2_fakesec_trailing_metadata_test
	$(OUT_DIR)\h2_fakesec_trailing_metadata_test.exe

h2_full_bad_hostname_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_bad_hostname_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_bad_hostname_test: h2_full_bad_hostname_test.exe
	echo Running h2_full_bad_hostname_test
	$(OUT_DIR)\h2_full_bad_hostname_test.exe

h2_full_binary_metadata_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_binary_metadata_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_binary_metadata_test: h2_full_binary_metadata_test.exe
	echo Running h2_full_binary_metadata_test
	$(OUT_DIR)\h2_full_binary_metadata_test.exe

h2_full_call_creds_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_call_creds_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_call_creds_test: h2_full_call_creds_test.exe
	echo Running h2_full_call_creds_test
	$(OUT_DIR)\h2_full_call_creds_test.exe

h2_full_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_after_accept_test: h2_full_cancel_after_accept_test.exe
	echo Running h2_full_cancel_after_accept_test
	$(OUT_DIR)\h2_full_cancel_after_accept_test.exe

h2_full_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_after_client_done_test: h2_full_cancel_after_client_done_test.exe
	echo Running h2_full_cancel_after_client_done_test
	$(OUT_DIR)\h2_full_cancel_after_client_done_test.exe

h2_full_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_after_invoke_test: h2_full_cancel_after_invoke_test.exe
	echo Running h2_full_cancel_after_invoke_test
	$(OUT_DIR)\h2_full_cancel_after_invoke_test.exe

h2_full_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_before_invoke_test: h2_full_cancel_before_invoke_test.exe
	echo Running h2_full_cancel_before_invoke_test
	$(OUT_DIR)\h2_full_cancel_before_invoke_test.exe

h2_full_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_in_a_vacuum_test: h2_full_cancel_in_a_vacuum_test.exe
	echo Running h2_full_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_full_cancel_in_a_vacuum_test.exe

h2_full_census_simple_request_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_census_simple_request_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_census_simple_request_test: h2_full_census_simple_request_test.exe
	echo Running h2_full_census_simple_request_test
	$(OUT_DIR)\h2_full_census_simple_request_test.exe

h2_full_channel_connectivity_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_channel_connectivity_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_channel_connectivity_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_channel_connectivity_test: h2_full_channel_connectivity_test.exe
	echo Running h2_full_channel_connectivity_test
	$(OUT_DIR)\h2_full_channel_connectivity_test.exe

h2_full_compressed_payload_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_compressed_payload_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_compressed_payload_test: h2_full_compressed_payload_test.exe
	echo Running h2_full_compressed_payload_test
	$(OUT_DIR)\h2_full_compressed_payload_test.exe

h2_full_default_host_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_default_host_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_default_host_test: h2_full_default_host_test.exe
	echo Running h2_full_default_host_test
	$(OUT_DIR)\h2_full_default_host_test.exe

h2_full_disappearing_server_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_disappearing_server_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_disappearing_server_test: h2_full_disappearing_server_test.exe
	echo Running h2_full_disappearing_server_test
	$(OUT_DIR)\h2_full_disappearing_server_test.exe

h2_full_empty_batch_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_empty_batch_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_empty_batch_test: h2_full_empty_batch_test.exe
	echo Running h2_full_empty_batch_test
	$(OUT_DIR)\h2_full_empty_batch_test.exe

h2_full_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_graceful_server_shutdown_test: h2_full_graceful_server_shutdown_test.exe
	echo Running h2_full_graceful_server_shutdown_test
	$(OUT_DIR)\h2_full_graceful_server_shutdown_test.exe

h2_full_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_high_initial_seqno_test: h2_full_high_initial_seqno_test.exe
	echo Running h2_full_high_initial_seqno_test
	$(OUT_DIR)\h2_full_high_initial_seqno_test.exe

h2_full_invoke_large_request_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_invoke_large_request_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_invoke_large_request_test: h2_full_invoke_large_request_test.exe
	echo Running h2_full_invoke_large_request_test
	$(OUT_DIR)\h2_full_invoke_large_request_test.exe

h2_full_large_metadata_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_large_metadata_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_large_metadata_test: h2_full_large_metadata_test.exe
	echo Running h2_full_large_metadata_test
	$(OUT_DIR)\h2_full_large_metadata_test.exe

h2_full_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_max_concurrent_streams_test: h2_full_max_concurrent_streams_test.exe
	echo Running h2_full_max_concurrent_streams_test
	$(OUT_DIR)\h2_full_max_concurrent_streams_test.exe

h2_full_max_message_length_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_max_message_length_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_max_message_length_test: h2_full_max_message_length_test.exe
	echo Running h2_full_max_message_length_test
	$(OUT_DIR)\h2_full_max_message_length_test.exe

h2_full_metadata_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_metadata_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_metadata_test: h2_full_metadata_test.exe
	echo Running h2_full_metadata_test
	$(OUT_DIR)\h2_full_metadata_test.exe

h2_full_no_op_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_no_op_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_no_op_test: h2_full_no_op_test.exe
	echo Running h2_full_no_op_test
	$(OUT_DIR)\h2_full_no_op_test.exe

h2_full_payload_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_payload_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_payload_test: h2_full_payload_test.exe
	echo Running h2_full_payload_test
	$(OUT_DIR)\h2_full_payload_test.exe

h2_full_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_ping_pong_streaming_test: h2_full_ping_pong_streaming_test.exe
	echo Running h2_full_ping_pong_streaming_test
	$(OUT_DIR)\h2_full_ping_pong_streaming_test.exe

h2_full_registered_call_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_registered_call_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_registered_call_test: h2_full_registered_call_test.exe
	echo Running h2_full_registered_call_test
	$(OUT_DIR)\h2_full_registered_call_test.exe

h2_full_request_with_flags_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_request_with_flags_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_request_with_flags_test: h2_full_request_with_flags_test.exe
	echo Running h2_full_request_with_flags_test
	$(OUT_DIR)\h2_full_request_with_flags_test.exe

h2_full_request_with_payload_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_request_with_payload_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_request_with_payload_test: h2_full_request_with_payload_test.exe
	echo Running h2_full_request_with_payload_test
	$(OUT_DIR)\h2_full_request_with_payload_test.exe

h2_full_server_finishes_request_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_server_finishes_request_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_server_finishes_request_test: h2_full_server_finishes_request_test.exe
	echo Running h2_full_server_finishes_request_test
	$(OUT_DIR)\h2_full_server_finishes_request_test.exe

h2_full_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_shutdown_finishes_calls_test: h2_full_shutdown_finishes_calls_test.exe
	echo Running h2_full_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_full_shutdown_finishes_calls_test.exe

h2_full_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_shutdown_finishes_tags_test: h2_full_shutdown_finishes_tags_test.exe
	echo Running h2_full_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_full_shutdown_finishes_tags_test.exe

h2_full_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_simple_delayed_request_test: h2_full_simple_delayed_request_test.exe
	echo Running h2_full_simple_delayed_request_test
	$(OUT_DIR)\h2_full_simple_delayed_request_test.exe

h2_full_simple_request_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_simple_request_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_simple_request_test: h2_full_simple_request_test.exe
	echo Running h2_full_simple_request_test
	$(OUT_DIR)\h2_full_simple_request_test.exe

h2_full_trailing_metadata_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_trailing_metadata_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_trailing_metadata_test: h2_full_trailing_metadata_test.exe
	echo Running h2_full_trailing_metadata_test
	$(OUT_DIR)\h2_full_trailing_metadata_test.exe

h2_oauth2_bad_hostname_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_bad_hostname_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_bad_hostname_test: h2_oauth2_bad_hostname_test.exe
	echo Running h2_oauth2_bad_hostname_test
	$(OUT_DIR)\h2_oauth2_bad_hostname_test.exe

h2_oauth2_binary_metadata_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_binary_metadata_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_binary_metadata_test: h2_oauth2_binary_metadata_test.exe
	echo Running h2_oauth2_binary_metadata_test
	$(OUT_DIR)\h2_oauth2_binary_metadata_test.exe

h2_oauth2_call_creds_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_call_creds_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_call_creds_test: h2_oauth2_call_creds_test.exe
	echo Running h2_oauth2_call_creds_test
	$(OUT_DIR)\h2_oauth2_call_creds_test.exe

h2_oauth2_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_cancel_after_accept_test: h2_oauth2_cancel_after_accept_test.exe
	echo Running h2_oauth2_cancel_after_accept_test
	$(OUT_DIR)\h2_oauth2_cancel_after_accept_test.exe

h2_oauth2_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_cancel_after_client_done_test: h2_oauth2_cancel_after_client_done_test.exe
	echo Running h2_oauth2_cancel_after_client_done_test
	$(OUT_DIR)\h2_oauth2_cancel_after_client_done_test.exe

h2_oauth2_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_cancel_after_invoke_test: h2_oauth2_cancel_after_invoke_test.exe
	echo Running h2_oauth2_cancel_after_invoke_test
	$(OUT_DIR)\h2_oauth2_cancel_after_invoke_test.exe

h2_oauth2_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_cancel_before_invoke_test: h2_oauth2_cancel_before_invoke_test.exe
	echo Running h2_oauth2_cancel_before_invoke_test
	$(OUT_DIR)\h2_oauth2_cancel_before_invoke_test.exe

h2_oauth2_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_cancel_in_a_vacuum_test: h2_oauth2_cancel_in_a_vacuum_test.exe
	echo Running h2_oauth2_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_oauth2_cancel_in_a_vacuum_test.exe

h2_oauth2_census_simple_request_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_census_simple_request_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_census_simple_request_test: h2_oauth2_census_simple_request_test.exe
	echo Running h2_oauth2_census_simple_request_test
	$(OUT_DIR)\h2_oauth2_census_simple_request_test.exe

h2_oauth2_channel_connectivity_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_channel_connectivity_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_channel_connectivity_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_channel_connectivity_test: h2_oauth2_channel_connectivity_test.exe
	echo Running h2_oauth2_channel_connectivity_test
	$(OUT_DIR)\h2_oauth2_channel_connectivity_test.exe

h2_oauth2_compressed_payload_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_compressed_payload_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_compressed_payload_test: h2_oauth2_compressed_payload_test.exe
	echo Running h2_oauth2_compressed_payload_test
	$(OUT_DIR)\h2_oauth2_compressed_payload_test.exe

h2_oauth2_default_host_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_default_host_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_default_host_test: h2_oauth2_default_host_test.exe
	echo Running h2_oauth2_default_host_test
	$(OUT_DIR)\h2_oauth2_default_host_test.exe

h2_oauth2_disappearing_server_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_disappearing_server_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_disappearing_server_test: h2_oauth2_disappearing_server_test.exe
	echo Running h2_oauth2_disappearing_server_test
	$(OUT_DIR)\h2_oauth2_disappearing_server_test.exe

h2_oauth2_empty_batch_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_empty_batch_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_empty_batch_test: h2_oauth2_empty_batch_test.exe
	echo Running h2_oauth2_empty_batch_test
	$(OUT_DIR)\h2_oauth2_empty_batch_test.exe

h2_oauth2_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_graceful_server_shutdown_test: h2_oauth2_graceful_server_shutdown_test.exe
	echo Running h2_oauth2_graceful_server_shutdown_test
	$(OUT_DIR)\h2_oauth2_graceful_server_shutdown_test.exe

h2_oauth2_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_high_initial_seqno_test: h2_oauth2_high_initial_seqno_test.exe
	echo Running h2_oauth2_high_initial_seqno_test
	$(OUT_DIR)\h2_oauth2_high_initial_seqno_test.exe

h2_oauth2_invoke_large_request_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_invoke_large_request_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_invoke_large_request_test: h2_oauth2_invoke_large_request_test.exe
	echo Running h2_oauth2_invoke_large_request_test
	$(OUT_DIR)\h2_oauth2_invoke_large_request_test.exe

h2_oauth2_large_metadata_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_large_metadata_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_large_metadata_test: h2_oauth2_large_metadata_test.exe
	echo Running h2_oauth2_large_metadata_test
	$(OUT_DIR)\h2_oauth2_large_metadata_test.exe

h2_oauth2_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_max_concurrent_streams_test: h2_oauth2_max_concurrent_streams_test.exe
	echo Running h2_oauth2_max_concurrent_streams_test
	$(OUT_DIR)\h2_oauth2_max_concurrent_streams_test.exe

h2_oauth2_max_message_length_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_max_message_length_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_max_message_length_test: h2_oauth2_max_message_length_test.exe
	echo Running h2_oauth2_max_message_length_test
	$(OUT_DIR)\h2_oauth2_max_message_length_test.exe

h2_oauth2_metadata_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_metadata_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_metadata_test: h2_oauth2_metadata_test.exe
	echo Running h2_oauth2_metadata_test
	$(OUT_DIR)\h2_oauth2_metadata_test.exe

h2_oauth2_no_op_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_no_op_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_no_op_test: h2_oauth2_no_op_test.exe
	echo Running h2_oauth2_no_op_test
	$(OUT_DIR)\h2_oauth2_no_op_test.exe

h2_oauth2_payload_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_payload_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_payload_test: h2_oauth2_payload_test.exe
	echo Running h2_oauth2_payload_test
	$(OUT_DIR)\h2_oauth2_payload_test.exe

h2_oauth2_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_ping_pong_streaming_test: h2_oauth2_ping_pong_streaming_test.exe
	echo Running h2_oauth2_ping_pong_streaming_test
	$(OUT_DIR)\h2_oauth2_ping_pong_streaming_test.exe

h2_oauth2_registered_call_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_registered_call_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_registered_call_test: h2_oauth2_registered_call_test.exe
	echo Running h2_oauth2_registered_call_test
	$(OUT_DIR)\h2_oauth2_registered_call_test.exe

h2_oauth2_request_with_flags_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_request_with_flags_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_request_with_flags_test: h2_oauth2_request_with_flags_test.exe
	echo Running h2_oauth2_request_with_flags_test
	$(OUT_DIR)\h2_oauth2_request_with_flags_test.exe

h2_oauth2_request_with_payload_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_request_with_payload_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_request_with_payload_test: h2_oauth2_request_with_payload_test.exe
	echo Running h2_oauth2_request_with_payload_test
	$(OUT_DIR)\h2_oauth2_request_with_payload_test.exe

h2_oauth2_server_finishes_request_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_server_finishes_request_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_server_finishes_request_test: h2_oauth2_server_finishes_request_test.exe
	echo Running h2_oauth2_server_finishes_request_test
	$(OUT_DIR)\h2_oauth2_server_finishes_request_test.exe

h2_oauth2_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_shutdown_finishes_calls_test: h2_oauth2_shutdown_finishes_calls_test.exe
	echo Running h2_oauth2_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_oauth2_shutdown_finishes_calls_test.exe

h2_oauth2_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_shutdown_finishes_tags_test: h2_oauth2_shutdown_finishes_tags_test.exe
	echo Running h2_oauth2_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_oauth2_shutdown_finishes_tags_test.exe

h2_oauth2_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_simple_delayed_request_test: h2_oauth2_simple_delayed_request_test.exe
	echo Running h2_oauth2_simple_delayed_request_test
	$(OUT_DIR)\h2_oauth2_simple_delayed_request_test.exe

h2_oauth2_simple_request_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_simple_request_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_simple_request_test: h2_oauth2_simple_request_test.exe
	echo Running h2_oauth2_simple_request_test
	$(OUT_DIR)\h2_oauth2_simple_request_test.exe

h2_oauth2_trailing_metadata_test.exe: Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_oauth2_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_oauth2_trailing_metadata_test.exe" Debug\end2end_fixture_h2_oauth2.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_oauth2_trailing_metadata_test: h2_oauth2_trailing_metadata_test.exe
	echo Running h2_oauth2_trailing_metadata_test
	$(OUT_DIR)\h2_oauth2_trailing_metadata_test.exe

h2_proxy_bad_hostname_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_bad_hostname_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_bad_hostname_test: h2_proxy_bad_hostname_test.exe
	echo Running h2_proxy_bad_hostname_test
	$(OUT_DIR)\h2_proxy_bad_hostname_test.exe

h2_proxy_binary_metadata_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_binary_metadata_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_binary_metadata_test: h2_proxy_binary_metadata_test.exe
	echo Running h2_proxy_binary_metadata_test
	$(OUT_DIR)\h2_proxy_binary_metadata_test.exe

h2_proxy_call_creds_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_call_creds_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_call_creds_test: h2_proxy_call_creds_test.exe
	echo Running h2_proxy_call_creds_test
	$(OUT_DIR)\h2_proxy_call_creds_test.exe

h2_proxy_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_after_accept_test: h2_proxy_cancel_after_accept_test.exe
	echo Running h2_proxy_cancel_after_accept_test
	$(OUT_DIR)\h2_proxy_cancel_after_accept_test.exe

h2_proxy_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_after_client_done_test: h2_proxy_cancel_after_client_done_test.exe
	echo Running h2_proxy_cancel_after_client_done_test
	$(OUT_DIR)\h2_proxy_cancel_after_client_done_test.exe

h2_proxy_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_after_invoke_test: h2_proxy_cancel_after_invoke_test.exe
	echo Running h2_proxy_cancel_after_invoke_test
	$(OUT_DIR)\h2_proxy_cancel_after_invoke_test.exe

h2_proxy_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_before_invoke_test: h2_proxy_cancel_before_invoke_test.exe
	echo Running h2_proxy_cancel_before_invoke_test
	$(OUT_DIR)\h2_proxy_cancel_before_invoke_test.exe

h2_proxy_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_in_a_vacuum_test: h2_proxy_cancel_in_a_vacuum_test.exe
	echo Running h2_proxy_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_proxy_cancel_in_a_vacuum_test.exe

h2_proxy_census_simple_request_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_census_simple_request_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_census_simple_request_test: h2_proxy_census_simple_request_test.exe
	echo Running h2_proxy_census_simple_request_test
	$(OUT_DIR)\h2_proxy_census_simple_request_test.exe

h2_proxy_default_host_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_default_host_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_default_host_test: h2_proxy_default_host_test.exe
	echo Running h2_proxy_default_host_test
	$(OUT_DIR)\h2_proxy_default_host_test.exe

h2_proxy_disappearing_server_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_disappearing_server_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_disappearing_server_test: h2_proxy_disappearing_server_test.exe
	echo Running h2_proxy_disappearing_server_test
	$(OUT_DIR)\h2_proxy_disappearing_server_test.exe

h2_proxy_empty_batch_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_empty_batch_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_empty_batch_test: h2_proxy_empty_batch_test.exe
	echo Running h2_proxy_empty_batch_test
	$(OUT_DIR)\h2_proxy_empty_batch_test.exe

h2_proxy_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_graceful_server_shutdown_test: h2_proxy_graceful_server_shutdown_test.exe
	echo Running h2_proxy_graceful_server_shutdown_test
	$(OUT_DIR)\h2_proxy_graceful_server_shutdown_test.exe

h2_proxy_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_high_initial_seqno_test: h2_proxy_high_initial_seqno_test.exe
	echo Running h2_proxy_high_initial_seqno_test
	$(OUT_DIR)\h2_proxy_high_initial_seqno_test.exe

h2_proxy_invoke_large_request_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_invoke_large_request_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_invoke_large_request_test: h2_proxy_invoke_large_request_test.exe
	echo Running h2_proxy_invoke_large_request_test
	$(OUT_DIR)\h2_proxy_invoke_large_request_test.exe

h2_proxy_large_metadata_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_large_metadata_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_large_metadata_test: h2_proxy_large_metadata_test.exe
	echo Running h2_proxy_large_metadata_test
	$(OUT_DIR)\h2_proxy_large_metadata_test.exe

h2_proxy_max_message_length_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_max_message_length_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_max_message_length_test: h2_proxy_max_message_length_test.exe
	echo Running h2_proxy_max_message_length_test
	$(OUT_DIR)\h2_proxy_max_message_length_test.exe

h2_proxy_metadata_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_metadata_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_metadata_test: h2_proxy_metadata_test.exe
	echo Running h2_proxy_metadata_test
	$(OUT_DIR)\h2_proxy_metadata_test.exe

h2_proxy_no_op_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_no_op_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_no_op_test: h2_proxy_no_op_test.exe
	echo Running h2_proxy_no_op_test
	$(OUT_DIR)\h2_proxy_no_op_test.exe

h2_proxy_payload_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_payload_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_payload_test: h2_proxy_payload_test.exe
	echo Running h2_proxy_payload_test
	$(OUT_DIR)\h2_proxy_payload_test.exe

h2_proxy_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_ping_pong_streaming_test: h2_proxy_ping_pong_streaming_test.exe
	echo Running h2_proxy_ping_pong_streaming_test
	$(OUT_DIR)\h2_proxy_ping_pong_streaming_test.exe

h2_proxy_registered_call_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_registered_call_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_registered_call_test: h2_proxy_registered_call_test.exe
	echo Running h2_proxy_registered_call_test
	$(OUT_DIR)\h2_proxy_registered_call_test.exe

h2_proxy_request_with_payload_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_request_with_payload_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_request_with_payload_test: h2_proxy_request_with_payload_test.exe
	echo Running h2_proxy_request_with_payload_test
	$(OUT_DIR)\h2_proxy_request_with_payload_test.exe

h2_proxy_server_finishes_request_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_server_finishes_request_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_server_finishes_request_test: h2_proxy_server_finishes_request_test.exe
	echo Running h2_proxy_server_finishes_request_test
	$(OUT_DIR)\h2_proxy_server_finishes_request_test.exe

h2_proxy_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_shutdown_finishes_calls_test: h2_proxy_shutdown_finishes_calls_test.exe
	echo Running h2_proxy_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_proxy_shutdown_finishes_calls_test.exe

h2_proxy_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_shutdown_finishes_tags_test: h2_proxy_shutdown_finishes_tags_test.exe
	echo Running h2_proxy_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_proxy_shutdown_finishes_tags_test.exe

h2_proxy_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_simple_delayed_request_test: h2_proxy_simple_delayed_request_test.exe
	echo Running h2_proxy_simple_delayed_request_test
	$(OUT_DIR)\h2_proxy_simple_delayed_request_test.exe

h2_proxy_simple_request_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_simple_request_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_simple_request_test: h2_proxy_simple_request_test.exe
	echo Running h2_proxy_simple_request_test
	$(OUT_DIR)\h2_proxy_simple_request_test.exe

h2_proxy_trailing_metadata_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_trailing_metadata_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_trailing_metadata_test: h2_proxy_trailing_metadata_test.exe
	echo Running h2_proxy_trailing_metadata_test
	$(OUT_DIR)\h2_proxy_trailing_metadata_test.exe

h2_sockpair_bad_hostname_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_bad_hostname_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_bad_hostname_test: h2_sockpair_bad_hostname_test.exe
	echo Running h2_sockpair_bad_hostname_test
	$(OUT_DIR)\h2_sockpair_bad_hostname_test.exe

h2_sockpair_binary_metadata_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_binary_metadata_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_binary_metadata_test: h2_sockpair_binary_metadata_test.exe
	echo Running h2_sockpair_binary_metadata_test
	$(OUT_DIR)\h2_sockpair_binary_metadata_test.exe

h2_sockpair_call_creds_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_call_creds_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_call_creds_test: h2_sockpair_call_creds_test.exe
	echo Running h2_sockpair_call_creds_test
	$(OUT_DIR)\h2_sockpair_call_creds_test.exe

h2_sockpair_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_after_accept_test: h2_sockpair_cancel_after_accept_test.exe
	echo Running h2_sockpair_cancel_after_accept_test
	$(OUT_DIR)\h2_sockpair_cancel_after_accept_test.exe

h2_sockpair_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_after_client_done_test: h2_sockpair_cancel_after_client_done_test.exe
	echo Running h2_sockpair_cancel_after_client_done_test
	$(OUT_DIR)\h2_sockpair_cancel_after_client_done_test.exe

h2_sockpair_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_after_invoke_test: h2_sockpair_cancel_after_invoke_test.exe
	echo Running h2_sockpair_cancel_after_invoke_test
	$(OUT_DIR)\h2_sockpair_cancel_after_invoke_test.exe

h2_sockpair_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_before_invoke_test: h2_sockpair_cancel_before_invoke_test.exe
	echo Running h2_sockpair_cancel_before_invoke_test
	$(OUT_DIR)\h2_sockpair_cancel_before_invoke_test.exe

h2_sockpair_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_in_a_vacuum_test: h2_sockpair_cancel_in_a_vacuum_test.exe
	echo Running h2_sockpair_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_sockpair_cancel_in_a_vacuum_test.exe

h2_sockpair_census_simple_request_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_census_simple_request_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_census_simple_request_test: h2_sockpair_census_simple_request_test.exe
	echo Running h2_sockpair_census_simple_request_test
	$(OUT_DIR)\h2_sockpair_census_simple_request_test.exe

h2_sockpair_compressed_payload_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_compressed_payload_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_compressed_payload_test: h2_sockpair_compressed_payload_test.exe
	echo Running h2_sockpair_compressed_payload_test
	$(OUT_DIR)\h2_sockpair_compressed_payload_test.exe

h2_sockpair_empty_batch_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_empty_batch_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_empty_batch_test: h2_sockpair_empty_batch_test.exe
	echo Running h2_sockpair_empty_batch_test
	$(OUT_DIR)\h2_sockpair_empty_batch_test.exe

h2_sockpair_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_graceful_server_shutdown_test: h2_sockpair_graceful_server_shutdown_test.exe
	echo Running h2_sockpair_graceful_server_shutdown_test
	$(OUT_DIR)\h2_sockpair_graceful_server_shutdown_test.exe

h2_sockpair_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_high_initial_seqno_test: h2_sockpair_high_initial_seqno_test.exe
	echo Running h2_sockpair_high_initial_seqno_test
	$(OUT_DIR)\h2_sockpair_high_initial_seqno_test.exe

h2_sockpair_invoke_large_request_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_invoke_large_request_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_invoke_large_request_test: h2_sockpair_invoke_large_request_test.exe
	echo Running h2_sockpair_invoke_large_request_test
	$(OUT_DIR)\h2_sockpair_invoke_large_request_test.exe

h2_sockpair_large_metadata_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_large_metadata_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_large_metadata_test: h2_sockpair_large_metadata_test.exe
	echo Running h2_sockpair_large_metadata_test
	$(OUT_DIR)\h2_sockpair_large_metadata_test.exe

h2_sockpair_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_max_concurrent_streams_test: h2_sockpair_max_concurrent_streams_test.exe
	echo Running h2_sockpair_max_concurrent_streams_test
	$(OUT_DIR)\h2_sockpair_max_concurrent_streams_test.exe

h2_sockpair_max_message_length_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_max_message_length_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_max_message_length_test: h2_sockpair_max_message_length_test.exe
	echo Running h2_sockpair_max_message_length_test
	$(OUT_DIR)\h2_sockpair_max_message_length_test.exe

h2_sockpair_metadata_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_metadata_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_metadata_test: h2_sockpair_metadata_test.exe
	echo Running h2_sockpair_metadata_test
	$(OUT_DIR)\h2_sockpair_metadata_test.exe

h2_sockpair_no_op_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_no_op_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_no_op_test: h2_sockpair_no_op_test.exe
	echo Running h2_sockpair_no_op_test
	$(OUT_DIR)\h2_sockpair_no_op_test.exe

h2_sockpair_payload_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_payload_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_payload_test: h2_sockpair_payload_test.exe
	echo Running h2_sockpair_payload_test
	$(OUT_DIR)\h2_sockpair_payload_test.exe

h2_sockpair_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_ping_pong_streaming_test: h2_sockpair_ping_pong_streaming_test.exe
	echo Running h2_sockpair_ping_pong_streaming_test
	$(OUT_DIR)\h2_sockpair_ping_pong_streaming_test.exe

h2_sockpair_registered_call_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_registered_call_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_registered_call_test: h2_sockpair_registered_call_test.exe
	echo Running h2_sockpair_registered_call_test
	$(OUT_DIR)\h2_sockpair_registered_call_test.exe

h2_sockpair_request_with_flags_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_request_with_flags_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_request_with_flags_test: h2_sockpair_request_with_flags_test.exe
	echo Running h2_sockpair_request_with_flags_test
	$(OUT_DIR)\h2_sockpair_request_with_flags_test.exe

h2_sockpair_request_with_payload_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_request_with_payload_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_request_with_payload_test: h2_sockpair_request_with_payload_test.exe
	echo Running h2_sockpair_request_with_payload_test
	$(OUT_DIR)\h2_sockpair_request_with_payload_test.exe

h2_sockpair_server_finishes_request_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_server_finishes_request_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_server_finishes_request_test: h2_sockpair_server_finishes_request_test.exe
	echo Running h2_sockpair_server_finishes_request_test
	$(OUT_DIR)\h2_sockpair_server_finishes_request_test.exe

h2_sockpair_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_shutdown_finishes_calls_test: h2_sockpair_shutdown_finishes_calls_test.exe
	echo Running h2_sockpair_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_sockpair_shutdown_finishes_calls_test.exe

h2_sockpair_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_shutdown_finishes_tags_test: h2_sockpair_shutdown_finishes_tags_test.exe
	echo Running h2_sockpair_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_sockpair_shutdown_finishes_tags_test.exe

h2_sockpair_simple_request_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_simple_request_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_simple_request_test: h2_sockpair_simple_request_test.exe
	echo Running h2_sockpair_simple_request_test
	$(OUT_DIR)\h2_sockpair_simple_request_test.exe

h2_sockpair_trailing_metadata_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_trailing_metadata_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_trailing_metadata_test: h2_sockpair_trailing_metadata_test.exe
	echo Running h2_sockpair_trailing_metadata_test
	$(OUT_DIR)\h2_sockpair_trailing_metadata_test.exe

h2_sockpair+trace_bad_hostname_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_bad_hostname_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_bad_hostname_test: h2_sockpair+trace_bad_hostname_test.exe
	echo Running h2_sockpair+trace_bad_hostname_test
	$(OUT_DIR)\h2_sockpair+trace_bad_hostname_test.exe

h2_sockpair+trace_binary_metadata_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_binary_metadata_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_binary_metadata_test: h2_sockpair+trace_binary_metadata_test.exe
	echo Running h2_sockpair+trace_binary_metadata_test
	$(OUT_DIR)\h2_sockpair+trace_binary_metadata_test.exe

h2_sockpair+trace_call_creds_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_call_creds_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_call_creds_test: h2_sockpair+trace_call_creds_test.exe
	echo Running h2_sockpair+trace_call_creds_test
	$(OUT_DIR)\h2_sockpair+trace_call_creds_test.exe

h2_sockpair+trace_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_after_accept_test: h2_sockpair+trace_cancel_after_accept_test.exe
	echo Running h2_sockpair+trace_cancel_after_accept_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_after_accept_test.exe

h2_sockpair+trace_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_after_client_done_test: h2_sockpair+trace_cancel_after_client_done_test.exe
	echo Running h2_sockpair+trace_cancel_after_client_done_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_after_client_done_test.exe

h2_sockpair+trace_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_after_invoke_test: h2_sockpair+trace_cancel_after_invoke_test.exe
	echo Running h2_sockpair+trace_cancel_after_invoke_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_after_invoke_test.exe

h2_sockpair+trace_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_before_invoke_test: h2_sockpair+trace_cancel_before_invoke_test.exe
	echo Running h2_sockpair+trace_cancel_before_invoke_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_before_invoke_test.exe

h2_sockpair+trace_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_in_a_vacuum_test: h2_sockpair+trace_cancel_in_a_vacuum_test.exe
	echo Running h2_sockpair+trace_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_in_a_vacuum_test.exe

h2_sockpair+trace_census_simple_request_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_census_simple_request_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_census_simple_request_test: h2_sockpair+trace_census_simple_request_test.exe
	echo Running h2_sockpair+trace_census_simple_request_test
	$(OUT_DIR)\h2_sockpair+trace_census_simple_request_test.exe

h2_sockpair+trace_compressed_payload_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_compressed_payload_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_compressed_payload_test: h2_sockpair+trace_compressed_payload_test.exe
	echo Running h2_sockpair+trace_compressed_payload_test
	$(OUT_DIR)\h2_sockpair+trace_compressed_payload_test.exe

h2_sockpair+trace_empty_batch_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_empty_batch_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_empty_batch_test: h2_sockpair+trace_empty_batch_test.exe
	echo Running h2_sockpair+trace_empty_batch_test
	$(OUT_DIR)\h2_sockpair+trace_empty_batch_test.exe

h2_sockpair+trace_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_graceful_server_shutdown_test: h2_sockpair+trace_graceful_server_shutdown_test.exe
	echo Running h2_sockpair+trace_graceful_server_shutdown_test
	$(OUT_DIR)\h2_sockpair+trace_graceful_server_shutdown_test.exe

h2_sockpair+trace_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_high_initial_seqno_test: h2_sockpair+trace_high_initial_seqno_test.exe
	echo Running h2_sockpair+trace_high_initial_seqno_test
	$(OUT_DIR)\h2_sockpair+trace_high_initial_seqno_test.exe

h2_sockpair+trace_invoke_large_request_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_invoke_large_request_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_invoke_large_request_test: h2_sockpair+trace_invoke_large_request_test.exe
	echo Running h2_sockpair+trace_invoke_large_request_test
	$(OUT_DIR)\h2_sockpair+trace_invoke_large_request_test.exe

h2_sockpair+trace_large_metadata_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_large_metadata_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_large_metadata_test: h2_sockpair+trace_large_metadata_test.exe
	echo Running h2_sockpair+trace_large_metadata_test
	$(OUT_DIR)\h2_sockpair+trace_large_metadata_test.exe

h2_sockpair+trace_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_max_concurrent_streams_test: h2_sockpair+trace_max_concurrent_streams_test.exe
	echo Running h2_sockpair+trace_max_concurrent_streams_test
	$(OUT_DIR)\h2_sockpair+trace_max_concurrent_streams_test.exe

h2_sockpair+trace_max_message_length_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_max_message_length_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_max_message_length_test: h2_sockpair+trace_max_message_length_test.exe
	echo Running h2_sockpair+trace_max_message_length_test
	$(OUT_DIR)\h2_sockpair+trace_max_message_length_test.exe

h2_sockpair+trace_metadata_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_metadata_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_metadata_test: h2_sockpair+trace_metadata_test.exe
	echo Running h2_sockpair+trace_metadata_test
	$(OUT_DIR)\h2_sockpair+trace_metadata_test.exe

h2_sockpair+trace_no_op_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_no_op_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_no_op_test: h2_sockpair+trace_no_op_test.exe
	echo Running h2_sockpair+trace_no_op_test
	$(OUT_DIR)\h2_sockpair+trace_no_op_test.exe

h2_sockpair+trace_payload_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_payload_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_payload_test: h2_sockpair+trace_payload_test.exe
	echo Running h2_sockpair+trace_payload_test
	$(OUT_DIR)\h2_sockpair+trace_payload_test.exe

h2_sockpair+trace_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_ping_pong_streaming_test: h2_sockpair+trace_ping_pong_streaming_test.exe
	echo Running h2_sockpair+trace_ping_pong_streaming_test
	$(OUT_DIR)\h2_sockpair+trace_ping_pong_streaming_test.exe

h2_sockpair+trace_registered_call_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_registered_call_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_registered_call_test: h2_sockpair+trace_registered_call_test.exe
	echo Running h2_sockpair+trace_registered_call_test
	$(OUT_DIR)\h2_sockpair+trace_registered_call_test.exe

h2_sockpair+trace_request_with_flags_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_request_with_flags_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_request_with_flags_test: h2_sockpair+trace_request_with_flags_test.exe
	echo Running h2_sockpair+trace_request_with_flags_test
	$(OUT_DIR)\h2_sockpair+trace_request_with_flags_test.exe

h2_sockpair+trace_request_with_payload_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_request_with_payload_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_request_with_payload_test: h2_sockpair+trace_request_with_payload_test.exe
	echo Running h2_sockpair+trace_request_with_payload_test
	$(OUT_DIR)\h2_sockpair+trace_request_with_payload_test.exe

h2_sockpair+trace_server_finishes_request_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_server_finishes_request_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_server_finishes_request_test: h2_sockpair+trace_server_finishes_request_test.exe
	echo Running h2_sockpair+trace_server_finishes_request_test
	$(OUT_DIR)\h2_sockpair+trace_server_finishes_request_test.exe

h2_sockpair+trace_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_shutdown_finishes_calls_test: h2_sockpair+trace_shutdown_finishes_calls_test.exe
	echo Running h2_sockpair+trace_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_calls_test.exe

h2_sockpair+trace_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_shutdown_finishes_tags_test: h2_sockpair+trace_shutdown_finishes_tags_test.exe
	echo Running h2_sockpair+trace_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_tags_test.exe

h2_sockpair+trace_simple_request_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_simple_request_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_simple_request_test: h2_sockpair+trace_simple_request_test.exe
	echo Running h2_sockpair+trace_simple_request_test
	$(OUT_DIR)\h2_sockpair+trace_simple_request_test.exe

h2_sockpair+trace_trailing_metadata_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_trailing_metadata_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_trailing_metadata_test: h2_sockpair+trace_trailing_metadata_test.exe
	echo Running h2_sockpair+trace_trailing_metadata_test
	$(OUT_DIR)\h2_sockpair+trace_trailing_metadata_test.exe

h2_sockpair_1byte_bad_hostname_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_bad_hostname_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_bad_hostname_test: h2_sockpair_1byte_bad_hostname_test.exe
	echo Running h2_sockpair_1byte_bad_hostname_test
	$(OUT_DIR)\h2_sockpair_1byte_bad_hostname_test.exe

h2_sockpair_1byte_binary_metadata_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_binary_metadata_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_binary_metadata_test: h2_sockpair_1byte_binary_metadata_test.exe
	echo Running h2_sockpair_1byte_binary_metadata_test
	$(OUT_DIR)\h2_sockpair_1byte_binary_metadata_test.exe

h2_sockpair_1byte_call_creds_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_call_creds_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_call_creds_test: h2_sockpair_1byte_call_creds_test.exe
	echo Running h2_sockpair_1byte_call_creds_test
	$(OUT_DIR)\h2_sockpair_1byte_call_creds_test.exe

h2_sockpair_1byte_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_after_accept_test: h2_sockpair_1byte_cancel_after_accept_test.exe
	echo Running h2_sockpair_1byte_cancel_after_accept_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_after_accept_test.exe

h2_sockpair_1byte_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_after_client_done_test: h2_sockpair_1byte_cancel_after_client_done_test.exe
	echo Running h2_sockpair_1byte_cancel_after_client_done_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_after_client_done_test.exe

h2_sockpair_1byte_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_after_invoke_test: h2_sockpair_1byte_cancel_after_invoke_test.exe
	echo Running h2_sockpair_1byte_cancel_after_invoke_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_after_invoke_test.exe

h2_sockpair_1byte_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_before_invoke_test: h2_sockpair_1byte_cancel_before_invoke_test.exe
	echo Running h2_sockpair_1byte_cancel_before_invoke_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_before_invoke_test.exe

h2_sockpair_1byte_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_in_a_vacuum_test: h2_sockpair_1byte_cancel_in_a_vacuum_test.exe
	echo Running h2_sockpair_1byte_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_in_a_vacuum_test.exe

h2_sockpair_1byte_census_simple_request_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_census_simple_request_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_census_simple_request_test: h2_sockpair_1byte_census_simple_request_test.exe
	echo Running h2_sockpair_1byte_census_simple_request_test
	$(OUT_DIR)\h2_sockpair_1byte_census_simple_request_test.exe

h2_sockpair_1byte_compressed_payload_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_compressed_payload_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_compressed_payload_test: h2_sockpair_1byte_compressed_payload_test.exe
	echo Running h2_sockpair_1byte_compressed_payload_test
	$(OUT_DIR)\h2_sockpair_1byte_compressed_payload_test.exe

h2_sockpair_1byte_empty_batch_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_empty_batch_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_empty_batch_test: h2_sockpair_1byte_empty_batch_test.exe
	echo Running h2_sockpair_1byte_empty_batch_test
	$(OUT_DIR)\h2_sockpair_1byte_empty_batch_test.exe

h2_sockpair_1byte_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_graceful_server_shutdown_test: h2_sockpair_1byte_graceful_server_shutdown_test.exe
	echo Running h2_sockpair_1byte_graceful_server_shutdown_test
	$(OUT_DIR)\h2_sockpair_1byte_graceful_server_shutdown_test.exe

h2_sockpair_1byte_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_high_initial_seqno_test: h2_sockpair_1byte_high_initial_seqno_test.exe
	echo Running h2_sockpair_1byte_high_initial_seqno_test
	$(OUT_DIR)\h2_sockpair_1byte_high_initial_seqno_test.exe

h2_sockpair_1byte_invoke_large_request_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_invoke_large_request_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_invoke_large_request_test: h2_sockpair_1byte_invoke_large_request_test.exe
	echo Running h2_sockpair_1byte_invoke_large_request_test
	$(OUT_DIR)\h2_sockpair_1byte_invoke_large_request_test.exe

h2_sockpair_1byte_large_metadata_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_large_metadata_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_large_metadata_test: h2_sockpair_1byte_large_metadata_test.exe
	echo Running h2_sockpair_1byte_large_metadata_test
	$(OUT_DIR)\h2_sockpair_1byte_large_metadata_test.exe

h2_sockpair_1byte_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_max_concurrent_streams_test: h2_sockpair_1byte_max_concurrent_streams_test.exe
	echo Running h2_sockpair_1byte_max_concurrent_streams_test
	$(OUT_DIR)\h2_sockpair_1byte_max_concurrent_streams_test.exe

h2_sockpair_1byte_max_message_length_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_max_message_length_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_max_message_length_test: h2_sockpair_1byte_max_message_length_test.exe
	echo Running h2_sockpair_1byte_max_message_length_test
	$(OUT_DIR)\h2_sockpair_1byte_max_message_length_test.exe

h2_sockpair_1byte_metadata_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_metadata_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_metadata_test: h2_sockpair_1byte_metadata_test.exe
	echo Running h2_sockpair_1byte_metadata_test
	$(OUT_DIR)\h2_sockpair_1byte_metadata_test.exe

h2_sockpair_1byte_no_op_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_no_op_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_no_op_test: h2_sockpair_1byte_no_op_test.exe
	echo Running h2_sockpair_1byte_no_op_test
	$(OUT_DIR)\h2_sockpair_1byte_no_op_test.exe

h2_sockpair_1byte_payload_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_payload_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_payload_test: h2_sockpair_1byte_payload_test.exe
	echo Running h2_sockpair_1byte_payload_test
	$(OUT_DIR)\h2_sockpair_1byte_payload_test.exe

h2_sockpair_1byte_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_ping_pong_streaming_test: h2_sockpair_1byte_ping_pong_streaming_test.exe
	echo Running h2_sockpair_1byte_ping_pong_streaming_test
	$(OUT_DIR)\h2_sockpair_1byte_ping_pong_streaming_test.exe

h2_sockpair_1byte_registered_call_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_registered_call_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_registered_call_test: h2_sockpair_1byte_registered_call_test.exe
	echo Running h2_sockpair_1byte_registered_call_test
	$(OUT_DIR)\h2_sockpair_1byte_registered_call_test.exe

h2_sockpair_1byte_request_with_flags_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_request_with_flags_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_request_with_flags_test: h2_sockpair_1byte_request_with_flags_test.exe
	echo Running h2_sockpair_1byte_request_with_flags_test
	$(OUT_DIR)\h2_sockpair_1byte_request_with_flags_test.exe

h2_sockpair_1byte_request_with_payload_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_request_with_payload_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_request_with_payload_test: h2_sockpair_1byte_request_with_payload_test.exe
	echo Running h2_sockpair_1byte_request_with_payload_test
	$(OUT_DIR)\h2_sockpair_1byte_request_with_payload_test.exe

h2_sockpair_1byte_server_finishes_request_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_server_finishes_request_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_server_finishes_request_test: h2_sockpair_1byte_server_finishes_request_test.exe
	echo Running h2_sockpair_1byte_server_finishes_request_test
	$(OUT_DIR)\h2_sockpair_1byte_server_finishes_request_test.exe

h2_sockpair_1byte_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_shutdown_finishes_calls_test: h2_sockpair_1byte_shutdown_finishes_calls_test.exe
	echo Running h2_sockpair_1byte_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_calls_test.exe

h2_sockpair_1byte_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_shutdown_finishes_tags_test: h2_sockpair_1byte_shutdown_finishes_tags_test.exe
	echo Running h2_sockpair_1byte_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_tags_test.exe

h2_sockpair_1byte_simple_request_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_simple_request_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_simple_request_test: h2_sockpair_1byte_simple_request_test.exe
	echo Running h2_sockpair_1byte_simple_request_test
	$(OUT_DIR)\h2_sockpair_1byte_simple_request_test.exe

h2_sockpair_1byte_trailing_metadata_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_trailing_metadata_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_trailing_metadata_test: h2_sockpair_1byte_trailing_metadata_test.exe
	echo Running h2_sockpair_1byte_trailing_metadata_test
	$(OUT_DIR)\h2_sockpair_1byte_trailing_metadata_test.exe

h2_ssl_bad_hostname_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_bad_hostname_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_bad_hostname_test: h2_ssl_bad_hostname_test.exe
	echo Running h2_ssl_bad_hostname_test
	$(OUT_DIR)\h2_ssl_bad_hostname_test.exe

h2_ssl_binary_metadata_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_binary_metadata_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_binary_metadata_test: h2_ssl_binary_metadata_test.exe
	echo Running h2_ssl_binary_metadata_test
	$(OUT_DIR)\h2_ssl_binary_metadata_test.exe

h2_ssl_call_creds_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_call_creds_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_call_creds_test: h2_ssl_call_creds_test.exe
	echo Running h2_ssl_call_creds_test
	$(OUT_DIR)\h2_ssl_call_creds_test.exe

h2_ssl_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_cancel_after_accept_test: h2_ssl_cancel_after_accept_test.exe
	echo Running h2_ssl_cancel_after_accept_test
	$(OUT_DIR)\h2_ssl_cancel_after_accept_test.exe

h2_ssl_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_cancel_after_client_done_test: h2_ssl_cancel_after_client_done_test.exe
	echo Running h2_ssl_cancel_after_client_done_test
	$(OUT_DIR)\h2_ssl_cancel_after_client_done_test.exe

h2_ssl_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_cancel_after_invoke_test: h2_ssl_cancel_after_invoke_test.exe
	echo Running h2_ssl_cancel_after_invoke_test
	$(OUT_DIR)\h2_ssl_cancel_after_invoke_test.exe

h2_ssl_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_cancel_before_invoke_test: h2_ssl_cancel_before_invoke_test.exe
	echo Running h2_ssl_cancel_before_invoke_test
	$(OUT_DIR)\h2_ssl_cancel_before_invoke_test.exe

h2_ssl_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_cancel_in_a_vacuum_test: h2_ssl_cancel_in_a_vacuum_test.exe
	echo Running h2_ssl_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_ssl_cancel_in_a_vacuum_test.exe

h2_ssl_census_simple_request_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_census_simple_request_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_census_simple_request_test: h2_ssl_census_simple_request_test.exe
	echo Running h2_ssl_census_simple_request_test
	$(OUT_DIR)\h2_ssl_census_simple_request_test.exe

h2_ssl_channel_connectivity_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_channel_connectivity_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_channel_connectivity_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_channel_connectivity.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_channel_connectivity_test: h2_ssl_channel_connectivity_test.exe
	echo Running h2_ssl_channel_connectivity_test
	$(OUT_DIR)\h2_ssl_channel_connectivity_test.exe

h2_ssl_compressed_payload_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_compressed_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_compressed_payload_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_compressed_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_compressed_payload_test: h2_ssl_compressed_payload_test.exe
	echo Running h2_ssl_compressed_payload_test
	$(OUT_DIR)\h2_ssl_compressed_payload_test.exe

h2_ssl_default_host_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_default_host_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_default_host_test: h2_ssl_default_host_test.exe
	echo Running h2_ssl_default_host_test
	$(OUT_DIR)\h2_ssl_default_host_test.exe

h2_ssl_disappearing_server_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_disappearing_server_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_disappearing_server_test: h2_ssl_disappearing_server_test.exe
	echo Running h2_ssl_disappearing_server_test
	$(OUT_DIR)\h2_ssl_disappearing_server_test.exe

h2_ssl_empty_batch_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_empty_batch_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_empty_batch_test: h2_ssl_empty_batch_test.exe
	echo Running h2_ssl_empty_batch_test
	$(OUT_DIR)\h2_ssl_empty_batch_test.exe

h2_ssl_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_graceful_server_shutdown_test: h2_ssl_graceful_server_shutdown_test.exe
	echo Running h2_ssl_graceful_server_shutdown_test
	$(OUT_DIR)\h2_ssl_graceful_server_shutdown_test.exe

h2_ssl_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_high_initial_seqno_test: h2_ssl_high_initial_seqno_test.exe
	echo Running h2_ssl_high_initial_seqno_test
	$(OUT_DIR)\h2_ssl_high_initial_seqno_test.exe

h2_ssl_invoke_large_request_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_invoke_large_request_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_invoke_large_request_test: h2_ssl_invoke_large_request_test.exe
	echo Running h2_ssl_invoke_large_request_test
	$(OUT_DIR)\h2_ssl_invoke_large_request_test.exe

h2_ssl_large_metadata_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_large_metadata_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_large_metadata_test: h2_ssl_large_metadata_test.exe
	echo Running h2_ssl_large_metadata_test
	$(OUT_DIR)\h2_ssl_large_metadata_test.exe

h2_ssl_max_concurrent_streams_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_max_concurrent_streams_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_max_concurrent_streams_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_max_concurrent_streams.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_max_concurrent_streams_test: h2_ssl_max_concurrent_streams_test.exe
	echo Running h2_ssl_max_concurrent_streams_test
	$(OUT_DIR)\h2_ssl_max_concurrent_streams_test.exe

h2_ssl_max_message_length_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_max_message_length_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_max_message_length_test: h2_ssl_max_message_length_test.exe
	echo Running h2_ssl_max_message_length_test
	$(OUT_DIR)\h2_ssl_max_message_length_test.exe

h2_ssl_metadata_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_metadata_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_metadata_test: h2_ssl_metadata_test.exe
	echo Running h2_ssl_metadata_test
	$(OUT_DIR)\h2_ssl_metadata_test.exe

h2_ssl_no_op_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_no_op_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_no_op_test: h2_ssl_no_op_test.exe
	echo Running h2_ssl_no_op_test
	$(OUT_DIR)\h2_ssl_no_op_test.exe

h2_ssl_payload_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_payload_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_payload_test: h2_ssl_payload_test.exe
	echo Running h2_ssl_payload_test
	$(OUT_DIR)\h2_ssl_payload_test.exe

h2_ssl_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_ping_pong_streaming_test: h2_ssl_ping_pong_streaming_test.exe
	echo Running h2_ssl_ping_pong_streaming_test
	$(OUT_DIR)\h2_ssl_ping_pong_streaming_test.exe

h2_ssl_registered_call_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_registered_call_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_registered_call_test: h2_ssl_registered_call_test.exe
	echo Running h2_ssl_registered_call_test
	$(OUT_DIR)\h2_ssl_registered_call_test.exe

h2_ssl_request_with_flags_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_request_with_flags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_request_with_flags_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_request_with_flags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_request_with_flags_test: h2_ssl_request_with_flags_test.exe
	echo Running h2_ssl_request_with_flags_test
	$(OUT_DIR)\h2_ssl_request_with_flags_test.exe

h2_ssl_request_with_payload_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_request_with_payload_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_request_with_payload_test: h2_ssl_request_with_payload_test.exe
	echo Running h2_ssl_request_with_payload_test
	$(OUT_DIR)\h2_ssl_request_with_payload_test.exe

h2_ssl_server_finishes_request_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_server_finishes_request_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_server_finishes_request_test: h2_ssl_server_finishes_request_test.exe
	echo Running h2_ssl_server_finishes_request_test
	$(OUT_DIR)\h2_ssl_server_finishes_request_test.exe

h2_ssl_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_shutdown_finishes_calls_test: h2_ssl_shutdown_finishes_calls_test.exe
	echo Running h2_ssl_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_ssl_shutdown_finishes_calls_test.exe

h2_ssl_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_shutdown_finishes_tags_test: h2_ssl_shutdown_finishes_tags_test.exe
	echo Running h2_ssl_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_ssl_shutdown_finishes_tags_test.exe

h2_ssl_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_simple_delayed_request_test: h2_ssl_simple_delayed_request_test.exe
	echo Running h2_ssl_simple_delayed_request_test
	$(OUT_DIR)\h2_ssl_simple_delayed_request_test.exe

h2_ssl_simple_request_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_simple_request_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_simple_request_test: h2_ssl_simple_request_test.exe
	echo Running h2_ssl_simple_request_test
	$(OUT_DIR)\h2_ssl_simple_request_test.exe

h2_ssl_trailing_metadata_test.exe: Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_trailing_metadata_test.exe" Debug\end2end_fixture_h2_ssl.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_trailing_metadata_test: h2_ssl_trailing_metadata_test.exe
	echo Running h2_ssl_trailing_metadata_test
	$(OUT_DIR)\h2_ssl_trailing_metadata_test.exe

h2_ssl_proxy_bad_hostname_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_bad_hostname_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_bad_hostname_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_bad_hostname.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_bad_hostname_test: h2_ssl_proxy_bad_hostname_test.exe
	echo Running h2_ssl_proxy_bad_hostname_test
	$(OUT_DIR)\h2_ssl_proxy_bad_hostname_test.exe

h2_ssl_proxy_binary_metadata_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_binary_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_binary_metadata_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_binary_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_binary_metadata_test: h2_ssl_proxy_binary_metadata_test.exe
	echo Running h2_ssl_proxy_binary_metadata_test
	$(OUT_DIR)\h2_ssl_proxy_binary_metadata_test.exe

h2_ssl_proxy_call_creds_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_call_creds_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_call_creds_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_call_creds.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_call_creds_test: h2_ssl_proxy_call_creds_test.exe
	echo Running h2_ssl_proxy_call_creds_test
	$(OUT_DIR)\h2_ssl_proxy_call_creds_test.exe

h2_ssl_proxy_cancel_after_accept_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_cancel_after_accept_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_cancel_after_accept_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_after_accept.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_cancel_after_accept_test: h2_ssl_proxy_cancel_after_accept_test.exe
	echo Running h2_ssl_proxy_cancel_after_accept_test
	$(OUT_DIR)\h2_ssl_proxy_cancel_after_accept_test.exe

h2_ssl_proxy_cancel_after_client_done_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_cancel_after_client_done_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_cancel_after_client_done_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_after_client_done.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_cancel_after_client_done_test: h2_ssl_proxy_cancel_after_client_done_test.exe
	echo Running h2_ssl_proxy_cancel_after_client_done_test
	$(OUT_DIR)\h2_ssl_proxy_cancel_after_client_done_test.exe

h2_ssl_proxy_cancel_after_invoke_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_cancel_after_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_cancel_after_invoke_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_after_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_cancel_after_invoke_test: h2_ssl_proxy_cancel_after_invoke_test.exe
	echo Running h2_ssl_proxy_cancel_after_invoke_test
	$(OUT_DIR)\h2_ssl_proxy_cancel_after_invoke_test.exe

h2_ssl_proxy_cancel_before_invoke_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_cancel_before_invoke_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_cancel_before_invoke_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_before_invoke.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_cancel_before_invoke_test: h2_ssl_proxy_cancel_before_invoke_test.exe
	echo Running h2_ssl_proxy_cancel_before_invoke_test
	$(OUT_DIR)\h2_ssl_proxy_cancel_before_invoke_test.exe

h2_ssl_proxy_cancel_in_a_vacuum_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_cancel_in_a_vacuum_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_cancel_in_a_vacuum_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_cancel_in_a_vacuum_test: h2_ssl_proxy_cancel_in_a_vacuum_test.exe
	echo Running h2_ssl_proxy_cancel_in_a_vacuum_test
	$(OUT_DIR)\h2_ssl_proxy_cancel_in_a_vacuum_test.exe

h2_ssl_proxy_census_simple_request_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_census_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_census_simple_request_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_census_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_census_simple_request_test: h2_ssl_proxy_census_simple_request_test.exe
	echo Running h2_ssl_proxy_census_simple_request_test
	$(OUT_DIR)\h2_ssl_proxy_census_simple_request_test.exe

h2_ssl_proxy_default_host_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_default_host_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_default_host_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_default_host.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_default_host_test: h2_ssl_proxy_default_host_test.exe
	echo Running h2_ssl_proxy_default_host_test
	$(OUT_DIR)\h2_ssl_proxy_default_host_test.exe

h2_ssl_proxy_disappearing_server_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_disappearing_server_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_disappearing_server_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_disappearing_server.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_disappearing_server_test: h2_ssl_proxy_disappearing_server_test.exe
	echo Running h2_ssl_proxy_disappearing_server_test
	$(OUT_DIR)\h2_ssl_proxy_disappearing_server_test.exe

h2_ssl_proxy_empty_batch_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_empty_batch_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_empty_batch_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_empty_batch.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_empty_batch_test: h2_ssl_proxy_empty_batch_test.exe
	echo Running h2_ssl_proxy_empty_batch_test
	$(OUT_DIR)\h2_ssl_proxy_empty_batch_test.exe

h2_ssl_proxy_graceful_server_shutdown_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_graceful_server_shutdown_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_graceful_server_shutdown_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_graceful_server_shutdown_test: h2_ssl_proxy_graceful_server_shutdown_test.exe
	echo Running h2_ssl_proxy_graceful_server_shutdown_test
	$(OUT_DIR)\h2_ssl_proxy_graceful_server_shutdown_test.exe

h2_ssl_proxy_high_initial_seqno_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_high_initial_seqno_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_high_initial_seqno_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_high_initial_seqno.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_high_initial_seqno_test: h2_ssl_proxy_high_initial_seqno_test.exe
	echo Running h2_ssl_proxy_high_initial_seqno_test
	$(OUT_DIR)\h2_ssl_proxy_high_initial_seqno_test.exe

h2_ssl_proxy_invoke_large_request_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_invoke_large_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_invoke_large_request_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_invoke_large_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_invoke_large_request_test: h2_ssl_proxy_invoke_large_request_test.exe
	echo Running h2_ssl_proxy_invoke_large_request_test
	$(OUT_DIR)\h2_ssl_proxy_invoke_large_request_test.exe

h2_ssl_proxy_large_metadata_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_large_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_large_metadata_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_large_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_large_metadata_test: h2_ssl_proxy_large_metadata_test.exe
	echo Running h2_ssl_proxy_large_metadata_test
	$(OUT_DIR)\h2_ssl_proxy_large_metadata_test.exe

h2_ssl_proxy_max_message_length_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_max_message_length_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_max_message_length_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_max_message_length.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_max_message_length_test: h2_ssl_proxy_max_message_length_test.exe
	echo Running h2_ssl_proxy_max_message_length_test
	$(OUT_DIR)\h2_ssl_proxy_max_message_length_test.exe

h2_ssl_proxy_metadata_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_metadata_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_metadata_test: h2_ssl_proxy_metadata_test.exe
	echo Running h2_ssl_proxy_metadata_test
	$(OUT_DIR)\h2_ssl_proxy_metadata_test.exe

h2_ssl_proxy_no_op_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_no_op_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_no_op_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_no_op.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_no_op_test: h2_ssl_proxy_no_op_test.exe
	echo Running h2_ssl_proxy_no_op_test
	$(OUT_DIR)\h2_ssl_proxy_no_op_test.exe

h2_ssl_proxy_payload_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_payload_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_payload_test: h2_ssl_proxy_payload_test.exe
	echo Running h2_ssl_proxy_payload_test
	$(OUT_DIR)\h2_ssl_proxy_payload_test.exe

h2_ssl_proxy_ping_pong_streaming_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_ping_pong_streaming_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_ping_pong_streaming_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_ping_pong_streaming.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_ping_pong_streaming_test: h2_ssl_proxy_ping_pong_streaming_test.exe
	echo Running h2_ssl_proxy_ping_pong_streaming_test
	$(OUT_DIR)\h2_ssl_proxy_ping_pong_streaming_test.exe

h2_ssl_proxy_registered_call_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_registered_call_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_registered_call_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_registered_call.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_registered_call_test: h2_ssl_proxy_registered_call_test.exe
	echo Running h2_ssl_proxy_registered_call_test
	$(OUT_DIR)\h2_ssl_proxy_registered_call_test.exe

h2_ssl_proxy_request_with_payload_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_request_with_payload_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_request_with_payload_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_request_with_payload.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_request_with_payload_test: h2_ssl_proxy_request_with_payload_test.exe
	echo Running h2_ssl_proxy_request_with_payload_test
	$(OUT_DIR)\h2_ssl_proxy_request_with_payload_test.exe

h2_ssl_proxy_server_finishes_request_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_server_finishes_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_server_finishes_request_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_server_finishes_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_server_finishes_request_test: h2_ssl_proxy_server_finishes_request_test.exe
	echo Running h2_ssl_proxy_server_finishes_request_test
	$(OUT_DIR)\h2_ssl_proxy_server_finishes_request_test.exe

h2_ssl_proxy_shutdown_finishes_calls_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_shutdown_finishes_calls_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_shutdown_finishes_calls_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_shutdown_finishes_calls_test: h2_ssl_proxy_shutdown_finishes_calls_test.exe
	echo Running h2_ssl_proxy_shutdown_finishes_calls_test
	$(OUT_DIR)\h2_ssl_proxy_shutdown_finishes_calls_test.exe

h2_ssl_proxy_shutdown_finishes_tags_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_shutdown_finishes_tags_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_shutdown_finishes_tags_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_shutdown_finishes_tags_test: h2_ssl_proxy_shutdown_finishes_tags_test.exe
	echo Running h2_ssl_proxy_shutdown_finishes_tags_test
	$(OUT_DIR)\h2_ssl_proxy_shutdown_finishes_tags_test.exe

h2_ssl_proxy_simple_delayed_request_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_simple_delayed_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_simple_delayed_request_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_simple_delayed_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_simple_delayed_request_test: h2_ssl_proxy_simple_delayed_request_test.exe
	echo Running h2_ssl_proxy_simple_delayed_request_test
	$(OUT_DIR)\h2_ssl_proxy_simple_delayed_request_test.exe

h2_ssl_proxy_simple_request_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_simple_request_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_simple_request_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_simple_request.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_simple_request_test: h2_ssl_proxy_simple_request_test.exe
	echo Running h2_ssl_proxy_simple_request_test
	$(OUT_DIR)\h2_ssl_proxy_simple_request_test.exe

h2_ssl_proxy_trailing_metadata_test.exe: Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib build_grpc_test_util build_grpc build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_ssl_proxy_trailing_metadata_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_ssl_proxy_trailing_metadata_test.exe" Debug\end2end_fixture_h2_ssl_proxy.lib Debug\end2end_test_trailing_metadata.lib Debug\end2end_certs.lib Debug\grpc_test_util.lib Debug\grpc.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_ssl_proxy_trailing_metadata_test: h2_ssl_proxy_trailing_metadata_test.exe
	echo Running h2_ssl_proxy_trailing_metadata_test
	$(OUT_DIR)\h2_ssl_proxy_trailing_metadata_test.exe

h2_compress_bad_hostname_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_bad_hostname.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_bad_hostname_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_bad_hostname_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_bad_hostname.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_bad_hostname_nosec_test: h2_compress_bad_hostname_nosec_test.exe
	echo Running h2_compress_bad_hostname_nosec_test
	$(OUT_DIR)\h2_compress_bad_hostname_nosec_test.exe

h2_compress_binary_metadata_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_binary_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_binary_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_binary_metadata_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_binary_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_binary_metadata_nosec_test: h2_compress_binary_metadata_nosec_test.exe
	echo Running h2_compress_binary_metadata_nosec_test
	$(OUT_DIR)\h2_compress_binary_metadata_nosec_test.exe

h2_compress_cancel_after_accept_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_accept.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_after_accept_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_after_accept_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_accept.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_after_accept_nosec_test: h2_compress_cancel_after_accept_nosec_test.exe
	echo Running h2_compress_cancel_after_accept_nosec_test
	$(OUT_DIR)\h2_compress_cancel_after_accept_nosec_test.exe

h2_compress_cancel_after_client_done_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_client_done.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_after_client_done_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_after_client_done_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_client_done.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_after_client_done_nosec_test: h2_compress_cancel_after_client_done_nosec_test.exe
	echo Running h2_compress_cancel_after_client_done_nosec_test
	$(OUT_DIR)\h2_compress_cancel_after_client_done_nosec_test.exe

h2_compress_cancel_after_invoke_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_after_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_after_invoke_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_after_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_after_invoke_nosec_test: h2_compress_cancel_after_invoke_nosec_test.exe
	echo Running h2_compress_cancel_after_invoke_nosec_test
	$(OUT_DIR)\h2_compress_cancel_after_invoke_nosec_test.exe

h2_compress_cancel_before_invoke_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_before_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_before_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_before_invoke_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_before_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_before_invoke_nosec_test: h2_compress_cancel_before_invoke_nosec_test.exe
	echo Running h2_compress_cancel_before_invoke_nosec_test
	$(OUT_DIR)\h2_compress_cancel_before_invoke_nosec_test.exe

h2_compress_cancel_in_a_vacuum_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_in_a_vacuum.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_cancel_in_a_vacuum_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_cancel_in_a_vacuum_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_cancel_in_a_vacuum_nosec_test: h2_compress_cancel_in_a_vacuum_nosec_test.exe
	echo Running h2_compress_cancel_in_a_vacuum_nosec_test
	$(OUT_DIR)\h2_compress_cancel_in_a_vacuum_nosec_test.exe

h2_compress_census_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_census_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_census_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_census_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_census_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_census_simple_request_nosec_test: h2_compress_census_simple_request_nosec_test.exe
	echo Running h2_compress_census_simple_request_nosec_test
	$(OUT_DIR)\h2_compress_census_simple_request_nosec_test.exe

h2_compress_channel_connectivity_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_channel_connectivity.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_channel_connectivity_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_channel_connectivity_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_channel_connectivity.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_channel_connectivity_nosec_test: h2_compress_channel_connectivity_nosec_test.exe
	echo Running h2_compress_channel_connectivity_nosec_test
	$(OUT_DIR)\h2_compress_channel_connectivity_nosec_test.exe

h2_compress_compressed_payload_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_compressed_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_compressed_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_compressed_payload_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_compressed_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_compressed_payload_nosec_test: h2_compress_compressed_payload_nosec_test.exe
	echo Running h2_compress_compressed_payload_nosec_test
	$(OUT_DIR)\h2_compress_compressed_payload_nosec_test.exe

h2_compress_default_host_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_default_host.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_default_host_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_default_host_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_default_host.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_default_host_nosec_test: h2_compress_default_host_nosec_test.exe
	echo Running h2_compress_default_host_nosec_test
	$(OUT_DIR)\h2_compress_default_host_nosec_test.exe

h2_compress_disappearing_server_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_disappearing_server.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_disappearing_server_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_disappearing_server_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_disappearing_server.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_disappearing_server_nosec_test: h2_compress_disappearing_server_nosec_test.exe
	echo Running h2_compress_disappearing_server_nosec_test
	$(OUT_DIR)\h2_compress_disappearing_server_nosec_test.exe

h2_compress_empty_batch_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_empty_batch.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_empty_batch_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_empty_batch_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_empty_batch.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_empty_batch_nosec_test: h2_compress_empty_batch_nosec_test.exe
	echo Running h2_compress_empty_batch_nosec_test
	$(OUT_DIR)\h2_compress_empty_batch_nosec_test.exe

h2_compress_graceful_server_shutdown_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_graceful_server_shutdown.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_graceful_server_shutdown_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_graceful_server_shutdown_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_graceful_server_shutdown_nosec_test: h2_compress_graceful_server_shutdown_nosec_test.exe
	echo Running h2_compress_graceful_server_shutdown_nosec_test
	$(OUT_DIR)\h2_compress_graceful_server_shutdown_nosec_test.exe

h2_compress_high_initial_seqno_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_high_initial_seqno.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_high_initial_seqno_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_high_initial_seqno_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_high_initial_seqno.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_high_initial_seqno_nosec_test: h2_compress_high_initial_seqno_nosec_test.exe
	echo Running h2_compress_high_initial_seqno_nosec_test
	$(OUT_DIR)\h2_compress_high_initial_seqno_nosec_test.exe

h2_compress_invoke_large_request_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_invoke_large_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_invoke_large_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_invoke_large_request_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_invoke_large_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_invoke_large_request_nosec_test: h2_compress_invoke_large_request_nosec_test.exe
	echo Running h2_compress_invoke_large_request_nosec_test
	$(OUT_DIR)\h2_compress_invoke_large_request_nosec_test.exe

h2_compress_large_metadata_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_large_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_large_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_large_metadata_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_large_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_large_metadata_nosec_test: h2_compress_large_metadata_nosec_test.exe
	echo Running h2_compress_large_metadata_nosec_test
	$(OUT_DIR)\h2_compress_large_metadata_nosec_test.exe

h2_compress_max_concurrent_streams_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_concurrent_streams.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_max_concurrent_streams_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_max_concurrent_streams_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_concurrent_streams.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_max_concurrent_streams_nosec_test: h2_compress_max_concurrent_streams_nosec_test.exe
	echo Running h2_compress_max_concurrent_streams_nosec_test
	$(OUT_DIR)\h2_compress_max_concurrent_streams_nosec_test.exe

h2_compress_max_message_length_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_message_length.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_max_message_length_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_max_message_length_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_max_message_length.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_max_message_length_nosec_test: h2_compress_max_message_length_nosec_test.exe
	echo Running h2_compress_max_message_length_nosec_test
	$(OUT_DIR)\h2_compress_max_message_length_nosec_test.exe

h2_compress_metadata_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_metadata_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_metadata_nosec_test: h2_compress_metadata_nosec_test.exe
	echo Running h2_compress_metadata_nosec_test
	$(OUT_DIR)\h2_compress_metadata_nosec_test.exe

h2_compress_no_op_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_no_op.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_no_op_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_no_op_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_no_op.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_no_op_nosec_test: h2_compress_no_op_nosec_test.exe
	echo Running h2_compress_no_op_nosec_test
	$(OUT_DIR)\h2_compress_no_op_nosec_test.exe

h2_compress_payload_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_payload_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_payload_nosec_test: h2_compress_payload_nosec_test.exe
	echo Running h2_compress_payload_nosec_test
	$(OUT_DIR)\h2_compress_payload_nosec_test.exe

h2_compress_ping_pong_streaming_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_ping_pong_streaming.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_ping_pong_streaming_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_ping_pong_streaming_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_ping_pong_streaming.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_ping_pong_streaming_nosec_test: h2_compress_ping_pong_streaming_nosec_test.exe
	echo Running h2_compress_ping_pong_streaming_nosec_test
	$(OUT_DIR)\h2_compress_ping_pong_streaming_nosec_test.exe

h2_compress_registered_call_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_registered_call.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_registered_call_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_registered_call_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_registered_call.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_registered_call_nosec_test: h2_compress_registered_call_nosec_test.exe
	echo Running h2_compress_registered_call_nosec_test
	$(OUT_DIR)\h2_compress_registered_call_nosec_test.exe

h2_compress_request_with_flags_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_flags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_request_with_flags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_request_with_flags_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_flags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_request_with_flags_nosec_test: h2_compress_request_with_flags_nosec_test.exe
	echo Running h2_compress_request_with_flags_nosec_test
	$(OUT_DIR)\h2_compress_request_with_flags_nosec_test.exe

h2_compress_request_with_payload_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_request_with_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_request_with_payload_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_request_with_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_request_with_payload_nosec_test: h2_compress_request_with_payload_nosec_test.exe
	echo Running h2_compress_request_with_payload_nosec_test
	$(OUT_DIR)\h2_compress_request_with_payload_nosec_test.exe

h2_compress_server_finishes_request_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_server_finishes_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_server_finishes_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_server_finishes_request_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_server_finishes_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_server_finishes_request_nosec_test: h2_compress_server_finishes_request_nosec_test.exe
	echo Running h2_compress_server_finishes_request_nosec_test
	$(OUT_DIR)\h2_compress_server_finishes_request_nosec_test.exe

h2_compress_shutdown_finishes_calls_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_calls.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_shutdown_finishes_calls_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_shutdown_finishes_calls_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_shutdown_finishes_calls_nosec_test: h2_compress_shutdown_finishes_calls_nosec_test.exe
	echo Running h2_compress_shutdown_finishes_calls_nosec_test
	$(OUT_DIR)\h2_compress_shutdown_finishes_calls_nosec_test.exe

h2_compress_shutdown_finishes_tags_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_tags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_shutdown_finishes_tags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_shutdown_finishes_tags_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_shutdown_finishes_tags_nosec_test: h2_compress_shutdown_finishes_tags_nosec_test.exe
	echo Running h2_compress_shutdown_finishes_tags_nosec_test
	$(OUT_DIR)\h2_compress_shutdown_finishes_tags_nosec_test.exe

h2_compress_simple_delayed_request_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_delayed_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_simple_delayed_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_simple_delayed_request_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_delayed_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_simple_delayed_request_nosec_test: h2_compress_simple_delayed_request_nosec_test.exe
	echo Running h2_compress_simple_delayed_request_nosec_test
	$(OUT_DIR)\h2_compress_simple_delayed_request_nosec_test.exe

h2_compress_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_simple_request_nosec_test: h2_compress_simple_request_nosec_test.exe
	echo Running h2_compress_simple_request_nosec_test
	$(OUT_DIR)\h2_compress_simple_request_nosec_test.exe

h2_compress_trailing_metadata_nosec_test.exe: Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_trailing_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_compress_trailing_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_compress_trailing_metadata_nosec_test.exe" Debug\end2end_fixture_h2_compress.lib Debug\end2end_test_trailing_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_compress_trailing_metadata_nosec_test: h2_compress_trailing_metadata_nosec_test.exe
	echo Running h2_compress_trailing_metadata_nosec_test
	$(OUT_DIR)\h2_compress_trailing_metadata_nosec_test.exe

h2_full_bad_hostname_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_bad_hostname.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_bad_hostname_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_bad_hostname_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_bad_hostname.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_bad_hostname_nosec_test: h2_full_bad_hostname_nosec_test.exe
	echo Running h2_full_bad_hostname_nosec_test
	$(OUT_DIR)\h2_full_bad_hostname_nosec_test.exe

h2_full_binary_metadata_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_binary_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_binary_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_binary_metadata_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_binary_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_binary_metadata_nosec_test: h2_full_binary_metadata_nosec_test.exe
	echo Running h2_full_binary_metadata_nosec_test
	$(OUT_DIR)\h2_full_binary_metadata_nosec_test.exe

h2_full_cancel_after_accept_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_accept.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_after_accept_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_after_accept_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_accept.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_after_accept_nosec_test: h2_full_cancel_after_accept_nosec_test.exe
	echo Running h2_full_cancel_after_accept_nosec_test
	$(OUT_DIR)\h2_full_cancel_after_accept_nosec_test.exe

h2_full_cancel_after_client_done_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_client_done.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_after_client_done_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_after_client_done_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_client_done.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_after_client_done_nosec_test: h2_full_cancel_after_client_done_nosec_test.exe
	echo Running h2_full_cancel_after_client_done_nosec_test
	$(OUT_DIR)\h2_full_cancel_after_client_done_nosec_test.exe

h2_full_cancel_after_invoke_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_after_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_after_invoke_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_after_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_after_invoke_nosec_test: h2_full_cancel_after_invoke_nosec_test.exe
	echo Running h2_full_cancel_after_invoke_nosec_test
	$(OUT_DIR)\h2_full_cancel_after_invoke_nosec_test.exe

h2_full_cancel_before_invoke_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_before_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_before_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_before_invoke_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_before_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_before_invoke_nosec_test: h2_full_cancel_before_invoke_nosec_test.exe
	echo Running h2_full_cancel_before_invoke_nosec_test
	$(OUT_DIR)\h2_full_cancel_before_invoke_nosec_test.exe

h2_full_cancel_in_a_vacuum_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_in_a_vacuum.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_cancel_in_a_vacuum_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_cancel_in_a_vacuum_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_cancel_in_a_vacuum_nosec_test: h2_full_cancel_in_a_vacuum_nosec_test.exe
	echo Running h2_full_cancel_in_a_vacuum_nosec_test
	$(OUT_DIR)\h2_full_cancel_in_a_vacuum_nosec_test.exe

h2_full_census_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_census_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_census_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_census_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_census_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_census_simple_request_nosec_test: h2_full_census_simple_request_nosec_test.exe
	echo Running h2_full_census_simple_request_nosec_test
	$(OUT_DIR)\h2_full_census_simple_request_nosec_test.exe

h2_full_channel_connectivity_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_channel_connectivity.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_channel_connectivity_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_channel_connectivity_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_channel_connectivity.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_channel_connectivity_nosec_test: h2_full_channel_connectivity_nosec_test.exe
	echo Running h2_full_channel_connectivity_nosec_test
	$(OUT_DIR)\h2_full_channel_connectivity_nosec_test.exe

h2_full_compressed_payload_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_compressed_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_compressed_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_compressed_payload_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_compressed_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_compressed_payload_nosec_test: h2_full_compressed_payload_nosec_test.exe
	echo Running h2_full_compressed_payload_nosec_test
	$(OUT_DIR)\h2_full_compressed_payload_nosec_test.exe

h2_full_default_host_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_default_host.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_default_host_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_default_host_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_default_host.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_default_host_nosec_test: h2_full_default_host_nosec_test.exe
	echo Running h2_full_default_host_nosec_test
	$(OUT_DIR)\h2_full_default_host_nosec_test.exe

h2_full_disappearing_server_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_disappearing_server.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_disappearing_server_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_disappearing_server_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_disappearing_server.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_disappearing_server_nosec_test: h2_full_disappearing_server_nosec_test.exe
	echo Running h2_full_disappearing_server_nosec_test
	$(OUT_DIR)\h2_full_disappearing_server_nosec_test.exe

h2_full_empty_batch_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_empty_batch.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_empty_batch_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_empty_batch_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_empty_batch.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_empty_batch_nosec_test: h2_full_empty_batch_nosec_test.exe
	echo Running h2_full_empty_batch_nosec_test
	$(OUT_DIR)\h2_full_empty_batch_nosec_test.exe

h2_full_graceful_server_shutdown_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_graceful_server_shutdown.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_graceful_server_shutdown_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_graceful_server_shutdown_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_graceful_server_shutdown_nosec_test: h2_full_graceful_server_shutdown_nosec_test.exe
	echo Running h2_full_graceful_server_shutdown_nosec_test
	$(OUT_DIR)\h2_full_graceful_server_shutdown_nosec_test.exe

h2_full_high_initial_seqno_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_high_initial_seqno.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_high_initial_seqno_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_high_initial_seqno_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_high_initial_seqno.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_high_initial_seqno_nosec_test: h2_full_high_initial_seqno_nosec_test.exe
	echo Running h2_full_high_initial_seqno_nosec_test
	$(OUT_DIR)\h2_full_high_initial_seqno_nosec_test.exe

h2_full_invoke_large_request_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_invoke_large_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_invoke_large_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_invoke_large_request_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_invoke_large_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_invoke_large_request_nosec_test: h2_full_invoke_large_request_nosec_test.exe
	echo Running h2_full_invoke_large_request_nosec_test
	$(OUT_DIR)\h2_full_invoke_large_request_nosec_test.exe

h2_full_large_metadata_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_large_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_large_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_large_metadata_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_large_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_large_metadata_nosec_test: h2_full_large_metadata_nosec_test.exe
	echo Running h2_full_large_metadata_nosec_test
	$(OUT_DIR)\h2_full_large_metadata_nosec_test.exe

h2_full_max_concurrent_streams_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_concurrent_streams.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_max_concurrent_streams_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_max_concurrent_streams_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_concurrent_streams.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_max_concurrent_streams_nosec_test: h2_full_max_concurrent_streams_nosec_test.exe
	echo Running h2_full_max_concurrent_streams_nosec_test
	$(OUT_DIR)\h2_full_max_concurrent_streams_nosec_test.exe

h2_full_max_message_length_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_message_length.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_max_message_length_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_max_message_length_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_max_message_length.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_max_message_length_nosec_test: h2_full_max_message_length_nosec_test.exe
	echo Running h2_full_max_message_length_nosec_test
	$(OUT_DIR)\h2_full_max_message_length_nosec_test.exe

h2_full_metadata_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_metadata_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_metadata_nosec_test: h2_full_metadata_nosec_test.exe
	echo Running h2_full_metadata_nosec_test
	$(OUT_DIR)\h2_full_metadata_nosec_test.exe

h2_full_no_op_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_no_op.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_no_op_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_no_op_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_no_op.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_no_op_nosec_test: h2_full_no_op_nosec_test.exe
	echo Running h2_full_no_op_nosec_test
	$(OUT_DIR)\h2_full_no_op_nosec_test.exe

h2_full_payload_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_payload_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_payload_nosec_test: h2_full_payload_nosec_test.exe
	echo Running h2_full_payload_nosec_test
	$(OUT_DIR)\h2_full_payload_nosec_test.exe

h2_full_ping_pong_streaming_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_ping_pong_streaming.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_ping_pong_streaming_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_ping_pong_streaming_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_ping_pong_streaming.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_ping_pong_streaming_nosec_test: h2_full_ping_pong_streaming_nosec_test.exe
	echo Running h2_full_ping_pong_streaming_nosec_test
	$(OUT_DIR)\h2_full_ping_pong_streaming_nosec_test.exe

h2_full_registered_call_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_registered_call.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_registered_call_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_registered_call_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_registered_call.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_registered_call_nosec_test: h2_full_registered_call_nosec_test.exe
	echo Running h2_full_registered_call_nosec_test
	$(OUT_DIR)\h2_full_registered_call_nosec_test.exe

h2_full_request_with_flags_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_flags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_request_with_flags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_request_with_flags_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_flags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_request_with_flags_nosec_test: h2_full_request_with_flags_nosec_test.exe
	echo Running h2_full_request_with_flags_nosec_test
	$(OUT_DIR)\h2_full_request_with_flags_nosec_test.exe

h2_full_request_with_payload_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_request_with_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_request_with_payload_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_request_with_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_request_with_payload_nosec_test: h2_full_request_with_payload_nosec_test.exe
	echo Running h2_full_request_with_payload_nosec_test
	$(OUT_DIR)\h2_full_request_with_payload_nosec_test.exe

h2_full_server_finishes_request_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_server_finishes_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_server_finishes_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_server_finishes_request_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_server_finishes_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_server_finishes_request_nosec_test: h2_full_server_finishes_request_nosec_test.exe
	echo Running h2_full_server_finishes_request_nosec_test
	$(OUT_DIR)\h2_full_server_finishes_request_nosec_test.exe

h2_full_shutdown_finishes_calls_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_calls.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_shutdown_finishes_calls_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_shutdown_finishes_calls_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_shutdown_finishes_calls_nosec_test: h2_full_shutdown_finishes_calls_nosec_test.exe
	echo Running h2_full_shutdown_finishes_calls_nosec_test
	$(OUT_DIR)\h2_full_shutdown_finishes_calls_nosec_test.exe

h2_full_shutdown_finishes_tags_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_tags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_shutdown_finishes_tags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_shutdown_finishes_tags_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_shutdown_finishes_tags_nosec_test: h2_full_shutdown_finishes_tags_nosec_test.exe
	echo Running h2_full_shutdown_finishes_tags_nosec_test
	$(OUT_DIR)\h2_full_shutdown_finishes_tags_nosec_test.exe

h2_full_simple_delayed_request_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_delayed_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_simple_delayed_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_simple_delayed_request_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_delayed_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_simple_delayed_request_nosec_test: h2_full_simple_delayed_request_nosec_test.exe
	echo Running h2_full_simple_delayed_request_nosec_test
	$(OUT_DIR)\h2_full_simple_delayed_request_nosec_test.exe

h2_full_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_simple_request_nosec_test: h2_full_simple_request_nosec_test.exe
	echo Running h2_full_simple_request_nosec_test
	$(OUT_DIR)\h2_full_simple_request_nosec_test.exe

h2_full_trailing_metadata_nosec_test.exe: Debug\end2end_fixture_h2_full.lib Debug\end2end_test_trailing_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_full_trailing_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_full_trailing_metadata_nosec_test.exe" Debug\end2end_fixture_h2_full.lib Debug\end2end_test_trailing_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_full_trailing_metadata_nosec_test: h2_full_trailing_metadata_nosec_test.exe
	echo Running h2_full_trailing_metadata_nosec_test
	$(OUT_DIR)\h2_full_trailing_metadata_nosec_test.exe

h2_proxy_bad_hostname_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_bad_hostname.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_bad_hostname_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_bad_hostname_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_bad_hostname.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_bad_hostname_nosec_test: h2_proxy_bad_hostname_nosec_test.exe
	echo Running h2_proxy_bad_hostname_nosec_test
	$(OUT_DIR)\h2_proxy_bad_hostname_nosec_test.exe

h2_proxy_binary_metadata_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_binary_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_binary_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_binary_metadata_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_binary_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_binary_metadata_nosec_test: h2_proxy_binary_metadata_nosec_test.exe
	echo Running h2_proxy_binary_metadata_nosec_test
	$(OUT_DIR)\h2_proxy_binary_metadata_nosec_test.exe

h2_proxy_cancel_after_accept_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_accept.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_after_accept_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_after_accept_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_accept.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_after_accept_nosec_test: h2_proxy_cancel_after_accept_nosec_test.exe
	echo Running h2_proxy_cancel_after_accept_nosec_test
	$(OUT_DIR)\h2_proxy_cancel_after_accept_nosec_test.exe

h2_proxy_cancel_after_client_done_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_client_done.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_after_client_done_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_after_client_done_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_client_done.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_after_client_done_nosec_test: h2_proxy_cancel_after_client_done_nosec_test.exe
	echo Running h2_proxy_cancel_after_client_done_nosec_test
	$(OUT_DIR)\h2_proxy_cancel_after_client_done_nosec_test.exe

h2_proxy_cancel_after_invoke_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_after_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_after_invoke_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_after_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_after_invoke_nosec_test: h2_proxy_cancel_after_invoke_nosec_test.exe
	echo Running h2_proxy_cancel_after_invoke_nosec_test
	$(OUT_DIR)\h2_proxy_cancel_after_invoke_nosec_test.exe

h2_proxy_cancel_before_invoke_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_before_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_before_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_before_invoke_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_before_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_before_invoke_nosec_test: h2_proxy_cancel_before_invoke_nosec_test.exe
	echo Running h2_proxy_cancel_before_invoke_nosec_test
	$(OUT_DIR)\h2_proxy_cancel_before_invoke_nosec_test.exe

h2_proxy_cancel_in_a_vacuum_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_in_a_vacuum.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_cancel_in_a_vacuum_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_cancel_in_a_vacuum_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_cancel_in_a_vacuum_nosec_test: h2_proxy_cancel_in_a_vacuum_nosec_test.exe
	echo Running h2_proxy_cancel_in_a_vacuum_nosec_test
	$(OUT_DIR)\h2_proxy_cancel_in_a_vacuum_nosec_test.exe

h2_proxy_census_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_census_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_census_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_census_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_census_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_census_simple_request_nosec_test: h2_proxy_census_simple_request_nosec_test.exe
	echo Running h2_proxy_census_simple_request_nosec_test
	$(OUT_DIR)\h2_proxy_census_simple_request_nosec_test.exe

h2_proxy_default_host_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_default_host.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_default_host_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_default_host_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_default_host.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_default_host_nosec_test: h2_proxy_default_host_nosec_test.exe
	echo Running h2_proxy_default_host_nosec_test
	$(OUT_DIR)\h2_proxy_default_host_nosec_test.exe

h2_proxy_disappearing_server_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_disappearing_server.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_disappearing_server_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_disappearing_server_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_disappearing_server.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_disappearing_server_nosec_test: h2_proxy_disappearing_server_nosec_test.exe
	echo Running h2_proxy_disappearing_server_nosec_test
	$(OUT_DIR)\h2_proxy_disappearing_server_nosec_test.exe

h2_proxy_empty_batch_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_empty_batch.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_empty_batch_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_empty_batch_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_empty_batch.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_empty_batch_nosec_test: h2_proxy_empty_batch_nosec_test.exe
	echo Running h2_proxy_empty_batch_nosec_test
	$(OUT_DIR)\h2_proxy_empty_batch_nosec_test.exe

h2_proxy_graceful_server_shutdown_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_graceful_server_shutdown.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_graceful_server_shutdown_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_graceful_server_shutdown_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_graceful_server_shutdown_nosec_test: h2_proxy_graceful_server_shutdown_nosec_test.exe
	echo Running h2_proxy_graceful_server_shutdown_nosec_test
	$(OUT_DIR)\h2_proxy_graceful_server_shutdown_nosec_test.exe

h2_proxy_high_initial_seqno_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_high_initial_seqno.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_high_initial_seqno_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_high_initial_seqno_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_high_initial_seqno.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_high_initial_seqno_nosec_test: h2_proxy_high_initial_seqno_nosec_test.exe
	echo Running h2_proxy_high_initial_seqno_nosec_test
	$(OUT_DIR)\h2_proxy_high_initial_seqno_nosec_test.exe

h2_proxy_invoke_large_request_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_invoke_large_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_invoke_large_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_invoke_large_request_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_invoke_large_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_invoke_large_request_nosec_test: h2_proxy_invoke_large_request_nosec_test.exe
	echo Running h2_proxy_invoke_large_request_nosec_test
	$(OUT_DIR)\h2_proxy_invoke_large_request_nosec_test.exe

h2_proxy_large_metadata_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_large_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_large_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_large_metadata_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_large_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_large_metadata_nosec_test: h2_proxy_large_metadata_nosec_test.exe
	echo Running h2_proxy_large_metadata_nosec_test
	$(OUT_DIR)\h2_proxy_large_metadata_nosec_test.exe

h2_proxy_max_message_length_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_max_message_length.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_max_message_length_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_max_message_length_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_max_message_length.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_max_message_length_nosec_test: h2_proxy_max_message_length_nosec_test.exe
	echo Running h2_proxy_max_message_length_nosec_test
	$(OUT_DIR)\h2_proxy_max_message_length_nosec_test.exe

h2_proxy_metadata_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_metadata_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_metadata_nosec_test: h2_proxy_metadata_nosec_test.exe
	echo Running h2_proxy_metadata_nosec_test
	$(OUT_DIR)\h2_proxy_metadata_nosec_test.exe

h2_proxy_no_op_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_no_op.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_no_op_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_no_op_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_no_op.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_no_op_nosec_test: h2_proxy_no_op_nosec_test.exe
	echo Running h2_proxy_no_op_nosec_test
	$(OUT_DIR)\h2_proxy_no_op_nosec_test.exe

h2_proxy_payload_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_payload_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_payload_nosec_test: h2_proxy_payload_nosec_test.exe
	echo Running h2_proxy_payload_nosec_test
	$(OUT_DIR)\h2_proxy_payload_nosec_test.exe

h2_proxy_ping_pong_streaming_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_ping_pong_streaming.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_ping_pong_streaming_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_ping_pong_streaming_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_ping_pong_streaming.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_ping_pong_streaming_nosec_test: h2_proxy_ping_pong_streaming_nosec_test.exe
	echo Running h2_proxy_ping_pong_streaming_nosec_test
	$(OUT_DIR)\h2_proxy_ping_pong_streaming_nosec_test.exe

h2_proxy_registered_call_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_registered_call.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_registered_call_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_registered_call_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_registered_call.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_registered_call_nosec_test: h2_proxy_registered_call_nosec_test.exe
	echo Running h2_proxy_registered_call_nosec_test
	$(OUT_DIR)\h2_proxy_registered_call_nosec_test.exe

h2_proxy_request_with_payload_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_request_with_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_request_with_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_request_with_payload_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_request_with_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_request_with_payload_nosec_test: h2_proxy_request_with_payload_nosec_test.exe
	echo Running h2_proxy_request_with_payload_nosec_test
	$(OUT_DIR)\h2_proxy_request_with_payload_nosec_test.exe

h2_proxy_server_finishes_request_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_server_finishes_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_server_finishes_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_server_finishes_request_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_server_finishes_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_server_finishes_request_nosec_test: h2_proxy_server_finishes_request_nosec_test.exe
	echo Running h2_proxy_server_finishes_request_nosec_test
	$(OUT_DIR)\h2_proxy_server_finishes_request_nosec_test.exe

h2_proxy_shutdown_finishes_calls_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_calls.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_shutdown_finishes_calls_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_shutdown_finishes_calls_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_shutdown_finishes_calls_nosec_test: h2_proxy_shutdown_finishes_calls_nosec_test.exe
	echo Running h2_proxy_shutdown_finishes_calls_nosec_test
	$(OUT_DIR)\h2_proxy_shutdown_finishes_calls_nosec_test.exe

h2_proxy_shutdown_finishes_tags_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_tags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_shutdown_finishes_tags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_shutdown_finishes_tags_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_shutdown_finishes_tags_nosec_test: h2_proxy_shutdown_finishes_tags_nosec_test.exe
	echo Running h2_proxy_shutdown_finishes_tags_nosec_test
	$(OUT_DIR)\h2_proxy_shutdown_finishes_tags_nosec_test.exe

h2_proxy_simple_delayed_request_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_delayed_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_simple_delayed_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_simple_delayed_request_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_delayed_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_simple_delayed_request_nosec_test: h2_proxy_simple_delayed_request_nosec_test.exe
	echo Running h2_proxy_simple_delayed_request_nosec_test
	$(OUT_DIR)\h2_proxy_simple_delayed_request_nosec_test.exe

h2_proxy_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_simple_request_nosec_test: h2_proxy_simple_request_nosec_test.exe
	echo Running h2_proxy_simple_request_nosec_test
	$(OUT_DIR)\h2_proxy_simple_request_nosec_test.exe

h2_proxy_trailing_metadata_nosec_test.exe: Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_trailing_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_proxy_trailing_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_proxy_trailing_metadata_nosec_test.exe" Debug\end2end_fixture_h2_proxy.lib Debug\end2end_test_trailing_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_proxy_trailing_metadata_nosec_test: h2_proxy_trailing_metadata_nosec_test.exe
	echo Running h2_proxy_trailing_metadata_nosec_test
	$(OUT_DIR)\h2_proxy_trailing_metadata_nosec_test.exe

h2_sockpair_bad_hostname_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_bad_hostname.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_bad_hostname_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_bad_hostname_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_bad_hostname.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_bad_hostname_nosec_test: h2_sockpair_bad_hostname_nosec_test.exe
	echo Running h2_sockpair_bad_hostname_nosec_test
	$(OUT_DIR)\h2_sockpair_bad_hostname_nosec_test.exe

h2_sockpair_binary_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_binary_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_binary_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_binary_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_binary_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_binary_metadata_nosec_test: h2_sockpair_binary_metadata_nosec_test.exe
	echo Running h2_sockpair_binary_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_binary_metadata_nosec_test.exe

h2_sockpair_cancel_after_accept_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_accept.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_after_accept_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_after_accept_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_accept.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_after_accept_nosec_test: h2_sockpair_cancel_after_accept_nosec_test.exe
	echo Running h2_sockpair_cancel_after_accept_nosec_test
	$(OUT_DIR)\h2_sockpair_cancel_after_accept_nosec_test.exe

h2_sockpair_cancel_after_client_done_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_client_done.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_after_client_done_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_after_client_done_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_client_done.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_after_client_done_nosec_test: h2_sockpair_cancel_after_client_done_nosec_test.exe
	echo Running h2_sockpair_cancel_after_client_done_nosec_test
	$(OUT_DIR)\h2_sockpair_cancel_after_client_done_nosec_test.exe

h2_sockpair_cancel_after_invoke_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_after_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_after_invoke_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_after_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_after_invoke_nosec_test: h2_sockpair_cancel_after_invoke_nosec_test.exe
	echo Running h2_sockpair_cancel_after_invoke_nosec_test
	$(OUT_DIR)\h2_sockpair_cancel_after_invoke_nosec_test.exe

h2_sockpair_cancel_before_invoke_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_before_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_before_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_before_invoke_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_before_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_before_invoke_nosec_test: h2_sockpair_cancel_before_invoke_nosec_test.exe
	echo Running h2_sockpair_cancel_before_invoke_nosec_test
	$(OUT_DIR)\h2_sockpair_cancel_before_invoke_nosec_test.exe

h2_sockpair_cancel_in_a_vacuum_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_in_a_vacuum.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_cancel_in_a_vacuum_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_cancel_in_a_vacuum_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_cancel_in_a_vacuum_nosec_test: h2_sockpair_cancel_in_a_vacuum_nosec_test.exe
	echo Running h2_sockpair_cancel_in_a_vacuum_nosec_test
	$(OUT_DIR)\h2_sockpair_cancel_in_a_vacuum_nosec_test.exe

h2_sockpair_census_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_census_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_census_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_census_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_census_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_census_simple_request_nosec_test: h2_sockpair_census_simple_request_nosec_test.exe
	echo Running h2_sockpair_census_simple_request_nosec_test
	$(OUT_DIR)\h2_sockpair_census_simple_request_nosec_test.exe

h2_sockpair_compressed_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_compressed_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_compressed_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_compressed_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_compressed_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_compressed_payload_nosec_test: h2_sockpair_compressed_payload_nosec_test.exe
	echo Running h2_sockpair_compressed_payload_nosec_test
	$(OUT_DIR)\h2_sockpair_compressed_payload_nosec_test.exe

h2_sockpair_empty_batch_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_empty_batch.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_empty_batch_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_empty_batch_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_empty_batch.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_empty_batch_nosec_test: h2_sockpair_empty_batch_nosec_test.exe
	echo Running h2_sockpair_empty_batch_nosec_test
	$(OUT_DIR)\h2_sockpair_empty_batch_nosec_test.exe

h2_sockpair_graceful_server_shutdown_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_graceful_server_shutdown.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_graceful_server_shutdown_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_graceful_server_shutdown_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_graceful_server_shutdown_nosec_test: h2_sockpair_graceful_server_shutdown_nosec_test.exe
	echo Running h2_sockpair_graceful_server_shutdown_nosec_test
	$(OUT_DIR)\h2_sockpair_graceful_server_shutdown_nosec_test.exe

h2_sockpair_high_initial_seqno_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_high_initial_seqno.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_high_initial_seqno_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_high_initial_seqno_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_high_initial_seqno.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_high_initial_seqno_nosec_test: h2_sockpair_high_initial_seqno_nosec_test.exe
	echo Running h2_sockpair_high_initial_seqno_nosec_test
	$(OUT_DIR)\h2_sockpair_high_initial_seqno_nosec_test.exe

h2_sockpair_invoke_large_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_invoke_large_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_invoke_large_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_invoke_large_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_invoke_large_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_invoke_large_request_nosec_test: h2_sockpair_invoke_large_request_nosec_test.exe
	echo Running h2_sockpair_invoke_large_request_nosec_test
	$(OUT_DIR)\h2_sockpair_invoke_large_request_nosec_test.exe

h2_sockpair_large_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_large_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_large_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_large_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_large_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_large_metadata_nosec_test: h2_sockpair_large_metadata_nosec_test.exe
	echo Running h2_sockpair_large_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_large_metadata_nosec_test.exe

h2_sockpair_max_concurrent_streams_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_concurrent_streams.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_max_concurrent_streams_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_max_concurrent_streams_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_concurrent_streams.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_max_concurrent_streams_nosec_test: h2_sockpair_max_concurrent_streams_nosec_test.exe
	echo Running h2_sockpair_max_concurrent_streams_nosec_test
	$(OUT_DIR)\h2_sockpair_max_concurrent_streams_nosec_test.exe

h2_sockpair_max_message_length_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_message_length.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_max_message_length_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_max_message_length_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_max_message_length.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_max_message_length_nosec_test: h2_sockpair_max_message_length_nosec_test.exe
	echo Running h2_sockpair_max_message_length_nosec_test
	$(OUT_DIR)\h2_sockpair_max_message_length_nosec_test.exe

h2_sockpair_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_metadata_nosec_test: h2_sockpair_metadata_nosec_test.exe
	echo Running h2_sockpair_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_metadata_nosec_test.exe

h2_sockpair_no_op_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_no_op.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_no_op_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_no_op_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_no_op.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_no_op_nosec_test: h2_sockpair_no_op_nosec_test.exe
	echo Running h2_sockpair_no_op_nosec_test
	$(OUT_DIR)\h2_sockpair_no_op_nosec_test.exe

h2_sockpair_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_payload_nosec_test: h2_sockpair_payload_nosec_test.exe
	echo Running h2_sockpair_payload_nosec_test
	$(OUT_DIR)\h2_sockpair_payload_nosec_test.exe

h2_sockpair_ping_pong_streaming_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_ping_pong_streaming.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_ping_pong_streaming_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_ping_pong_streaming_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_ping_pong_streaming.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_ping_pong_streaming_nosec_test: h2_sockpair_ping_pong_streaming_nosec_test.exe
	echo Running h2_sockpair_ping_pong_streaming_nosec_test
	$(OUT_DIR)\h2_sockpair_ping_pong_streaming_nosec_test.exe

h2_sockpair_registered_call_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_registered_call.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_registered_call_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_registered_call_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_registered_call.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_registered_call_nosec_test: h2_sockpair_registered_call_nosec_test.exe
	echo Running h2_sockpair_registered_call_nosec_test
	$(OUT_DIR)\h2_sockpair_registered_call_nosec_test.exe

h2_sockpair_request_with_flags_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_flags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_request_with_flags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_request_with_flags_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_flags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_request_with_flags_nosec_test: h2_sockpair_request_with_flags_nosec_test.exe
	echo Running h2_sockpair_request_with_flags_nosec_test
	$(OUT_DIR)\h2_sockpair_request_with_flags_nosec_test.exe

h2_sockpair_request_with_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_request_with_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_request_with_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_request_with_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_request_with_payload_nosec_test: h2_sockpair_request_with_payload_nosec_test.exe
	echo Running h2_sockpair_request_with_payload_nosec_test
	$(OUT_DIR)\h2_sockpair_request_with_payload_nosec_test.exe

h2_sockpair_server_finishes_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_server_finishes_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_server_finishes_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_server_finishes_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_server_finishes_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_server_finishes_request_nosec_test: h2_sockpair_server_finishes_request_nosec_test.exe
	echo Running h2_sockpair_server_finishes_request_nosec_test
	$(OUT_DIR)\h2_sockpair_server_finishes_request_nosec_test.exe

h2_sockpair_shutdown_finishes_calls_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_calls.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_shutdown_finishes_calls_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_shutdown_finishes_calls_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_shutdown_finishes_calls_nosec_test: h2_sockpair_shutdown_finishes_calls_nosec_test.exe
	echo Running h2_sockpair_shutdown_finishes_calls_nosec_test
	$(OUT_DIR)\h2_sockpair_shutdown_finishes_calls_nosec_test.exe

h2_sockpair_shutdown_finishes_tags_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_tags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_shutdown_finishes_tags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_shutdown_finishes_tags_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_shutdown_finishes_tags_nosec_test: h2_sockpair_shutdown_finishes_tags_nosec_test.exe
	echo Running h2_sockpair_shutdown_finishes_tags_nosec_test
	$(OUT_DIR)\h2_sockpair_shutdown_finishes_tags_nosec_test.exe

h2_sockpair_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_simple_request_nosec_test: h2_sockpair_simple_request_nosec_test.exe
	echo Running h2_sockpair_simple_request_nosec_test
	$(OUT_DIR)\h2_sockpair_simple_request_nosec_test.exe

h2_sockpair_trailing_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_trailing_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_trailing_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_trailing_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair.lib Debug\end2end_test_trailing_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_trailing_metadata_nosec_test: h2_sockpair_trailing_metadata_nosec_test.exe
	echo Running h2_sockpair_trailing_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_trailing_metadata_nosec_test.exe

h2_sockpair+trace_bad_hostname_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_bad_hostname.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_bad_hostname_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_bad_hostname_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_bad_hostname.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_bad_hostname_nosec_test: h2_sockpair+trace_bad_hostname_nosec_test.exe
	echo Running h2_sockpair+trace_bad_hostname_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_bad_hostname_nosec_test.exe

h2_sockpair+trace_binary_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_binary_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_binary_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_binary_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_binary_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_binary_metadata_nosec_test: h2_sockpair+trace_binary_metadata_nosec_test.exe
	echo Running h2_sockpair+trace_binary_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_binary_metadata_nosec_test.exe

h2_sockpair+trace_cancel_after_accept_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_accept.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_after_accept_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_after_accept_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_accept.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_after_accept_nosec_test: h2_sockpair+trace_cancel_after_accept_nosec_test.exe
	echo Running h2_sockpair+trace_cancel_after_accept_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_after_accept_nosec_test.exe

h2_sockpair+trace_cancel_after_client_done_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_client_done.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_after_client_done_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_after_client_done_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_client_done.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_after_client_done_nosec_test: h2_sockpair+trace_cancel_after_client_done_nosec_test.exe
	echo Running h2_sockpair+trace_cancel_after_client_done_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_after_client_done_nosec_test.exe

h2_sockpair+trace_cancel_after_invoke_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_after_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_after_invoke_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_after_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_after_invoke_nosec_test: h2_sockpair+trace_cancel_after_invoke_nosec_test.exe
	echo Running h2_sockpair+trace_cancel_after_invoke_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_after_invoke_nosec_test.exe

h2_sockpair+trace_cancel_before_invoke_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_before_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_before_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_before_invoke_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_before_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_before_invoke_nosec_test: h2_sockpair+trace_cancel_before_invoke_nosec_test.exe
	echo Running h2_sockpair+trace_cancel_before_invoke_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_before_invoke_nosec_test.exe

h2_sockpair+trace_cancel_in_a_vacuum_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_in_a_vacuum.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_cancel_in_a_vacuum_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_cancel_in_a_vacuum_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_cancel_in_a_vacuum_nosec_test: h2_sockpair+trace_cancel_in_a_vacuum_nosec_test.exe
	echo Running h2_sockpair+trace_cancel_in_a_vacuum_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_cancel_in_a_vacuum_nosec_test.exe

h2_sockpair+trace_census_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_census_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_census_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_census_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_census_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_census_simple_request_nosec_test: h2_sockpair+trace_census_simple_request_nosec_test.exe
	echo Running h2_sockpair+trace_census_simple_request_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_census_simple_request_nosec_test.exe

h2_sockpair+trace_compressed_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_compressed_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_compressed_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_compressed_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_compressed_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_compressed_payload_nosec_test: h2_sockpair+trace_compressed_payload_nosec_test.exe
	echo Running h2_sockpair+trace_compressed_payload_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_compressed_payload_nosec_test.exe

h2_sockpair+trace_empty_batch_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_empty_batch.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_empty_batch_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_empty_batch_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_empty_batch.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_empty_batch_nosec_test: h2_sockpair+trace_empty_batch_nosec_test.exe
	echo Running h2_sockpair+trace_empty_batch_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_empty_batch_nosec_test.exe

h2_sockpair+trace_graceful_server_shutdown_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_graceful_server_shutdown.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_graceful_server_shutdown_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_graceful_server_shutdown_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_graceful_server_shutdown_nosec_test: h2_sockpair+trace_graceful_server_shutdown_nosec_test.exe
	echo Running h2_sockpair+trace_graceful_server_shutdown_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_graceful_server_shutdown_nosec_test.exe

h2_sockpair+trace_high_initial_seqno_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_high_initial_seqno.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_high_initial_seqno_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_high_initial_seqno_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_high_initial_seqno.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_high_initial_seqno_nosec_test: h2_sockpair+trace_high_initial_seqno_nosec_test.exe
	echo Running h2_sockpair+trace_high_initial_seqno_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_high_initial_seqno_nosec_test.exe

h2_sockpair+trace_invoke_large_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_invoke_large_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_invoke_large_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_invoke_large_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_invoke_large_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_invoke_large_request_nosec_test: h2_sockpair+trace_invoke_large_request_nosec_test.exe
	echo Running h2_sockpair+trace_invoke_large_request_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_invoke_large_request_nosec_test.exe

h2_sockpair+trace_large_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_large_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_large_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_large_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_large_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_large_metadata_nosec_test: h2_sockpair+trace_large_metadata_nosec_test.exe
	echo Running h2_sockpair+trace_large_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_large_metadata_nosec_test.exe

h2_sockpair+trace_max_concurrent_streams_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_concurrent_streams.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_max_concurrent_streams_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_max_concurrent_streams_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_concurrent_streams.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_max_concurrent_streams_nosec_test: h2_sockpair+trace_max_concurrent_streams_nosec_test.exe
	echo Running h2_sockpair+trace_max_concurrent_streams_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_max_concurrent_streams_nosec_test.exe

h2_sockpair+trace_max_message_length_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_message_length.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_max_message_length_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_max_message_length_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_max_message_length.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_max_message_length_nosec_test: h2_sockpair+trace_max_message_length_nosec_test.exe
	echo Running h2_sockpair+trace_max_message_length_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_max_message_length_nosec_test.exe

h2_sockpair+trace_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_metadata_nosec_test: h2_sockpair+trace_metadata_nosec_test.exe
	echo Running h2_sockpair+trace_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_metadata_nosec_test.exe

h2_sockpair+trace_no_op_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_no_op.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_no_op_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_no_op_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_no_op.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_no_op_nosec_test: h2_sockpair+trace_no_op_nosec_test.exe
	echo Running h2_sockpair+trace_no_op_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_no_op_nosec_test.exe

h2_sockpair+trace_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_payload_nosec_test: h2_sockpair+trace_payload_nosec_test.exe
	echo Running h2_sockpair+trace_payload_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_payload_nosec_test.exe

h2_sockpair+trace_ping_pong_streaming_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_ping_pong_streaming.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_ping_pong_streaming_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_ping_pong_streaming_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_ping_pong_streaming.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_ping_pong_streaming_nosec_test: h2_sockpair+trace_ping_pong_streaming_nosec_test.exe
	echo Running h2_sockpair+trace_ping_pong_streaming_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_ping_pong_streaming_nosec_test.exe

h2_sockpair+trace_registered_call_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_registered_call.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_registered_call_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_registered_call_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_registered_call.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_registered_call_nosec_test: h2_sockpair+trace_registered_call_nosec_test.exe
	echo Running h2_sockpair+trace_registered_call_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_registered_call_nosec_test.exe

h2_sockpair+trace_request_with_flags_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_flags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_request_with_flags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_request_with_flags_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_flags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_request_with_flags_nosec_test: h2_sockpair+trace_request_with_flags_nosec_test.exe
	echo Running h2_sockpair+trace_request_with_flags_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_request_with_flags_nosec_test.exe

h2_sockpair+trace_request_with_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_request_with_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_request_with_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_request_with_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_request_with_payload_nosec_test: h2_sockpair+trace_request_with_payload_nosec_test.exe
	echo Running h2_sockpair+trace_request_with_payload_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_request_with_payload_nosec_test.exe

h2_sockpair+trace_server_finishes_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_server_finishes_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_server_finishes_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_server_finishes_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_server_finishes_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_server_finishes_request_nosec_test: h2_sockpair+trace_server_finishes_request_nosec_test.exe
	echo Running h2_sockpair+trace_server_finishes_request_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_server_finishes_request_nosec_test.exe

h2_sockpair+trace_shutdown_finishes_calls_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_calls.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_shutdown_finishes_calls_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_calls_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_shutdown_finishes_calls_nosec_test: h2_sockpair+trace_shutdown_finishes_calls_nosec_test.exe
	echo Running h2_sockpair+trace_shutdown_finishes_calls_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_calls_nosec_test.exe

h2_sockpair+trace_shutdown_finishes_tags_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_tags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_shutdown_finishes_tags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_tags_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_shutdown_finishes_tags_nosec_test: h2_sockpair+trace_shutdown_finishes_tags_nosec_test.exe
	echo Running h2_sockpair+trace_shutdown_finishes_tags_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_shutdown_finishes_tags_nosec_test.exe

h2_sockpair+trace_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_simple_request_nosec_test: h2_sockpair+trace_simple_request_nosec_test.exe
	echo Running h2_sockpair+trace_simple_request_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_simple_request_nosec_test.exe

h2_sockpair+trace_trailing_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_trailing_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair+trace_trailing_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair+trace_trailing_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair+trace.lib Debug\end2end_test_trailing_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair+trace_trailing_metadata_nosec_test: h2_sockpair+trace_trailing_metadata_nosec_test.exe
	echo Running h2_sockpair+trace_trailing_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair+trace_trailing_metadata_nosec_test.exe

h2_sockpair_1byte_bad_hostname_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_bad_hostname.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_bad_hostname_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_bad_hostname_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_bad_hostname.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_bad_hostname_nosec_test: h2_sockpair_1byte_bad_hostname_nosec_test.exe
	echo Running h2_sockpair_1byte_bad_hostname_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_bad_hostname_nosec_test.exe

h2_sockpair_1byte_binary_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_binary_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_binary_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_binary_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_binary_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_binary_metadata_nosec_test: h2_sockpair_1byte_binary_metadata_nosec_test.exe
	echo Running h2_sockpair_1byte_binary_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_binary_metadata_nosec_test.exe

h2_sockpair_1byte_cancel_after_accept_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_accept.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_after_accept_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_after_accept_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_accept.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_after_accept_nosec_test: h2_sockpair_1byte_cancel_after_accept_nosec_test.exe
	echo Running h2_sockpair_1byte_cancel_after_accept_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_after_accept_nosec_test.exe

h2_sockpair_1byte_cancel_after_client_done_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_client_done.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_after_client_done_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_after_client_done_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_client_done.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_after_client_done_nosec_test: h2_sockpair_1byte_cancel_after_client_done_nosec_test.exe
	echo Running h2_sockpair_1byte_cancel_after_client_done_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_after_client_done_nosec_test.exe

h2_sockpair_1byte_cancel_after_invoke_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_after_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_after_invoke_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_after_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_after_invoke_nosec_test: h2_sockpair_1byte_cancel_after_invoke_nosec_test.exe
	echo Running h2_sockpair_1byte_cancel_after_invoke_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_after_invoke_nosec_test.exe

h2_sockpair_1byte_cancel_before_invoke_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_before_invoke.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_before_invoke_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_before_invoke_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_before_invoke.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_before_invoke_nosec_test: h2_sockpair_1byte_cancel_before_invoke_nosec_test.exe
	echo Running h2_sockpair_1byte_cancel_before_invoke_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_before_invoke_nosec_test.exe

h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_in_a_vacuum.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_cancel_in_a_vacuum.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test: h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test.exe
	echo Running h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_cancel_in_a_vacuum_nosec_test.exe

h2_sockpair_1byte_census_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_census_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_census_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_census_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_census_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_census_simple_request_nosec_test: h2_sockpair_1byte_census_simple_request_nosec_test.exe
	echo Running h2_sockpair_1byte_census_simple_request_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_census_simple_request_nosec_test.exe

h2_sockpair_1byte_compressed_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_compressed_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_compressed_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_compressed_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_compressed_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_compressed_payload_nosec_test: h2_sockpair_1byte_compressed_payload_nosec_test.exe
	echo Running h2_sockpair_1byte_compressed_payload_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_compressed_payload_nosec_test.exe

h2_sockpair_1byte_empty_batch_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_empty_batch.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_empty_batch_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_empty_batch_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_empty_batch.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_empty_batch_nosec_test: h2_sockpair_1byte_empty_batch_nosec_test.exe
	echo Running h2_sockpair_1byte_empty_batch_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_empty_batch_nosec_test.exe

h2_sockpair_1byte_graceful_server_shutdown_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_graceful_server_shutdown.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_graceful_server_shutdown_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_graceful_server_shutdown_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_graceful_server_shutdown.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_graceful_server_shutdown_nosec_test: h2_sockpair_1byte_graceful_server_shutdown_nosec_test.exe
	echo Running h2_sockpair_1byte_graceful_server_shutdown_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_graceful_server_shutdown_nosec_test.exe

h2_sockpair_1byte_high_initial_seqno_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_high_initial_seqno.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_high_initial_seqno_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_high_initial_seqno_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_high_initial_seqno.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_high_initial_seqno_nosec_test: h2_sockpair_1byte_high_initial_seqno_nosec_test.exe
	echo Running h2_sockpair_1byte_high_initial_seqno_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_high_initial_seqno_nosec_test.exe

h2_sockpair_1byte_invoke_large_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_invoke_large_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_invoke_large_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_invoke_large_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_invoke_large_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_invoke_large_request_nosec_test: h2_sockpair_1byte_invoke_large_request_nosec_test.exe
	echo Running h2_sockpair_1byte_invoke_large_request_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_invoke_large_request_nosec_test.exe

h2_sockpair_1byte_large_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_large_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_large_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_large_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_large_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_large_metadata_nosec_test: h2_sockpair_1byte_large_metadata_nosec_test.exe
	echo Running h2_sockpair_1byte_large_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_large_metadata_nosec_test.exe

h2_sockpair_1byte_max_concurrent_streams_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_concurrent_streams.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_max_concurrent_streams_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_max_concurrent_streams_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_concurrent_streams.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_max_concurrent_streams_nosec_test: h2_sockpair_1byte_max_concurrent_streams_nosec_test.exe
	echo Running h2_sockpair_1byte_max_concurrent_streams_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_max_concurrent_streams_nosec_test.exe

h2_sockpair_1byte_max_message_length_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_message_length.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_max_message_length_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_max_message_length_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_max_message_length.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_max_message_length_nosec_test: h2_sockpair_1byte_max_message_length_nosec_test.exe
	echo Running h2_sockpair_1byte_max_message_length_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_max_message_length_nosec_test.exe

h2_sockpair_1byte_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_metadata_nosec_test: h2_sockpair_1byte_metadata_nosec_test.exe
	echo Running h2_sockpair_1byte_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_metadata_nosec_test.exe

h2_sockpair_1byte_no_op_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_no_op.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_no_op_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_no_op_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_no_op.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_no_op_nosec_test: h2_sockpair_1byte_no_op_nosec_test.exe
	echo Running h2_sockpair_1byte_no_op_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_no_op_nosec_test.exe

h2_sockpair_1byte_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_payload_nosec_test: h2_sockpair_1byte_payload_nosec_test.exe
	echo Running h2_sockpair_1byte_payload_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_payload_nosec_test.exe

h2_sockpair_1byte_ping_pong_streaming_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_ping_pong_streaming.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_ping_pong_streaming_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_ping_pong_streaming_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_ping_pong_streaming.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_ping_pong_streaming_nosec_test: h2_sockpair_1byte_ping_pong_streaming_nosec_test.exe
	echo Running h2_sockpair_1byte_ping_pong_streaming_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_ping_pong_streaming_nosec_test.exe

h2_sockpair_1byte_registered_call_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_registered_call.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_registered_call_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_registered_call_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_registered_call.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_registered_call_nosec_test: h2_sockpair_1byte_registered_call_nosec_test.exe
	echo Running h2_sockpair_1byte_registered_call_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_registered_call_nosec_test.exe

h2_sockpair_1byte_request_with_flags_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_flags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_request_with_flags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_request_with_flags_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_flags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_request_with_flags_nosec_test: h2_sockpair_1byte_request_with_flags_nosec_test.exe
	echo Running h2_sockpair_1byte_request_with_flags_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_request_with_flags_nosec_test.exe

h2_sockpair_1byte_request_with_payload_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_payload.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_request_with_payload_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_request_with_payload_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_request_with_payload.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_request_with_payload_nosec_test: h2_sockpair_1byte_request_with_payload_nosec_test.exe
	echo Running h2_sockpair_1byte_request_with_payload_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_request_with_payload_nosec_test.exe

h2_sockpair_1byte_server_finishes_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_server_finishes_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_server_finishes_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_server_finishes_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_server_finishes_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_server_finishes_request_nosec_test: h2_sockpair_1byte_server_finishes_request_nosec_test.exe
	echo Running h2_sockpair_1byte_server_finishes_request_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_server_finishes_request_nosec_test.exe

h2_sockpair_1byte_shutdown_finishes_calls_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_calls.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_shutdown_finishes_calls_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_calls_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_calls.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_shutdown_finishes_calls_nosec_test: h2_sockpair_1byte_shutdown_finishes_calls_nosec_test.exe
	echo Running h2_sockpair_1byte_shutdown_finishes_calls_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_calls_nosec_test.exe

h2_sockpair_1byte_shutdown_finishes_tags_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_tags.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_shutdown_finishes_tags_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_tags_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_shutdown_finishes_tags.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_shutdown_finishes_tags_nosec_test: h2_sockpair_1byte_shutdown_finishes_tags_nosec_test.exe
	echo Running h2_sockpair_1byte_shutdown_finishes_tags_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_shutdown_finishes_tags_nosec_test.exe

h2_sockpair_1byte_simple_request_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_simple_request.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_simple_request_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_simple_request_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_simple_request.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_simple_request_nosec_test: h2_sockpair_1byte_simple_request_nosec_test.exe
	echo Running h2_sockpair_1byte_simple_request_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_simple_request_nosec_test.exe

h2_sockpair_1byte_trailing_metadata_nosec_test.exe: Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_trailing_metadata.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building h2_sockpair_1byte_trailing_metadata_nosec_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\vsprojects\dummy.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\h2_sockpair_1byte_trailing_metadata_nosec_test.exe" Debug\end2end_fixture_h2_sockpair_1byte.lib Debug\end2end_test_trailing_metadata.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\dummy.obj 
h2_sockpair_1byte_trailing_metadata_nosec_test: h2_sockpair_1byte_trailing_metadata_nosec_test.exe
	echo Running h2_sockpair_1byte_trailing_metadata_nosec_test
	$(OUT_DIR)\h2_sockpair_1byte_trailing_metadata_nosec_test.exe

connection_prefix_bad_client_test.exe: Debug\bad_client_test.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building connection_prefix_bad_client_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\bad_client\tests\connection_prefix.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\connection_prefix_bad_client_test.exe" Debug\bad_client_test.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\connection_prefix.obj 
connection_prefix_bad_client_test: connection_prefix_bad_client_test.exe
	echo Running connection_prefix_bad_client_test
	$(OUT_DIR)\connection_prefix_bad_client_test.exe

initial_settings_frame_bad_client_test.exe: Debug\bad_client_test.lib build_grpc_test_util_unsecure build_grpc_unsecure build_gpr_test_util build_gpr $(OUT_DIR)
	echo Building initial_settings_frame_bad_client_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\bad_client\tests\initial_settings_frame.c 
	$(LINK) $(LFLAGS) /OUT:"$(OUT_DIR)\initial_settings_frame_bad_client_test.exe" Debug\bad_client_test.lib Debug\grpc_test_util_unsecure.lib Debug\grpc_unsecure.lib Debug\gpr_test_util.lib Debug\gpr.lib $(LIBS) $(OUT_DIR)\initial_settings_frame.obj 
initial_settings_frame_bad_client_test: initial_settings_frame_bad_client_test.exe
	echo Running initial_settings_frame_bad_client_test
	$(OUT_DIR)\initial_settings_frame_bad_client_test.exe

build_gpr:
	msbuild grpc.sln /t:gpr /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

build_gpr_test_util:
	msbuild grpc.sln /t:gpr_test_util /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

build_grpc:
	msbuild grpc.sln /t:grpc /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

build_grpc_test_util:
	msbuild grpc.sln /t:grpc_test_util /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

build_grpc_test_util_unsecure:
	msbuild grpc.sln /t:grpc_test_util_unsecure /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

build_grpc_unsecure:
	msbuild grpc.sln /t:grpc_unsecure /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

Debug\reconnect_server.lib: $(OUT_DIR)
	echo Building reconnect_server
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\util\reconnect_server.c 
	$(LIBTOOL) /OUT:"Debug\reconnect_server.lib" $(OUT_DIR)\reconnect_server.obj 

build_grpc++:
	msbuild grpc.sln /t:grpc++ /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

Debug\grpc++_test_config.lib: $(OUT_DIR)
	echo Building grpc++_test_config
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\test_config.cc 
	$(LIBTOOL) /OUT:"Debug\grpc++_test_config.lib" $(OUT_DIR)\test_config.obj 

Debug\grpc++_test_util.lib: $(OUT_DIR)
	echo Building grpc++_test_util
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\util\cli_call.cc $(REPO_ROOT)\test\cpp\util\create_test_channel.cc $(REPO_ROOT)\test\cpp\util\string_ref_helper.cc $(REPO_ROOT)\test\cpp\util\subprocess.cc $(REPO_ROOT)\test\cpp\util\messages.pb.cc $(REPO_ROOT)\test\cpp\util\messages.grpc.pb.cc $(REPO_ROOT)\test\cpp\util\echo.pb.cc $(REPO_ROOT)\test\cpp\util\echo.grpc.pb.cc $(REPO_ROOT)\test\cpp\util\echo_duplicate.pb.cc $(REPO_ROOT)\test\cpp\util\echo_duplicate.grpc.pb.cc 
	$(LIBTOOL) /OUT:"Debug\grpc++_test_util.lib" $(OUT_DIR)\cli_call.obj $(OUT_DIR)\create_test_channel.obj $(OUT_DIR)\string_ref_helper.obj $(OUT_DIR)\subprocess.obj $(OUT_DIR)\messages.pb.obj $(OUT_DIR)\messages.grpc.pb.obj $(OUT_DIR)\echo.pb.obj $(OUT_DIR)\echo.grpc.pb.obj $(OUT_DIR)\echo_duplicate.pb.obj $(OUT_DIR)\echo_duplicate.grpc.pb.obj 

build_grpc++_unsecure:
	msbuild grpc.sln /t:grpc++_unsecure /p:Configuration=Debug /p:Linkage-grpc_dependencies_zlib=static

Debug\interop_client_helper.lib: $(OUT_DIR)
	echo Building interop_client_helper
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\interop\client_helper.cc $(REPO_ROOT)\test\proto\messages.pb.cc $(REPO_ROOT)\test\proto\messages.grpc.pb.cc 
	$(LIBTOOL) /OUT:"Debug\interop_client_helper.lib" $(OUT_DIR)\client_helper.obj $(OUT_DIR)\messages.pb.obj $(OUT_DIR)\messages.grpc.pb.obj 

Debug\interop_client_main.lib: $(OUT_DIR)
	echo Building interop_client_main
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\interop\client.cc $(REPO_ROOT)\test\cpp\interop\interop_client.cc $(REPO_ROOT)\test\proto\empty.pb.cc $(REPO_ROOT)\test\proto\empty.grpc.pb.cc $(REPO_ROOT)\test\proto\messages.pb.cc $(REPO_ROOT)\test\proto\messages.grpc.pb.cc $(REPO_ROOT)\test\proto\test.pb.cc $(REPO_ROOT)\test\proto\test.grpc.pb.cc 
	$(LIBTOOL) /OUT:"Debug\interop_client_main.lib" $(OUT_DIR)\client.obj $(OUT_DIR)\interop_client.obj $(OUT_DIR)\empty.pb.obj $(OUT_DIR)\empty.grpc.pb.obj $(OUT_DIR)\messages.pb.obj $(OUT_DIR)\messages.grpc.pb.obj $(OUT_DIR)\test.pb.obj $(OUT_DIR)\test.grpc.pb.obj 

Debug\interop_server_helper.lib: $(OUT_DIR)
	echo Building interop_server_helper
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\interop\server_helper.cc 
	$(LIBTOOL) /OUT:"Debug\interop_server_helper.lib" $(OUT_DIR)\server_helper.obj 

Debug\interop_server_main.lib: $(OUT_DIR)
	echo Building interop_server_main
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\interop\server.cc $(REPO_ROOT)\test\proto\empty.pb.cc $(REPO_ROOT)\test\proto\empty.grpc.pb.cc $(REPO_ROOT)\test\proto\messages.pb.cc $(REPO_ROOT)\test\proto\messages.grpc.pb.cc $(REPO_ROOT)\test\proto\test.pb.cc $(REPO_ROOT)\test\proto\test.grpc.pb.cc 
	$(LIBTOOL) /OUT:"Debug\interop_server_main.lib" $(OUT_DIR)\server.obj $(OUT_DIR)\empty.pb.obj $(OUT_DIR)\empty.grpc.pb.obj $(OUT_DIR)\messages.pb.obj $(OUT_DIR)\messages.grpc.pb.obj $(OUT_DIR)\test.pb.obj $(OUT_DIR)\test.grpc.pb.obj 

Debug\qps.lib: $(OUT_DIR)
	echo Building qps
    $(CC) $(CXXFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\cpp\qps\client_async.cc $(REPO_ROOT)\test\cpp\qps\client_sync.cc $(REPO_ROOT)\test\cpp\qps\driver.cc $(REPO_ROOT)\test\cpp\qps\perf_db_client.cc $(REPO_ROOT)\test\cpp\qps\qps_worker.cc $(REPO_ROOT)\test\cpp\qps\report.cc $(REPO_ROOT)\test\cpp\qps\server_async.cc $(REPO_ROOT)\test\cpp\qps\server_sync.cc $(REPO_ROOT)\test\cpp\qps\timer.cc $(REPO_ROOT)\test\cpp\util\benchmark_config.cc $(REPO_ROOT)\test\cpp\qps\qpstest.pb.cc $(REPO_ROOT)\test\cpp\qps\qpstest.grpc.pb.cc $(REPO_ROOT)\test\cpp\qps\perf_db.pb.cc $(REPO_ROOT)\test\cpp\qps\perf_db.grpc.pb.cc 
	$(LIBTOOL) /OUT:"Debug\qps.lib" $(OUT_DIR)\client_async.obj $(OUT_DIR)\client_sync.obj $(OUT_DIR)\driver.obj $(OUT_DIR)\perf_db_client.obj $(OUT_DIR)\qps_worker.obj $(OUT_DIR)\report.obj $(OUT_DIR)\server_async.obj $(OUT_DIR)\server_sync.obj $(OUT_DIR)\timer.obj $(OUT_DIR)\benchmark_config.obj $(OUT_DIR)\qpstest.pb.obj $(OUT_DIR)\qpstest.grpc.pb.obj $(OUT_DIR)\perf_db.pb.obj $(OUT_DIR)\perf_db.grpc.pb.obj 

Debug\end2end_fixture_h2_compress.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_compress
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_compress.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_compress.lib" $(OUT_DIR)\h2_compress.obj 

Debug\end2end_fixture_h2_fakesec.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_fakesec
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_fakesec.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_fakesec.lib" $(OUT_DIR)\h2_fakesec.obj 

Debug\end2end_fixture_h2_full.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_full
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_full.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_full.lib" $(OUT_DIR)\h2_full.obj 

Debug\end2end_fixture_h2_oauth2.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_oauth2
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_oauth2.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_oauth2.lib" $(OUT_DIR)\h2_oauth2.obj 

Debug\end2end_fixture_h2_proxy.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_proxy
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_proxy.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_proxy.lib" $(OUT_DIR)\h2_proxy.obj 

Debug\end2end_fixture_h2_sockpair.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_sockpair
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_sockpair.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_sockpair.lib" $(OUT_DIR)\h2_sockpair.obj 

Debug\end2end_fixture_h2_sockpair+trace.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_sockpair+trace
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_sockpair+trace.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_sockpair+trace.lib" $(OUT_DIR)\h2_sockpair+trace.obj 

Debug\end2end_fixture_h2_sockpair_1byte.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_sockpair_1byte
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_sockpair_1byte.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_sockpair_1byte.lib" $(OUT_DIR)\h2_sockpair_1byte.obj 

Debug\end2end_fixture_h2_ssl.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_ssl
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_ssl.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_ssl.lib" $(OUT_DIR)\h2_ssl.obj 

Debug\end2end_fixture_h2_ssl_proxy.lib: $(OUT_DIR)
	echo Building end2end_fixture_h2_ssl_proxy
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\fixtures\h2_ssl_proxy.c 
	$(LIBTOOL) /OUT:"Debug\end2end_fixture_h2_ssl_proxy.lib" $(OUT_DIR)\h2_ssl_proxy.obj 

Debug\end2end_test_bad_hostname.lib: $(OUT_DIR)
	echo Building end2end_test_bad_hostname
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\bad_hostname.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_bad_hostname.lib" $(OUT_DIR)\bad_hostname.obj 

Debug\end2end_test_binary_metadata.lib: $(OUT_DIR)
	echo Building end2end_test_binary_metadata
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\binary_metadata.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_binary_metadata.lib" $(OUT_DIR)\binary_metadata.obj 

Debug\end2end_test_call_creds.lib: $(OUT_DIR)
	echo Building end2end_test_call_creds
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\call_creds.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_call_creds.lib" $(OUT_DIR)\call_creds.obj 

Debug\end2end_test_cancel_after_accept.lib: $(OUT_DIR)
	echo Building end2end_test_cancel_after_accept
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\cancel_after_accept.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_cancel_after_accept.lib" $(OUT_DIR)\cancel_after_accept.obj 

Debug\end2end_test_cancel_after_client_done.lib: $(OUT_DIR)
	echo Building end2end_test_cancel_after_client_done
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\cancel_after_client_done.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_cancel_after_client_done.lib" $(OUT_DIR)\cancel_after_client_done.obj 

Debug\end2end_test_cancel_after_invoke.lib: $(OUT_DIR)
	echo Building end2end_test_cancel_after_invoke
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\cancel_after_invoke.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_cancel_after_invoke.lib" $(OUT_DIR)\cancel_after_invoke.obj 

Debug\end2end_test_cancel_before_invoke.lib: $(OUT_DIR)
	echo Building end2end_test_cancel_before_invoke
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\cancel_before_invoke.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_cancel_before_invoke.lib" $(OUT_DIR)\cancel_before_invoke.obj 

Debug\end2end_test_cancel_in_a_vacuum.lib: $(OUT_DIR)
	echo Building end2end_test_cancel_in_a_vacuum
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\cancel_in_a_vacuum.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_cancel_in_a_vacuum.lib" $(OUT_DIR)\cancel_in_a_vacuum.obj 

Debug\end2end_test_census_simple_request.lib: $(OUT_DIR)
	echo Building end2end_test_census_simple_request
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\census_simple_request.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_census_simple_request.lib" $(OUT_DIR)\census_simple_request.obj 

Debug\end2end_test_channel_connectivity.lib: $(OUT_DIR)
	echo Building end2end_test_channel_connectivity
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\channel_connectivity.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_channel_connectivity.lib" $(OUT_DIR)\channel_connectivity.obj 

Debug\end2end_test_compressed_payload.lib: $(OUT_DIR)
	echo Building end2end_test_compressed_payload
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\compressed_payload.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_compressed_payload.lib" $(OUT_DIR)\compressed_payload.obj 

Debug\end2end_test_default_host.lib: $(OUT_DIR)
	echo Building end2end_test_default_host
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\default_host.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_default_host.lib" $(OUT_DIR)\default_host.obj 

Debug\end2end_test_disappearing_server.lib: $(OUT_DIR)
	echo Building end2end_test_disappearing_server
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\disappearing_server.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_disappearing_server.lib" $(OUT_DIR)\disappearing_server.obj 

Debug\end2end_test_empty_batch.lib: $(OUT_DIR)
	echo Building end2end_test_empty_batch
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\empty_batch.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_empty_batch.lib" $(OUT_DIR)\empty_batch.obj 

Debug\end2end_test_graceful_server_shutdown.lib: $(OUT_DIR)
	echo Building end2end_test_graceful_server_shutdown
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\graceful_server_shutdown.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_graceful_server_shutdown.lib" $(OUT_DIR)\graceful_server_shutdown.obj 

Debug\end2end_test_high_initial_seqno.lib: $(OUT_DIR)
	echo Building end2end_test_high_initial_seqno
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\high_initial_seqno.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_high_initial_seqno.lib" $(OUT_DIR)\high_initial_seqno.obj 

Debug\end2end_test_invoke_large_request.lib: $(OUT_DIR)
	echo Building end2end_test_invoke_large_request
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\invoke_large_request.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_invoke_large_request.lib" $(OUT_DIR)\invoke_large_request.obj 

Debug\end2end_test_large_metadata.lib: $(OUT_DIR)
	echo Building end2end_test_large_metadata
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\large_metadata.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_large_metadata.lib" $(OUT_DIR)\large_metadata.obj 

Debug\end2end_test_max_concurrent_streams.lib: $(OUT_DIR)
	echo Building end2end_test_max_concurrent_streams
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\max_concurrent_streams.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_max_concurrent_streams.lib" $(OUT_DIR)\max_concurrent_streams.obj 

Debug\end2end_test_max_message_length.lib: $(OUT_DIR)
	echo Building end2end_test_max_message_length
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\max_message_length.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_max_message_length.lib" $(OUT_DIR)\max_message_length.obj 

Debug\end2end_test_metadata.lib: $(OUT_DIR)
	echo Building end2end_test_metadata
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\metadata.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_metadata.lib" $(OUT_DIR)\metadata.obj 

Debug\end2end_test_no_op.lib: $(OUT_DIR)
	echo Building end2end_test_no_op
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\no_op.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_no_op.lib" $(OUT_DIR)\no_op.obj 

Debug\end2end_test_payload.lib: $(OUT_DIR)
	echo Building end2end_test_payload
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\payload.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_payload.lib" $(OUT_DIR)\payload.obj 

Debug\end2end_test_ping_pong_streaming.lib: $(OUT_DIR)
	echo Building end2end_test_ping_pong_streaming
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\ping_pong_streaming.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_ping_pong_streaming.lib" $(OUT_DIR)\ping_pong_streaming.obj 

Debug\end2end_test_registered_call.lib: $(OUT_DIR)
	echo Building end2end_test_registered_call
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\registered_call.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_registered_call.lib" $(OUT_DIR)\registered_call.obj 

Debug\end2end_test_request_with_flags.lib: $(OUT_DIR)
	echo Building end2end_test_request_with_flags
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\request_with_flags.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_request_with_flags.lib" $(OUT_DIR)\request_with_flags.obj 

Debug\end2end_test_request_with_payload.lib: $(OUT_DIR)
	echo Building end2end_test_request_with_payload
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\request_with_payload.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_request_with_payload.lib" $(OUT_DIR)\request_with_payload.obj 

Debug\end2end_test_server_finishes_request.lib: $(OUT_DIR)
	echo Building end2end_test_server_finishes_request
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\server_finishes_request.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_server_finishes_request.lib" $(OUT_DIR)\server_finishes_request.obj 

Debug\end2end_test_shutdown_finishes_calls.lib: $(OUT_DIR)
	echo Building end2end_test_shutdown_finishes_calls
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\shutdown_finishes_calls.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_shutdown_finishes_calls.lib" $(OUT_DIR)\shutdown_finishes_calls.obj 

Debug\end2end_test_shutdown_finishes_tags.lib: $(OUT_DIR)
	echo Building end2end_test_shutdown_finishes_tags
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\shutdown_finishes_tags.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_shutdown_finishes_tags.lib" $(OUT_DIR)\shutdown_finishes_tags.obj 

Debug\end2end_test_simple_delayed_request.lib: $(OUT_DIR)
	echo Building end2end_test_simple_delayed_request
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\simple_delayed_request.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_simple_delayed_request.lib" $(OUT_DIR)\simple_delayed_request.obj 

Debug\end2end_test_simple_request.lib: $(OUT_DIR)
	echo Building end2end_test_simple_request
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\simple_request.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_simple_request.lib" $(OUT_DIR)\simple_request.obj 

Debug\end2end_test_trailing_metadata.lib: $(OUT_DIR)
	echo Building end2end_test_trailing_metadata
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\tests\trailing_metadata.c 
	$(LIBTOOL) /OUT:"Debug\end2end_test_trailing_metadata.lib" $(OUT_DIR)\trailing_metadata.obj 

Debug\end2end_certs.lib: $(OUT_DIR)
	echo Building end2end_certs
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\end2end\data\test_root_cert.c $(REPO_ROOT)\test\core\end2end\data\server1_cert.c $(REPO_ROOT)\test\core\end2end\data\server1_key.c 
	$(LIBTOOL) /OUT:"Debug\end2end_certs.lib" $(OUT_DIR)\test_root_cert.obj $(OUT_DIR)\server1_cert.obj $(OUT_DIR)\server1_key.obj 

Debug\bad_client_test.lib: $(OUT_DIR)
	echo Building bad_client_test
	$(CC) $(CFLAGS) /Fo:$(OUT_DIR)\ $(REPO_ROOT)\test\core\bad_client\bad_client.c 
	$(LIBTOOL) /OUT:"Debug\bad_client_test.lib" $(OUT_DIR)\bad_client.obj 
