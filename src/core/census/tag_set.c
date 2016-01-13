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

#include "tag_set.h"

#include <grpc/census.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/port_platform.h>
#include <grpc/support/sync.h>
#include <grpc/support/useful.h>
#include <stdbool.h>
#include <string.h>
#include "src/core/support/murmur_hash.h"
#include "src/core/support/string.h"

// Functions in this file support the public tag_set API, as well as
// encoding/decoding tag_sets as part of context transmission across
// RPC's. The overall requirements (in approximate priority order) for the
// tag_set representations:
// 1. Efficient conversion to/from wire format
// 2. Minimal bytes used on-wire
// 3. Efficient tag set creation
// 4. Efficient lookup of value for a key
// 5. Efficient lookup of value for an index (to support iteration)
// 6. Minimal memory footprint
//
// Notes on tradeoffs/decisions:
// * tag includes 1 byte length of key, as well as nil-terminating byte. These
//   are to aid in efficient parsing and the ability to directly return key
//   strings. This is more important than saving a single byte/tag on the wire.
// * The wire encoding uses only single byte values. This eliminates the need
//   to handle endian-ness conversions.
// * Keep all tag information (keys/values/flags) in a single memory buffer,
//   that can be directly copied to the wire. This makes iteration by index
//   somewhat less efficient.
// * Binary tags are encoded seperately from non-binary tags. There are a
//   primarily because non-binary tags are far more likely to be repeated
//   across multiple RPC calls, so are more efficiently cached and
//   compressed in any metadata schemes.
// * deleted/modified tags are kept in memory, just marked with a deleted
//   flag. This enables faster processing TODO: benchmark this
// * all lengths etc. are restricted to one byte. This eliminates endian
//   issues.

// Structure representing a set of tags. Essentially a count of number of tags
// present, and pointer to a chunk of memory that contains the per-tag details.
struct tag_set {
  int ntags;        // number of tags.
  int ntags_alloc;  // ntags + number of deleted tags (total number of tags
                    // in all of kvm).
  size_t kvm_size;  // number of bytes allocated for key/value storage.
  size_t kvm_used;  // number of bytes of used key/value memory
  char *kvm;        // key/value memory. Consists of repeated entries of:
                    //   Offset  Size  Description
                    //     0      1    Key length, including trailing 0. (K)
                    //     1      1    Value length. (V)
                    //     2      1    Flags
                    //     3      K    Key bytes
                    //     3 + K  V    Value bytes
                    //
                    // We refer to the first 3 entries as the 'tag header'.
};

// Number of bytes in tag header.
#define TAG_HEADER_SIZE 3  // key length (1) + value length (1) + flags (1)
// Offsets to tag header entries.
#define KEY_LEN_OFFSET 0
#define VALUE_LEN_OFFSET 1
#define FLAG_OFFSET 2

// raw_tag represents the raw-storage form of a tag in the kvm of a tag_set.
struct raw_tag {
  uint8_t key_len;
  uint8_t value_len;
  uint8_t flags;
  char *key;
  char *value;
};

// use reserved flag bit for indication of deleted tag.
#define CENSUS_TAG_DELETED CENSUS_TAG_RESERVED
#define CENSUS_TAG_IS_DELETED(flags) (flags & CENSUS_TAG_DELETED)

// Primary (external) representation of a tag set. Composed of 3 underlying
// tag_set structs, one for each of the binary/printable propagated tags, and
// one for everything else.
struct census_tag_set {
  struct tag_set propagated_tags;
  struct tag_set propagated_binary_tags;
  struct tag_set local_tags;
};

// Extract a raw tag given a pointer (raw) to the tag header. Allow for some
// extra bytes in the tag header (see encode/decode for usage: allows for
// future expansion of the tag header).
static char *decode_tag(struct raw_tag *tag, char *header, int offset) {
  tag->key_len = (uint8_t)(*header++);
  tag->value_len = (uint8_t)(*header++);
  tag->flags = (uint8_t)(*header++);
  header += offset;
  tag->key = header;
  header += tag->key_len;
  tag->value = header;
  return header + tag->value_len;
}

