# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# AUTO-GENERATED FROM `$REPO_ROOT/templates/src/python/grpcio/grpc_core_dependencies.py.template`!!!

CORE_SOURCE_FILES = [
    'src/core/lib/gpr/alloc.cc',
    'src/core/lib/gpr/arena.cc',
    'src/core/lib/gpr/atm.cc',
    'src/core/lib/gpr/cpu_iphone.cc',
    'src/core/lib/gpr/cpu_linux.cc',
    'src/core/lib/gpr/cpu_posix.cc',
    'src/core/lib/gpr/cpu_windows.cc',
    'src/core/lib/gpr/env_linux.cc',
    'src/core/lib/gpr/env_posix.cc',
    'src/core/lib/gpr/env_windows.cc',
    'src/core/lib/gpr/fork.cc',
    'src/core/lib/gpr/host_port.cc',
    'src/core/lib/gpr/log.cc',
    'src/core/lib/gpr/log_android.cc',
    'src/core/lib/gpr/log_linux.cc',
    'src/core/lib/gpr/log_posix.cc',
    'src/core/lib/gpr/log_windows.cc',
    'src/core/lib/gpr/mpscq.cc',
    'src/core/lib/gpr/murmur_hash.cc',
    'src/core/lib/gpr/string.cc',
    'src/core/lib/gpr/string_posix.cc',
    'src/core/lib/gpr/string_util_windows.cc',
    'src/core/lib/gpr/string_windows.cc',
    'src/core/lib/gpr/sync.cc',
    'src/core/lib/gpr/sync_posix.cc',
    'src/core/lib/gpr/sync_windows.cc',
    'src/core/lib/gpr/thd.cc',
    'src/core/lib/gpr/thd_posix.cc',
    'src/core/lib/gpr/thd_windows.cc',
    'src/core/lib/gpr/time.cc',
    'src/core/lib/gpr/time_posix.cc',
    'src/core/lib/gpr/time_precise.cc',
    'src/core/lib/gpr/time_windows.cc',
    'src/core/lib/gpr/tls_pthread.cc',
    'src/core/lib/gpr/tmpfile_msys.cc',
    'src/core/lib/gpr/tmpfile_posix.cc',
    'src/core/lib/gpr/tmpfile_windows.cc',
    'src/core/lib/gpr/wrap_memcpy.cc',
    'src/core/lib/profiling/basic_timers.cc',
    'src/core/lib/profiling/stap_timers.cc',
    'src/core/lib/surface/init.cc',
    'src/core/lib/avl/avl.cc',
    'src/core/lib/backoff/backoff.cc',
    'src/core/lib/channel/channel_args.cc',
    'src/core/lib/channel/channel_stack.cc',
    'src/core/lib/channel/channel_stack_builder.cc',
    'src/core/lib/channel/connected_channel.cc',
    'src/core/lib/channel/handshaker.cc',
    'src/core/lib/channel/handshaker_factory.cc',
    'src/core/lib/channel/handshaker_registry.cc',
    'src/core/lib/compression/compression.cc',
    'src/core/lib/compression/compression_internal.cc',
    'src/core/lib/compression/message_compress.cc',
    'src/core/lib/compression/stream_compression.cc',
    'src/core/lib/compression/stream_compression_gzip.cc',
    'src/core/lib/compression/stream_compression_identity.cc',
    'src/core/lib/debug/stats.cc',
    'src/core/lib/debug/stats_data.cc',
    'src/core/lib/http/format_request.cc',
    'src/core/lib/http/httpcli.cc',
    'src/core/lib/http/parser.cc',
    'src/core/lib/iomgr/call_combiner.cc',
    'src/core/lib/iomgr/combiner.cc',
    'src/core/lib/iomgr/endpoint.cc',
    'src/core/lib/iomgr/endpoint_pair_posix.cc',
    'src/core/lib/iomgr/endpoint_pair_uv.cc',
    'src/core/lib/iomgr/endpoint_pair_windows.cc',
    'src/core/lib/iomgr/error.cc',
    'src/core/lib/iomgr/ev_epoll1_linux.cc',
    'src/core/lib/iomgr/ev_epollex_linux.cc',
    'src/core/lib/iomgr/ev_epollsig_linux.cc',
    'src/core/lib/iomgr/ev_poll_posix.cc',
    'src/core/lib/iomgr/ev_posix.cc',
    'src/core/lib/iomgr/ev_windows.cc',
    'src/core/lib/iomgr/exec_ctx.cc',
    'src/core/lib/iomgr/executor.cc',
    'src/core/lib/iomgr/fork_posix.cc',
    'src/core/lib/iomgr/fork_windows.cc',
    'src/core/lib/iomgr/gethostname_fallback.cc',
    'src/core/lib/iomgr/gethostname_host_name_max.cc',
    'src/core/lib/iomgr/gethostname_sysconf.cc',
    'src/core/lib/iomgr/iocp_windows.cc',
    'src/core/lib/iomgr/iomgr.cc',
    'src/core/lib/iomgr/iomgr_posix.cc',
    'src/core/lib/iomgr/iomgr_uv.cc',
    'src/core/lib/iomgr/iomgr_windows.cc',
    'src/core/lib/iomgr/is_epollexclusive_available.cc',
    'src/core/lib/iomgr/load_file.cc',
    'src/core/lib/iomgr/lockfree_event.cc',
    'src/core/lib/iomgr/network_status_tracker.cc',
    'src/core/lib/iomgr/polling_entity.cc',
    'src/core/lib/iomgr/pollset_set_uv.cc',
    'src/core/lib/iomgr/pollset_set_windows.cc',
    'src/core/lib/iomgr/pollset_uv.cc',
    'src/core/lib/iomgr/pollset_windows.cc',
    'src/core/lib/iomgr/resolve_address_posix.cc',
    'src/core/lib/iomgr/resolve_address_uv.cc',
    'src/core/lib/iomgr/resolve_address_windows.cc',
    'src/core/lib/iomgr/resource_quota.cc',
    'src/core/lib/iomgr/sockaddr_utils.cc',
    'src/core/lib/iomgr/socket_factory_posix.cc',
    'src/core/lib/iomgr/socket_mutator.cc',
    'src/core/lib/iomgr/socket_utils_common_posix.cc',
    'src/core/lib/iomgr/socket_utils_linux.cc',
    'src/core/lib/iomgr/socket_utils_posix.cc',
    'src/core/lib/iomgr/socket_utils_uv.cc',
    'src/core/lib/iomgr/socket_utils_windows.cc',
    'src/core/lib/iomgr/socket_windows.cc',
    'src/core/lib/iomgr/tcp_client_posix.cc',
    'src/core/lib/iomgr/tcp_client_uv.cc',
    'src/core/lib/iomgr/tcp_client_windows.cc',
    'src/core/lib/iomgr/tcp_posix.cc',
    'src/core/lib/iomgr/tcp_server_posix.cc',
    'src/core/lib/iomgr/tcp_server_utils_posix_common.cc',
    'src/core/lib/iomgr/tcp_server_utils_posix_ifaddrs.cc',
    'src/core/lib/iomgr/tcp_server_utils_posix_noifaddrs.cc',
    'src/core/lib/iomgr/tcp_server_uv.cc',
    'src/core/lib/iomgr/tcp_server_windows.cc',
    'src/core/lib/iomgr/tcp_uv.cc',
    'src/core/lib/iomgr/tcp_windows.cc',
    'src/core/lib/iomgr/time_averaged_stats.cc',
    'src/core/lib/iomgr/timer_generic.cc',
    'src/core/lib/iomgr/timer_heap.cc',
    'src/core/lib/iomgr/timer_manager.cc',
    'src/core/lib/iomgr/timer_uv.cc',
    'src/core/lib/iomgr/udp_server.cc',
    'src/core/lib/iomgr/unix_sockets_posix.cc',
    'src/core/lib/iomgr/unix_sockets_posix_noop.cc',
    'src/core/lib/iomgr/wakeup_fd_cv.cc',
    'src/core/lib/iomgr/wakeup_fd_eventfd.cc',
    'src/core/lib/iomgr/wakeup_fd_nospecial.cc',
    'src/core/lib/iomgr/wakeup_fd_pipe.cc',
    'src/core/lib/iomgr/wakeup_fd_posix.cc',
    'src/core/lib/json/json.cc',
    'src/core/lib/json/json_reader.cc',
    'src/core/lib/json/json_string.cc',
    'src/core/lib/json/json_writer.cc',
    'src/core/lib/slice/b64.cc',
    'src/core/lib/slice/percent_encoding.cc',
    'src/core/lib/slice/slice.cc',
    'src/core/lib/slice/slice_buffer.cc',
    'src/core/lib/slice/slice_hash_table.cc',
    'src/core/lib/slice/slice_intern.cc',
    'src/core/lib/slice/slice_string_helpers.cc',
    'src/core/lib/surface/api_trace.cc',
    'src/core/lib/surface/byte_buffer.cc',
    'src/core/lib/surface/byte_buffer_reader.cc',
    'src/core/lib/surface/call.cc',
    'src/core/lib/surface/call_details.cc',
    'src/core/lib/surface/call_log_batch.cc',
    'src/core/lib/surface/channel.cc',
    'src/core/lib/surface/channel_init.cc',
    'src/core/lib/surface/channel_ping.cc',
    'src/core/lib/surface/channel_stack_type.cc',
    'src/core/lib/surface/completion_queue.cc',
    'src/core/lib/surface/completion_queue_factory.cc',
    'src/core/lib/surface/event_string.cc',
    'src/core/lib/surface/lame_client.cc',
    'src/core/lib/surface/metadata_array.cc',
    'src/core/lib/surface/server.cc',
    'src/core/lib/surface/validate_metadata.cc',
    'src/core/lib/surface/version.cc',
    'src/core/lib/transport/bdp_estimator.cc',
    'src/core/lib/transport/byte_stream.cc',
    'src/core/lib/transport/connectivity_state.cc',
    'src/core/lib/transport/error_utils.cc',
    'src/core/lib/transport/metadata.cc',
    'src/core/lib/transport/metadata_batch.cc',
    'src/core/lib/transport/pid_controller.cc',
    'src/core/lib/transport/service_config.cc',
    'src/core/lib/transport/static_metadata.cc',
    'src/core/lib/transport/status_conversion.cc',
    'src/core/lib/transport/timeout_encoding.cc',
    'src/core/lib/transport/transport.cc',
    'src/core/lib/transport/transport_op_string.cc',
    'src/core/lib/debug/trace.cc',
    'src/core/ext/transport/chttp2/server/secure/server_secure_chttp2.cc',
    'src/core/ext/transport/chttp2/transport/bin_decoder.cc',
    'src/core/ext/transport/chttp2/transport/bin_encoder.cc',
    'src/core/ext/transport/chttp2/transport/chttp2_plugin.cc',
    'src/core/ext/transport/chttp2/transport/chttp2_transport.cc',
    'src/core/ext/transport/chttp2/transport/flow_control.cc',
    'src/core/ext/transport/chttp2/transport/frame_data.cc',
    'src/core/ext/transport/chttp2/transport/frame_goaway.cc',
    'src/core/ext/transport/chttp2/transport/frame_ping.cc',
    'src/core/ext/transport/chttp2/transport/frame_rst_stream.cc',
    'src/core/ext/transport/chttp2/transport/frame_settings.cc',
    'src/core/ext/transport/chttp2/transport/frame_window_update.cc',
    'src/core/ext/transport/chttp2/transport/hpack_encoder.cc',
    'src/core/ext/transport/chttp2/transport/hpack_parser.cc',
    'src/core/ext/transport/chttp2/transport/hpack_table.cc',
    'src/core/ext/transport/chttp2/transport/http2_settings.cc',
    'src/core/ext/transport/chttp2/transport/huffsyms.cc',
    'src/core/ext/transport/chttp2/transport/incoming_metadata.cc',
    'src/core/ext/transport/chttp2/transport/parsing.cc',
    'src/core/ext/transport/chttp2/transport/stream_lists.cc',
    'src/core/ext/transport/chttp2/transport/stream_map.cc',
    'src/core/ext/transport/chttp2/transport/varint.cc',
    'src/core/ext/transport/chttp2/transport/writing.cc',
    'src/core/ext/transport/chttp2/alpn/alpn.cc',
    'src/core/ext/filters/http/client/http_client_filter.cc',
    'src/core/ext/filters/http/http_filters_plugin.cc',
    'src/core/ext/filters/http/message_compress/message_compress_filter.cc',
    'src/core/ext/filters/http/server/http_server_filter.cc',
    'src/core/lib/http/httpcli_security_connector.cc',
    'src/core/lib/security/context/security_context.cc',
    'src/core/lib/security/credentials/composite/composite_credentials.cc',
    'src/core/lib/security/credentials/credentials.cc',
    'src/core/lib/security/credentials/credentials_metadata.cc',
    'src/core/lib/security/credentials/fake/fake_credentials.cc',
    'src/core/lib/security/credentials/google_default/credentials_generic.cc',
    'src/core/lib/security/credentials/google_default/google_default_credentials.cc',
    'src/core/lib/security/credentials/iam/iam_credentials.cc',
    'src/core/lib/security/credentials/jwt/json_token.cc',
    'src/core/lib/security/credentials/jwt/jwt_credentials.cc',
    'src/core/lib/security/credentials/jwt/jwt_verifier.cc',
    'src/core/lib/security/credentials/oauth2/oauth2_credentials.cc',
    'src/core/lib/security/credentials/plugin/plugin_credentials.cc',
    'src/core/lib/security/credentials/ssl/ssl_credentials.cc',
    'src/core/lib/security/security_connector/security_connector.cc',
    'src/core/lib/security/transport/client_auth_filter.cc',
    'src/core/lib/security/transport/lb_targets_info.cc',
    'src/core/lib/security/transport/secure_endpoint.cc',
    'src/core/lib/security/transport/security_handshaker.cc',
    'src/core/lib/security/transport/server_auth_filter.cc',
    'src/core/lib/security/transport/tsi_error.cc',
    'src/core/lib/security/util/json_util.cc',
    'src/core/lib/surface/init_secure.cc',
    'src/core/tsi/alts_transport_security.cc',
    'src/core/tsi/fake_transport_security.cc',
    'src/core/tsi/ssl_transport_security.cc',
    'src/core/tsi/transport_security_grpc.cc',
    'src/core/tsi/transport_security.cc',
    'src/core/tsi/transport_security_adapter.cc',
    'src/core/ext/transport/chttp2/server/chttp2_server.cc',
    'src/core/ext/transport/chttp2/client/secure/secure_channel_create.cc',
    'src/core/ext/filters/client_channel/backup_poller.cc',
    'src/core/ext/filters/client_channel/channel_connectivity.cc',
    'src/core/ext/filters/client_channel/client_channel.cc',
    'src/core/ext/filters/client_channel/client_channel_factory.cc',
    'src/core/ext/filters/client_channel/client_channel_plugin.cc',
    'src/core/ext/filters/client_channel/connector.cc',
    'src/core/ext/filters/client_channel/http_connect_handshaker.cc',
    'src/core/ext/filters/client_channel/http_proxy.cc',
    'src/core/ext/filters/client_channel/lb_policy.cc',
    'src/core/ext/filters/client_channel/lb_policy_factory.cc',
    'src/core/ext/filters/client_channel/lb_policy_registry.cc',
    'src/core/ext/filters/client_channel/parse_address.cc',
    'src/core/ext/filters/client_channel/proxy_mapper.cc',
    'src/core/ext/filters/client_channel/proxy_mapper_registry.cc',
    'src/core/ext/filters/client_channel/resolver.cc',
    'src/core/ext/filters/client_channel/resolver_registry.cc',
    'src/core/ext/filters/client_channel/retry_throttle.cc',
    'src/core/ext/filters/client_channel/subchannel.cc',
    'src/core/ext/filters/client_channel/subchannel_index.cc',
    'src/core/ext/filters/client_channel/uri_parser.cc',
    'src/core/ext/filters/deadline/deadline_filter.cc',
    'src/core/ext/transport/chttp2/client/chttp2_connector.cc',
    'src/core/ext/transport/chttp2/server/insecure/server_chttp2.cc',
    'src/core/ext/transport/chttp2/server/insecure/server_chttp2_posix.cc',
    'src/core/ext/transport/chttp2/client/insecure/channel_create.cc',
    'src/core/ext/transport/chttp2/client/insecure/channel_create_posix.cc',
    'src/core/ext/transport/inproc/inproc_plugin.cc',
    'src/core/ext/transport/inproc/inproc_transport.cc',
    'src/core/ext/filters/client_channel/lb_policy/grpclb/client_load_reporting_filter.cc',
    'src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb.cc',
    'src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb_channel_secure.cc',
    'src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb_client_stats.cc',
    'src/core/ext/filters/client_channel/lb_policy/grpclb/load_balancer_api.cc',
    'src/core/ext/filters/client_channel/lb_policy/grpclb/proto/grpc/lb/v1/load_balancer.pb.c',
    'third_party/nanopb/pb_common.c',
    'third_party/nanopb/pb_decode.c',
    'third_party/nanopb/pb_encode.c',
    'src/core/ext/filters/client_channel/resolver/fake/fake_resolver.cc',
    'src/core/ext/filters/client_channel/lb_policy/pick_first/pick_first.cc',
    'src/core/ext/filters/client_channel/lb_policy/subchannel_list.cc',
    'src/core/ext/filters/client_channel/lb_policy/round_robin/round_robin.cc',
    'src/core/ext/filters/client_channel/resolver/dns/c_ares/dns_resolver_ares.cc',
    'src/core/ext/filters/client_channel/resolver/dns/c_ares/grpc_ares_ev_driver_posix.cc',
    'src/core/ext/filters/client_channel/resolver/dns/c_ares/grpc_ares_wrapper.cc',
    'src/core/ext/filters/client_channel/resolver/dns/c_ares/grpc_ares_wrapper_fallback.cc',
    'src/core/ext/filters/client_channel/resolver/dns/native/dns_resolver.cc',
    'src/core/ext/filters/client_channel/resolver/sockaddr/sockaddr_resolver.cc',
    'src/core/ext/filters/load_reporting/server_load_reporting_filter.cc',
    'src/core/ext/filters/load_reporting/server_load_reporting_plugin.cc',
    'src/core/ext/census/grpc_context.cc',
    'src/core/ext/filters/max_age/max_age_filter.cc',
    'src/core/ext/filters/message_size/message_size_filter.cc',
    'src/core/ext/filters/workarounds/workaround_cronet_compression_filter.cc',
    'src/core/ext/filters/workarounds/workaround_utils.cc',
    'src/core/plugin_registry/grpc_plugin_registry.cc',
    'src/boringssl/err_data.c',
    'third_party/boringssl/crypto/asn1/a_bitstr.c',
    'third_party/boringssl/crypto/asn1/a_bool.c',
    'third_party/boringssl/crypto/asn1/a_d2i_fp.c',
    'third_party/boringssl/crypto/asn1/a_dup.c',
    'third_party/boringssl/crypto/asn1/a_enum.c',
    'third_party/boringssl/crypto/asn1/a_gentm.c',
    'third_party/boringssl/crypto/asn1/a_i2d_fp.c',
    'third_party/boringssl/crypto/asn1/a_int.c',
    'third_party/boringssl/crypto/asn1/a_mbstr.c',
    'third_party/boringssl/crypto/asn1/a_object.c',
    'third_party/boringssl/crypto/asn1/a_octet.c',
    'third_party/boringssl/crypto/asn1/a_print.c',
    'third_party/boringssl/crypto/asn1/a_strnid.c',
    'third_party/boringssl/crypto/asn1/a_time.c',
    'third_party/boringssl/crypto/asn1/a_type.c',
    'third_party/boringssl/crypto/asn1/a_utctm.c',
    'third_party/boringssl/crypto/asn1/a_utf8.c',
    'third_party/boringssl/crypto/asn1/asn1_lib.c',
    'third_party/boringssl/crypto/asn1/asn1_par.c',
    'third_party/boringssl/crypto/asn1/asn_pack.c',
    'third_party/boringssl/crypto/asn1/f_enum.c',
    'third_party/boringssl/crypto/asn1/f_int.c',
    'third_party/boringssl/crypto/asn1/f_string.c',
    'third_party/boringssl/crypto/asn1/tasn_dec.c',
    'third_party/boringssl/crypto/asn1/tasn_enc.c',
    'third_party/boringssl/crypto/asn1/tasn_fre.c',
    'third_party/boringssl/crypto/asn1/tasn_new.c',
    'third_party/boringssl/crypto/asn1/tasn_typ.c',
    'third_party/boringssl/crypto/asn1/tasn_utl.c',
    'third_party/boringssl/crypto/asn1/time_support.c',
    'third_party/boringssl/crypto/base64/base64.c',
    'third_party/boringssl/crypto/bio/bio.c',
    'third_party/boringssl/crypto/bio/bio_mem.c',
    'third_party/boringssl/crypto/bio/connect.c',
    'third_party/boringssl/crypto/bio/fd.c',
    'third_party/boringssl/crypto/bio/file.c',
    'third_party/boringssl/crypto/bio/hexdump.c',
    'third_party/boringssl/crypto/bio/pair.c',
    'third_party/boringssl/crypto/bio/printf.c',
    'third_party/boringssl/crypto/bio/socket.c',
    'third_party/boringssl/crypto/bio/socket_helper.c',
    'third_party/boringssl/crypto/bn_extra/bn_asn1.c',
    'third_party/boringssl/crypto/bn_extra/convert.c',
    'third_party/boringssl/crypto/buf/buf.c',
    'third_party/boringssl/crypto/bytestring/asn1_compat.c',
    'third_party/boringssl/crypto/bytestring/ber.c',
    'third_party/boringssl/crypto/bytestring/cbb.c',
    'third_party/boringssl/crypto/bytestring/cbs.c',
    'third_party/boringssl/crypto/chacha/chacha.c',
    'third_party/boringssl/crypto/cipher_extra/cipher_extra.c',
    'third_party/boringssl/crypto/cipher_extra/derive_key.c',
    'third_party/boringssl/crypto/cipher_extra/e_aesctrhmac.c',
    'third_party/boringssl/crypto/cipher_extra/e_aesgcmsiv.c',
    'third_party/boringssl/crypto/cipher_extra/e_chacha20poly1305.c',
    'third_party/boringssl/crypto/cipher_extra/e_null.c',
    'third_party/boringssl/crypto/cipher_extra/e_rc2.c',
    'third_party/boringssl/crypto/cipher_extra/e_rc4.c',
    'third_party/boringssl/crypto/cipher_extra/e_ssl3.c',
    'third_party/boringssl/crypto/cipher_extra/e_tls.c',
    'third_party/boringssl/crypto/cipher_extra/tls_cbc.c',
    'third_party/boringssl/crypto/cmac/cmac.c',
    'third_party/boringssl/crypto/conf/conf.c',
    'third_party/boringssl/crypto/cpu-aarch64-linux.c',
    'third_party/boringssl/crypto/cpu-arm-linux.c',
    'third_party/boringssl/crypto/cpu-arm.c',
    'third_party/boringssl/crypto/cpu-intel.c',
    'third_party/boringssl/crypto/cpu-ppc64le.c',
    'third_party/boringssl/crypto/crypto.c',
    'third_party/boringssl/crypto/curve25519/spake25519.c',
    'third_party/boringssl/crypto/curve25519/x25519-x86_64.c',
    'third_party/boringssl/crypto/dh/check.c',
    'third_party/boringssl/crypto/dh/dh.c',
    'third_party/boringssl/crypto/dh/dh_asn1.c',
    'third_party/boringssl/crypto/dh/params.c',
    'third_party/boringssl/crypto/digest_extra/digest_extra.c',
    'third_party/boringssl/crypto/dsa/dsa.c',
    'third_party/boringssl/crypto/dsa/dsa_asn1.c',
    'third_party/boringssl/crypto/ec_extra/ec_asn1.c',
    'third_party/boringssl/crypto/ecdh/ecdh.c',
    'third_party/boringssl/crypto/ecdsa_extra/ecdsa_asn1.c',
    'third_party/boringssl/crypto/engine/engine.c',
    'third_party/boringssl/crypto/err/err.c',
    'third_party/boringssl/crypto/evp/digestsign.c',
    'third_party/boringssl/crypto/evp/evp.c',
    'third_party/boringssl/crypto/evp/evp_asn1.c',
    'third_party/boringssl/crypto/evp/evp_ctx.c',
    'third_party/boringssl/crypto/evp/p_dsa_asn1.c',
    'third_party/boringssl/crypto/evp/p_ec.c',
    'third_party/boringssl/crypto/evp/p_ec_asn1.c',
    'third_party/boringssl/crypto/evp/p_ed25519.c',
    'third_party/boringssl/crypto/evp/p_ed25519_asn1.c',
    'third_party/boringssl/crypto/evp/p_rsa.c',
    'third_party/boringssl/crypto/evp/p_rsa_asn1.c',
    'third_party/boringssl/crypto/evp/pbkdf.c',
    'third_party/boringssl/crypto/evp/print.c',
    'third_party/boringssl/crypto/evp/scrypt.c',
    'third_party/boringssl/crypto/evp/sign.c',
    'third_party/boringssl/crypto/ex_data.c',
    'third_party/boringssl/crypto/fipsmodule/bcm.c',
    'third_party/boringssl/crypto/fipsmodule/is_fips.c',
    'third_party/boringssl/crypto/hkdf/hkdf.c',
    'third_party/boringssl/crypto/lhash/lhash.c',
    'third_party/boringssl/crypto/mem.c',
    'third_party/boringssl/crypto/obj/obj.c',
    'third_party/boringssl/crypto/obj/obj_xref.c',
    'third_party/boringssl/crypto/pem/pem_all.c',
    'third_party/boringssl/crypto/pem/pem_info.c',
    'third_party/boringssl/crypto/pem/pem_lib.c',
    'third_party/boringssl/crypto/pem/pem_oth.c',
    'third_party/boringssl/crypto/pem/pem_pk8.c',
    'third_party/boringssl/crypto/pem/pem_pkey.c',
    'third_party/boringssl/crypto/pem/pem_x509.c',
    'third_party/boringssl/crypto/pem/pem_xaux.c',
    'third_party/boringssl/crypto/pkcs7/pkcs7.c',
    'third_party/boringssl/crypto/pkcs7/pkcs7_x509.c',
    'third_party/boringssl/crypto/pkcs8/p5_pbev2.c',
    'third_party/boringssl/crypto/pkcs8/pkcs8.c',
    'third_party/boringssl/crypto/pkcs8/pkcs8_x509.c',
    'third_party/boringssl/crypto/poly1305/poly1305.c',
    'third_party/boringssl/crypto/poly1305/poly1305_arm.c',
    'third_party/boringssl/crypto/poly1305/poly1305_vec.c',
    'third_party/boringssl/crypto/pool/pool.c',
    'third_party/boringssl/crypto/rand_extra/deterministic.c',
    'third_party/boringssl/crypto/rand_extra/forkunsafe.c',
    'third_party/boringssl/crypto/rand_extra/fuchsia.c',
    'third_party/boringssl/crypto/rand_extra/rand_extra.c',
    'third_party/boringssl/crypto/rand_extra/windows.c',
    'third_party/boringssl/crypto/rc4/rc4.c',
    'third_party/boringssl/crypto/refcount_c11.c',
    'third_party/boringssl/crypto/refcount_lock.c',
    'third_party/boringssl/crypto/rsa_extra/rsa_asn1.c',
    'third_party/boringssl/crypto/stack/stack.c',
    'third_party/boringssl/crypto/thread.c',
    'third_party/boringssl/crypto/thread_none.c',
    'third_party/boringssl/crypto/thread_pthread.c',
    'third_party/boringssl/crypto/thread_win.c',
    'third_party/boringssl/crypto/x509/a_digest.c',
    'third_party/boringssl/crypto/x509/a_sign.c',
    'third_party/boringssl/crypto/x509/a_strex.c',
    'third_party/boringssl/crypto/x509/a_verify.c',
    'third_party/boringssl/crypto/x509/algorithm.c',
    'third_party/boringssl/crypto/x509/asn1_gen.c',
    'third_party/boringssl/crypto/x509/by_dir.c',
    'third_party/boringssl/crypto/x509/by_file.c',
    'third_party/boringssl/crypto/x509/i2d_pr.c',
    'third_party/boringssl/crypto/x509/rsa_pss.c',
    'third_party/boringssl/crypto/x509/t_crl.c',
    'third_party/boringssl/crypto/x509/t_req.c',
    'third_party/boringssl/crypto/x509/t_x509.c',
    'third_party/boringssl/crypto/x509/t_x509a.c',
    'third_party/boringssl/crypto/x509/x509.c',
    'third_party/boringssl/crypto/x509/x509_att.c',
    'third_party/boringssl/crypto/x509/x509_cmp.c',
    'third_party/boringssl/crypto/x509/x509_d2.c',
    'third_party/boringssl/crypto/x509/x509_def.c',
    'third_party/boringssl/crypto/x509/x509_ext.c',
    'third_party/boringssl/crypto/x509/x509_lu.c',
    'third_party/boringssl/crypto/x509/x509_obj.c',
    'third_party/boringssl/crypto/x509/x509_r2x.c',
    'third_party/boringssl/crypto/x509/x509_req.c',
    'third_party/boringssl/crypto/x509/x509_set.c',
    'third_party/boringssl/crypto/x509/x509_trs.c',
    'third_party/boringssl/crypto/x509/x509_txt.c',
    'third_party/boringssl/crypto/x509/x509_v3.c',
    'third_party/boringssl/crypto/x509/x509_vfy.c',
    'third_party/boringssl/crypto/x509/x509_vpm.c',
    'third_party/boringssl/crypto/x509/x509cset.c',
    'third_party/boringssl/crypto/x509/x509name.c',
    'third_party/boringssl/crypto/x509/x509rset.c',
    'third_party/boringssl/crypto/x509/x509spki.c',
    'third_party/boringssl/crypto/x509/x_algor.c',
    'third_party/boringssl/crypto/x509/x_all.c',
    'third_party/boringssl/crypto/x509/x_attrib.c',
    'third_party/boringssl/crypto/x509/x_crl.c',
    'third_party/boringssl/crypto/x509/x_exten.c',
    'third_party/boringssl/crypto/x509/x_info.c',
    'third_party/boringssl/crypto/x509/x_name.c',
    'third_party/boringssl/crypto/x509/x_pkey.c',
    'third_party/boringssl/crypto/x509/x_pubkey.c',
    'third_party/boringssl/crypto/x509/x_req.c',
    'third_party/boringssl/crypto/x509/x_sig.c',
    'third_party/boringssl/crypto/x509/x_spki.c',
    'third_party/boringssl/crypto/x509/x_val.c',
    'third_party/boringssl/crypto/x509/x_x509.c',
    'third_party/boringssl/crypto/x509/x_x509a.c',
    'third_party/boringssl/crypto/x509v3/pcy_cache.c',
    'third_party/boringssl/crypto/x509v3/pcy_data.c',
    'third_party/boringssl/crypto/x509v3/pcy_lib.c',
    'third_party/boringssl/crypto/x509v3/pcy_map.c',
    'third_party/boringssl/crypto/x509v3/pcy_node.c',
    'third_party/boringssl/crypto/x509v3/pcy_tree.c',
    'third_party/boringssl/crypto/x509v3/v3_akey.c',
    'third_party/boringssl/crypto/x509v3/v3_akeya.c',
    'third_party/boringssl/crypto/x509v3/v3_alt.c',
    'third_party/boringssl/crypto/x509v3/v3_bcons.c',
    'third_party/boringssl/crypto/x509v3/v3_bitst.c',
    'third_party/boringssl/crypto/x509v3/v3_conf.c',
    'third_party/boringssl/crypto/x509v3/v3_cpols.c',
    'third_party/boringssl/crypto/x509v3/v3_crld.c',
    'third_party/boringssl/crypto/x509v3/v3_enum.c',
    'third_party/boringssl/crypto/x509v3/v3_extku.c',
    'third_party/boringssl/crypto/x509v3/v3_genn.c',
    'third_party/boringssl/crypto/x509v3/v3_ia5.c',
    'third_party/boringssl/crypto/x509v3/v3_info.c',
    'third_party/boringssl/crypto/x509v3/v3_int.c',
    'third_party/boringssl/crypto/x509v3/v3_lib.c',
    'third_party/boringssl/crypto/x509v3/v3_ncons.c',
    'third_party/boringssl/crypto/x509v3/v3_pci.c',
    'third_party/boringssl/crypto/x509v3/v3_pcia.c',
    'third_party/boringssl/crypto/x509v3/v3_pcons.c',
    'third_party/boringssl/crypto/x509v3/v3_pku.c',
    'third_party/boringssl/crypto/x509v3/v3_pmaps.c',
    'third_party/boringssl/crypto/x509v3/v3_prn.c',
    'third_party/boringssl/crypto/x509v3/v3_purp.c',
    'third_party/boringssl/crypto/x509v3/v3_skey.c',
    'third_party/boringssl/crypto/x509v3/v3_sxnet.c',
    'third_party/boringssl/crypto/x509v3/v3_utl.c',
    'third_party/boringssl/ssl/bio_ssl.cc',
    'third_party/boringssl/ssl/custom_extensions.cc',
    'third_party/boringssl/ssl/d1_both.cc',
    'third_party/boringssl/ssl/d1_lib.cc',
    'third_party/boringssl/ssl/d1_pkt.cc',
    'third_party/boringssl/ssl/d1_srtp.cc',
    'third_party/boringssl/ssl/dtls_method.cc',
    'third_party/boringssl/ssl/dtls_record.cc',
    'third_party/boringssl/ssl/handshake.cc',
    'third_party/boringssl/ssl/handshake_client.cc',
    'third_party/boringssl/ssl/handshake_server.cc',
    'third_party/boringssl/ssl/s3_both.cc',
    'third_party/boringssl/ssl/s3_lib.cc',
    'third_party/boringssl/ssl/s3_pkt.cc',
    'third_party/boringssl/ssl/ssl_aead_ctx.cc',
    'third_party/boringssl/ssl/ssl_asn1.cc',
    'third_party/boringssl/ssl/ssl_buffer.cc',
    'third_party/boringssl/ssl/ssl_cert.cc',
    'third_party/boringssl/ssl/ssl_cipher.cc',
    'third_party/boringssl/ssl/ssl_file.cc',
    'third_party/boringssl/ssl/ssl_key_share.cc',
    'third_party/boringssl/ssl/ssl_lib.cc',
    'third_party/boringssl/ssl/ssl_privkey.cc',
    'third_party/boringssl/ssl/ssl_session.cc',
    'third_party/boringssl/ssl/ssl_stat.cc',
    'third_party/boringssl/ssl/ssl_transcript.cc',
    'third_party/boringssl/ssl/ssl_versions.cc',
    'third_party/boringssl/ssl/ssl_x509.cc',
    'third_party/boringssl/ssl/t1_enc.cc',
    'third_party/boringssl/ssl/t1_lib.cc',
    'third_party/boringssl/ssl/tls13_both.cc',
    'third_party/boringssl/ssl/tls13_client.cc',
    'third_party/boringssl/ssl/tls13_enc.cc',
    'third_party/boringssl/ssl/tls13_server.cc',
    'third_party/boringssl/ssl/tls_method.cc',
    'third_party/boringssl/ssl/tls_record.cc',
    'third_party/boringssl/third_party/fiat/curve25519.c',
    'third_party/zlib/adler32.c',
    'third_party/zlib/compress.c',
    'third_party/zlib/crc32.c',
    'third_party/zlib/deflate.c',
    'third_party/zlib/gzclose.c',
    'third_party/zlib/gzlib.c',
    'third_party/zlib/gzread.c',
    'third_party/zlib/gzwrite.c',
    'third_party/zlib/infback.c',
    'third_party/zlib/inffast.c',
    'third_party/zlib/inflate.c',
    'third_party/zlib/inftrees.c',
    'third_party/zlib/trees.c',
    'third_party/zlib/uncompr.c',
    'third_party/zlib/zutil.c',
    'third_party/cares/cares/ares__close_sockets.c',
    'third_party/cares/cares/ares__get_hostent.c',
    'third_party/cares/cares/ares__read_line.c',
    'third_party/cares/cares/ares__timeval.c',
    'third_party/cares/cares/ares_cancel.c',
    'third_party/cares/cares/ares_create_query.c',
    'third_party/cares/cares/ares_data.c',
    'third_party/cares/cares/ares_destroy.c',
    'third_party/cares/cares/ares_expand_name.c',
    'third_party/cares/cares/ares_expand_string.c',
    'third_party/cares/cares/ares_fds.c',
    'third_party/cares/cares/ares_free_hostent.c',
    'third_party/cares/cares/ares_free_string.c',
    'third_party/cares/cares/ares_getenv.c',
    'third_party/cares/cares/ares_gethostbyaddr.c',
    'third_party/cares/cares/ares_gethostbyname.c',
    'third_party/cares/cares/ares_getnameinfo.c',
    'third_party/cares/cares/ares_getopt.c',
    'third_party/cares/cares/ares_getsock.c',
    'third_party/cares/cares/ares_init.c',
    'third_party/cares/cares/ares_library_init.c',
    'third_party/cares/cares/ares_llist.c',
    'third_party/cares/cares/ares_mkquery.c',
    'third_party/cares/cares/ares_nowarn.c',
    'third_party/cares/cares/ares_options.c',
    'third_party/cares/cares/ares_parse_a_reply.c',
    'third_party/cares/cares/ares_parse_aaaa_reply.c',
    'third_party/cares/cares/ares_parse_mx_reply.c',
    'third_party/cares/cares/ares_parse_naptr_reply.c',
    'third_party/cares/cares/ares_parse_ns_reply.c',
    'third_party/cares/cares/ares_parse_ptr_reply.c',
    'third_party/cares/cares/ares_parse_soa_reply.c',
    'third_party/cares/cares/ares_parse_srv_reply.c',
    'third_party/cares/cares/ares_parse_txt_reply.c',
    'third_party/cares/cares/ares_platform.c',
    'third_party/cares/cares/ares_process.c',
    'third_party/cares/cares/ares_query.c',
    'third_party/cares/cares/ares_search.c',
    'third_party/cares/cares/ares_send.c',
    'third_party/cares/cares/ares_strcasecmp.c',
    'third_party/cares/cares/ares_strdup.c',
    'third_party/cares/cares/ares_strerror.c',
    'third_party/cares/cares/ares_timeout.c',
    'third_party/cares/cares/ares_version.c',
    'third_party/cares/cares/ares_writev.c',
    'third_party/cares/cares/bitncmp.c',
    'third_party/cares/cares/inet_net_pton.c',
    'third_party/cares/cares/inet_ntop.c',
    'third_party/cares/cares/windows_port.c',
]
