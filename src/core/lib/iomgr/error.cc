/*
 *
 * Copyright 2016 gRPC authors.
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
 *
 */
#include <grpc/support/port_platform.h>

#include "src/core/lib/iomgr/error.h"

#include <inttypes.h>
#include <string.h>

#include <grpc/status.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>

#ifdef GPR_WINDOWS
#include <grpc/support/log_windows.h>
#endif

#include "src/core/lib/debug/trace.h"
#include "src/core/lib/gpr/useful.h"
#include "src/core/lib/iomgr/error_internal.h"
#include "src/core/lib/profiling/timers.h"
#include "src/core/lib/slice/slice_internal.h"

grpc_core::DebugOnlyTraceFlag grpc_trace_error_refcount(false,
                                                        "error_refcount");
grpc_core::DebugOnlyTraceFlag grpc_trace_closure(false, "closure");

static const char* error_int_name(grpc_error_ints key) {
  switch (key) {
    case GRPC_ERROR_INT_ERRNO:
      return "errno";
    case GRPC_ERROR_INT_FILE_LINE:
      return "file_line";
    case GRPC_ERROR_INT_STREAM_ID:
      return "stream_id";
    case GRPC_ERROR_INT_GRPC_STATUS:
      return "grpc_status";
    case GRPC_ERROR_INT_OFFSET:
      return "offset";
    case GRPC_ERROR_INT_INDEX:
      return "index";
    case GRPC_ERROR_INT_SIZE:
      return "size";
    case GRPC_ERROR_INT_HTTP2_ERROR:
      return "http2_error";
    case GRPC_ERROR_INT_TSI_CODE:
      return "tsi_code";
    case GRPC_ERROR_INT_SECURITY_STATUS:
      return "security_status";
    case GRPC_ERROR_INT_FD:
      return "fd";
    case GRPC_ERROR_INT_WSA_ERROR:
      return "wsa_error";
    case GRPC_ERROR_INT_HTTP_STATUS:
      return "http_status";
    case GRPC_ERROR_INT_LIMIT:
      return "limit";
    case GRPC_ERROR_INT_OCCURRED_DURING_WRITE:
      return "occurred_during_write";
    case GRPC_ERROR_INT_CHANNEL_CONNECTIVITY_STATE:
      return "channel_connectivity_state";
    case GRPC_ERROR_INT_MAX:
      GPR_UNREACHABLE_CODE(return "unknown");
  }
  GPR_UNREACHABLE_CODE(return "unknown");
}

static const char* error_str_name(grpc_error_strs key) {
  switch (key) {
    case GRPC_ERROR_STR_KEY:
      return "key";
    case GRPC_ERROR_STR_VALUE:
      return "value";
    case GRPC_ERROR_STR_DESCRIPTION:
      return "description";
    case GRPC_ERROR_STR_OS_ERROR:
      return "os_error";
    case GRPC_ERROR_STR_TARGET_ADDRESS:
      return "target_address";
    case GRPC_ERROR_STR_SYSCALL:
      return "syscall";
    case GRPC_ERROR_STR_FILE:
      return "file";
    case GRPC_ERROR_STR_GRPC_MESSAGE:
      return "grpc_message";
    case GRPC_ERROR_STR_RAW_BYTES:
      return "raw_bytes";
    case GRPC_ERROR_STR_TSI_ERROR:
      return "tsi_error";
    case GRPC_ERROR_STR_FILENAME:
      return "filename";
    case GRPC_ERROR_STR_QUEUED_BUFFERS:
      return "queued_buffers";
    case GRPC_ERROR_STR_MAX:
      GPR_UNREACHABLE_CODE(return "unknown");
  }
  GPR_UNREACHABLE_CODE(return "unknown");
}

static const char* error_time_name(grpc_error_times key) {
  switch (key) {
    case GRPC_ERROR_TIME_CREATED:
      return "created";
    case GRPC_ERROR_TIME_MAX:
      GPR_UNREACHABLE_CODE(return "unknown");
  }
  GPR_UNREACHABLE_CODE(return "unknown");
}

