/*
 *
 * Copyright 2016, Google Inc.
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

#include <grpc++/impl/codegen/core_codegen_interface.h>
#include <grpc/impl/codegen/grpc_types.h>
#include <grpc/byte_buffer.h>

namespace grpc {

class CoreCodegen : public CoreCodegenInterface {
 private:
  grpc_completion_queue* grpc_completion_queue_create(void* reserved) override;

  void grpc_completion_queue_destroy(grpc_completion_queue* cq) override;

  grpc_event grpc_completion_queue_pluck(grpc_completion_queue* cq, void* tag,
                                         gpr_timespec deadline,
                                         void* reserved) override;

  void* gpr_malloc(size_t size) override;

  void gpr_free(void* p) override;

  void grpc_byte_buffer_destroy(grpc_byte_buffer* bb) override;

  void grpc_metadata_array_init(grpc_metadata_array* array) override;

  void grpc_metadata_array_destroy(grpc_metadata_array* array) override;

  void assert_fail(const char* failed_assertion) override;

  Status SerializeProto(const grpc::protobuf::Message& msg,
                        grpc_byte_buffer** bp) override;

  Status DeserializeProto(grpc_byte_buffer* buffer,
                          grpc::protobuf::Message* msg,
                          int max_message_size) override;
};

}  // namespace grpc
