/*
 * Copyright 2017 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Automatically generated by tools/codegen/core/gen_stats_data.py
 */

#ifndef GRPC_CORE_LIB_DEBUG_STATS_DATA_H
#define GRPC_CORE_LIB_DEBUG_STATS_DATA_H

#include <grpc/support/port_platform.h>

#include <inttypes.h>
#include "src/core/lib/iomgr/exec_ctx.h"

typedef enum {
  GRPC_STATS_COUNTER_CLIENT_CALLS_CREATED,
  GRPC_STATS_COUNTER_SERVER_CALLS_CREATED,
  GRPC_STATS_COUNTER_CQS_CREATED,
  GRPC_STATS_COUNTER_CLIENT_CHANNELS_CREATED,
  GRPC_STATS_COUNTER_CLIENT_SUBCHANNELS_CREATED,
  GRPC_STATS_COUNTER_SERVER_CHANNELS_CREATED,
  GRPC_STATS_COUNTER_SYSCALL_POLL,
  GRPC_STATS_COUNTER_SYSCALL_WAIT,
  GRPC_STATS_COUNTER_POLLSET_KICK,
  GRPC_STATS_COUNTER_POLLSET_KICKED_WITHOUT_POLLER,
  GRPC_STATS_COUNTER_POLLSET_KICKED_AGAIN,
  GRPC_STATS_COUNTER_POLLSET_KICK_WAKEUP_FD,
  GRPC_STATS_COUNTER_POLLSET_KICK_WAKEUP_CV,
  GRPC_STATS_COUNTER_POLLSET_KICK_OWN_THREAD,
  GRPC_STATS_COUNTER_HISTOGRAM_SLOW_LOOKUPS,
  GRPC_STATS_COUNTER_SYSCALL_WRITE,
  GRPC_STATS_COUNTER_SYSCALL_READ,
  GRPC_STATS_COUNTER_TCP_BACKUP_POLLERS_CREATED,
  GRPC_STATS_COUNTER_TCP_BACKUP_POLLER_POLLS,
  GRPC_STATS_COUNTER_HTTP2_OP_BATCHES,
  GRPC_STATS_COUNTER_HTTP2_OP_CANCEL,
  GRPC_STATS_COUNTER_HTTP2_OP_SEND_INITIAL_METADATA,
  GRPC_STATS_COUNTER_HTTP2_OP_SEND_MESSAGE,
  GRPC_STATS_COUNTER_HTTP2_OP_SEND_TRAILING_METADATA,
  GRPC_STATS_COUNTER_HTTP2_OP_RECV_INITIAL_METADATA,
  GRPC_STATS_COUNTER_HTTP2_OP_RECV_MESSAGE,
  GRPC_STATS_COUNTER_HTTP2_OP_RECV_TRAILING_METADATA,
  GRPC_STATS_COUNTER_HTTP2_SETTINGS_WRITES,
  GRPC_STATS_COUNTER_HTTP2_PINGS_SENT,
  GRPC_STATS_COUNTER_HTTP2_WRITES_BEGUN,
  GRPC_STATS_COUNTER_HTTP2_WRITES_OFFLOADED,
  GRPC_STATS_COUNTER_HTTP2_WRITES_CONTINUED,
  GRPC_STATS_COUNTER_HTTP2_PARTIAL_WRITES,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_INITIAL_WRITE,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_START_NEW_STREAM,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_MESSAGE,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_INITIAL_METADATA,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_TRAILING_METADATA,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_RETRY_SEND_PING,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_CONTINUE_PINGS,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_GOAWAY_SENT,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_RST_STREAM,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_CLOSE_FROM_API,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_STREAM_FLOW_CONTROL,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_SETTINGS,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_BDP_ESTIMATOR_PING,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_SETTING,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_UPDATE,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_APPLICATION_PING,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_KEEPALIVE_PING,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL_UNSTALLED,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_PING_RESPONSE,
  GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_FORCE_RST_STREAM,
  GRPC_STATS_COUNTER_HTTP2_SPURIOUS_WRITES_BEGUN,
  GRPC_STATS_COUNTER_HPACK_RECV_INDEXED,
  GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_INCIDX,
  GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_INCIDX_V,
  GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NOTIDX,
  GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NOTIDX_V,
  GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NVRIDX,
  GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NVRIDX_V,
  GRPC_STATS_COUNTER_HPACK_RECV_UNCOMPRESSED,
  GRPC_STATS_COUNTER_HPACK_RECV_HUFFMAN,
  GRPC_STATS_COUNTER_HPACK_RECV_BINARY,
  GRPC_STATS_COUNTER_HPACK_RECV_BINARY_BASE64,
  GRPC_STATS_COUNTER_HPACK_SEND_INDEXED,
  GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_INCIDX,
  GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_INCIDX_V,
  GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NOTIDX,
  GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NOTIDX_V,
  GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NVRIDX,
  GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NVRIDX_V,
  GRPC_STATS_COUNTER_HPACK_SEND_UNCOMPRESSED,
  GRPC_STATS_COUNTER_HPACK_SEND_HUFFMAN,
  GRPC_STATS_COUNTER_HPACK_SEND_BINARY,
  GRPC_STATS_COUNTER_HPACK_SEND_BINARY_BASE64,
  GRPC_STATS_COUNTER_COMBINER_LOCKS_INITIATED,
  GRPC_STATS_COUNTER_COMBINER_LOCKS_SCHEDULED_ITEMS,
  GRPC_STATS_COUNTER_COMBINER_LOCKS_SCHEDULED_FINAL_ITEMS,
  GRPC_STATS_COUNTER_COMBINER_LOCKS_OFFLOADED,
  GRPC_STATS_COUNTER_CALL_COMBINER_LOCKS_INITIATED,
  GRPC_STATS_COUNTER_CALL_COMBINER_LOCKS_SCHEDULED_ITEMS,
  GRPC_STATS_COUNTER_CALL_COMBINER_SET_NOTIFY_ON_CANCEL,
  GRPC_STATS_COUNTER_CALL_COMBINER_CANCELLED,
  GRPC_STATS_COUNTER_EXECUTOR_SCHEDULED_SHORT_ITEMS,
  GRPC_STATS_COUNTER_EXECUTOR_SCHEDULED_LONG_ITEMS,
  GRPC_STATS_COUNTER_EXECUTOR_SCHEDULED_TO_SELF,
  GRPC_STATS_COUNTER_EXECUTOR_WAKEUP_INITIATED,
  GRPC_STATS_COUNTER_EXECUTOR_QUEUE_DRAINED,
  GRPC_STATS_COUNTER_EXECUTOR_PUSH_RETRIES,
  GRPC_STATS_COUNTER_SERVER_REQUESTED_CALLS,
  GRPC_STATS_COUNTER_SERVER_SLOWPATH_REQUESTS_QUEUED,
  GRPC_STATS_COUNTER_CQ_EV_QUEUE_TRYLOCK_FAILURES,
  GRPC_STATS_COUNTER_CQ_EV_QUEUE_TRYLOCK_SUCCESSES,
  GRPC_STATS_COUNTER_CQ_EV_QUEUE_TRANSIENT_POP_FAILURES,
  GRPC_STATS_COUNTER_COUNT
} grpc_stats_counters;
extern const char* grpc_stats_counter_name[GRPC_STATS_COUNTER_COUNT];
extern const char* grpc_stats_counter_doc[GRPC_STATS_COUNTER_COUNT];
typedef enum {
  GRPC_STATS_HISTOGRAM_CALL_INITIAL_SIZE,
  GRPC_STATS_HISTOGRAM_POLL_EVENTS_RETURNED,
  GRPC_STATS_HISTOGRAM_TCP_WRITE_SIZE,
  GRPC_STATS_HISTOGRAM_TCP_WRITE_IOV_SIZE,
  GRPC_STATS_HISTOGRAM_TCP_READ_SIZE,
  GRPC_STATS_HISTOGRAM_TCP_READ_OFFER,
  GRPC_STATS_HISTOGRAM_TCP_READ_OFFER_IOV_SIZE,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_MESSAGE_SIZE,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_INITIAL_METADATA_PER_WRITE,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_MESSAGE_PER_WRITE,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_TRAILING_METADATA_PER_WRITE,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_FLOWCTL_PER_WRITE,
  GRPC_STATS_HISTOGRAM_SERVER_CQS_CHECKED,
  GRPC_STATS_HISTOGRAM_COUNT
} grpc_stats_histograms;
extern const char* grpc_stats_histogram_name[GRPC_STATS_HISTOGRAM_COUNT];
extern const char* grpc_stats_histogram_doc[GRPC_STATS_HISTOGRAM_COUNT];
typedef enum {
  GRPC_STATS_HISTOGRAM_CALL_INITIAL_SIZE_FIRST_SLOT = 0,
  GRPC_STATS_HISTOGRAM_CALL_INITIAL_SIZE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_POLL_EVENTS_RETURNED_FIRST_SLOT = 64,
  GRPC_STATS_HISTOGRAM_POLL_EVENTS_RETURNED_BUCKETS = 128,
  GRPC_STATS_HISTOGRAM_TCP_WRITE_SIZE_FIRST_SLOT = 192,
  GRPC_STATS_HISTOGRAM_TCP_WRITE_SIZE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_TCP_WRITE_IOV_SIZE_FIRST_SLOT = 256,
  GRPC_STATS_HISTOGRAM_TCP_WRITE_IOV_SIZE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_TCP_READ_SIZE_FIRST_SLOT = 320,
  GRPC_STATS_HISTOGRAM_TCP_READ_SIZE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_TCP_READ_OFFER_FIRST_SLOT = 384,
  GRPC_STATS_HISTOGRAM_TCP_READ_OFFER_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_TCP_READ_OFFER_IOV_SIZE_FIRST_SLOT = 448,
  GRPC_STATS_HISTOGRAM_TCP_READ_OFFER_IOV_SIZE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_MESSAGE_SIZE_FIRST_SLOT = 512,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_MESSAGE_SIZE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_INITIAL_METADATA_PER_WRITE_FIRST_SLOT = 576,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_INITIAL_METADATA_PER_WRITE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_MESSAGE_PER_WRITE_FIRST_SLOT = 640,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_MESSAGE_PER_WRITE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_TRAILING_METADATA_PER_WRITE_FIRST_SLOT = 704,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_TRAILING_METADATA_PER_WRITE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_FLOWCTL_PER_WRITE_FIRST_SLOT = 768,
  GRPC_STATS_HISTOGRAM_HTTP2_SEND_FLOWCTL_PER_WRITE_BUCKETS = 64,
  GRPC_STATS_HISTOGRAM_SERVER_CQS_CHECKED_FIRST_SLOT = 832,
  GRPC_STATS_HISTOGRAM_SERVER_CQS_CHECKED_BUCKETS = 8,
  GRPC_STATS_HISTOGRAM_BUCKETS = 840
} grpc_stats_histogram_constants;
#if defined(GRPC_COLLECT_STATS) || !defined(NDEBUG)
#define GRPC_STATS_INC_CLIENT_CALLS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CLIENT_CALLS_CREATED)
#define GRPC_STATS_INC_SERVER_CALLS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SERVER_CALLS_CREATED)
#define GRPC_STATS_INC_CQS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CQS_CREATED)
#define GRPC_STATS_INC_CLIENT_CHANNELS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CLIENT_CHANNELS_CREATED)
#define GRPC_STATS_INC_CLIENT_SUBCHANNELS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CLIENT_SUBCHANNELS_CREATED)
#define GRPC_STATS_INC_SERVER_CHANNELS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SERVER_CHANNELS_CREATED)
#define GRPC_STATS_INC_SYSCALL_POLL() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SYSCALL_POLL)
#define GRPC_STATS_INC_SYSCALL_WAIT() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SYSCALL_WAIT)
#define GRPC_STATS_INC_POLLSET_KICK() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_POLLSET_KICK)
#define GRPC_STATS_INC_POLLSET_KICKED_WITHOUT_POLLER() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_POLLSET_KICKED_WITHOUT_POLLER)
#define GRPC_STATS_INC_POLLSET_KICKED_AGAIN() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_POLLSET_KICKED_AGAIN)
#define GRPC_STATS_INC_POLLSET_KICK_WAKEUP_FD() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_POLLSET_KICK_WAKEUP_FD)
#define GRPC_STATS_INC_POLLSET_KICK_WAKEUP_CV() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_POLLSET_KICK_WAKEUP_CV)
#define GRPC_STATS_INC_POLLSET_KICK_OWN_THREAD() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_POLLSET_KICK_OWN_THREAD)
#define GRPC_STATS_INC_HISTOGRAM_SLOW_LOOKUPS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HISTOGRAM_SLOW_LOOKUPS)
#define GRPC_STATS_INC_SYSCALL_WRITE() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SYSCALL_WRITE)
#define GRPC_STATS_INC_SYSCALL_READ() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SYSCALL_READ)
#define GRPC_STATS_INC_TCP_BACKUP_POLLERS_CREATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_TCP_BACKUP_POLLERS_CREATED)
#define GRPC_STATS_INC_TCP_BACKUP_POLLER_POLLS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_TCP_BACKUP_POLLER_POLLS)
#define GRPC_STATS_INC_HTTP2_OP_BATCHES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_BATCHES)
#define GRPC_STATS_INC_HTTP2_OP_CANCEL() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_CANCEL)
#define GRPC_STATS_INC_HTTP2_OP_SEND_INITIAL_METADATA() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_SEND_INITIAL_METADATA)
#define GRPC_STATS_INC_HTTP2_OP_SEND_MESSAGE() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_SEND_MESSAGE)
#define GRPC_STATS_INC_HTTP2_OP_SEND_TRAILING_METADATA() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_SEND_TRAILING_METADATA)
#define GRPC_STATS_INC_HTTP2_OP_RECV_INITIAL_METADATA() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_RECV_INITIAL_METADATA)
#define GRPC_STATS_INC_HTTP2_OP_RECV_MESSAGE() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_RECV_MESSAGE)
#define GRPC_STATS_INC_HTTP2_OP_RECV_TRAILING_METADATA() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_OP_RECV_TRAILING_METADATA)
#define GRPC_STATS_INC_HTTP2_SETTINGS_WRITES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_SETTINGS_WRITES)
#define GRPC_STATS_INC_HTTP2_PINGS_SENT() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_PINGS_SENT)
#define GRPC_STATS_INC_HTTP2_WRITES_BEGUN() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_WRITES_BEGUN)
#define GRPC_STATS_INC_HTTP2_WRITES_OFFLOADED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_WRITES_OFFLOADED)
#define GRPC_STATS_INC_HTTP2_WRITES_CONTINUED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_WRITES_CONTINUED)
#define GRPC_STATS_INC_HTTP2_PARTIAL_WRITES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_PARTIAL_WRITES)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_INITIAL_WRITE() \
  GRPC_STATS_INC_COUNTER(                                          \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_INITIAL_WRITE)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_START_NEW_STREAM() \
  GRPC_STATS_INC_COUNTER(                                             \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_START_NEW_STREAM)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_MESSAGE() \
  GRPC_STATS_INC_COUNTER(                                         \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_MESSAGE)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_INITIAL_METADATA() \
  GRPC_STATS_INC_COUNTER(                                                  \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_INITIAL_METADATA)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_TRAILING_METADATA() \
  GRPC_STATS_INC_COUNTER(                                                   \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_TRAILING_METADATA)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_RETRY_SEND_PING() \
  GRPC_STATS_INC_COUNTER(                                            \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_RETRY_SEND_PING)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_CONTINUE_PINGS() \
  GRPC_STATS_INC_COUNTER(                                           \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_CONTINUE_PINGS)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_GOAWAY_SENT() \
  GRPC_STATS_INC_COUNTER(                                        \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_GOAWAY_SENT)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_RST_STREAM() \
  GRPC_STATS_INC_COUNTER(                                       \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_RST_STREAM)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_CLOSE_FROM_API() \
  GRPC_STATS_INC_COUNTER(                                           \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_CLOSE_FROM_API)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_STREAM_FLOW_CONTROL() \
  GRPC_STATS_INC_COUNTER(                                                \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_STREAM_FLOW_CONTROL)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL() \
  GRPC_STATS_INC_COUNTER(                                                   \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_SETTINGS() \
  GRPC_STATS_INC_COUNTER(                                          \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_SEND_SETTINGS)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_BDP_ESTIMATOR_PING() \
  GRPC_STATS_INC_COUNTER(                                               \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_BDP_ESTIMATOR_PING)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_SETTING() \
  GRPC_STATS_INC_COUNTER(                                                              \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_SETTING)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_UPDATE() \
  GRPC_STATS_INC_COUNTER(                                                             \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_UPDATE)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_APPLICATION_PING() \
  GRPC_STATS_INC_COUNTER(                                             \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_APPLICATION_PING)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_KEEPALIVE_PING() \
  GRPC_STATS_INC_COUNTER(                                           \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_KEEPALIVE_PING)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL_UNSTALLED() \
  GRPC_STATS_INC_COUNTER(                                                             \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL_UNSTALLED)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_PING_RESPONSE() \
  GRPC_STATS_INC_COUNTER(                                          \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_PING_RESPONSE)
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_FORCE_RST_STREAM() \
  GRPC_STATS_INC_COUNTER(                                             \
      GRPC_STATS_COUNTER_HTTP2_INITIATE_WRITE_DUE_TO_FORCE_RST_STREAM)
#define GRPC_STATS_INC_HTTP2_SPURIOUS_WRITES_BEGUN() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HTTP2_SPURIOUS_WRITES_BEGUN)
#define GRPC_STATS_INC_HPACK_RECV_INDEXED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_INDEXED)
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_INCIDX() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_INCIDX)
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_INCIDX_V() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_INCIDX_V)
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NOTIDX() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NOTIDX)
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NOTIDX_V() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NOTIDX_V)
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NVRIDX() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NVRIDX)
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NVRIDX_V() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_LITHDR_NVRIDX_V)
#define GRPC_STATS_INC_HPACK_RECV_UNCOMPRESSED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_UNCOMPRESSED)
#define GRPC_STATS_INC_HPACK_RECV_HUFFMAN() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_HUFFMAN)
#define GRPC_STATS_INC_HPACK_RECV_BINARY() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_BINARY)
#define GRPC_STATS_INC_HPACK_RECV_BINARY_BASE64() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_RECV_BINARY_BASE64)
#define GRPC_STATS_INC_HPACK_SEND_INDEXED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_INDEXED)
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_INCIDX() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_INCIDX)
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_INCIDX_V() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_INCIDX_V)
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NOTIDX() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NOTIDX)
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NOTIDX_V() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NOTIDX_V)
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NVRIDX() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NVRIDX)
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NVRIDX_V() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_LITHDR_NVRIDX_V)
#define GRPC_STATS_INC_HPACK_SEND_UNCOMPRESSED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_UNCOMPRESSED)
#define GRPC_STATS_INC_HPACK_SEND_HUFFMAN() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_HUFFMAN)
#define GRPC_STATS_INC_HPACK_SEND_BINARY() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_BINARY)
#define GRPC_STATS_INC_HPACK_SEND_BINARY_BASE64() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_HPACK_SEND_BINARY_BASE64)
#define GRPC_STATS_INC_COMBINER_LOCKS_INITIATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_COMBINER_LOCKS_INITIATED)
#define GRPC_STATS_INC_COMBINER_LOCKS_SCHEDULED_ITEMS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_COMBINER_LOCKS_SCHEDULED_ITEMS)
#define GRPC_STATS_INC_COMBINER_LOCKS_SCHEDULED_FINAL_ITEMS() \
  GRPC_STATS_INC_COUNTER(                                     \
      GRPC_STATS_COUNTER_COMBINER_LOCKS_SCHEDULED_FINAL_ITEMS)
