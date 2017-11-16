/*
 *
 * Copyright 2015 gRPC authors.
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

#ifndef GRPC_CORE_LIB_HTTP_FORMAT_REQUEST_H
#define GRPC_CORE_LIB_HTTP_FORMAT_REQUEST_H

#include <grpc/slice.h>
#include "src/core/lib/http/httpcli.h"



grpc_slice grpc_httpcli_format_get_request(const grpc_httpcli_request* request);
grpc_slice grpc_httpcli_format_post_request(const grpc_httpcli_request* request,
                                            const char* body_bytes,
                                            size_t body_size);
grpc_slice grpc_httpcli_format_connect_request(
    const grpc_httpcli_request* request);



#endif /* GRPC_CORE_LIB_HTTP_FORMAT_REQUEST_H */
