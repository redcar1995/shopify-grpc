/*
 *
 * Copyright 2014, Google Inc.
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

#ifndef __GRPC_INTERNAL_SECURITY_JSON_TOKEN_H_
#define __GRPC_INTERNAL_SECURITY_JSON_TOKEN_H_

#include <grpc/support/slice.h>
#include <openssl/rsa.h>

/* --- auth_json_key parsing. Exposed for testing only. --- */

typedef struct {
  char *type;
  char *private_key_id;
  char *client_id;
  char *client_email;
  RSA *private_key;
} grpc_auth_json_key;

/* Returns 1 if the object is valid, 0 otherwise. */
int grpc_auth_json_key_is_valid(grpc_auth_json_key *json_key);

/* Creates a json_key object from string. Returns an invalid object if a parsing
   error has been encountered. */
grpc_auth_json_key grpc_auth_json_key_create_from_string(
    const char *json_string);

/* Destructs the object. */
void grpc_auth_json_key_destruct(grpc_auth_json_key *json_key);

/* --- json token encoding and signing. --- */

/* Caller is responsible for calling gpr_free on the returned value. May return
   NULL on invalid input. */
char *grpc_jwt_encode_and_sign(const grpc_auth_json_key *json_key,
                               const char *scope, gpr_timespec token_lifetime);

#endif /* __GRPC_INTERNAL_SECURITY_JSON_TOKEN_H_ */