#define GRPC_STATS_INC_COMBINER_LOCKS_OFFLOADED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_COMBINER_LOCKS_OFFLOADED)
#define GRPC_STATS_INC_CALL_COMBINER_LOCKS_INITIATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CALL_COMBINER_LOCKS_INITIATED)
#define GRPC_STATS_INC_CALL_COMBINER_LOCKS_SCHEDULED_ITEMS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CALL_COMBINER_LOCKS_SCHEDULED_ITEMS)
#define GRPC_STATS_INC_CALL_COMBINER_SET_NOTIFY_ON_CANCEL() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CALL_COMBINER_SET_NOTIFY_ON_CANCEL)
#define GRPC_STATS_INC_CALL_COMBINER_CANCELLED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CALL_COMBINER_CANCELLED)
#define GRPC_STATS_INC_EXECUTOR_SCHEDULED_SHORT_ITEMS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_EXECUTOR_SCHEDULED_SHORT_ITEMS)
#define GRPC_STATS_INC_EXECUTOR_SCHEDULED_LONG_ITEMS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_EXECUTOR_SCHEDULED_LONG_ITEMS)
#define GRPC_STATS_INC_EXECUTOR_SCHEDULED_TO_SELF() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_EXECUTOR_SCHEDULED_TO_SELF)
#define GRPC_STATS_INC_EXECUTOR_WAKEUP_INITIATED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_EXECUTOR_WAKEUP_INITIATED)
#define GRPC_STATS_INC_EXECUTOR_QUEUE_DRAINED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_EXECUTOR_QUEUE_DRAINED)
#define GRPC_STATS_INC_EXECUTOR_PUSH_RETRIES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_EXECUTOR_PUSH_RETRIES)
#define GRPC_STATS_INC_SERVER_REQUESTED_CALLS() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SERVER_REQUESTED_CALLS)
#define GRPC_STATS_INC_SERVER_SLOWPATH_REQUESTS_QUEUED() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_SERVER_SLOWPATH_REQUESTS_QUEUED)
#define GRPC_STATS_INC_CQ_EV_QUEUE_TRYLOCK_FAILURES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CQ_EV_QUEUE_TRYLOCK_FAILURES)
#define GRPC_STATS_INC_CQ_EV_QUEUE_TRYLOCK_SUCCESSES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CQ_EV_QUEUE_TRYLOCK_SUCCESSES)
#define GRPC_STATS_INC_CQ_EV_QUEUE_TRANSIENT_POP_FAILURES() \
  GRPC_STATS_INC_COUNTER(GRPC_STATS_COUNTER_CQ_EV_QUEUE_TRANSIENT_POP_FAILURES)