#ifndef NDEBUG
grpc_error* grpc_error_do_ref(grpc_error* err, const char* file, int line) {
  if (grpc_trace_error_refcount.enabled()) {
    gpr_log(GPR_DEBUG, "%p: %" PRIdPTR " -> %" PRIdPTR " [%s:%d]", err,
            gpr_atm_no_barrier_load(&err->atomics.refs.count),
            gpr_atm_no_barrier_load(&err->atomics.refs.count) + 1, file, line);
  }
  gpr_ref(&err->atomics.refs);
  return err;
}
#else
grpc_error* grpc_error_do_ref(grpc_error* err) {
  gpr_ref(&err->atomics.refs);
  return err;
}
#endif

static void unref_errs(grpc_error* err) {
  uint8_t slot = err->first_err;
  while (slot != UINT8_MAX) {
    grpc_linked_error* lerr =
        reinterpret_cast<grpc_linked_error*>(err->arena + slot);
    GRPC_ERROR_UNREF(lerr->err);
    GPR_ASSERT(err->last_err == slot ? lerr->next == UINT8_MAX
                                     : lerr->next != UINT8_MAX);
    slot = lerr->next;
  }
}

static void unref_strs(grpc_error* err) {
  for (size_t which = 0; which < GRPC_ERROR_STR_MAX; ++which) {
    uint8_t slot = err->strs[which];
    if (slot != UINT8_MAX) {
      grpc_slice_unref_internal(
          *reinterpret_cast<grpc_slice*>(err->arena + slot));
    }
  }
}

static void error_destroy(grpc_error* err) {
  GPR_ASSERT(!grpc_error_is_special(err));
  unref_errs(err);
  unref_strs(err);
  gpr_free(
      reinterpret_cast<void*>(gpr_atm_acq_load(&err->atomics.error_string)));
  gpr_free(err);
}

#ifndef NDEBUG
void grpc_error_do_unref(grpc_error* err, const char* file, int line) {
  if (grpc_trace_error_refcount.enabled()) {
    gpr_log(GPR_DEBUG, "%p: %" PRIdPTR " -> %" PRIdPTR " [%s:%d]", err,
            gpr_atm_no_barrier_load(&err->atomics.refs.count),
            gpr_atm_no_barrier_load(&err->atomics.refs.count) - 1, file, line);
  }
  if (gpr_unref(&err->atomics.refs)) {
    error_destroy(err);
  }
}
#else
void grpc_error_do_unref(grpc_error* err) {
  if (gpr_unref(&err->atomics.refs)) {
    error_destroy(err);
  }
}
#endif

static uint8_t get_placement(grpc_error** err, size_t size) {
  GPR_ASSERT(*err);
  uint8_t slots = static_cast<uint8_t>(size / sizeof(intptr_t));
  if ((*err)->arena_size + slots > (*err)->arena_capacity) {
    (*err)->arena_capacity = static_cast<uint8_t> GPR_MIN(
        UINT8_MAX - 1, (3 * (*err)->arena_capacity / 2));
    if ((*err)->arena_size + slots > (*err)->arena_capacity) {
      return UINT8_MAX;
    }
#ifndef NDEBUG
    grpc_error* orig = *err;
#endif
    *err = static_cast<grpc_error*>(gpr_realloc(
        *err, sizeof(grpc_error) + (*err)->arena_capacity * sizeof(intptr_t)));
#ifndef NDEBUG
    if (grpc_trace_error_refcount.enabled()) {
      if (*err != orig) {
        gpr_log(GPR_DEBUG, "realloc %p -> %p", orig, *err);
      }
    }
#endif
  }
  uint8_t placement = (*err)->arena_size;
  (*err)->arena_size = static_cast<uint8_t>((*err)->arena_size + slots);
  return placement;
}

