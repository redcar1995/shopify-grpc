/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "src/core/client_config/subchannel.h"

#include <string.h>

#include <grpc/support/alloc.h>

#include "src/core/channel/channel_args.h"
#include "src/core/channel/connected_channel.h"
#include "src/core/channel/connectivity_state.h"

typedef struct {
  gpr_refcount refs;
  grpc_subchannel *subchannel;
} connection;

typedef struct waiting_for_connect {
  struct waiting_for_connect *next;
  grpc_iomgr_closure *notify;
  grpc_transport_stream_op *initial_op;
  grpc_subchannel_call **target;
} waiting_for_connect;

struct grpc_subchannel {
  gpr_refcount refs;
  grpc_connector *connector;

  /** non-transport related channel filters */
  const grpc_channel_filter **filters;
  size_t num_filters;
  /** channel arguments */
  grpc_channel_args *args;
  /** address to connect to */
  struct sockaddr *addr;
  size_t addr_len;
  /** metadata context */
  grpc_mdctx *mdctx;

  /** set during connection */
  grpc_connect_out_args connecting_result;

  /** callback for connection finishing */
  grpc_iomgr_closure connected;

  /** pollset_set tracking who's interested in a connection
      being setup */
  grpc_pollset_set pollset_set;

  /** mutex protecting remaining elements */
  gpr_mu mu;

  /** active connection */
  connection *active;
  /** are we connecting */
  int connecting;
  /** things waiting for a connection */
  waiting_for_connect *waiting;
  /** connectivity state tracking */
  grpc_connectivity_state_tracker state_tracker;
};

struct grpc_subchannel_call {
  connection *connection;
  gpr_refcount refs;
};

#define SUBCHANNEL_CALL_TO_CALL_STACK(call) ((grpc_call_stack *)((call) + 1))
#define CHANNEL_STACK_FROM_CONNECTION(con) ((grpc_channel_stack *)((con) + 1))

static grpc_subchannel_call *create_call(connection *con, grpc_transport_stream_op *initial_op);
static void connectivity_state_changed_locked(grpc_subchannel *c);
static grpc_connectivity_state compute_connectivity_locked(grpc_subchannel *c);
static gpr_timespec compute_connect_deadline(grpc_subchannel *c);
static void subchannel_connected(void *subchannel, int iomgr_success);

/*
 * connection implementation
 */

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
#define CONNECTION_REF(c, r) connection_ref((c), __FILE__, __LINE__, (r))
#define CONNECTION_UNREF(c, r) connection_unref((c), __FILE__, __LINE__, (r))
#else
#define CONNECTION_REF(c, r) connection_ref((c))
#define CONNECTION_UNREF(c, r) connection_unref((c))
#endif

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
static void connection_ref(connection *c, const char *file, int line, const char *reason) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "SUBCONN:%p   ref %d -> %d %s",
          c, (int)c->refs.count, (int)c->refs.count + 1,
          reason);
#else
static void connection_ref(connection *c) { 
#endif
	gpr_ref(&c->refs); 
}

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
static void connection_unref(connection *c, const char *file, int line, const char *reason) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "SUBCONN:%p unref %d -> %d %s",
          c, (int)c->refs.count, (int)c->refs.count - 1,
          reason);
#else
static void connection_unref(connection *c) {
#endif
  if (gpr_unref(&c->refs)) {
  	GRPC_SUBCHANNEL_UNREF(c->subchannel, "connection");
    gpr_free(c);
  }
}

/*
 * grpc_subchannel implementation
 */

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
void grpc_subchannel_ref(grpc_subchannel *c, const char *file, int line, const char *reason) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "SUBCHAN:%p   ref %d -> %d %s",
          c, (int)c->refs.count, (int)c->refs.count + 1,
          reason);
#else
void grpc_subchannel_ref(grpc_subchannel *c) { 
#endif
	gpr_ref(&c->refs); 
}

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
void grpc_subchannel_unref(grpc_subchannel *c, const char *file, int line, const char *reason) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "SUBCHAN:%p unref %d -> %d %s",
          c, (int)c->refs.count, (int)c->refs.count - 1,
          reason);
#else
void grpc_subchannel_unref(grpc_subchannel *c) {
#endif
  if (gpr_unref(&c->refs)) {
  	if (c->active != NULL) CONNECTION_UNREF(c->active, "subchannel");
    gpr_free(c->filters);
    grpc_channel_args_destroy(c->args);
    gpr_free(c->addr);
    grpc_mdctx_unref(c->mdctx);
    grpc_pollset_set_destroy(&c->pollset_set);
	  grpc_connectivity_state_destroy(&c->state_tracker);
    gpr_free(c);
  }
}

