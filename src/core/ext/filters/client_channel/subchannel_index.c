//
//
// Copyright 2016 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//

#include "src/core/ext/filters/client_channel/subchannel_index.h"

#include <stdbool.h>
#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/avl.h>
#include <grpc/support/string_util.h>
#include <grpc/support/tls.h>

#include "src/core/lib/channel/channel_args.h"

// a map of subchannel_key --> subchannel, used for detecting connections
// to the same destination in order to share them
static gpr_avl g_subchannel_index;

static gpr_mu g_mu;

struct grpc_subchannel_key {
  grpc_subchannel_args args;
};

GPR_TLS_DECL(subchannel_index_exec_ctx);

static bool g_force_creation = false;

static void enter_ctx(grpc_exec_ctx *exec_ctx) {
  GPR_ASSERT(gpr_tls_get(&subchannel_index_exec_ctx) == 0);
  gpr_tls_set(&subchannel_index_exec_ctx, (intptr_t)exec_ctx);
}

static void leave_ctx(grpc_exec_ctx *exec_ctx) {
  GPR_ASSERT(gpr_tls_get(&subchannel_index_exec_ctx) == (intptr_t)exec_ctx);
  gpr_tls_set(&subchannel_index_exec_ctx, 0);
}

static grpc_exec_ctx *current_ctx() {
  grpc_exec_ctx *c = (grpc_exec_ctx *)gpr_tls_get(&subchannel_index_exec_ctx);
  GPR_ASSERT(c != NULL);
  return c;
}

static grpc_subchannel_key *create_key(
    const grpc_subchannel_args *args,
    grpc_channel_args *(*copy_channel_args)(const grpc_channel_args *args)) {
  grpc_subchannel_key *k = gpr_malloc(sizeof(*k));
  k->args.filter_count = args->filter_count;
  if (k->args.filter_count > 0) {
    k->args.filters =
        gpr_malloc(sizeof(*k->args.filters) * k->args.filter_count);
    memcpy((grpc_channel_filter *)k->args.filters, args->filters,
           sizeof(*k->args.filters) * k->args.filter_count);
  } else {
    k->args.filters = NULL;
  }
  k->args.args = copy_channel_args(args->args);
  return k;
}

grpc_subchannel_key *grpc_subchannel_key_create(
    const grpc_subchannel_args *args) {
  return create_key(args, grpc_channel_args_normalize);
}

static grpc_subchannel_key *subchannel_key_copy(grpc_subchannel_key *k) {
  return create_key(&k->args, grpc_channel_args_copy);
}

int grpc_subchannel_key_compare(const grpc_subchannel_key *a,
                                const grpc_subchannel_key *b) {
  if (g_force_creation) return false;
  int c = GPR_ICMP(a->args.filter_count, b->args.filter_count);
  if (c != 0) return c;
  if (a->args.filter_count > 0) {
    c = memcmp(a->args.filters, b->args.filters,
               a->args.filter_count * sizeof(*a->args.filters));
    if (c != 0) return c;
  }
  return grpc_channel_args_compare(a->args.args, b->args.args);
}

void grpc_subchannel_key_destroy(grpc_exec_ctx *exec_ctx,
                                 grpc_subchannel_key *k) {
  gpr_free((grpc_channel_args *)k->args.filters);
  grpc_channel_args_destroy(exec_ctx, (grpc_channel_args *)k->args.args);
  gpr_free(k);
}

static void sck_avl_destroy(void *p) {
  grpc_subchannel_key_destroy(current_ctx(), p);
}

static void *sck_avl_copy(void *p) { return subchannel_key_copy(p); }

static long sck_avl_compare(void *a, void *b) {
  return grpc_subchannel_key_compare(a, b);
}

static void scv_avl_destroy(void *p) {
  GRPC_SUBCHANNEL_WEAK_UNREF(current_ctx(), p, "subchannel_index");
}

static void *scv_avl_copy(void *p) {
  GRPC_SUBCHANNEL_WEAK_REF(p, "subchannel_index");
  return p;
}

static const gpr_avl_vtable subchannel_avl_vtable = {
    .destroy_key = sck_avl_destroy,
    .copy_key = sck_avl_copy,
    .compare_keys = sck_avl_compare,
    .destroy_value = scv_avl_destroy,
    .copy_value = scv_avl_copy};