static void internal_set_int(grpc_error** err, grpc_error_ints which,
                             intptr_t value) {
  uint8_t slot = (*err)->ints[which];
  if (slot == UINT8_MAX) {
    slot = get_placement(err, sizeof(value));
    if (slot == UINT8_MAX) {
      gpr_log(GPR_ERROR, "Error %p is full, dropping int {\"%s\":%" PRIiPTR "}",
              *err, error_int_name(which), value);
      return;
    }
  }
  (*err)->ints[which] = slot;
  (*err)->arena[slot] = value;
}

static void internal_set_str(grpc_error** err, grpc_error_strs which,
                             const grpc_slice& value) {
  uint8_t slot = (*err)->strs[which];
  if (slot == UINT8_MAX) {
    slot = get_placement(err, sizeof(value));
    if (slot == UINT8_MAX) {
      char* str = grpc_slice_to_c_string(value);
      gpr_log(GPR_ERROR, "Error %p is full, dropping string {\"%s\":\"%s\"}",
              *err, error_str_name(which), str);
      gpr_free(str);
      return;
    }
  } else {
    grpc_slice_unref_internal(
        *reinterpret_cast<grpc_slice*>((*err)->arena + slot));
  }
  (*err)->strs[which] = slot;
  memcpy((*err)->arena + slot, &value, sizeof(value));
}

static char* fmt_time(gpr_timespec tm);
static void internal_set_time(grpc_error** err, grpc_error_times which,
                              gpr_timespec value) {
  uint8_t slot = (*err)->times[which];
  if (slot == UINT8_MAX) {
    slot = get_placement(err, sizeof(value));
    if (slot == UINT8_MAX) {
      char* time_str = fmt_time(value);
      gpr_log(GPR_ERROR, "Error %p is full, dropping \"%s\":\"%s\"}", *err,
              error_time_name(which), time_str);
      gpr_free(time_str);
      return;
    }
  }
  (*err)->times[which] = slot;
  memcpy((*err)->arena + slot, &value, sizeof(value));
}

static void internal_add_error(grpc_error** err, grpc_error* new_err) {
  grpc_linked_error new_last = {new_err, UINT8_MAX};
  uint8_t slot = get_placement(err, sizeof(grpc_linked_error));
  if (slot == UINT8_MAX) {
    gpr_log(GPR_ERROR, "Error %p is full, dropping error %p = %s", *err,
            new_err, grpc_error_string(new_err));
    GRPC_ERROR_UNREF(new_err);
    return;
  }
  if ((*err)->first_err == UINT8_MAX) {
    GPR_ASSERT((*err)->last_err == UINT8_MAX);
    (*err)->last_err = slot;
    (*err)->first_err = slot;
  } else {
    GPR_ASSERT((*err)->last_err != UINT8_MAX);
    grpc_linked_error* old_last =
        reinterpret_cast<grpc_linked_error*>((*err)->arena + (*err)->last_err);
    old_last->next = slot;
    (*err)->last_err = slot;
  }
  memcpy((*err)->arena + slot, &new_last, sizeof(grpc_linked_error));
}

#define SLOTS_PER_INT (1)  // == (sizeof(intptr_t) / sizeof(intptr_t))
#define SLOTS_PER_STR (sizeof(grpc_slice) / sizeof(intptr_t))
#define SLOTS_PER_TIME (sizeof(gpr_timespec) / sizeof(intptr_t))
#define SLOTS_PER_LINKED_ERROR (sizeof(grpc_linked_error) / sizeof(intptr_t))

// size of storing one int and two slices and a timespec. For line, desc, file,
// and time created
#define DEFAULT_ERROR_CAPACITY \
  (SLOTS_PER_INT + (SLOTS_PER_STR * 2) + SLOTS_PER_TIME)

// It is very common to include and extra int and string in an error
#define SURPLUS_CAPACITY (2 * SLOTS_PER_INT + SLOTS_PER_TIME)

static gpr_atm g_error_creation_allowed = true;

void grpc_disable_error_creation() {
  gpr_atm_no_barrier_store(&g_error_creation_allowed, false);
}

void grpc_enable_error_creation() {
  gpr_atm_no_barrier_store(&g_error_creation_allowed, true);
}