#define GRPC_STATS_INC_CALL_INITIAL_SIZE(value) \
  grpc_stats_inc_call_initial_size((int)(value))
void grpc_stats_inc_call_initial_size(int x);
#define GRPC_STATS_INC_POLL_EVENTS_RETURNED(value) \
  grpc_stats_inc_poll_events_returned((int)(value))
void grpc_stats_inc_poll_events_returned(int x);
#define GRPC_STATS_INC_TCP_WRITE_SIZE(value) \
  grpc_stats_inc_tcp_write_size((int)(value))
void grpc_stats_inc_tcp_write_size(int x);
#define GRPC_STATS_INC_TCP_WRITE_IOV_SIZE(value) \
  grpc_stats_inc_tcp_write_iov_size((int)(value))
void grpc_stats_inc_tcp_write_iov_size(int x);
#define GRPC_STATS_INC_TCP_READ_SIZE(value) \
  grpc_stats_inc_tcp_read_size((int)(value))
void grpc_stats_inc_tcp_read_size(int x);
#define GRPC_STATS_INC_TCP_READ_OFFER(value) \
  grpc_stats_inc_tcp_read_offer((int)(value))
void grpc_stats_inc_tcp_read_offer(int x);
#define GRPC_STATS_INC_TCP_READ_OFFER_IOV_SIZE(value) \
  grpc_stats_inc_tcp_read_offer_iov_size((int)(value))
