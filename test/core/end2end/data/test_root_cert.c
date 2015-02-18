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

const char test_root_cert[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43,
    0x45, 0x52, 0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x49, 0x49, 0x43, 0x49, 0x7a, 0x43, 0x43,
    0x41, 0x59, 0x77, 0x43, 0x43, 0x51, 0x43, 0x46, 0x54, 0x62, 0x46, 0x37,
    0x58, 0x4e, 0x53, 0x76, 0x76, 0x6a, 0x41, 0x4e, 0x42, 0x67, 0x6b, 0x71,
    0x68, 0x6b, 0x69, 0x47, 0x39, 0x77, 0x30, 0x42, 0x41, 0x51, 0x55, 0x46,
    0x41, 0x44, 0x42, 0x57, 0x4d, 0x51, 0x73, 0x77, 0x43, 0x51, 0x59, 0x44,
    0x56, 0x51, 0x51, 0x47, 0x45, 0x77, 0x4a, 0x42, 0x0a, 0x56, 0x54, 0x45,
    0x54, 0x4d, 0x42, 0x45, 0x47, 0x41, 0x31, 0x55, 0x45, 0x43, 0x41, 0x77,
    0x4b, 0x55, 0x32, 0x39, 0x74, 0x5a, 0x53, 0x31, 0x54, 0x64, 0x47, 0x46,
    0x30, 0x5a, 0x54, 0x45, 0x68, 0x4d, 0x42, 0x38, 0x47, 0x41, 0x31, 0x55,
    0x45, 0x43, 0x67, 0x77, 0x59, 0x53, 0x57, 0x35, 0x30, 0x5a, 0x58, 0x4a,
    0x75, 0x5a, 0x58, 0x51, 0x67, 0x56, 0x32, 0x6c, 0x6b, 0x5a, 0x32, 0x6c,
    0x30, 0x0a, 0x63, 0x79, 0x42, 0x51, 0x64, 0x48, 0x6b, 0x67, 0x54, 0x48,
    0x52, 0x6b, 0x4d, 0x51, 0x38, 0x77, 0x44, 0x51, 0x59, 0x44, 0x56, 0x51,
    0x51, 0x44, 0x44, 0x41, 0x5a, 0x30, 0x5a, 0x58, 0x4e, 0x30, 0x59, 0x32,
    0x45, 0x77, 0x48, 0x68, 0x63, 0x4e, 0x4d, 0x54, 0x51, 0x77, 0x4e, 0x7a,
    0x45, 0x33, 0x4d, 0x6a, 0x4d, 0x78, 0x4e, 0x7a, 0x55, 0x78, 0x57, 0x68,
    0x63, 0x4e, 0x4d, 0x6a, 0x51, 0x77, 0x0a, 0x4e, 0x7a, 0x45, 0x30, 0x4d,
    0x6a, 0x4d, 0x78, 0x4e, 0x7a, 0x55, 0x78, 0x57, 0x6a, 0x42, 0x57, 0x4d,
    0x51, 0x73, 0x77, 0x43, 0x51, 0x59, 0x44, 0x56, 0x51, 0x51, 0x47, 0x45,
    0x77, 0x4a, 0x42, 0x56, 0x54, 0x45, 0x54, 0x4d, 0x42, 0x45, 0x47, 0x41,
    0x31, 0x55, 0x45, 0x43, 0x41, 0x77, 0x4b, 0x55, 0x32, 0x39, 0x74, 0x5a,
    0x53, 0x31, 0x54, 0x64, 0x47, 0x46, 0x30, 0x5a, 0x54, 0x45, 0x68, 0x0a,
    0x4d, 0x42, 0x38, 0x47, 0x41, 0x31, 0x55, 0x45, 0x43, 0x67, 0x77, 0x59,
    0x53, 0x57, 0x35, 0x30, 0x5a, 0x58, 0x4a, 0x75, 0x5a, 0x58, 0x51, 0x67,
    0x56, 0x32, 0x6c, 0x6b, 0x5a, 0x32, 0x6c, 0x30, 0x63, 0x79, 0x42, 0x51,
    0x64, 0x48, 0x6b, 0x67, 0x54, 0x48, 0x52, 0x6b, 0x4d, 0x51, 0x38, 0x77,
    0x44, 0x51, 0x59, 0x44, 0x56, 0x51, 0x51, 0x44, 0x44, 0x41, 0x5a, 0x30,
    0x5a, 0x58, 0x4e, 0x30, 0x0a, 0x59, 0x32, 0x45, 0x77, 0x67, 0x5a, 0x38,
    0x77, 0x44, 0x51, 0x59, 0x4a, 0x4b, 0x6f, 0x5a, 0x49, 0x68, 0x76, 0x63,
    0x4e, 0x41, 0x51, 0x45, 0x42, 0x42, 0x51, 0x41, 0x44, 0x67, 0x59, 0x30,
    0x41, 0x4d, 0x49, 0x47, 0x4a, 0x41, 0x6f, 0x47, 0x42, 0x41, 0x4d, 0x42,
    0x41, 0x33, 0x77, 0x56, 0x65, 0x54, 0x47, 0x48, 0x5a, 0x52, 0x31, 0x52,
    0x79, 0x65, 0x2f, 0x69, 0x2b, 0x4a, 0x38, 0x61, 0x32, 0x0a, 0x63, 0x75,
    0x35, 0x67, 0x58, 0x77, 0x46, 0x56, 0x36, 0x54, 0x6e, 0x4f, 0x62, 0x7a,
    0x47, 0x4d, 0x37, 0x62, 0x4c, 0x46, 0x43, 0x4f, 0x35, 0x69, 0x39, 0x76,
    0x34, 0x6d, 0x4c, 0x6f, 0x34, 0x69, 0x46, 0x7a, 0x50, 0x73, 0x48, 0x6d,
    0x57, 0x44, 0x55, 0x78, 0x4b, 0x53, 0x33, 0x59, 0x38, 0x69, 0x58, 0x62,
    0x75, 0x30, 0x65, 0x59, 0x42, 0x6c, 0x4c, 0x6f, 0x4e, 0x59, 0x30, 0x6c,
    0x53, 0x76, 0x0a, 0x78, 0x44, 0x78, 0x33, 0x33, 0x4f, 0x2b, 0x44, 0x75,
    0x77, 0x4d, 0x6d, 0x56, 0x4e, 0x2b, 0x44, 0x7a, 0x53, 0x44, 0x2b, 0x45,
    0x6f, 0x64, 0x39, 0x7a, 0x66, 0x76, 0x77, 0x4f, 0x57, 0x48, 0x73, 0x61,
    0x7a, 0x59, 0x43, 0x5a, 0x54, 0x32, 0x50, 0x68, 0x4e, 0x78, 0x6e, 0x56,
    0x57, 0x49, 0x75, 0x4a, 0x58, 0x56, 0x69, 0x59, 0x34, 0x4a, 0x41, 0x48,
    0x55, 0x47, 0x6f, 0x64, 0x6a, 0x78, 0x2b, 0x0a, 0x51, 0x41, 0x69, 0x36,
    0x79, 0x43, 0x41, 0x75, 0x72, 0x55, 0x5a, 0x47, 0x76, 0x59, 0x58, 0x47,
    0x67, 0x5a, 0x53, 0x42, 0x41, 0x67, 0x4d, 0x42, 0x41, 0x41, 0x45, 0x77,
    0x44, 0x51, 0x59, 0x4a, 0x4b, 0x6f, 0x5a, 0x49, 0x68, 0x76, 0x63, 0x4e,
    0x41, 0x51, 0x45, 0x46, 0x42, 0x51, 0x41, 0x44, 0x67, 0x59, 0x45, 0x41,
    0x51, 0x6f, 0x51, 0x56, 0x44, 0x38, 0x62, 0x77, 0x64, 0x74, 0x57, 0x4a,
    0x0a, 0x41, 0x6e, 0x69, 0x47, 0x42, 0x77, 0x63, 0x43, 0x66, 0x71, 0x59,
    0x79, 0x48, 0x2b, 0x2f, 0x4b, 0x70, 0x41, 0x31, 0x30, 0x41, 0x63, 0x65,
    0x62, 0x4a, 0x56, 0x56, 0x54, 0x79, 0x59, 0x62, 0x59, 0x76, 0x49, 0x39,
    0x51, 0x38, 0x64, 0x36, 0x52, 0x53, 0x56, 0x75, 0x34, 0x50, 0x5a, 0x79,
    0x39, 0x4f, 0x41, 0x4c, 0x48, 0x52, 0x2f, 0x51, 0x72, 0x57, 0x42, 0x64,
    0x59, 0x54, 0x41, 0x79, 0x7a, 0x0a, 0x66, 0x4e, 0x41, 0x6d, 0x63, 0x32,
    0x63, 0x6d, 0x64, 0x6b, 0x53, 0x52, 0x4a, 0x7a, 0x6a, 0x68, 0x49, 0x61,
    0x4f, 0x73, 0x74, 0x6e, 0x51, 0x79, 0x31, 0x4a, 0x2b, 0x46, 0x6b, 0x30,
    0x54, 0x39, 0x58, 0x79, 0x76, 0x51, 0x74, 0x71, 0x34, 0x39, 0x39, 0x79,
    0x46, 0x62, 0x71, 0x39, 0x78, 0x6f, 0x67, 0x55, 0x56, 0x6c, 0x45, 0x47,
    0x48, 0x36, 0x32, 0x78, 0x50, 0x36, 0x76, 0x48, 0x30, 0x59, 0x0a, 0x35,
    0x75, 0x6b, 0x4b, 0x2f, 0x2f, 0x64, 0x43, 0x50, 0x41, 0x7a, 0x41, 0x31,
    0x31, 0x59, 0x75, 0x58, 0x32, 0x72, 0x6e, 0x65, 0x78, 0x30, 0x4a, 0x68,
    0x75, 0x54, 0x51, 0x66, 0x63, 0x49, 0x3d, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d,
    0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43, 0x45, 0x52, 0x54, 0x49, 0x46, 0x49,
    0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, 0x00};