grpc_error* grpc_error_create(const char* file, int line,
                              const grpc_slice& desc, grpc_error** referencing,
                              size_t num_referencing) {
  GPR_TIMER_SCOPE("grpc_error_create", 0);
  uint8_t initial_arena_capacity = static_cast<uint8_t>(
      DEFAULT_ERROR_CAPACITY +
      static_cast<uint8_t>(num_referencing * SLOTS_PER_LINKED_ERROR) +
      SURPLUS_CAPACITY);
  grpc_error* err = static_cast<grpc_error*>(
      gpr_malloc(sizeof(*err) + initial_arena_capacity * sizeof(intptr_t)));
  if (err == nullptr) {  // TODO(ctiller): make gpr_malloc return NULL
    return GRPC_ERROR_OOM;
  }
#ifndef NDEBUG
  if (!gpr_atm_no_barrier_load(&g_error_creation_allowed)) {
    gpr_log(GPR_ERROR,
            "Error creation occurred when error creation was disabled [%s:%d]",
            file, line);
    abort();
  }
  if (grpc_trace_error_refcount.enabled()) {
    gpr_log(GPR_DEBUG, "%p create [%s:%d]", err, file, line);
  }
#endif

  err->arena_size = 0;
  err->arena_capacity = initial_arena_capacity;
  err->first_err = UINT8_MAX;
  err->last_err = UINT8_MAX;

  memset(err->ints, UINT8_MAX, GRPC_ERROR_INT_MAX);
  memset(err->strs, UINT8_MAX, GRPC_ERROR_STR_MAX);
  memset(err->times, UINT8_MAX, GRPC_ERROR_TIME_MAX);

  internal_set_int(&err, GRPC_ERROR_INT_FILE_LINE, line);
  internal_set_str(&err, GRPC_ERROR_STR_FILE,
                   grpc_slice_from_static_string(file));
  internal_set_str(&err, GRPC_ERROR_STR_DESCRIPTION, desc);

  for (size_t i = 0; i < num_referencing; ++i) {
    if (referencing[i] == GRPC_ERROR_NONE) continue;
    internal_add_error(
        &err,
        GRPC_ERROR_REF(
            referencing[i]));  // TODO(ncteisen), change ownership semantics
  }

  internal_set_time(&err, GRPC_ERROR_TIME_CREATED, gpr_now(GPR_CLOCK_REALTIME));

  gpr_atm_no_barrier_store(&err->atomics.error_string, 0);
  gpr_ref_init(&err->atomics.refs, 1);
  return err;
}

static void ref_strs(grpc_error* err) {
  for (size_t i = 0; i < GRPC_ERROR_STR_MAX; ++i) {
    uint8_t slot = err->strs[i];
    if (slot != UINT8_MAX) {
      grpc_slice_ref_internal(
          *reinterpret_cast<grpc_slice*>(err->arena + slot));
    }
  }
}

static void ref_errs(grpc_error* err) {
  uint8_t slot = err->first_err;
  while (slot != UINT8_MAX) {
    grpc_linked_error* lerr =
        reinterpret_cast<grpc_linked_error*>(err->arena + slot);
    GRPC_ERROR_REF(lerr->err);
    slot = lerr->next;
  }
}