void grpc_subchannel_add_interested_party(grpc_subchannel *c,
                                          grpc_pollset *pollset) {
  grpc_pollset_set_add_pollset(&c->pollset_set, pollset);
}

void grpc_subchannel_del_interested_party(grpc_subchannel *c,
                                          grpc_pollset *pollset) {
  grpc_pollset_set_del_pollset(&c->pollset_set, pollset);
}

grpc_subchannel *grpc_subchannel_create(grpc_connector *connector,
                                        grpc_subchannel_args *args) {
  grpc_subchannel *c = gpr_malloc(sizeof(*c));
  memset(c, 0, sizeof(*c));
  gpr_ref_init(&c->refs, 1);
  c->connector = connector;
  grpc_connector_ref(c->connector);
  c->num_filters = args->filter_count;
  c->filters = gpr_malloc(sizeof(grpc_channel_filter *) * c->num_filters);
  memcpy(c->filters, args->filters,
         sizeof(grpc_channel_filter *) * c->num_filters);
  c->addr = gpr_malloc(args->addr_len);
  memcpy(c->addr, args->addr, args->addr_len);
  c->addr_len = args->addr_len;
  c->args = grpc_channel_args_copy(args->args);
  c->mdctx = args->mdctx;
  grpc_mdctx_ref(c->mdctx);
  grpc_pollset_set_init(&c->pollset_set);
  grpc_iomgr_closure_init(&c->connected, subchannel_connected, c);
  grpc_connectivity_state_init(&c->state_tracker, GRPC_CHANNEL_IDLE);
  gpr_mu_init(&c->mu);
  return c;
}

static void start_connect(grpc_subchannel *c) {
	grpc_connect_in_args args;

	args.interested_parties = &c->pollset_set;
	args.addr = c->addr;
	args.addr_len = c->addr_len;
	args.deadline = compute_connect_deadline(c);
	args.channel_args = c->args;
	args.metadata_context = c->mdctx;

  grpc_connector_connect(c->connector, &args, &c->connecting_result, &c->connected);
}

void grpc_subchannel_create_call(grpc_subchannel *c,
                                 grpc_transport_stream_op *initial_op,
                                 grpc_subchannel_call **target,
                                 grpc_iomgr_closure *notify) {
  connection *con;
  gpr_mu_lock(&c->mu);
  if (c->active != NULL) {
    con = c->active;
    CONNECTION_REF(con, "call");
    gpr_mu_unlock(&c->mu);

    *target = create_call(con, initial_op);
    notify->cb(notify->cb_arg, 1);
  } else {
    waiting_for_connect *w4c = gpr_malloc(sizeof(*w4c));
    w4c->next = c->waiting;
    w4c->notify = notify;
    w4c->initial_op = initial_op;
    w4c->target = target;
    c->waiting = w4c;
    grpc_subchannel_add_interested_party(c, initial_op->bind_pollset);
    if (!c->connecting) {
      c->connecting = 1;
      connectivity_state_changed_locked(c);
      GRPC_SUBCHANNEL_REF(c, "connection");
      gpr_mu_unlock(&c->mu);

      start_connect(c);
    } else {
      gpr_mu_unlock(&c->mu);
    }
  }
}

grpc_connectivity_state grpc_subchannel_check_connectivity(grpc_subchannel *c) {
  grpc_connectivity_state state;
  gpr_mu_lock(&c->mu);
  state = grpc_connectivity_state_check(&c->state_tracker);
  gpr_mu_unlock(&c->mu);
  return state;
}

void grpc_subchannel_notify_on_state_change(grpc_subchannel *c,
                                            grpc_connectivity_state *state,
                                            grpc_iomgr_closure *notify) {
  int do_connect = 0;
  gpr_mu_lock(&c->mu);
  if (grpc_connectivity_state_notify_on_state_change(&c->state_tracker, state, notify)) {
  	do_connect = 1;
    c->connecting = 1;
    GRPC_SUBCHANNEL_REF(c, "connection");
    grpc_connectivity_state_set(&c->state_tracker, compute_connectivity_locked(c));
  }
  gpr_mu_unlock(&c->mu);
  if (do_connect) {
  	start_connect(c);
  }
}

void grpc_subchannel_process_transport_op(grpc_subchannel *c, grpc_transport_op *op) {
	abort();
}