void grpc_subchannel_index_init(void) {
  g_subchannel_index = gpr_avl_create(&subchannel_avl_vtable);
  gpr_mu_init(&g_mu);
  gpr_tls_init(&subchannel_index_exec_ctx);
}

void grpc_subchannel_index_shutdown(void) {
  gpr_mu_destroy(&g_mu);
  gpr_avl_unref(g_subchannel_index);
  gpr_tls_destroy(&subchannel_index_exec_ctx);
}

grpc_subchannel *grpc_subchannel_index_find(grpc_exec_ctx *exec_ctx,
                                            grpc_subchannel_key *key) {
  enter_ctx(exec_ctx);

  // Lock, and take a reference to the subchannel index.
  // We don't need to do the search under a lock as avl's are immutable.
  gpr_mu_lock(&g_mu);
  gpr_avl index = gpr_avl_ref(g_subchannel_index);
  gpr_mu_unlock(&g_mu);

  grpc_subchannel *c =
      GRPC_SUBCHANNEL_REF_FROM_WEAK_REF(gpr_avl_get(index, key), "index_find");
  gpr_avl_unref(index);

  leave_ctx(exec_ctx);
  return c;
}

grpc_subchannel *grpc_subchannel_index_register(grpc_exec_ctx *exec_ctx,
                                                grpc_subchannel_key *key,
                                                grpc_subchannel *constructed) {
  enter_ctx(exec_ctx);

  grpc_subchannel *c = NULL;
  bool need_to_unref_constructed;

  while (c == NULL) {
    need_to_unref_constructed = false;

    // Compare and swap loop:
    // - take a reference to the current index
    gpr_mu_lock(&g_mu);
    gpr_avl index = gpr_avl_ref(g_subchannel_index);
    gpr_mu_unlock(&g_mu);

    // - Check to see if a subchannel already exists
    c = gpr_avl_get(index, key);
    if (c != NULL) {
      c = GRPC_SUBCHANNEL_REF_FROM_WEAK_REF(c, "index_register");
    }
    if (c != NULL) {
      // yes -> we're done
      need_to_unref_constructed = true;
    } else {
      // no -> update the avl and compare/swap
      gpr_avl updated =
          gpr_avl_add(gpr_avl_ref(index), subchannel_key_copy(key),
                      GRPC_SUBCHANNEL_WEAK_REF(constructed, "index_register"));

      // it may happen (but it's expected to be unlikely)
      // that some other thread has changed the index:
      // compare/swap here to check that, and retry as necessary
      gpr_mu_lock(&g_mu);
      if (index.root == g_subchannel_index.root) {
        GPR_SWAP(gpr_avl, updated, g_subchannel_index);
        c = constructed;
      }
      gpr_mu_unlock(&g_mu);

      gpr_avl_unref(updated);
    }
    gpr_avl_unref(index);
  }

  leave_ctx(exec_ctx);

  if (need_to_unref_constructed) {
    GRPC_SUBCHANNEL_UNREF(exec_ctx, constructed, "index_register");
  }

  return c;
}

void grpc_subchannel_index_unregister(grpc_exec_ctx *exec_ctx,
                                      grpc_subchannel_key *key,
                                      grpc_subchannel *constructed) {
  enter_ctx(exec_ctx);

  bool done = false;
  while (!done) {
    // Compare and swap loop:
    // - take a reference to the current index
    gpr_mu_lock(&g_mu);
    gpr_avl index = gpr_avl_ref(g_subchannel_index);
    gpr_mu_unlock(&g_mu);

    // Check to see if this key still refers to the previously
    // registered subchannel
    grpc_subchannel *c = gpr_avl_get(index, key);
    if (c != constructed) {
      gpr_avl_unref(index);
      break;
    }

    // compare and swap the update (some other thread may have
    // mutated the index behind us)
    gpr_avl updated = gpr_avl_remove(gpr_avl_ref(index), key);

    gpr_mu_lock(&g_mu);
    if (index.root == g_subchannel_index.root) {
      GPR_SWAP(gpr_avl, updated, g_subchannel_index);
      done = true;
    }
    gpr_mu_unlock(&g_mu);

    gpr_avl_unref(updated);
    gpr_avl_unref(index);
  }

  leave_ctx(exec_ctx);
}

void grpc_subchannel_index_test_only_set_force_creation(bool force_creation) {
  g_force_creation = force_creation;
}