static grpc_error* copy_error_and_unref(grpc_error* in) {
  GPR_TIMER_SCOPE("copy_error_and_unref", 0);
  grpc_error* out;
  if (grpc_error_is_special(in)) {
    out = GRPC_ERROR_CREATE_FROM_STATIC_STRING("unknown");
    if (in == GRPC_ERROR_NONE) {
      internal_set_str(&out, GRPC_ERROR_STR_DESCRIPTION,
                       grpc_slice_from_static_string("no error"));
      internal_set_int(&out, GRPC_ERROR_INT_GRPC_STATUS, GRPC_STATUS_OK);
    } else if (in == GRPC_ERROR_OOM) {
      internal_set_str(&out, GRPC_ERROR_STR_DESCRIPTION,
                       grpc_slice_from_static_string("oom"));
    } else if (in == GRPC_ERROR_CANCELLED) {
      internal_set_str(&out, GRPC_ERROR_STR_DESCRIPTION,
                       grpc_slice_from_static_string("cancelled"));
      internal_set_int(&out, GRPC_ERROR_INT_GRPC_STATUS, GRPC_STATUS_CANCELLED);
    }
  } else if (gpr_ref_is_unique(&in->atomics.refs)) {
    out = in;
  } else {
    uint8_t new_arena_capacity = in->arena_capacity;
    // the returned err will be added to, so we ensure this is room to avoid
    // unneeded allocations.
    if (in->arena_capacity - in->arena_size <
        static_cast<uint8_t> SLOTS_PER_STR) {
      new_arena_capacity = static_cast<uint8_t>(3 * new_arena_capacity / 2);
    }
    out = static_cast<grpc_error*>(
        gpr_malloc(sizeof(*in) + new_arena_capacity * sizeof(intptr_t)));
#ifndef NDEBUG
    if (grpc_trace_error_refcount.enabled()) {
      gpr_log(GPR_DEBUG, "%p create copying %p", out, in);
    }
#endif
    // bulk memcpy of the rest of the struct.
    // NOLINTNEXTLINE(bugprone-sizeof-expression)
    size_t skip = sizeof(&out->atomics);
    memcpy(reinterpret_cast<void*>((uintptr_t)out + skip),
           (void*)((uintptr_t)in + skip),
           sizeof(*in) + (in->arena_size * sizeof(intptr_t)) - skip);
    // manually set the atomics and the new capacity
    gpr_atm_no_barrier_store(&out->atomics.error_string, 0);
    gpr_ref_init(&out->atomics.refs, 1);
    out->arena_capacity = new_arena_capacity;
    ref_strs(out);
    ref_errs(out);
    GRPC_ERROR_UNREF(in);
  }
  return out;
}

grpc_error* grpc_error_set_int(grpc_error* src, grpc_error_ints which,
                               intptr_t value) {
  GPR_TIMER_SCOPE("grpc_error_set_int", 0);
  grpc_error* new_err = copy_error_and_unref(src);
  internal_set_int(&new_err, which, value);
  return new_err;
}

struct special_error_status_map {
  grpc_status_code code;
  const char* msg;
  size_t len;
};
const special_error_status_map error_status_map[] = {
    {GRPC_STATUS_OK, "", 0},                // GRPC_ERROR_NONE
    {GRPC_STATUS_INVALID_ARGUMENT, "", 0},  // GRPC_ERROR_RESERVED_1
    {GRPC_STATUS_RESOURCE_EXHAUSTED, "Out of memory",
     strlen("Out of memory")},              // GRPC_ERROR_OOM
    {GRPC_STATUS_INVALID_ARGUMENT, "", 0},  // GRPC_ERROR_RESERVED_2
    {GRPC_STATUS_CANCELLED, "Cancelled",
     strlen("Cancelled")},  // GRPC_ERROR_CANCELLED
};

bool grpc_error_get_int(grpc_error* err, grpc_error_ints which, intptr_t* p) {
  GPR_TIMER_SCOPE("grpc_error_get_int", 0);
  if (grpc_error_is_special(err)) {
    if (which != GRPC_ERROR_INT_GRPC_STATUS) return false;
    *p = error_status_map[reinterpret_cast<size_t>(err)].code;
    return true;
  }
  uint8_t slot = err->ints[which];
  if (slot != UINT8_MAX) {
    if (p != nullptr) *p = err->arena[slot];
    return true;
  }
  return false;
}

grpc_error* grpc_error_set_str(grpc_error* src, grpc_error_strs which,
                               const grpc_slice& str) {
  GPR_TIMER_SCOPE("grpc_error_set_str", 0);
  grpc_error* new_err = copy_error_and_unref(src);
  internal_set_str(&new_err, which, str);
  return new_err;
}