// Make a copy (in 'to') of an existing tag_set.
static void tag_set_copy(struct tag_set *to, const struct tag_set *from) {
  memcpy(to, from, sizeof(struct tag_set));
  to->kvm = gpr_malloc(to->kvm_size);
  memcpy(to->kvm, from->kvm, to->kvm_used);
}

// We may want to keep information about a deleted tag for a short time,
// in case we can reuse the space (same tag is reinserted). This structure
// is used for that purpose.
struct deleted_tag_info {
  struct raw_tag raw;    // raw tag information.
  uint8_t *raw_flags_p;  // pointer to original flags
  struct tag_set *tags;  // the tag set from which tag was deleted.
};

// Delete a tag from a tag set, if it exists. Returns true if the tag was
// originally present (and is now deleted), false if it wasn't.
static bool tag_set_delete_tag(struct tag_set *tags,
                               struct deleted_tag_info *dtag, const char *key,
                               size_t key_len) {
  char *kvp = tags->kvm;
  for (int i = 0; i < tags->ntags_alloc; i++) {
    dtag->raw_flags_p = (uint8_t *)(kvp + FLAG_OFFSET);
    kvp = decode_tag(&dtag->raw, kvp, 0);
    if (CENSUS_TAG_IS_DELETED(dtag->raw.flags)) continue;
    if ((key_len == dtag->raw.key_len) &&
        (memcmp(key, dtag->raw.key, key_len) == 0)) {
      *(dtag->raw_flags_p) |= CENSUS_TAG_DELETED;
      tags->ntags--;
      return true;
    }
  }
  return false;
}

// Delete a tag from a tag set. If the tag is found in any of the underlying
// tag sets, *and* that tag set corresponds to the one in which the tag (if
// later inserted) would be placed, then fills in dtag, and returns true.
// Returns false otherwise. This information is later used to optimize the
// placement of the tag if the value space can be reused, effectively
// "undeleting" the tag.
static bool cts_delete_tag(census_tag_set *tags, const census_tag *tag,
                           size_t key_len, struct deleted_tag_info *dtag) {
  // use the to-be-deleted tag flags as a hint to determine the order in which
  // we delete from the underlying tag sets.
  if (CENSUS_TAG_IS_PROPAGATED(tag->flags)) {
    if (CENSUS_TAG_IS_BINARY(tag->flags)) {
      if (tag_set_delete_tag(&tags->propagated_binary_tags, dtag, tag->key,
                             key_len)) {
        dtag->tags = &tags->propagated_binary_tags;
        return true;
      }
      if (tag_set_delete_tag(&tags->propagated_tags, dtag, tag->key, key_len))
        return false;
      tag_set_delete_tag(&tags->local_tags, dtag, tag->key, key_len);
    } else {
      if (tag_set_delete_tag(&tags->propagated_tags, dtag, tag->key, key_len)) {
        dtag->tags = &tags->propagated_tags;
        return true;
      }
      if (tag_set_delete_tag(&tags->propagated_binary_tags, dtag, tag->key,
                             key_len))
        return false;
      tag_set_delete_tag(&tags->local_tags, dtag, tag->key, key_len);
    }
  } else {
    if (tag_set_delete_tag(&tags->local_tags, dtag, tag->key, key_len)) {
      dtag->tags = &tags->local_tags;
      return true;
    }
    if (tag_set_delete_tag(&tags->propagated_tags, dtag, tag->key, key_len))
      return false;
    tag_set_delete_tag(&tags->propagated_binary_tags, dtag, tag->key, key_len);
  }
  return false;
}

