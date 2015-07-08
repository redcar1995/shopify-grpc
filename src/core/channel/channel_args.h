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

#ifndef GRPC_INTERNAL_CORE_CHANNEL_CHANNEL_ARGS_H
#define GRPC_INTERNAL_CORE_CHANNEL_CHANNEL_ARGS_H

#include <grpc/compression.h>
#include <grpc/grpc.h>

/* Copy some arguments */
grpc_channel_args *grpc_channel_args_copy(const grpc_channel_args *src);

/** Copy some arguments and add the to_add parameter in the end.
   If to_add is NULL, it is equivalent to call grpc_channel_args_copy. */
grpc_channel_args *grpc_channel_args_copy_and_add(const grpc_channel_args *src,
                                                  const grpc_arg *to_add,
                                                  size_t num_to_add);

/** Copy args from a then args from b into a new channel args */
grpc_channel_args *grpc_channel_args_merge(const grpc_channel_args *a,
                                           const grpc_channel_args *b);

/** Destroy arguments created by grpc_channel_args_copy */
void grpc_channel_args_destroy(grpc_channel_args *a);

/** Reads census_enabled settings from channel args. Returns 1 if census_enabled
 * is specified in channel args, otherwise returns 0. */
int grpc_channel_args_is_census_enabled(const grpc_channel_args *a);

/** Returns the compression level set in \a a. */
grpc_compression_level grpc_channel_args_get_compression_level(
    const grpc_channel_args *a);

/** Returns a channel arg instance with compression enabled. If \a a is
 * non-NULL, its args are copied. N.B. GRPC_COMPRESS_LEVEL_NONE disables
 * compression for the channel. */
grpc_channel_args *grpc_channel_args_set_compression_level(
    grpc_channel_args *a, grpc_compression_level level);

#endif /* GRPC_INTERNAL_CORE_CHANNEL_CHANNEL_ARGS_H */