bool grpc_error_get_str(grpc_error* err, grpc_error_strs which,
                        grpc_slice* str) {
  if (grpc_error_is_special(err)) {
    if (which != GRPC_ERROR_STR_GRPC_MESSAGE) return false;
    const special_error_status_map& msg =
        error_status_map[reinterpret_cast<size_t>(err)];
    str->refcount = &grpc_core::kNoopRefcount;
    str->data.refcounted.bytes =
        reinterpret_cast<uint8_t*>(const_cast<char*>(msg.msg));
    str->data.refcounted.length = msg.len;
    return true;
  }
  uint8_t slot = err->strs[which];
  if (slot != UINT8_MAX) {
    *str = *reinterpret_cast<grpc_slice*>(err->arena + slot);
    return true;
  } else {
    return false;
  }
}

grpc_error* grpc_error_add_child(grpc_error* src, grpc_error* child) {
  GPR_TIMER_SCOPE("grpc_error_add_child", 0);
  if (src != GRPC_ERROR_NONE) {
    if (child == GRPC_ERROR_NONE) {
      /* \a child is empty. Simply return the ref to \a src */
      return src;
    } else if (child != src) {
      grpc_error* new_err = copy_error_and_unref(src);
      internal_add_error(&new_err, child);
      return new_err;
    } else {
      /* \a src and \a child are the same. Drop one of the references and return
       * the other */
      GRPC_ERROR_UNREF(child);
      return src;
    }
  } else {
    /* \a src is empty. Simply return the ref to \a child */
    return child;
  }
}

static const char* no_error_string = "\"No Error\"";
static const char* oom_error_string = "\"Out of memory\"";
static const char* cancelled_error_string = "\"Cancelled\"";

struct kv_pair {
  char* key;
  char* value;
};
struct kv_pairs {
  kv_pair* kvs;
  size_t num_kvs;
  size_t cap_kvs;
};
static void append_chr(char c, char** s, size_t* sz, size_t* cap) {
  if (*sz == *cap) {
    *cap = GPR_MAX(8, 3 * *cap / 2);
    *s = static_cast<char*>(gpr_realloc(*s, *cap));
  }
  (*s)[(*sz)++] = c;
}

static void append_str(const char* str, char** s, size_t* sz, size_t* cap) {
  for (const char* c = str; *c; c++) {
    append_chr(*c, s, sz, cap);
  }
}

static void append_esc_str(const uint8_t* str, size_t len, char** s, size_t* sz,
                           size_t* cap) {
  static const char* hex = "0123456789abcdef";
  append_chr('"', s, sz, cap);
  for (size_t i = 0; i < len; i++, str++) {
    if (*str < 32 || *str >= 127) {
      append_chr('\\', s, sz, cap);
      switch (*str) {
        case '\b':
          append_chr('b', s, sz, cap);
          break;
        case '\f':
          append_chr('f', s, sz, cap);
          break;
        case '\n':
          append_chr('n', s, sz, cap);
          break;
        case '\r':
          append_chr('r', s, sz, cap);
          break;
        case '\t':
          append_chr('t', s, sz, cap);
          break;
        default:
          append_chr('u', s, sz, cap);
          append_chr('0', s, sz, cap);
          append_chr('0', s, sz, cap);
          append_chr(hex[*str >> 4], s, sz, cap);
          append_chr(hex[*str & 0x0f], s, sz, cap);
          break;
      }
    } else {
      append_chr(static_cast<char>(*str), s, sz, cap);
    }
  }
  append_chr('"', s, sz, cap);
}

static void append_kv(kv_pairs* kvs, char* key, char* value) {
  if (kvs->num_kvs == kvs->cap_kvs) {
    kvs->cap_kvs = GPR_MAX(3 * kvs->cap_kvs / 2, 4);
    kvs->kvs = static_cast<kv_pair*>(
        gpr_realloc(kvs->kvs, sizeof(*kvs->kvs) * kvs->cap_kvs));
  }
  kvs->kvs[kvs->num_kvs].key = key;
  kvs->kvs[kvs->num_kvs].value = value;
  kvs->num_kvs++;
}

