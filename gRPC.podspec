# GRPC CocoaPods podspec
# This file has been automatically generated from a template file.
# Please look at the templates directory instead.
# This file can be regenerated from the template by running
# tools/buildgen/generate_projects.sh

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


Pod::Spec.new do |s|
  s.name     = 'gRPC'
  version = '0.12.0'
  s.version  = version
  s.summary  = 'gRPC client library for iOS/OSX'
  s.homepage = 'http://www.grpc.io'
  s.license  = 'New BSD'
  s.authors  = { 'The gRPC contributors' => 'grpc-packages@google.com' }

  s.source = { :git => 'https://github.com/grpc/grpc.git',
               :tag => "release-#{version.gsub(/\./, '_')}-objectivec-#{version}" }


  s.ios.deployment_target = '7.1'
  s.osx.deployment_target = '10.9'
  s.requires_arc = true

  objc_dir = 'src/objective-c'

  # Reactive Extensions library for iOS.
  s.subspec 'RxLibrary' do |ss|
    src_dir = "#{objc_dir}/RxLibrary"
    ss.source_files = "#{src_dir}/*.{h,m}", "#{src_dir}/**/*.{h,m}"
    ss.private_header_files = "#{src_dir}/private/*.h"
    ss.header_mappings_dir = "#{objc_dir}"
  end

  # Core cross-platform gRPC library, written in C.
  s.subspec 'C-Core' do |ss|
    ss.source_files = 'src/core/profiling/timers.h',
                      'src/core/support/block_annotate.h',
                      'src/core/support/env.h',
                      'src/core/support/file.h',
                      'src/core/support/murmur_hash.h',
                      'src/core/support/stack_lockfree.h',
                      'src/core/support/string.h',
                      'src/core/support/string_win32.h',
                      'src/core/support/thd_internal.h',
                      'src/core/support/time_precise.h',
                      'include/grpc/support/alloc.h',
                      'include/grpc/support/atm.h',
                      'include/grpc/support/atm_gcc_atomic.h',
                      'include/grpc/support/atm_gcc_sync.h',
                      'include/grpc/support/atm_win32.h',
                      'include/grpc/support/avl.h',
                      'include/grpc/support/cmdline.h',
                      'include/grpc/support/cpu.h',
                      'include/grpc/support/histogram.h',
                      'include/grpc/support/host_port.h',
                      'include/grpc/support/log.h',
                      'include/grpc/support/log_win32.h',
                      'include/grpc/support/port_platform.h',
                      'include/grpc/support/slice.h',
                      'include/grpc/support/slice_buffer.h',
                      'include/grpc/support/string_util.h',
                      'include/grpc/support/subprocess.h',
                      'include/grpc/support/sync.h',
                      'include/grpc/support/sync_generic.h',
                      'include/grpc/support/sync_posix.h',
                      'include/grpc/support/sync_win32.h',
                      'include/grpc/support/thd.h',
                      'include/grpc/support/time.h',
                      'include/grpc/support/tls.h',
                      'include/grpc/support/tls_gcc.h',
                      'include/grpc/support/tls_msvc.h',
                      'include/grpc/support/tls_pthread.h',
                      'include/grpc/support/useful.h',
                      'include/grpc/impl/codegen/alloc.h',
                      'include/grpc/impl/codegen/atm.h',
                      'include/grpc/impl/codegen/atm_gcc_atomic.h',
                      'include/grpc/impl/codegen/atm_gcc_sync.h',
                      'include/grpc/impl/codegen/atm_win32.h',
                      'include/grpc/impl/codegen/byte_buffer.h',
                      'include/grpc/impl/codegen/compression_types.h',
                      'include/grpc/impl/codegen/connectivity_state.h',
                      'include/grpc/impl/codegen/grpc_types.h',
                      'include/grpc/impl/codegen/log.h',
                      'include/grpc/impl/codegen/port_platform.h',
                      'include/grpc/impl/codegen/propagation_bits.h',
                      'include/grpc/impl/codegen/slice.h',
                      'include/grpc/impl/codegen/slice_buffer.h',
                      'include/grpc/impl/codegen/status.h',
                      'include/grpc/impl/codegen/sync.h',
                      'include/grpc/impl/codegen/sync_generic.h',
                      'include/grpc/impl/codegen/sync_posix.h',
                      'include/grpc/impl/codegen/sync_win32.h',
                      'include/grpc/impl/codegen/time.h',
                      'src/core/profiling/basic_timers.c',
                      'src/core/profiling/stap_timers.c',
                      'src/core/support/alloc.c',
                      'src/core/support/avl.c',
                      'src/core/support/cmdline.c',
                      'src/core/support/cpu_iphone.c',
                      'src/core/support/cpu_linux.c',
                      'src/core/support/cpu_posix.c',
                      'src/core/support/cpu_windows.c',
                      'src/core/support/env_linux.c',
                      'src/core/support/env_posix.c',
                      'src/core/support/env_win32.c',
                      'src/core/support/file.c',
                      'src/core/support/file_posix.c',
                      'src/core/support/file_win32.c',
                      'src/core/support/histogram.c',
                      'src/core/support/host_port.c',
                      'src/core/support/log.c',
                      'src/core/support/log_android.c',
                      'src/core/support/log_linux.c',
                      'src/core/support/log_posix.c',
                      'src/core/support/log_win32.c',
                      'src/core/support/murmur_hash.c',
                      'src/core/support/slice.c',
                      'src/core/support/slice_buffer.c',
                      'src/core/support/stack_lockfree.c',
                      'src/core/support/string.c',
                      'src/core/support/string_posix.c',
                      'src/core/support/string_win32.c',
                      'src/core/support/subprocess_posix.c',
                      'src/core/support/sync.c',
                      'src/core/support/sync_posix.c',
                      'src/core/support/sync_win32.c',
                      'src/core/support/thd.c',
                      'src/core/support/thd_posix.c',
                      'src/core/support/thd_win32.c',
                      'src/core/support/time.c',
                      'src/core/support/time_posix.c',
                      'src/core/support/time_precise.c',
                      'src/core/support/time_win32.c',
                      'src/core/support/tls_pthread.c',
                      'src/core/security/auth_filters.h',
                      'src/core/security/base64.h',
                      'src/core/security/credentials.h',
                      'src/core/security/handshake.h',
                      'src/core/security/json_token.h',
                      'src/core/security/jwt_verifier.h',
                      'src/core/security/secure_endpoint.h',
                      'src/core/security/security_connector.h',
                      'src/core/security/security_context.h',
                      'src/core/tsi/fake_transport_security.h',
                      'src/core/tsi/ssl_transport_security.h',
                      'src/core/tsi/ssl_types.h',
                      'src/core/tsi/transport_security.h',
                      'src/core/tsi/transport_security_interface.h',
                      'src/core/census/grpc_filter.h',
                      'src/core/channel/channel_args.h',
                      'src/core/channel/channel_stack.h',
                      'src/core/channel/client_channel.h',
                      'src/core/channel/client_uchannel.h',
                      'src/core/channel/compress_filter.h',
                      'src/core/channel/connected_channel.h',
                      'src/core/channel/context.h',
                      'src/core/channel/http_client_filter.h',
                      'src/core/channel/http_server_filter.h',
                      'src/core/channel/subchannel_call_holder.h',
                      'src/core/client_config/client_config.h',
                      'src/core/client_config/connector.h',
                      'src/core/client_config/initial_connect_string.h',
                      'src/core/client_config/lb_policies/pick_first.h',
                      'src/core/client_config/lb_policies/round_robin.h',
                      'src/core/client_config/lb_policy.h',
                      'src/core/client_config/lb_policy_factory.h',
                      'src/core/client_config/lb_policy_registry.h',
                      'src/core/client_config/resolver.h',
                      'src/core/client_config/resolver_factory.h',
                      'src/core/client_config/resolver_registry.h',
                      'src/core/client_config/resolvers/dns_resolver.h',
                      'src/core/client_config/resolvers/sockaddr_resolver.h',
                      'src/core/client_config/subchannel.h',
                      'src/core/client_config/subchannel_factory.h',
                      'src/core/client_config/uri_parser.h',
                      'src/core/compression/algorithm_metadata.h',
                      'src/core/compression/message_compress.h',
                      'src/core/debug/trace.h',
                      'src/core/httpcli/format_request.h',
                      'src/core/httpcli/httpcli.h',
                      'src/core/httpcli/parser.h',
                      'src/core/iomgr/closure.h',
                      'src/core/iomgr/endpoint.h',
                      'src/core/iomgr/endpoint_pair.h',
                      'src/core/iomgr/exec_ctx.h',
                      'src/core/iomgr/executor.h',
                      'src/core/iomgr/fd_posix.h',
                      'src/core/iomgr/iocp_windows.h',
                      'src/core/iomgr/iomgr.h',
                      'src/core/iomgr/iomgr_internal.h',
                      'src/core/iomgr/iomgr_posix.h',
                      'src/core/iomgr/pollset.h',
                      'src/core/iomgr/pollset_posix.h',
                      'src/core/iomgr/pollset_set.h',
                      'src/core/iomgr/pollset_set_posix.h',
                      'src/core/iomgr/pollset_set_windows.h',
                      'src/core/iomgr/pollset_windows.h',
                      'src/core/iomgr/resolve_address.h',
                      'src/core/iomgr/sockaddr.h',
                      'src/core/iomgr/sockaddr_posix.h',
                      'src/core/iomgr/sockaddr_utils.h',
                      'src/core/iomgr/sockaddr_win32.h',
                      'src/core/iomgr/socket_utils_posix.h',
                      'src/core/iomgr/socket_windows.h',
                      'src/core/iomgr/tcp_client.h',
                      'src/core/iomgr/tcp_posix.h',
                      'src/core/iomgr/tcp_server.h',
                      'src/core/iomgr/tcp_windows.h',
                      'src/core/iomgr/time_averaged_stats.h',
                      'src/core/iomgr/timer.h',
                      'src/core/iomgr/timer_heap.h',
                      'src/core/iomgr/timer_internal.h',
                      'src/core/iomgr/udp_server.h',
                      'src/core/iomgr/wakeup_fd_pipe.h',
                      'src/core/iomgr/wakeup_fd_posix.h',
                      'src/core/iomgr/workqueue.h',
                      'src/core/iomgr/workqueue_posix.h',
                      'src/core/iomgr/workqueue_windows.h',
                      'src/core/json/json.h',
                      'src/core/json/json_common.h',
                      'src/core/json/json_reader.h',
                      'src/core/json/json_writer.h',
                      'src/core/statistics/census_interface.h',
                      'src/core/statistics/census_rpc_stats.h',
                      'src/core/surface/api_trace.h',
                      'src/core/surface/call.h',
                      'src/core/surface/call_test_only.h',
                      'src/core/surface/channel.h',
                      'src/core/surface/completion_queue.h',
                      'src/core/surface/event_string.h',
                      'src/core/surface/init.h',
                      'src/core/surface/server.h',
                      'src/core/surface/surface_trace.h',
                      'src/core/transport/byte_stream.h',
                      'src/core/transport/chttp2/alpn.h',
                      'src/core/transport/chttp2/bin_encoder.h',
                      'src/core/transport/chttp2/frame.h',
                      'src/core/transport/chttp2/frame_data.h',
                      'src/core/transport/chttp2/frame_goaway.h',
                      'src/core/transport/chttp2/frame_ping.h',
                      'src/core/transport/chttp2/frame_rst_stream.h',
                      'src/core/transport/chttp2/frame_settings.h',
                      'src/core/transport/chttp2/frame_window_update.h',
                      'src/core/transport/chttp2/hpack_encoder.h',
                      'src/core/transport/chttp2/hpack_parser.h',
                      'src/core/transport/chttp2/hpack_table.h',
                      'src/core/transport/chttp2/http2_errors.h',
                      'src/core/transport/chttp2/huffsyms.h',
                      'src/core/transport/chttp2/incoming_metadata.h',
                      'src/core/transport/chttp2/internal.h',
                      'src/core/transport/chttp2/status_conversion.h',
                      'src/core/transport/chttp2/stream_map.h',
                      'src/core/transport/chttp2/timeout_encoding.h',
                      'src/core/transport/chttp2/varint.h',
                      'src/core/transport/chttp2_transport.h',
                      'src/core/transport/connectivity_state.h',
                      'src/core/transport/metadata.h',
                      'src/core/transport/metadata_batch.h',
                      'src/core/transport/static_metadata.h',
                      'src/core/transport/transport.h',
                      'src/core/transport/transport_impl.h',
                      'src/core/census/aggregation.h',
                      'src/core/census/rpc_metric_id.h',
                      'include/grpc/grpc_security.h',
                      'include/grpc/byte_buffer.h',
                      'include/grpc/byte_buffer_reader.h',
                      'include/grpc/compression.h',
                      'include/grpc/grpc.h',
                      'include/grpc/status.h',
                      'include/grpc/census.h',
                      'src/core/httpcli/httpcli_security_connector.c',
                      'src/core/security/base64.c',
                      'src/core/security/client_auth_filter.c',
                      'src/core/security/credentials.c',
                      'src/core/security/credentials_metadata.c',
                      'src/core/security/credentials_posix.c',
                      'src/core/security/credentials_win32.c',
                      'src/core/security/google_default_credentials.c',
                      'src/core/security/handshake.c',
                      'src/core/security/json_token.c',
                      'src/core/security/jwt_verifier.c',
                      'src/core/security/secure_endpoint.c',
                      'src/core/security/security_connector.c',
                      'src/core/security/security_context.c',
                      'src/core/security/server_auth_filter.c',
                      'src/core/security/server_secure_chttp2.c',
                      'src/core/surface/init_secure.c',
                      'src/core/surface/secure_channel_create.c',
                      'src/core/tsi/fake_transport_security.c',
                      'src/core/tsi/ssl_transport_security.c',
                      'src/core/tsi/transport_security.c',
                      'src/core/census/grpc_context.c',
                      'src/core/census/grpc_filter.c',
                      'src/core/channel/channel_args.c',
                      'src/core/channel/channel_stack.c',
                      'src/core/channel/client_channel.c',
                      'src/core/channel/client_uchannel.c',
                      'src/core/channel/compress_filter.c',
                      'src/core/channel/connected_channel.c',
                      'src/core/channel/http_client_filter.c',
                      'src/core/channel/http_server_filter.c',
                      'src/core/channel/subchannel_call_holder.c',
                      'src/core/client_config/client_config.c',
                      'src/core/client_config/connector.c',
                      'src/core/client_config/default_initial_connect_string.c',
                      'src/core/client_config/initial_connect_string.c',
                      'src/core/client_config/lb_policies/pick_first.c',
                      'src/core/client_config/lb_policies/round_robin.c',
                      'src/core/client_config/lb_policy.c',
                      'src/core/client_config/lb_policy_factory.c',
                      'src/core/client_config/lb_policy_registry.c',
                      'src/core/client_config/resolver.c',
                      'src/core/client_config/resolver_factory.c',
                      'src/core/client_config/resolver_registry.c',
                      'src/core/client_config/resolvers/dns_resolver.c',
                      'src/core/client_config/resolvers/sockaddr_resolver.c',
                      'src/core/client_config/subchannel.c',
                      'src/core/client_config/subchannel_factory.c',
                      'src/core/client_config/uri_parser.c',
                      'src/core/compression/algorithm.c',
                      'src/core/compression/message_compress.c',
                      'src/core/debug/trace.c',
                      'src/core/httpcli/format_request.c',
                      'src/core/httpcli/httpcli.c',
                      'src/core/httpcli/parser.c',
                      'src/core/iomgr/closure.c',
                      'src/core/iomgr/endpoint.c',
                      'src/core/iomgr/endpoint_pair_posix.c',
                      'src/core/iomgr/endpoint_pair_windows.c',
                      'src/core/iomgr/exec_ctx.c',
                      'src/core/iomgr/executor.c',
                      'src/core/iomgr/fd_posix.c',
                      'src/core/iomgr/iocp_windows.c',
                      'src/core/iomgr/iomgr.c',
                      'src/core/iomgr/iomgr_posix.c',
                      'src/core/iomgr/iomgr_windows.c',
                      'src/core/iomgr/pollset_multipoller_with_epoll.c',
                      'src/core/iomgr/pollset_multipoller_with_poll_posix.c',
                      'src/core/iomgr/pollset_posix.c',
                      'src/core/iomgr/pollset_set_posix.c',
                      'src/core/iomgr/pollset_set_windows.c',
                      'src/core/iomgr/pollset_windows.c',
                      'src/core/iomgr/resolve_address_posix.c',
                      'src/core/iomgr/resolve_address_windows.c',
                      'src/core/iomgr/sockaddr_utils.c',
                      'src/core/iomgr/socket_utils_common_posix.c',
                      'src/core/iomgr/socket_utils_linux.c',
                      'src/core/iomgr/socket_utils_posix.c',
                      'src/core/iomgr/socket_windows.c',
                      'src/core/iomgr/tcp_client_posix.c',
                      'src/core/iomgr/tcp_client_windows.c',
                      'src/core/iomgr/tcp_posix.c',
                      'src/core/iomgr/tcp_server_posix.c',
                      'src/core/iomgr/tcp_server_windows.c',
                      'src/core/iomgr/tcp_windows.c',
                      'src/core/iomgr/time_averaged_stats.c',
                      'src/core/iomgr/timer.c',
                      'src/core/iomgr/timer_heap.c',
                      'src/core/iomgr/udp_server.c',
                      'src/core/iomgr/wakeup_fd_eventfd.c',
                      'src/core/iomgr/wakeup_fd_nospecial.c',
                      'src/core/iomgr/wakeup_fd_pipe.c',
                      'src/core/iomgr/wakeup_fd_posix.c',
                      'src/core/iomgr/workqueue_posix.c',
                      'src/core/iomgr/workqueue_windows.c',
                      'src/core/json/json.c',
                      'src/core/json/json_reader.c',
                      'src/core/json/json_string.c',
                      'src/core/json/json_writer.c',
                      'src/core/surface/alarm.c',
                      'src/core/surface/api_trace.c',
                      'src/core/surface/byte_buffer.c',
                      'src/core/surface/byte_buffer_reader.c',
                      'src/core/surface/call.c',
                      'src/core/surface/call_details.c',
                      'src/core/surface/call_log_batch.c',
                      'src/core/surface/channel.c',
                      'src/core/surface/channel_connectivity.c',
                      'src/core/surface/channel_create.c',
                      'src/core/surface/channel_ping.c',
                      'src/core/surface/completion_queue.c',
                      'src/core/surface/event_string.c',
                      'src/core/surface/init.c',
                      'src/core/surface/lame_client.c',
                      'src/core/surface/metadata_array.c',
                      'src/core/surface/server.c',
                      'src/core/surface/server_chttp2.c',
                      'src/core/surface/server_create.c',
                      'src/core/surface/validate_metadata.c',
                      'src/core/surface/version.c',
                      'src/core/transport/byte_stream.c',
                      'src/core/transport/chttp2/alpn.c',
                      'src/core/transport/chttp2/bin_encoder.c',
                      'src/core/transport/chttp2/frame_data.c',
                      'src/core/transport/chttp2/frame_goaway.c',
                      'src/core/transport/chttp2/frame_ping.c',
                      'src/core/transport/chttp2/frame_rst_stream.c',
                      'src/core/transport/chttp2/frame_settings.c',
                      'src/core/transport/chttp2/frame_window_update.c',
                      'src/core/transport/chttp2/hpack_encoder.c',
                      'src/core/transport/chttp2/hpack_parser.c',
                      'src/core/transport/chttp2/hpack_table.c',
                      'src/core/transport/chttp2/huffsyms.c',
                      'src/core/transport/chttp2/incoming_metadata.c',
                      'src/core/transport/chttp2/parsing.c',
                      'src/core/transport/chttp2/status_conversion.c',
                      'src/core/transport/chttp2/stream_lists.c',
                      'src/core/transport/chttp2/stream_map.c',
                      'src/core/transport/chttp2/timeout_encoding.c',
                      'src/core/transport/chttp2/varint.c',
                      'src/core/transport/chttp2/writing.c',
                      'src/core/transport/chttp2_transport.c',
                      'src/core/transport/connectivity_state.c',
                      'src/core/transport/metadata.c',
                      'src/core/transport/metadata_batch.c',
                      'src/core/transport/static_metadata.c',
                      'src/core/transport/transport.c',
                      'src/core/transport/transport_op_string.c',
                      'src/core/census/context.c',
                      'src/core/census/initialize.c',
                      'src/core/census/operation.c',
                      'src/core/census/placeholders.c',
                      'src/core/census/tracing.c'

    ss.private_header_files = 'src/core/profiling/timers.h',
                              'src/core/support/block_annotate.h',
                              'src/core/support/env.h',
                              'src/core/support/file.h',
                              'src/core/support/murmur_hash.h',
                              'src/core/support/stack_lockfree.h',
                              'src/core/support/string.h',
                              'src/core/support/string_win32.h',
                              'src/core/support/thd_internal.h',
                              'src/core/support/time_precise.h',
                              'src/core/security/auth_filters.h',
                              'src/core/security/base64.h',
                              'src/core/security/credentials.h',
                              'src/core/security/handshake.h',
                              'src/core/security/json_token.h',
                              'src/core/security/jwt_verifier.h',
                              'src/core/security/secure_endpoint.h',
                              'src/core/security/security_connector.h',
                              'src/core/security/security_context.h',
                              'src/core/tsi/fake_transport_security.h',
                              'src/core/tsi/ssl_transport_security.h',
                              'src/core/tsi/ssl_types.h',
                              'src/core/tsi/transport_security.h',
                              'src/core/tsi/transport_security_interface.h',
                              'src/core/census/grpc_filter.h',
                              'src/core/channel/channel_args.h',
                              'src/core/channel/channel_stack.h',
                              'src/core/channel/client_channel.h',
                              'src/core/channel/client_uchannel.h',
                              'src/core/channel/compress_filter.h',
                              'src/core/channel/connected_channel.h',
                              'src/core/channel/context.h',
                              'src/core/channel/http_client_filter.h',
                              'src/core/channel/http_server_filter.h',
                              'src/core/channel/subchannel_call_holder.h',
                              'src/core/client_config/client_config.h',
                              'src/core/client_config/connector.h',
                              'src/core/client_config/initial_connect_string.h',
                              'src/core/client_config/lb_policies/pick_first.h',
                              'src/core/client_config/lb_policies/round_robin.h',
                              'src/core/client_config/lb_policy.h',
                              'src/core/client_config/lb_policy_factory.h',
                              'src/core/client_config/lb_policy_registry.h',
                              'src/core/client_config/resolver.h',
                              'src/core/client_config/resolver_factory.h',
                              'src/core/client_config/resolver_registry.h',
                              'src/core/client_config/resolvers/dns_resolver.h',
                              'src/core/client_config/resolvers/sockaddr_resolver.h',
                              'src/core/client_config/subchannel.h',
                              'src/core/client_config/subchannel_factory.h',
                              'src/core/client_config/uri_parser.h',
                              'src/core/compression/algorithm_metadata.h',
                              'src/core/compression/message_compress.h',
                              'src/core/debug/trace.h',
                              'src/core/httpcli/format_request.h',
                              'src/core/httpcli/httpcli.h',
                              'src/core/httpcli/parser.h',
                              'src/core/iomgr/closure.h',
                              'src/core/iomgr/endpoint.h',
                              'src/core/iomgr/endpoint_pair.h',
                              'src/core/iomgr/exec_ctx.h',
                              'src/core/iomgr/executor.h',
                              'src/core/iomgr/fd_posix.h',
                              'src/core/iomgr/iocp_windows.h',
                              'src/core/iomgr/iomgr.h',
                              'src/core/iomgr/iomgr_internal.h',
                              'src/core/iomgr/iomgr_posix.h',
                              'src/core/iomgr/pollset.h',
                              'src/core/iomgr/pollset_posix.h',
                              'src/core/iomgr/pollset_set.h',
                              'src/core/iomgr/pollset_set_posix.h',
                              'src/core/iomgr/pollset_set_windows.h',
                              'src/core/iomgr/pollset_windows.h',
                              'src/core/iomgr/resolve_address.h',
                              'src/core/iomgr/sockaddr.h',
                              'src/core/iomgr/sockaddr_posix.h',
                              'src/core/iomgr/sockaddr_utils.h',
                              'src/core/iomgr/sockaddr_win32.h',
                              'src/core/iomgr/socket_utils_posix.h',
                              'src/core/iomgr/socket_windows.h',
                              'src/core/iomgr/tcp_client.h',
                              'src/core/iomgr/tcp_posix.h',
                              'src/core/iomgr/tcp_server.h',
                              'src/core/iomgr/tcp_windows.h',
                              'src/core/iomgr/time_averaged_stats.h',
                              'src/core/iomgr/timer.h',
                              'src/core/iomgr/timer_heap.h',
                              'src/core/iomgr/timer_internal.h',
                              'src/core/iomgr/udp_server.h',
                              'src/core/iomgr/wakeup_fd_pipe.h',
                              'src/core/iomgr/wakeup_fd_posix.h',
                              'src/core/iomgr/workqueue.h',
                              'src/core/iomgr/workqueue_posix.h',
                              'src/core/iomgr/workqueue_windows.h',
                              'src/core/json/json.h',
                              'src/core/json/json_common.h',
                              'src/core/json/json_reader.h',
                              'src/core/json/json_writer.h',
                              'src/core/statistics/census_interface.h',
                              'src/core/statistics/census_rpc_stats.h',
                              'src/core/surface/api_trace.h',
                              'src/core/surface/call.h',
                              'src/core/surface/call_test_only.h',
                              'src/core/surface/channel.h',
                              'src/core/surface/completion_queue.h',
                              'src/core/surface/event_string.h',
                              'src/core/surface/init.h',
                              'src/core/surface/server.h',
                              'src/core/surface/surface_trace.h',
                              'src/core/transport/byte_stream.h',
                              'src/core/transport/chttp2/alpn.h',
                              'src/core/transport/chttp2/bin_encoder.h',
                              'src/core/transport/chttp2/frame.h',
                              'src/core/transport/chttp2/frame_data.h',
                              'src/core/transport/chttp2/frame_goaway.h',
                              'src/core/transport/chttp2/frame_ping.h',
                              'src/core/transport/chttp2/frame_rst_stream.h',
                              'src/core/transport/chttp2/frame_settings.h',
                              'src/core/transport/chttp2/frame_window_update.h',
                              'src/core/transport/chttp2/hpack_encoder.h',
                              'src/core/transport/chttp2/hpack_parser.h',
                              'src/core/transport/chttp2/hpack_table.h',
                              'src/core/transport/chttp2/http2_errors.h',
                              'src/core/transport/chttp2/huffsyms.h',
                              'src/core/transport/chttp2/incoming_metadata.h',
                              'src/core/transport/chttp2/internal.h',
                              'src/core/transport/chttp2/status_conversion.h',
                              'src/core/transport/chttp2/stream_map.h',
                              'src/core/transport/chttp2/timeout_encoding.h',
                              'src/core/transport/chttp2/varint.h',
                              'src/core/transport/chttp2_transport.h',
                              'src/core/transport/connectivity_state.h',
                              'src/core/transport/metadata.h',
                              'src/core/transport/metadata_batch.h',
                              'src/core/transport/static_metadata.h',
                              'src/core/transport/transport.h',
                              'src/core/transport/transport_impl.h',
                              'src/core/census/aggregation.h',
                              'src/core/census/rpc_metric_id.h'

    ss.header_mappings_dir = '.'
    # This isn't officially supported in Cocoapods. We've asked for an alternative:
    # https://github.com/CocoaPods/CocoaPods/issues/4386
    ss.xcconfig = {
      'USE_HEADERMAP' => 'NO',
      'ALWAYS_SEARCH_USER_PATHS' => 'NO',
      'USER_HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/Headers/Private/gRPC"',
      'HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/Headers/Private/gRPC/include"'
    }

    ss.requires_arc = false
    ss.libraries = 'z'
    ss.dependency 'BoringSSL', '~> 2.0'

    # ss.compiler_flags = '-GCC_WARN_INHIBIT_ALL_WARNINGS', '-w'
  end

  # Objective-C wrapper around the core gRPC library.
  s.subspec 'GRPCClient' do |ss|
    src_dir = "#{objc_dir}/GRPCClient"
    ss.source_files = "#{src_dir}/*.{h,m}", "#{src_dir}/**/*.{h,m}"
    ss.private_header_files = "#{src_dir}/private/*.h"
    ss.header_mappings_dir = "#{objc_dir}"

    ss.dependency 'gRPC/C-Core'
    ss.dependency 'gRPC/RxLibrary'

    # Certificates, to be able to establish TLS connections:
    ss.resource_bundles = { 'gRPCCertificates' => ['etc/roots.pem'] }
  end

  # RPC library for ProtocolBuffers, based on gRPC
  s.subspec 'ProtoRPC' do |ss|
    src_dir = "#{objc_dir}/ProtoRPC"
    ss.source_files = "#{src_dir}/*.{h,m}"
    ss.header_mappings_dir = "#{objc_dir}"

    ss.dependency 'gRPC/GRPCClient'
    ss.dependency 'gRPC/RxLibrary'
    ss.dependency 'Protobuf', '~> 3.0.0-alpha-4'
  end
end
