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

#include "src/core/transport/stream_op.h"

#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

/* Exponential growth function: Given x, return a larger x.
   Currently we grow by 1.5 times upon reallocation. */
#define GROW(x) (3 * (x) / 2)

void grpc_sopb_init(grpc_stream_op_buffer *sopb) {
  sopb->ops = sopb->inlined_ops;
  sopb->nops = 0;
  sopb->capacity = GRPC_SOPB_INLINE_ELEMENTS;
}

void grpc_sopb_destroy(grpc_stream_op_buffer *sopb) {
  grpc_stream_ops_unref_owned_objects(sopb->ops, sopb->nops);
  if (sopb->ops != sopb->inlined_ops) gpr_free(sopb->ops);
}

void grpc_sopb_reset(grpc_stream_op_buffer *sopb) {
  grpc_stream_ops_unref_owned_objects(sopb->ops, sopb->nops);
  sopb->nops = 0;
}

void grpc_sopb_swap(grpc_stream_op_buffer *a, grpc_stream_op_buffer *b) {
  grpc_stream_op_buffer temp = *a;
  *a = *b;
  *b = temp;

  if (a->ops == b->inlined_ops) {
    a->ops = a->inlined_ops;
  }
  if (b->ops == a->inlined_ops) {
    b->ops = b->inlined_ops;
  }
}

void grpc_stream_ops_unref_owned_objects(grpc_stream_op *ops, size_t nops) {
  size_t i;
  for (i = 0; i < nops; i++) {
    switch (ops[i].type) {
      case GRPC_OP_SLICE:
        gpr_slice_unref(ops[i].data.slice);
        break;
      case GRPC_OP_METADATA:
        grpc_metadata_batch_destroy(&ops[i].data.metadata);
        break;
      case GRPC_OP_FLOW_CTL_CB:
        ops[i].data.flow_ctl_cb.cb(ops[i].data.flow_ctl_cb.arg, GRPC_OP_ERROR);
        break;
      case GRPC_NO_OP:
      case GRPC_OP_BEGIN_MESSAGE:
        break;
    }
  }
}

static void assert_contained_metadata_ok(grpc_stream_op *ops, size_t nops) {
#ifndef NDEBUG
  size_t i;
  for (i = 0; i < nops; i++) {
    if (ops[i].type == GRPC_OP_METADATA) {
      grpc_metadata_batch_assert_ok(&ops[i].data.metadata);
    }
  }
#endif
}

