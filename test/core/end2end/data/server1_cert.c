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

const char test_server1_cert[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43,
    0x45, 0x52, 0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x49, 0x49, 0x43, 0x6d, 0x7a, 0x43, 0x43,
    0x41, 0x67, 0x53, 0x67, 0x41, 0x77, 0x49, 0x42, 0x41, 0x67, 0x49, 0x42,
    0x41, 0x7a, 0x41, 0x4e, 0x42, 0x67, 0x6b, 0x71, 0x68, 0x6b, 0x69, 0x47,
    0x39, 0x77, 0x30, 0x42, 0x41, 0x51, 0x55, 0x46, 0x41, 0x44, 0x42, 0x57,
    0x4d, 0x51, 0x73, 0x77, 0x43, 0x51, 0x59, 0x44, 0x56, 0x51, 0x51, 0x47,
    0x45, 0x77, 0x4a, 0x42, 0x56, 0x54, 0x45, 0x54, 0x0a, 0x4d, 0x42, 0x45,
    0x47, 0x41, 0x31, 0x55, 0x45, 0x43, 0x41, 0x77, 0x4b, 0x55, 0x32, 0x39,
    0x74, 0x5a, 0x53, 0x31, 0x54, 0x64, 0x47, 0x46, 0x30, 0x5a, 0x54, 0x45,
    0x68, 0x4d, 0x42, 0x38, 0x47, 0x41, 0x31, 0x55, 0x45, 0x43, 0x67, 0x77,
    0x59, 0x53, 0x57, 0x35, 0x30, 0x5a, 0x58, 0x4a, 0x75, 0x5a, 0x58, 0x51,
    0x67, 0x56, 0x32, 0x6c, 0x6b, 0x5a, 0x32, 0x6c, 0x30, 0x63, 0x79, 0x42,
    0x51, 0x0a, 0x64, 0x48, 0x6b, 0x67, 0x54, 0x48, 0x52, 0x6b, 0x4d, 0x51,
    0x38, 0x77, 0x44, 0x51, 0x59, 0x44, 0x56, 0x51, 0x51, 0x44, 0x44, 0x41,
    0x5a, 0x30, 0x5a, 0x58, 0x4e, 0x30, 0x59, 0x32, 0x45, 0x77, 0x48, 0x68,
    0x63, 0x4e, 0x4d, 0x54, 0x51, 0x77, 0x4e, 0x7a, 0x49, 0x79, 0x4d, 0x44,
    0x59, 0x77, 0x4d, 0x44, 0x55, 0x33, 0x57, 0x68, 0x63, 0x4e, 0x4d, 0x6a,
    0x51, 0x77, 0x4e, 0x7a, 0x45, 0x35, 0x0a, 0x4d, 0x44, 0x59, 0x77, 0x4d,
    0x44, 0x55, 0x33, 0x57, 0x6a, 0x42, 0x6b, 0x4d, 0x51, 0x73, 0x77, 0x43,
    0x51, 0x59, 0x44, 0x56, 0x51, 0x51, 0x47, 0x45, 0x77, 0x4a, 0x56, 0x55,
    0x7a, 0x45, 0x52, 0x4d, 0x41, 0x38, 0x47, 0x41, 0x31, 0x55, 0x45, 0x43,
    0x42, 0x4d, 0x49, 0x53, 0x57, 0x78, 0x73, 0x61, 0x57, 0x35, 0x76, 0x61,
    0x58, 0x4d, 0x78, 0x45, 0x44, 0x41, 0x4f, 0x42, 0x67, 0x4e, 0x56, 0x0a,
    0x42, 0x41, 0x63, 0x54, 0x42, 0x30, 0x4e, 0x6f, 0x61, 0x57, 0x4e, 0x68,
    0x5a, 0x32, 0x38, 0x78, 0x46, 0x44, 0x41, 0x53, 0x42, 0x67, 0x4e, 0x56,
    0x42, 0x41, 0x6f, 0x54, 0x43, 0x30, 0x64, 0x76, 0x62, 0x32, 0x64, 0x73,
    0x5a, 0x53, 0x42, 0x4a, 0x62, 0x6d, 0x4d, 0x75, 0x4d, 0x52, 0x6f, 0x77,
    0x47, 0x41, 0x59, 0x44, 0x56, 0x51, 0x51, 0x44, 0x46, 0x42, 0x45, 0x71,
    0x4c, 0x6e, 0x52, 0x6c, 0x0a, 0x63, 0x33, 0x51, 0x75, 0x5a, 0x32, 0x39,
    0x76, 0x5a, 0x32, 0x78, 0x6c, 0x4c, 0x6d, 0x4e, 0x76, 0x62, 0x54, 0x43,
    0x42, 0x6e, 0x7a, 0x41, 0x4e, 0x42, 0x67, 0x6b, 0x71, 0x68, 0x6b, 0x69,
    0x47, 0x39, 0x77, 0x30, 0x42, 0x41, 0x51, 0x45, 0x46, 0x41, 0x41, 0x4f,
    0x42, 0x6a, 0x51, 0x41, 0x77, 0x67, 0x59, 0x6b, 0x43, 0x67, 0x59, 0x45,
    0x41, 0x34, 0x63, 0x4d, 0x56, 0x4a, 0x79, 0x67, 0x73, 0x0a, 0x4a, 0x55,
    0x6d, 0x6c, 0x67, 0x4d, 0x4d, 0x7a, 0x67, 0x64, 0x69, 0x30, 0x68, 0x31,
    0x58, 0x6f, 0x43, 0x52, 0x37, 0x2b, 0x77, 0x77, 0x31, 0x70, 0x6f, 0x70,
    0x30, 0x34, 0x4f, 0x4d, 0x4d, 0x79, 0x79, 0x37, 0x48, 0x2f, 0x69, 0x30,
    0x50, 0x4a, 0x32, 0x57, 0x36, 0x59, 0x33, 0x35, 0x2b, 0x62, 0x34, 0x43,
    0x4d, 0x38, 0x51, 0x72, 0x6b, 0x59, 0x65, 0x45, 0x61, 0x66, 0x55, 0x47,
    0x44, 0x4f, 0x0a, 0x52, 0x59, 0x58, 0x36, 0x79, 0x56, 0x2f, 0x63, 0x48,
    0x47, 0x47, 0x73, 0x44, 0x2f, 0x78, 0x30, 0x32, 0x79, 0x65, 0x36, 0x65,
    0x79, 0x31, 0x55, 0x44, 0x74, 0x6b, 0x47, 0x41, 0x44, 0x2f, 0x6d, 0x70,
    0x44, 0x45, 0x78, 0x38, 0x59, 0x43, 0x72, 0x6a, 0x41, 0x63, 0x31, 0x56,
    0x66, 0x76, 0x74, 0x38, 0x46, 0x6b, 0x36, 0x43, 0x6e, 0x31, 0x57, 0x56,
    0x49, 0x78, 0x56, 0x2f, 0x4a, 0x33, 0x30, 0x0a, 0x33, 0x78, 0x6a, 0x42,
    0x73, 0x46, 0x67, 0x42, 0x79, 0x51, 0x35, 0x35, 0x52, 0x42, 0x70, 0x31,
    0x4f, 0x4c, 0x5a, 0x66, 0x56, 0x4c, 0x6f, 0x36, 0x41, 0x6c, 0x65, 0x42,
    0x44, 0x53, 0x62, 0x63, 0x78, 0x61, 0x45, 0x43, 0x41, 0x77, 0x45, 0x41,
    0x41, 0x61, 0x4e, 0x72, 0x4d, 0x47, 0x6b, 0x77, 0x43, 0x51, 0x59, 0x44,
    0x56, 0x52, 0x30, 0x54, 0x42, 0x41, 0x49, 0x77, 0x41, 0x44, 0x41, 0x4c,
    0x0a, 0x42, 0x67, 0x4e, 0x56, 0x48, 0x51, 0x38, 0x45, 0x42, 0x41, 0x4d,
    0x43, 0x42, 0x65, 0x41, 0x77, 0x54, 0x77, 0x59, 0x44, 0x56, 0x52, 0x30,
    0x52, 0x42, 0x45, 0x67, 0x77, 0x52, 0x6f, 0x49, 0x51, 0x4b, 0x69, 0x35,
    0x30, 0x5a, 0x58, 0x4e, 0x30, 0x4c, 0x6d, 0x64, 0x76, 0x62, 0x32, 0x64,
    0x73, 0x5a, 0x53, 0x35, 0x6d, 0x63, 0x6f, 0x49, 0x59, 0x64, 0x32, 0x46,
    0x30, 0x5a, 0x58, 0x4a, 0x36, 0x0a, 0x62, 0x32, 0x39, 0x70, 0x4c, 0x6e,
    0x52, 0x6c, 0x63, 0x33, 0x51, 0x75, 0x5a, 0x32, 0x39, 0x76, 0x5a, 0x32,
    0x78, 0x6c, 0x4c, 0x6d, 0x4a, 0x6c, 0x67, 0x68, 0x49, 0x71, 0x4c, 0x6e,
    0x52, 0x6c, 0x63, 0x33, 0x51, 0x75, 0x65, 0x57, 0x39, 0x31, 0x64, 0x48,
    0x56, 0x69, 0x5a, 0x53, 0x35, 0x6a, 0x62, 0x32, 0x32, 0x48, 0x42, 0x4d,
    0x43, 0x6f, 0x41, 0x51, 0x4d, 0x77, 0x44, 0x51, 0x59, 0x4a, 0x0a, 0x4b,
    0x6f, 0x5a, 0x49, 0x68, 0x76, 0x63, 0x4e, 0x41, 0x51, 0x45, 0x46, 0x42,
    0x51, 0x41, 0x44, 0x67, 0x59, 0x45, 0x41, 0x4d, 0x32, 0x49, 0x69, 0x30,
    0x4c, 0x67, 0x54, 0x47, 0x62, 0x4a, 0x31, 0x6a, 0x34, 0x6f, 0x71, 0x58,
    0x39, 0x62, 0x78, 0x56, 0x63, 0x78, 0x6d, 0x2b, 0x2f, 0x52, 0x35, 0x59,
    0x66, 0x38, 0x6f, 0x69, 0x30, 0x61, 0x5a, 0x71, 0x54, 0x4a, 0x6c, 0x6e,
    0x4c, 0x59, 0x53, 0x0a, 0x77, 0x58, 0x63, 0x42, 0x79, 0x6b, 0x78, 0x54,
    0x78, 0x31, 0x38, 0x31, 0x73, 0x37, 0x57, 0x79, 0x66, 0x4a, 0x34, 0x39,
    0x57, 0x77, 0x72, 0x59, 0x58, 0x6f, 0x37, 0x38, 0x7a, 0x54, 0x44, 0x41,
    0x6e, 0x66, 0x31, 0x6d, 0x61, 0x30, 0x66, 0x50, 0x71, 0x33, 0x65, 0x34,
    0x6d, 0x70, 0x73, 0x70, 0x76, 0x79, 0x6e, 0x64, 0x4c, 0x68, 0x31, 0x61,
    0x2b, 0x4f, 0x61, 0x72, 0x48, 0x61, 0x31, 0x65, 0x0a, 0x61, 0x54, 0x30,
    0x44, 0x49, 0x49, 0x59, 0x6b, 0x37, 0x71, 0x65, 0x45, 0x61, 0x31, 0x59,
    0x63, 0x56, 0x6c, 0x6a, 0x78, 0x32, 0x4b, 0x79, 0x4c, 0x64, 0x30, 0x72,
    0x31, 0x42, 0x42, 0x41, 0x66, 0x72, 0x77, 0x79, 0x47, 0x61, 0x45, 0x50,
    0x56, 0x65, 0x4a, 0x51, 0x56, 0x59, 0x57, 0x61, 0x4f, 0x4a, 0x52, 0x55,
    0x32, 0x77, 0x65, 0x2f, 0x4b, 0x44, 0x34, 0x6f, 0x6a, 0x66, 0x39, 0x73,
    0x3d, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43,
    0x45, 0x52, 0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x00};