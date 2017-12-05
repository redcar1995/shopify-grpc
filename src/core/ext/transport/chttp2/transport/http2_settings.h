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
 * Automatically generated by tools/codegen/core/gen_settings_ids.py
 */

#ifndef GRPC_CORE_EXT_TRANSPORT_CHTTP2_TRANSPORT_HTTP2_SETTINGS_H
#define GRPC_CORE_EXT_TRANSPORT_CHTTP2_TRANSPORT_HTTP2_SETTINGS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  GRPC_CHTTP2_SETTINGS_HEADER_TABLE_SIZE = 0,               /* wire id 1 */
  GRPC_CHTTP2_SETTINGS_ENABLE_PUSH = 1,                     /* wire id 2 */
  GRPC_CHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS = 2,          /* wire id 3 */
  GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE = 3,             /* wire id 4 */
  GRPC_CHTTP2_SETTINGS_MAX_FRAME_SIZE = 4,                  /* wire id 5 */
  GRPC_CHTTP2_SETTINGS_MAX_HEADER_LIST_SIZE = 5,            /* wire id 6 */
  GRPC_CHTTP2_SETTINGS_GRPC_ALLOW_TRUE_BINARY_METADATA = 6, /* wire id 65027 */
} grpc_chttp2_setting_id;

#define GRPC_CHTTP2_NUM_SETTINGS 7

extern const uint16_t grpc_setting_id_to_wire_id[];

bool grpc_wire_id_to_setting_id(uint32_t wire_id, grpc_chttp2_setting_id* out);

typedef enum {
  GRPC_CHTTP2_CLAMP_INVALID_VALUE,
  GRPC_CHTTP2_DISCONNECT_ON_INVALID_VALUE
} grpc_chttp2_invalid_value_behavior;

typedef struct {
  const char* name;
  uint32_t default_value;
  uint32_t min_value;
  uint32_t max_value;
  grpc_chttp2_invalid_value_behavior invalid_value_behavior;
  uint32_t error_value;
} grpc_chttp2_setting_parameters;

extern const grpc_chttp2_setting_parameters
    grpc_chttp2_settings_parameters[GRPC_CHTTP2_NUM_SETTINGS];

#endif /* GRPC_CORE_EXT_TRANSPORT_CHTTP2_TRANSPORT_HTTP2_SETTINGS_H */