// Add a tag to a tag set.
static void tag_set_add_tag(struct tag_set *tags, const census_tag *tag,
                            size_t key_len) {
  const size_t tag_size = key_len + tag->value_len + TAG_HEADER_SIZE;
  // drop tag if too many.
  if (tags->ntags == CENSUS_MAX_TAGS) {
    return;
  }
  if (tags->kvm_used + tag_size > tags->kvm_size) {
    tags->kvm_size += 2 * CENSUS_MAX_TAG_KV_LEN + TAG_HEADER_SIZE;
    char *new_kvm = gpr_malloc(tags->kvm_size);
    memcpy(new_kvm, tags->kvm, tags->kvm_used);
    gpr_free(tags->kvm);
    tags->kvm = new_kvm;
  }
  char *kvp = tags->kvm + tags->kvm_used;
  *kvp++ = (char)key_len;
  *kvp++ = (char)tag->value_len;
  // ensure reserved flags are not used.
  *kvp++ = (char)(tag->flags & (CENSUS_TAG_PROPAGATE | CENSUS_TAG_STATS |
                                CENSUS_TAG_BINARY));
  memcpy(kvp, tag->key, key_len);
  kvp += key_len;
  memcpy(kvp, tag->value, tag->value_len);
  tags->kvm_used += tag_size;
  tags->ntags++;
  tags->ntags_alloc++;
}

// Add a tag to a census_tag_set
static void cts_add_tag(census_tag_set *tags, const census_tag *tag,
                        size_t key_len) {
  // first delete the tag if it is already present
  struct deleted_tag_info dtag;
  bool deleted_match = cts_delete_tag(tags, tag, key_len, &dtag);
  if (tag->value != NULL && tag->value_len != 0) {
    if (deleted_match && tag->value_len == dtag.raw.value_len) {
      // if we have a close match for tag being added to one just deleted,
      // only need to modify value and flags.
      memcpy(dtag.raw.value, tag->value, tag->value_len);
      *dtag.raw_flags_p = (tag->flags & (CENSUS_TAG_PROPAGATE |
                                         CENSUS_TAG_STATS | CENSUS_TAG_BINARY));
      dtag.tags->ntags++;
    } else {
      if (CENSUS_TAG_IS_PROPAGATED(tag->flags)) {
        if (CENSUS_TAG_IS_BINARY(tag->flags)) {
          tag_set_add_tag(&tags->propagated_binary_tags, tag, key_len);
        } else {
          tag_set_add_tag(&tags->propagated_tags, tag, key_len);
        }
      } else {
        tag_set_add_tag(&tags->local_tags, tag, key_len);
      }
    }
  }
}

census_tag_set *census_tag_set_create(const census_tag_set *base,
                                      const census_tag *tags, int ntags) {
  census_tag_set *new_ts = gpr_malloc(sizeof(census_tag_set));
  if (base == NULL) {
    memset(new_ts, 0, sizeof(census_tag_set));
  } else {
    tag_set_copy(&new_ts->propagated_tags, &base->propagated_tags);
    tag_set_copy(&new_ts->propagated_binary_tags,
                 &base->propagated_binary_tags);
    tag_set_copy(&new_ts->local_tags, &base->local_tags);
  }
  for (int i = 0; i < ntags; i++) {
    const census_tag *tag = &tags[i];
    size_t key_len = strlen(tag->key) + 1;
    // ignore the tag if it is too long/short.
    if (key_len != 1 && key_len <= CENSUS_MAX_TAG_KV_LEN &&
        tag->value_len <= CENSUS_MAX_TAG_KV_LEN) {
      cts_add_tag(new_ts, tag, key_len);
    }
  }
  return new_ts;
}

void census_tag_set_destroy(census_tag_set *tags) {
  gpr_free(tags->propagated_tags.kvm);
  gpr_free(tags->propagated_binary_tags.kvm);
  gpr_free(tags->local_tags.kvm);
  gpr_free(tags);
}

int census_tag_set_ntags(const census_tag_set *tags) {
  return tags->propagated_tags.ntags + tags->propagated_binary_tags.ntags +
         tags->local_tags.ntags;
}

// Get the nth tag in a tag set. The caller must validate that index is
// in range.
static void tag_set_get_tag_by_index(const struct tag_set *tags, int index,
                                     census_tag *tag) {
  GPR_ASSERT(index < tags->ntags);
  char *kvp = tags->kvm;
  for (;;) {
    struct raw_tag raw;
    kvp = decode_tag(&raw, kvp, 0);
    if (CENSUS_TAG_IS_DELETED(raw.flags)) {
      continue;
    } else if (index == 0) {
      tag->key = raw.key;
      tag->value = raw.value;
      tag->value_len = raw.value_len;
      tag->flags = raw.flags;
      return;
    }
    index--;
  }
  // NOT REACHED
}