static char* key_int(grpc_error_ints which) {
  return gpr_strdup(error_int_name(which));
}

static char* fmt_int(intptr_t p) {
  char* s;
  gpr_asprintf(&s, "%" PRIdPTR, p);
  return s;
}

static void collect_ints_kvs(grpc_error* err, kv_pairs* kvs) {
  for (size_t which = 0; which < GRPC_ERROR_INT_MAX; ++which) {
    uint8_t slot = err->ints[which];
    if (slot != UINT8_MAX) {
      append_kv(kvs, key_int(static_cast<grpc_error_ints>(which)),
                fmt_int(err->arena[slot]));
    }
  }
}

static char* key_str(grpc_error_strs which) {
  return gpr_strdup(error_str_name(which));
}

static char* fmt_str(const grpc_slice& slice) {
  char* s = nullptr;
  size_t sz = 0;
  size_t cap = 0;
  append_esc_str(GRPC_SLICE_START_PTR(slice), GRPC_SLICE_LENGTH(slice), &s, &sz,
                 &cap);
  append_chr(0, &s, &sz, &cap);
  return s;
}

static void collect_strs_kvs(grpc_error* err, kv_pairs* kvs) {
  for (size_t which = 0; which < GRPC_ERROR_STR_MAX; ++which) {
    uint8_t slot = err->strs[which];
    if (slot != UINT8_MAX) {
      append_kv(kvs, key_str(static_cast<grpc_error_strs>(which)),
                fmt_str(*reinterpret_cast<grpc_slice*>(err->arena + slot)));
    }
  }
}

static char* key_time(grpc_error_times which) {
  return gpr_strdup(error_time_name(which));
}

static char* fmt_time(gpr_timespec tm) {
  char* out;
  const char* pfx = "!!";
  switch (tm.clock_type) {
    case GPR_CLOCK_MONOTONIC:
      pfx = "@monotonic:";
      break;
    case GPR_CLOCK_REALTIME:
      pfx = "@";
      break;
    case GPR_CLOCK_PRECISE:
      pfx = "@precise:";
      break;
    case GPR_TIMESPAN:
      pfx = "";
      break;
  }
  gpr_asprintf(&out, "\"%s%" PRId64 ".%09d\"", pfx, tm.tv_sec, tm.tv_nsec);
  return out;
}

static void collect_times_kvs(grpc_error* err, kv_pairs* kvs) {
  for (size_t which = 0; which < GRPC_ERROR_TIME_MAX; ++which) {
    uint8_t slot = err->times[which];
    if (slot != UINT8_MAX) {
      append_kv(kvs, key_time(static_cast<grpc_error_times>(which)),
                fmt_time(*reinterpret_cast<gpr_timespec*>(err->arena + slot)));
    }
  }
}

static void add_errs(grpc_error* err, char** s, size_t* sz, size_t* cap) {
  uint8_t slot = err->first_err;
  bool first = true;
  while (slot != UINT8_MAX) {
    grpc_linked_error* lerr =
        reinterpret_cast<grpc_linked_error*>(err->arena + slot);
    if (!first) append_chr(',', s, sz, cap);
    first = false;
    const char* e = grpc_error_string(lerr->err);
    append_str(e, s, sz, cap);
    GPR_ASSERT(err->last_err == slot ? lerr->next == UINT8_MAX
                                     : lerr->next != UINT8_MAX);
    slot = lerr->next;
  }
}

static char* errs_string(grpc_error* err) {
  char* s = nullptr;
  size_t sz = 0;
  size_t cap = 0;
  append_chr('[', &s, &sz, &cap);
  add_errs(err, &s, &sz, &cap);
  append_chr(']', &s, &sz, &cap);
  append_chr(0, &s, &sz, &cap);
  return s;
}

static int cmp_kvs(const void* a, const void* b) {
  const kv_pair* ka = static_cast<const kv_pair*>(a);
  const kv_pair* kb = static_cast<const kv_pair*>(b);
  return strcmp(ka->key, kb->key);
}