void grpc_stats_inc_tcp_read_offer_iov_size(int x);
#define GRPC_STATS_INC_HTTP2_SEND_MESSAGE_SIZE(value) \
  grpc_stats_inc_http2_send_message_size((int)(value))
void grpc_stats_inc_http2_send_message_size(int x);
#define GRPC_STATS_INC_HTTP2_SEND_INITIAL_METADATA_PER_WRITE(value) \
  grpc_stats_inc_http2_send_initial_metadata_per_write((int)(value))
void grpc_stats_inc_http2_send_initial_metadata_per_write(int x);
#define GRPC_STATS_INC_HTTP2_SEND_MESSAGE_PER_WRITE(value) \
  grpc_stats_inc_http2_send_message_per_write((int)(value))
void grpc_stats_inc_http2_send_message_per_write(int x);
#define GRPC_STATS_INC_HTTP2_SEND_TRAILING_METADATA_PER_WRITE(value) \
  grpc_stats_inc_http2_send_trailing_metadata_per_write((int)(value))
void grpc_stats_inc_http2_send_trailing_metadata_per_write(int x);
#define GRPC_STATS_INC_HTTP2_SEND_FLOWCTL_PER_WRITE(value) \
  grpc_stats_inc_http2_send_flowctl_per_write((int)(value))