int census_tag_set_get_tag_by_index(const census_tag_set *tags, int index,
                                    census_tag *tag) {
  if (index < 0) return 0;
  if (index < tags->propagated_tags.ntags) {
    tag_set_get_tag_by_index(&tags->propagated_tags, index, tag);
    return 1;
  }
  index -= tags->propagated_tags.ntags;
  if (index < tags->propagated_binary_tags.ntags) {
    tag_set_get_tag_by_index(&tags->propagated_binary_tags, index, tag);
    return 1;
  }
  index -= tags->propagated_binary_tags.ntags;
  if (index < tags->local_tags.ntags) {
    tag_set_get_tag_by_index(&tags->local_tags, index, tag);
    return 1;
  }
  return 0;
}

// Find a tag in a tag_set by key. Return true if found, false otherwise.
static bool tag_set_get_tag_by_key(const struct tag_set *tags, const char *key,
                                   size_t key_len, census_tag *tag) {
  char *kvp = tags->kvm;
  for (int i = 0; i < tags->ntags; i++) {
    struct raw_tag raw;
    do {
      kvp = decode_tag(&raw, kvp, 0);
    } while (CENSUS_TAG_IS_DELETED(raw.flags));
    if (key_len == raw.key_len && memcmp(raw.key, key, key_len) == 0) {
      tag->key = raw.key;
      tag->value = raw.value;
      tag->value_len = raw.value_len;
      tag->flags = raw.flags;
      return true;
    }
  }
  return false;
}

int census_tag_set_get_tag_by_key(const census_tag_set *tags, const char *key,
                                  census_tag *tag) {
  size_t key_len = strlen(key) + 1;
  if (key_len == 1) {
    return 0;
  }
  if (tag_set_get_tag_by_key(&tags->propagated_tags, key, key_len, tag) ||
      tag_set_get_tag_by_key(&tags->propagated_binary_tags, key, key_len,
                             tag) ||
      tag_set_get_tag_by_key(&tags->local_tags, key, key_len, tag)) {
    return 1;
  }
  return 0;
}

// tag_set encoding and decoding functions.
//
// Wire format for tag sets on the wire:
//
// First, a tag set header:
//
// offset   bytes  description
//   0        1    version number
//   1        1    number of bytes in this header. This allows for future
//                 expansion.
//   2        1    number of bytes in each tag header.
//   3        1    ntags value from tag set.
//   4        1    ntags_alloc value from tag set.
//
//   This is followed by the key/value memory from struct tag_set.

#define ENCODED_VERSION 0      // Version number
#define ENCODED_HEADER_SIZE 5  // size of tag set header

// Pack a tag set into as few bytes as possible (eliding deleted tags). Assumes
// header is already generated.
static size_t tag_set_encode_packed(const struct tag_set *tags, char *buffer,
                                    size_t buf_size) {
  size_t encoded_size = 0;
  char *kvp = tags->kvm;
  for (int i = 0; i < tags->ntags_alloc; i++) {
    struct raw_tag raw;
    char *base = kvp;
    kvp = decode_tag(&raw, kvp, 0);
    size_t tag_size =
        TAG_HEADER_SIZE + (size_t)raw.key_len + (size_t)raw.value_len;
    if (!(CENSUS_TAG_IS_DELETED(raw.flags))) {
      if (tag_size > buf_size) {
        return 0;
      }
      memcpy(buffer, base, tag_size);
      buffer += tag_size;
      encoded_size += tag_size;
      buf_size -= tag_size;
    }
  }
  return encoded_size;
}