static char* finish_kvs(kv_pairs* kvs) {
  char* s = nullptr;
  size_t sz = 0;
  size_t cap = 0;

  append_chr('{', &s, &sz, &cap);
  for (size_t i = 0; i < kvs->num_kvs; i++) {
    if (i != 0) append_chr(',', &s, &sz, &cap);
    append_esc_str(reinterpret_cast<const uint8_t*>(kvs->kvs[i].key),
                   strlen(kvs->kvs[i].key), &s, &sz, &cap);
    gpr_free(kvs->kvs[i].key);
    append_chr(':', &s, &sz, &cap);
    append_str(kvs->kvs[i].value, &s, &sz, &cap);
    gpr_free(kvs->kvs[i].value);
  }
  append_chr('}', &s, &sz, &cap);
  append_chr(0, &s, &sz, &cap);

  gpr_free(kvs->kvs);
  return s;
}

const char* grpc_error_string(grpc_error* err) {
  GPR_TIMER_SCOPE("grpc_error_string", 0);
  if (err == GRPC_ERROR_NONE) return no_error_string;
  if (err == GRPC_ERROR_OOM) return oom_error_string;
  if (err == GRPC_ERROR_CANCELLED) return cancelled_error_string;

  void* p =
      reinterpret_cast<void*>(gpr_atm_acq_load(&err->atomics.error_string));
  if (p != nullptr) {
    return static_cast<const char*>(p);
  }

  kv_pairs kvs;
  memset(&kvs, 0, sizeof(kvs));

  collect_ints_kvs(err, &kvs);
  collect_strs_kvs(err, &kvs);
  collect_times_kvs(err, &kvs);
  if (err->first_err != UINT8_MAX) {
    append_kv(&kvs, gpr_strdup("referenced_errors"), errs_string(err));
  }

  qsort(kvs.kvs, kvs.num_kvs, sizeof(kv_pair), cmp_kvs);

  char* out = finish_kvs(&kvs);

  if (!gpr_atm_rel_cas(&err->atomics.error_string, 0, (gpr_atm)out)) {
    gpr_free(out);
    out = reinterpret_cast<char*>(gpr_atm_acq_load(&err->atomics.error_string));
  }

  return out;
}

grpc_error* grpc_os_error(const char* file, int line, int err,
                          const char* call_name) {
  return grpc_error_set_str(
      grpc_error_set_str(
          grpc_error_set_int(
              grpc_error_create(file, line,
                                grpc_slice_from_static_string(strerror(err)),
                                nullptr, 0),
              GRPC_ERROR_INT_ERRNO, err),
          GRPC_ERROR_STR_OS_ERROR,
          grpc_slice_from_static_string(strerror(err))),
      GRPC_ERROR_STR_SYSCALL, grpc_slice_from_copied_string(call_name));
}

#ifdef GPR_WINDOWS
grpc_error* grpc_wsa_error(const char* file, int line, int err,
                           const char* call_name) {
  char* utf8_message = gpr_format_message(err);
  grpc_error* error = grpc_error_set_str(
      grpc_error_set_str(
          grpc_error_set_int(
              grpc_error_create(file, line,
                                grpc_slice_from_static_string("OS Error"), NULL,
                                0),
              GRPC_ERROR_INT_WSA_ERROR, err),
          GRPC_ERROR_STR_OS_ERROR, grpc_slice_from_copied_string(utf8_message)),
      GRPC_ERROR_STR_SYSCALL, grpc_slice_from_static_string(call_name));
  gpr_free(utf8_message);
  return error;
}
#endif

bool grpc_log_error(const char* what, grpc_error* error, const char* file,
                    int line) {
  GPR_DEBUG_ASSERT(error != GRPC_ERROR_NONE);
  const char* msg = grpc_error_string(error);
  gpr_log(file, line, GPR_LOG_SEVERITY_ERROR, "%s: %s", what, msg);
  GRPC_ERROR_UNREF(error);
  return false;
}