void grpc_stats_inc_http2_send_flowctl_per_write(int x);
#define GRPC_STATS_INC_SERVER_CQS_CHECKED(value) \
  grpc_stats_inc_server_cqs_checked((int)(value))
void grpc_stats_inc_server_cqs_checked(int x);
#else
#define GRPC_STATS_INC_CLIENT_CALLS_CREATED()
#define GRPC_STATS_INC_SERVER_CALLS_CREATED()
#define GRPC_STATS_INC_CQS_CREATED()
#define GRPC_STATS_INC_CLIENT_CHANNELS_CREATED()
#define GRPC_STATS_INC_CLIENT_SUBCHANNELS_CREATED()
#define GRPC_STATS_INC_SERVER_CHANNELS_CREATED()
#define GRPC_STATS_INC_SYSCALL_POLL()
#define GRPC_STATS_INC_SYSCALL_WAIT()
#define GRPC_STATS_INC_POLLSET_KICK()
#define GRPC_STATS_INC_POLLSET_KICKED_WITHOUT_POLLER()
#define GRPC_STATS_INC_POLLSET_KICKED_AGAIN()
#define GRPC_STATS_INC_POLLSET_KICK_WAKEUP_FD()
#define GRPC_STATS_INC_POLLSET_KICK_WAKEUP_CV()
#define GRPC_STATS_INC_POLLSET_KICK_OWN_THREAD()
#define GRPC_STATS_INC_HISTOGRAM_SLOW_LOOKUPS()
#define GRPC_STATS_INC_SYSCALL_WRITE()
#define GRPC_STATS_INC_SYSCALL_READ()
#define GRPC_STATS_INC_TCP_BACKUP_POLLERS_CREATED()
#define GRPC_STATS_INC_TCP_BACKUP_POLLER_POLLS()
#define GRPC_STATS_INC_HTTP2_OP_BATCHES()
#define GRPC_STATS_INC_HTTP2_OP_CANCEL()
#define GRPC_STATS_INC_HTTP2_OP_SEND_INITIAL_METADATA()
#define GRPC_STATS_INC_HTTP2_OP_SEND_MESSAGE()
#define GRPC_STATS_INC_HTTP2_OP_SEND_TRAILING_METADATA()
#define GRPC_STATS_INC_HTTP2_OP_RECV_INITIAL_METADATA()
#define GRPC_STATS_INC_HTTP2_OP_RECV_MESSAGE()
#define GRPC_STATS_INC_HTTP2_OP_RECV_TRAILING_METADATA()
#define GRPC_STATS_INC_HTTP2_SETTINGS_WRITES()
#define GRPC_STATS_INC_HTTP2_PINGS_SENT()
#define GRPC_STATS_INC_HTTP2_WRITES_BEGUN()
#define GRPC_STATS_INC_HTTP2_WRITES_OFFLOADED()
#define GRPC_STATS_INC_HTTP2_WRITES_CONTINUED()
#define GRPC_STATS_INC_HTTP2_PARTIAL_WRITES()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_INITIAL_WRITE()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_START_NEW_STREAM()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_MESSAGE()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_INITIAL_METADATA()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_TRAILING_METADATA()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_RETRY_SEND_PING()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_CONTINUE_PINGS()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_GOAWAY_SENT()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_RST_STREAM()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_CLOSE_FROM_API()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_STREAM_FLOW_CONTROL()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_SEND_SETTINGS()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_BDP_ESTIMATOR_PING()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_SETTING()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_FLOW_CONTROL_UNSTALLED_BY_UPDATE()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_APPLICATION_PING()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_KEEPALIVE_PING()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_TRANSPORT_FLOW_CONTROL_UNSTALLED()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_PING_RESPONSE()
#define GRPC_STATS_INC_HTTP2_INITIATE_WRITE_DUE_TO_FORCE_RST_STREAM()
#define GRPC_STATS_INC_HTTP2_SPURIOUS_WRITES_BEGUN()
#define GRPC_STATS_INC_HPACK_RECV_INDEXED()
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_INCIDX()
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_INCIDX_V()
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NOTIDX()
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NOTIDX_V()
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NVRIDX()
#define GRPC_STATS_INC_HPACK_RECV_LITHDR_NVRIDX_V()
#define GRPC_STATS_INC_HPACK_RECV_UNCOMPRESSED()
#define GRPC_STATS_INC_HPACK_RECV_HUFFMAN()
#define GRPC_STATS_INC_HPACK_RECV_BINARY()
#define GRPC_STATS_INC_HPACK_RECV_BINARY_BASE64()
#define GRPC_STATS_INC_HPACK_SEND_INDEXED()
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_INCIDX()
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_INCIDX_V()
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NOTIDX()
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NOTIDX_V()
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NVRIDX()
#define GRPC_STATS_INC_HPACK_SEND_LITHDR_NVRIDX_V()
#define GRPC_STATS_INC_HPACK_SEND_UNCOMPRESSED()
#define GRPC_STATS_INC_HPACK_SEND_HUFFMAN()
#define GRPC_STATS_INC_HPACK_SEND_BINARY()
#define GRPC_STATS_INC_HPACK_SEND_BINARY_BASE64()
#define GRPC_STATS_INC_COMBINER_LOCKS_INITIATED()
#define GRPC_STATS_INC_COMBINER_LOCKS_SCHEDULED_ITEMS()
#define GRPC_STATS_INC_COMBINER_LOCKS_SCHEDULED_FINAL_ITEMS()
#define GRPC_STATS_INC_COMBINER_LOCKS_OFFLOADED()
#define GRPC_STATS_INC_CALL_COMBINER_LOCKS_INITIATED()
#define GRPC_STATS_INC_CALL_COMBINER_LOCKS_SCHEDULED_ITEMS()
#define GRPC_STATS_INC_CALL_COMBINER_SET_NOTIFY_ON_CANCEL()
#define GRPC_STATS_INC_CALL_COMBINER_CANCELLED()
#define GRPC_STATS_INC_EXECUTOR_SCHEDULED_SHORT_ITEMS()
#define GRPC_STATS_INC_EXECUTOR_SCHEDULED_LONG_ITEMS()
#define GRPC_STATS_INC_EXECUTOR_SCHEDULED_TO_SELF()
#define GRPC_STATS_INC_EXECUTOR_WAKEUP_INITIATED()
#define GRPC_STATS_INC_EXECUTOR_QUEUE_DRAINED()
#define GRPC_STATS_INC_EXECUTOR_PUSH_RETRIES()
#define GRPC_STATS_INC_SERVER_REQUESTED_CALLS()
#define GRPC_STATS_INC_SERVER_SLOWPATH_REQUESTS_QUEUED()
#define GRPC_STATS_INC_CQ_EV_QUEUE_TRYLOCK_FAILURES()
#define GRPC_STATS_INC_CQ_EV_QUEUE_TRYLOCK_SUCCESSES()
#define GRPC_STATS_INC_CQ_EV_QUEUE_TRANSIENT_POP_FAILURES()
#define GRPC_STATS_INC_CALL_INITIAL_SIZE(value)
#define GRPC_STATS_INC_POLL_EVENTS_RETURNED(value)
#define GRPC_STATS_INC_TCP_WRITE_SIZE(value)
#define GRPC_STATS_INC_TCP_WRITE_IOV_SIZE(value)
#define GRPC_STATS_INC_TCP_READ_SIZE(value)
#define GRPC_STATS_INC_TCP_READ_OFFER(value)
#define GRPC_STATS_INC_TCP_READ_OFFER_IOV_SIZE(value)
#define GRPC_STATS_INC_HTTP2_SEND_MESSAGE_SIZE(value)
#define GRPC_STATS_INC_HTTP2_SEND_INITIAL_METADATA_PER_WRITE(value)
#define GRPC_STATS_INC_HTTP2_SEND_MESSAGE_PER_WRITE(value)
#define GRPC_STATS_INC_HTTP2_SEND_TRAILING_METADATA_PER_WRITE(value)
#define GRPC_STATS_INC_HTTP2_SEND_FLOWCTL_PER_WRITE(value)
#define GRPC_STATS_INC_SERVER_CQS_CHECKED(value)
#endif /* defined(GRPC_COLLECT_STATS) || !defined(NDEBUG) */
extern const int grpc_stats_histo_buckets[13];
extern const int grpc_stats_histo_start[13];
extern const int* const grpc_stats_histo_bucket_boundaries[13];
extern void (*const grpc_stats_inc_histogram[13])(int x);

#endif /* GRPC_CORE_LIB_DEBUG_STATS_DATA_H */