// Encode a tag set. Returns 0 if buffer is too small.
static size_t tag_set_encode(const struct tag_set *tags, char *buffer,
                             size_t buf_size) {
  if (buf_size < ENCODED_HEADER_SIZE) {
    return 0;
  }
  buf_size -= ENCODED_HEADER_SIZE;
  *buffer++ = (char)ENCODED_VERSION;
  *buffer++ = (char)ENCODED_HEADER_SIZE;
  *buffer++ = (char)TAG_HEADER_SIZE;
  *buffer++ = (char)tags->ntags;
  if (tags->ntags == 0) {
    *buffer = (char)tags->ntags;
    return ENCODED_HEADER_SIZE;
  }
  if (buf_size < tags->kvm_used || tags->ntags_alloc > CENSUS_MAX_TAGS) {
    *buffer++ = (char)tags->ntags;
    size_t enc_size = tag_set_encode_packed(tags, buffer, buf_size);
    if (enc_size == 0) {
      return 0;
    }
    return ENCODED_HEADER_SIZE + enc_size;
  }
  *buffer++ = (char)tags->ntags_alloc;
  memcpy(buffer, tags->kvm, tags->kvm_used);
  return ENCODED_HEADER_SIZE + tags->kvm_used;
}

size_t census_tag_set_encode_propagated(const census_tag_set *tags,
                                        char *buffer, size_t buf_size) {
  return tag_set_encode(&tags->propagated_tags, buffer, buf_size);
}

size_t census_tag_set_encode_propagated_binary(const census_tag_set *tags,
                                               char *buffer, size_t buf_size) {
  return tag_set_encode(&tags->propagated_binary_tags, buffer, buf_size);
}

// Decode a tag set.
static void tag_set_decode(struct tag_set *tags, const char *buffer,
                           size_t size) {
  uint8_t version = (uint8_t)(*buffer++);
  uint8_t header_size = (uint8_t)(*buffer++);
  uint8_t tag_header_size = (uint8_t)(*buffer++);
  tags->ntags = (int)(*buffer++);
  if (tags->ntags == 0) {
    tags->ntags_alloc = 0;
    tags->kvm_size = 0;
    tags->kvm_used = 0;
    tags->kvm = NULL;
    return;
  }
  tags->ntags_alloc = (uint8_t)(*buffer++);
  if (header_size != ENCODED_HEADER_SIZE) {
    GPR_ASSERT(version != ENCODED_VERSION);
    GPR_ASSERT(ENCODED_HEADER_SIZE < header_size);
    buffer += (header_size - ENCODED_HEADER_SIZE);
  }
  tags->kvm_used = size - header_size;
  tags->kvm_size = tags->kvm_used + CENSUS_MAX_TAG_KV_LEN;
  tags->kvm = gpr_malloc(tags->kvm_size);
  if (tag_header_size != TAG_HEADER_SIZE) {
    // something new in the tag information. I don't understand it, so
    // don't copy it over.
    GPR_ASSERT(version != ENCODED_VERSION);
    GPR_ASSERT(tag_header_size > TAG_HEADER_SIZE);
    char *kvp = tags->kvm;
    for (int i = 0; i < tags->ntags_alloc; i++) {
      memcpy(kvp, buffer, TAG_HEADER_SIZE);
      kvp += header_size;
      struct raw_tag raw;
      buffer =
          decode_tag(&raw, (char *)buffer, tag_header_size - TAG_HEADER_SIZE);
      memcpy(kvp, raw.key, (size_t)raw.key_len + raw.value_len);
      kvp += raw.key_len + raw.value_len;
    }
  } else {
    memcpy(tags->kvm, buffer, tags->kvm_used);
  }
}

census_tag_set *census_tag_set_decode(const char *buffer, size_t size,
                                      const char *bin_buffer, size_t bin_size) {
  census_tag_set *new_ts = gpr_malloc(sizeof(census_tag_set));
  memset(&new_ts->local_tags, 0, sizeof(struct tag_set));
  if (buffer == NULL) {
    memset(&new_ts->propagated_tags, 0, sizeof(struct tag_set));
  } else {
    tag_set_decode(&new_ts->propagated_tags, buffer, size);
  }
  if (bin_buffer == NULL) {
    memset(&new_ts->propagated_binary_tags, 0, sizeof(struct tag_set));
  } else {
    tag_set_decode(&new_ts->propagated_binary_tags, bin_buffer, bin_size);
  }
  // TODO(aveitch): check that BINARY flag is correct for each type.
  return new_ts;
}
