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

#ifndef GRPC_CORE_TSI_FAKE_TRANSPORT_SECURITY_H
#define GRPC_CORE_TSI_FAKE_TRANSPORT_SECURITY_H

#include "src/core/tsi/transport_security_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Value for the TSI_CERTIFICATE_TYPE_PEER_PROPERTY property for FAKE certs. */
#define TSI_FAKE_CERTIFICATE_TYPE "FAKE"

/* Creates a fake handshaker that will create a fake frame protector.

   No cryptography is performed in these objects. They just simulate handshake
   messages going back and forth for the handshaker and do some framing on
   cleartext data for the protector.  */
tsi_handshaker *tsi_create_fake_handshaker(int is_client);

/* Creates a protector directly without going through the handshake phase. */
tsi_frame_protector *tsi_create_fake_frame_protector(
    size_t *max_protected_frame_size);

#ifdef __cplusplus
}
#endif

#endif /* GRPC_CORE_TSI_FAKE_TRANSPORT_SECURITY_H */