static void publish_transport(grpc_subchannel *c) {
	size_t channel_stack_size;
	connection *con;
	grpc_channel_stack *stk;
	size_t num_filters;
	const grpc_channel_filter **filters;
	waiting_for_connect *w4c;

	num_filters = c->num_filters + c->connecting_result.num_filters + 1;
	filters = gpr_malloc(sizeof(*filters) * num_filters);
	memcpy(filters, c->filters, sizeof(*filters) * c->num_filters);
	memcpy(filters + c->num_filters, c->connecting_result.filters, sizeof(*filters) * c->connecting_result.num_filters);
	filters[num_filters - 1] = &grpc_connected_channel_filter;

	channel_stack_size = grpc_channel_stack_size(filters, num_filters);
	con = gpr_malloc(sizeof(connection) + channel_stack_size);
	stk = (grpc_channel_stack *)(con + 1);

	gpr_ref_init(&con->refs, 1);
	con->subchannel = c;
	grpc_channel_stack_init(filters, num_filters, c->args, c->mdctx, stk);
	grpc_connected_channel_bind_transport(stk, c->connecting_result.transport);
	memset(&c->connecting_result, 0, sizeof(c->connecting_result));

	gpr_mu_lock(&c->mu);
	GPR_ASSERT(c->active == NULL);
	c->active = con;
	c->connecting = 0;
	connectivity_state_changed_locked(c);
	while ((w4c = c->waiting)) {
		abort(); /* not implemented */
	}
	gpr_mu_unlock(&c->mu);

	gpr_free(filters);
} 

static void subchannel_connected(void *arg, int iomgr_success) {
	grpc_subchannel *c = arg;
	if (c->connecting_result.transport) {
		publish_transport(c);
	} else {
		GRPC_SUBCHANNEL_UNREF(c, "connection");
		/* TODO(ctiller): retry after sleeping */
		abort();
	}
}

static gpr_timespec compute_connect_deadline(grpc_subchannel *c) {
  return gpr_time_add(gpr_now(), gpr_time_from_seconds(60));
}

static grpc_connectivity_state compute_connectivity_locked(grpc_subchannel *c) {
  if (c->connecting) {
    return GRPC_CHANNEL_CONNECTING;
  }
  if (c->active) {
    return GRPC_CHANNEL_READY;
  }
  return GRPC_CHANNEL_IDLE;
}

static void connectivity_state_changed_locked(grpc_subchannel *c) {
  grpc_connectivity_state current = compute_connectivity_locked(c);
  grpc_connectivity_state_set(&c->state_tracker, current);
}

/*
 * grpc_subchannel_call implementation
 */

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
void grpc_subchannel_call_ref(grpc_subchannel_call *c, const char *file, int line, const char *reason) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "SUBCALL:%p   ref %d -> %d %s",
          c, (int)c->refs.count, (int)c->refs.count + 1,
          reason);
#else
void grpc_subchannel_call_ref(grpc_subchannel_call *c) { 
#endif
  gpr_ref(&c->refs);
}

#ifdef GRPC_SUBCHANNEL_REFCOUNT_DEBUG
void grpc_subchannel_call_unref(grpc_subchannel_call *c, const char *file, int line, const char *reason) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "SUBCALL:%p unref %d -> %d %s",
          c, (int)c->refs.count, (int)c->refs.count - 1,
          reason);
#else
void grpc_subchannel_call_unref(grpc_subchannel_call *c) {
#endif
  if (gpr_unref(&c->refs)) {
    grpc_call_stack_destroy(SUBCHANNEL_CALL_TO_CALL_STACK(c));
    CONNECTION_UNREF(c->connection, "call");
    gpr_free(c);
  }
}

void grpc_subchannel_call_process_op(grpc_subchannel_call *call,
                                     grpc_transport_stream_op *op) {
  grpc_call_stack *call_stack = SUBCHANNEL_CALL_TO_CALL_STACK(call);
  grpc_call_element *top_elem = grpc_call_stack_element(call_stack, 0);
  top_elem->filter->start_transport_stream_op(top_elem, op);
}

grpc_subchannel_call *create_call(connection *con, grpc_transport_stream_op *initial_op) {
	grpc_channel_stack *chanstk = CHANNEL_STACK_FROM_CONNECTION(con);
  grpc_subchannel_call *call = gpr_malloc(sizeof(grpc_subchannel_call) + chanstk->call_stack_size);
  grpc_call_stack *callstk = SUBCHANNEL_CALL_TO_CALL_STACK(call);
  call->connection = con;
  gpr_ref_init(&call->refs, 1);
  grpc_call_stack_init(chanstk, NULL, initial_op, callstk);
  return call;
}