static void expandto(grpc_stream_op_buffer *sopb, size_t new_capacity) {
  sopb->capacity = new_capacity;
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
  if (sopb->ops == sopb->inlined_ops) {
    sopb->ops = gpr_malloc(sizeof(grpc_stream_op) * new_capacity);
    memcpy(sopb->ops, sopb->inlined_ops, sopb->nops * sizeof(grpc_stream_op));
  } else {
    sopb->ops = gpr_realloc(sopb->ops, sizeof(grpc_stream_op) * new_capacity);
  }
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

static grpc_stream_op *add(grpc_stream_op_buffer *sopb) {
  grpc_stream_op *out;

  assert_contained_metadata_ok(sopb->ops, sopb->nops);

  if (sopb->nops == sopb->capacity) {
    expandto(sopb, GROW(sopb->capacity));
  }
  out = sopb->ops + sopb->nops;
  sopb->nops++;
  return out;
}

void grpc_sopb_add_no_op(grpc_stream_op_buffer *sopb) {
  add(sopb)->type = GRPC_NO_OP;
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

void grpc_sopb_add_begin_message(grpc_stream_op_buffer *sopb, gpr_uint32 length,
                                 gpr_uint32 flags) {
  grpc_stream_op *op = add(sopb);
  op->type = GRPC_OP_BEGIN_MESSAGE;
  op->data.begin_message.length = length;
  op->data.begin_message.flags = flags;
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

void grpc_sopb_add_metadata(grpc_stream_op_buffer *sopb,
                            grpc_metadata_batch b) {
  grpc_stream_op *op = add(sopb);
  grpc_metadata_batch_assert_ok(&b);
  op->type = GRPC_OP_METADATA;
  op->data.metadata = b;
  grpc_metadata_batch_assert_ok(&op->data.metadata);
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

void grpc_sopb_add_slice(grpc_stream_op_buffer *sopb, gpr_slice slice) {
  grpc_stream_op *op = add(sopb);
  op->type = GRPC_OP_SLICE;
  op->data.slice = slice;
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

void grpc_sopb_add_flow_ctl_cb(grpc_stream_op_buffer *sopb,
                               void (*cb)(void *arg, grpc_op_error error),
                               void *arg) {
  grpc_stream_op *op = add(sopb);
  op->type = GRPC_OP_FLOW_CTL_CB;
  op->data.flow_ctl_cb.cb = cb;
  op->data.flow_ctl_cb.arg = arg;
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

void grpc_sopb_append(grpc_stream_op_buffer *sopb, grpc_stream_op *ops,
                      size_t nops) {
  size_t orig_nops = sopb->nops;
  size_t new_nops = orig_nops + nops;

  assert_contained_metadata_ok(ops, nops);
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
  if (new_nops > sopb->capacity) {
    expandto(sopb, GPR_MAX(GROW(sopb->capacity), new_nops));
  }

  memcpy(sopb->ops + orig_nops, ops, sizeof(grpc_stream_op) * nops);
  sopb->nops = new_nops;
  assert_contained_metadata_ok(sopb->ops, sopb->nops);
}

static void assert_valid_list(grpc_mdelem_list *list) {
#ifndef NDEBUG
  grpc_linked_mdelem *l;

  GPR_ASSERT((list->head == NULL) == (list->tail == NULL));
  if (!list->head) return;
  GPR_ASSERT(list->head->prev == NULL);
  GPR_ASSERT(list->tail->next == NULL);
  GPR_ASSERT((list->head == list->tail) == (list->head->next == NULL));

  for (l = list->head; l; l = l->next) {
    GPR_ASSERT(l->md);
    GPR_ASSERT((l->prev == NULL) == (l == list->head));
    GPR_ASSERT((l->next == NULL) == (l == list->tail));
    if (l->next) GPR_ASSERT(l->next->prev == l);
    if (l->prev) GPR_ASSERT(l->prev->next == l);
  }
#endif
}

#ifndef NDEBUG
void grpc_metadata_batch_assert_ok(grpc_metadata_batch *comd) {
  assert_valid_list(&comd->list);
  assert_valid_list(&comd->garbage);
}
#endif

void grpc_metadata_batch_init(grpc_metadata_batch *comd) {
  comd->list.head = comd->list.tail = comd->garbage.head = comd->garbage.tail =
      NULL;
  comd->deadline = gpr_inf_future;
}

void grpc_metadata_batch_destroy(grpc_metadata_batch *comd) {
  grpc_linked_mdelem *l;
  for (l = comd->list.head; l; l = l->next) {
    grpc_mdelem_unref(l->md);
  }
  for (l = comd->garbage.head; l; l = l->next) {
    grpc_mdelem_unref(l->md);
  }
}

void grpc_metadata_batch_add_head(grpc_metadata_batch *comd,
                                  grpc_linked_mdelem *storage,
                                  grpc_mdelem *elem_to_add) {
  GPR_ASSERT(elem_to_add);
  storage->md = elem_to_add;
  grpc_metadata_batch_link_head(comd, storage);
}

static void link_head(grpc_mdelem_list *list, grpc_linked_mdelem *storage) {
  assert_valid_list(list);
  GPR_ASSERT(storage->md);
  storage->prev = NULL;
  storage->next = list->head;
  if (list->head != NULL) {
    list->head->prev = storage;
  } else {
    list->tail = storage;
  }
  list->head = storage;
  assert_valid_list(list);
}

void grpc_metadata_batch_link_head(grpc_metadata_batch *comd,
                                   grpc_linked_mdelem *storage) {
  link_head(&comd->list, storage);
}

void grpc_metadata_batch_add_tail(grpc_metadata_batch *comd,
                                  grpc_linked_mdelem *storage,
                                  grpc_mdelem *elem_to_add) {
  GPR_ASSERT(elem_to_add);
  storage->md = elem_to_add;
  grpc_metadata_batch_link_tail(comd, storage);
}

static void link_tail(grpc_mdelem_list *list, grpc_linked_mdelem *storage) {
  assert_valid_list(list);
  GPR_ASSERT(storage->md);
  storage->prev = list->tail;
  storage->next = NULL;
  if (list->tail != NULL) {
    list->tail->next = storage;
  } else {
    list->head = storage;
  }
  list->tail = storage;
  assert_valid_list(list);
}

void grpc_metadata_batch_link_tail(grpc_metadata_batch *comd,
                                   grpc_linked_mdelem *storage) {
  link_tail(&comd->list, storage);
}

void grpc_metadata_batch_merge(grpc_metadata_batch *target,
                               grpc_metadata_batch *add) {
  grpc_linked_mdelem *l;
  grpc_linked_mdelem *next;
  for (l = add->list.head; l; l = next) {
    next = l->next;
    link_tail(&target->list, l);
  }
  for (l = add->garbage.head; l; l = next) {
    next = l->next;
    link_tail(&target->garbage, l);
  }
}

void grpc_metadata_batch_filter(grpc_metadata_batch *comd,
                                grpc_mdelem *(*filter)(void *user_data,
                                                       grpc_mdelem *elem),
                                void *user_data) {
  grpc_linked_mdelem *l;
  grpc_linked_mdelem *next;

  assert_valid_list(&comd->list);
  assert_valid_list(&comd->garbage);
  for (l = comd->list.head; l; l = next) {
    grpc_mdelem *orig = l->md;
    grpc_mdelem *filt = filter(user_data, orig);
    next = l->next;
    if (filt == NULL) {
      if (l->prev) {
        l->prev->next = l->next;
      }
      if (l->next) {
        l->next->prev = l->prev;
      }
      if (comd->list.head == l) {
        comd->list.head = l->next;
      }
      if (comd->list.tail == l) {
        comd->list.tail = l->prev;
      }
      assert_valid_list(&comd->list);
      link_head(&comd->garbage, l);
    } else if (filt != orig) {
      grpc_mdelem_unref(orig);
      l->md = filt;
    }
  }
  assert_valid_list(&comd->list);
  assert_valid_list(&comd->garbage);
}
