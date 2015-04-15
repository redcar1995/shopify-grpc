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

#include "src/core/support/file.h"

#include <errno.h>
#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

#include "src/core/support/string.h"

gpr_slice gpr_load_file(const char *filename, int add_null_terminator,
                        int *success) {
  unsigned char *contents = NULL;
  size_t contents_size = 0;
  unsigned char buf[4096];
  char *error_msg = NULL;
  gpr_slice result = gpr_empty_slice();
  FILE *file = fopen(filename, "rb");

  if (file == NULL) {
    gpr_asprintf(&error_msg, "Could not open file %s (error = %s).", filename,
                 strerror(errno));
    GPR_ASSERT(error_msg != NULL);
    goto end;
  }

  while (1) {
    size_t bytes_read = fread(buf, 1, sizeof(buf), file);
    if (bytes_read > 0) {
      contents = gpr_realloc(contents, contents_size + bytes_read);
      memcpy(contents + contents_size, buf, bytes_read);
      contents_size += bytes_read;
    }
    if (bytes_read < sizeof(buf)) {
      if (ferror(file)) {
        gpr_asprintf(&error_msg, "Error %s occured while reading file %s.",
                     strerror(errno), filename);
        GPR_ASSERT(error_msg != NULL);
        goto end;
      } else {
        GPR_ASSERT(feof(file));
        break;
      }
    }
  }
  if (success != NULL) *success = 1;
  if (add_null_terminator) {
    contents = gpr_realloc(contents, contents_size + 1);
    contents[contents_size++] = 0;
  }
  result = gpr_slice_new(contents, contents_size, gpr_free);

end:
  if (error_msg != NULL) {
    gpr_log(GPR_ERROR, "%s", error_msg);
    gpr_free(error_msg);
    if (success != NULL) *success = 0;
  }
  if (file != NULL) fclose(file);
  return result;
}
