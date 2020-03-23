/*
 *
 * Copyright 2016 gRPC authors.
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

extern const char test_self_signed_client_cert[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43,
    0x45, 0x52, 0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x49, 0x49, 0x44, 0x73, 0x7a, 0x43, 0x43,
    0x41, 0x70, 0x75, 0x67, 0x41, 0x77, 0x49, 0x42, 0x41, 0x67, 0x49, 0x55,
    0x4f, 0x4e, 0x57, 0x62, 0x6b, 0x55, 0x6e, 0x31, 0x6f, 0x62, 0x48, 0x43,
    0x77, 0x39, 0x4c, 0x37, 0x6c, 0x4d, 0x4e, 0x45, 0x45, 0x35, 0x52, 0x45,
    0x76, 0x62, 0x38, 0x77, 0x44, 0x51, 0x59, 0x4a, 0x4b, 0x6f, 0x5a, 0x49,
    0x68, 0x76, 0x63, 0x4e, 0x41, 0x51, 0x45, 0x4c, 0x0a, 0x42, 0x51, 0x41,
    0x77, 0x61, 0x54, 0x45, 0x4c, 0x4d, 0x41, 0x6b, 0x47, 0x41, 0x31, 0x55,
    0x45, 0x42, 0x68, 0x4d, 0x43, 0x51, 0x56, 0x55, 0x78, 0x45, 0x7a, 0x41,
    0x52, 0x42, 0x67, 0x4e, 0x56, 0x42, 0x41, 0x67, 0x4d, 0x43, 0x6c, 0x4e,
    0x76, 0x62, 0x57, 0x55, 0x74, 0x55, 0x33, 0x52, 0x68, 0x64, 0x47, 0x55,
    0x78, 0x49, 0x54, 0x41, 0x66, 0x42, 0x67, 0x4e, 0x56, 0x42, 0x41, 0x6f,
    0x4d, 0x0a, 0x47, 0x45, 0x6c, 0x75, 0x64, 0x47, 0x56, 0x79, 0x62, 0x6d,
    0x56, 0x30, 0x49, 0x46, 0x64, 0x70, 0x5a, 0x47, 0x64, 0x70, 0x64, 0x48,
    0x4d, 0x67, 0x55, 0x48, 0x52, 0x35, 0x49, 0x45, 0x78, 0x30, 0x5a, 0x44,
    0x45, 0x69, 0x4d, 0x43, 0x41, 0x47, 0x41, 0x31, 0x55, 0x45, 0x41, 0x77,
    0x77, 0x5a, 0x59, 0x6d, 0x46, 0x6b, 0x59, 0x32, 0x78, 0x70, 0x5a, 0x57,
    0x35, 0x30, 0x4c, 0x6e, 0x52, 0x6c, 0x0a, 0x63, 0x33, 0x51, 0x75, 0x5a,
    0x32, 0x39, 0x76, 0x5a, 0x32, 0x78, 0x6c, 0x4c, 0x6d, 0x4e, 0x76, 0x62,
    0x54, 0x41, 0x65, 0x46, 0x77, 0x30, 0x79, 0x4d, 0x44, 0x41, 0x7a, 0x4d,
    0x54, 0x63, 0x78, 0x4e, 0x7a, 0x51, 0x7a, 0x4d, 0x6a, 0x4e, 0x61, 0x46,
    0x77, 0x30, 0x7a, 0x4d, 0x44, 0x41, 0x7a, 0x4d, 0x54, 0x55, 0x78, 0x4e,
    0x7a, 0x51, 0x7a, 0x4d, 0x6a, 0x4e, 0x61, 0x4d, 0x47, 0x6b, 0x78, 0x0a,
    0x43, 0x7a, 0x41, 0x4a, 0x42, 0x67, 0x4e, 0x56, 0x42, 0x41, 0x59, 0x54,
    0x41, 0x6b, 0x46, 0x56, 0x4d, 0x52, 0x4d, 0x77, 0x45, 0x51, 0x59, 0x44,
    0x56, 0x51, 0x51, 0x49, 0x44, 0x41, 0x70, 0x54, 0x62, 0x32, 0x31, 0x6c,
    0x4c, 0x56, 0x4e, 0x30, 0x59, 0x58, 0x52, 0x6c, 0x4d, 0x53, 0x45, 0x77,
    0x48, 0x77, 0x59, 0x44, 0x56, 0x51, 0x51, 0x4b, 0x44, 0x42, 0x68, 0x4a,
    0x62, 0x6e, 0x52, 0x6c, 0x0a, 0x63, 0x6d, 0x35, 0x6c, 0x64, 0x43, 0x42,
    0x58, 0x61, 0x57, 0x52, 0x6e, 0x61, 0x58, 0x52, 0x7a, 0x49, 0x46, 0x42,
    0x30, 0x65, 0x53, 0x42, 0x4d, 0x64, 0x47, 0x51, 0x78, 0x49, 0x6a, 0x41,
    0x67, 0x42, 0x67, 0x4e, 0x56, 0x42, 0x41, 0x4d, 0x4d, 0x47, 0x57, 0x4a,
    0x68, 0x5a, 0x47, 0x4e, 0x73, 0x61, 0x57, 0x56, 0x75, 0x64, 0x43, 0x35,
    0x30, 0x5a, 0x58, 0x4e, 0x30, 0x4c, 0x6d, 0x64, 0x76, 0x0a, 0x62, 0x32,
    0x64, 0x73, 0x5a, 0x53, 0x35, 0x6a, 0x62, 0x32, 0x30, 0x77, 0x67, 0x67,
    0x45, 0x69, 0x4d, 0x41, 0x30, 0x47, 0x43, 0x53, 0x71, 0x47, 0x53, 0x49,
    0x62, 0x33, 0x44, 0x51, 0x45, 0x42, 0x41, 0x51, 0x55, 0x41, 0x41, 0x34,
    0x49, 0x42, 0x44, 0x77, 0x41, 0x77, 0x67, 0x67, 0x45, 0x4b, 0x41, 0x6f,
    0x49, 0x42, 0x41, 0x51, 0x44, 0x76, 0x64, 0x7a, 0x4b, 0x44, 0x54, 0x59,
    0x76, 0x52, 0x0a, 0x67, 0x6a, 0x42, 0x4f, 0x55, 0x4f, 0x72, 0x7a, 0x44,
    0x77, 0x6b, 0x41, 0x5a, 0x47, 0x77, 0x4e, 0x46, 0x48, 0x48, 0x6c, 0x4d,
    0x59, 0x79, 0x4d, 0x47, 0x49, 0x35, 0x74, 0x49, 0x74, 0x6a, 0x33, 0x74,
    0x43, 0x7a, 0x58, 0x6b, 0x62, 0x70, 0x4d, 0x30, 0x75, 0x7a, 0x33, 0x5a,
    0x6a, 0x48, 0x56, 0x61, 0x68, 0x75, 0x2b, 0x65, 0x59, 0x63, 0x2b, 0x4b,
    0x76, 0x59, 0x41, 0x70, 0x4d, 0x36, 0x34, 0x0a, 0x46, 0x32, 0x64, 0x42,
    0x62, 0x31, 0x36, 0x68, 0x73, 0x37, 0x31, 0x33, 0x46, 0x43, 0x6b, 0x38,
    0x6d, 0x69, 0x68, 0x59, 0x41, 0x42, 0x6a, 0x6e, 0x53, 0x6e, 0x64, 0x72,
    0x51, 0x73, 0x6c, 0x2f, 0x55, 0x32, 0x76, 0x38, 0x59, 0x46, 0x54, 0x37,
    0x44, 0x69, 0x70, 0x66, 0x4c, 0x52, 0x65, 0x71, 0x71, 0x61, 0x4f, 0x47,
    0x75, 0x32, 0x6f, 0x39, 0x48, 0x64, 0x76, 0x57, 0x66, 0x69, 0x55, 0x6c,
    0x0a, 0x61, 0x69, 0x43, 0x2f, 0x55, 0x47, 0x47, 0x66, 0x52, 0x2b, 0x59,
    0x62, 0x6c, 0x70, 0x4b, 0x37, 0x43, 0x47, 0x2b, 0x37, 0x2f, 0x68, 0x76,
    0x54, 0x58, 0x74, 0x55, 0x73, 0x4d, 0x77, 0x2b, 0x4f, 0x70, 0x70, 0x6f,
    0x65, 0x48, 0x39, 0x7a, 0x38, 0x37, 0x72, 0x68, 0x4f, 0x4a, 0x4d, 0x78,
    0x74, 0x69, 0x43, 0x37, 0x58, 0x77, 0x55, 0x35, 0x72, 0x68, 0x45, 0x6d,
    0x61, 0x62, 0x2f, 0x31, 0x66, 0x0a, 0x31, 0x58, 0x4d, 0x2f, 0x6e, 0x4c,
    0x6f, 0x5a, 0x72, 0x66, 0x44, 0x41, 0x63, 0x54, 0x62, 0x44, 0x79, 0x77,
    0x6f, 0x65, 0x75, 0x38, 0x32, 0x36, 0x53, 0x4a, 0x33, 0x6d, 0x69, 0x66,
    0x61, 0x6a, 0x71, 0x37, 0x6f, 0x4b, 0x33, 0x4c, 0x44, 0x64, 0x4e, 0x4c,
    0x6a, 0x57, 0x5a, 0x77, 0x66, 0x45, 0x73, 0x43, 0x4f, 0x31, 0x71, 0x70,
    0x32, 0x43, 0x34, 0x67, 0x4c, 0x76, 0x42, 0x6c, 0x4f, 0x4f, 0x0a, 0x4b,
    0x73, 0x57, 0x4f, 0x4c, 0x4e, 0x62, 0x79, 0x36, 0x42, 0x79, 0x78, 0x43,
    0x4f, 0x50, 0x6c, 0x43, 0x54, 0x61, 0x30, 0x55, 0x43, 0x61, 0x56, 0x75,
    0x6f, 0x4e, 0x63, 0x6c, 0x59, 0x6f, 0x6c, 0x37, 0x31, 0x6a, 0x79, 0x69,
    0x31, 0x37, 0x4b, 0x57, 0x2b, 0x4e, 0x6b, 0x30, 0x6e, 0x4e, 0x65, 0x39,
    0x79, 0x61, 0x56, 0x63, 0x79, 0x72, 0x36, 0x48, 0x30, 0x7a, 0x33, 0x62,
    0x49, 0x6d, 0x66, 0x0a, 0x4a, 0x68, 0x62, 0x53, 0x75, 0x34, 0x72, 0x7a,
    0x49, 0x39, 0x33, 0x6e, 0x41, 0x67, 0x4d, 0x42, 0x41, 0x41, 0x47, 0x6a,
    0x55, 0x7a, 0x42, 0x52, 0x4d, 0x42, 0x30, 0x47, 0x41, 0x31, 0x55, 0x64,
    0x44, 0x67, 0x51, 0x57, 0x42, 0x42, 0x54, 0x4b, 0x4a, 0x73, 0x6b, 0x45,
    0x59, 0x64, 0x32, 0x6e, 0x64, 0x72, 0x77, 0x69, 0x68, 0x50, 0x54, 0x67,
    0x32, 0x50, 0x7a, 0x59, 0x46, 0x2f, 0x6b, 0x50, 0x0a, 0x67, 0x7a, 0x41,
    0x66, 0x42, 0x67, 0x4e, 0x56, 0x48, 0x53, 0x4d, 0x45, 0x47, 0x44, 0x41,
    0x57, 0x67, 0x42, 0x54, 0x4b, 0x4a, 0x73, 0x6b, 0x45, 0x59, 0x64, 0x32,
    0x6e, 0x64, 0x72, 0x77, 0x69, 0x68, 0x50, 0x54, 0x67, 0x32, 0x50, 0x7a,
    0x59, 0x46, 0x2f, 0x6b, 0x50, 0x67, 0x7a, 0x41, 0x50, 0x42, 0x67, 0x4e,
    0x56, 0x48, 0x52, 0x4d, 0x42, 0x41, 0x66, 0x38, 0x45, 0x42, 0x54, 0x41,
    0x44, 0x0a, 0x41, 0x51, 0x48, 0x2f, 0x4d, 0x41, 0x30, 0x47, 0x43, 0x53,
    0x71, 0x47, 0x53, 0x49, 0x62, 0x33, 0x44, 0x51, 0x45, 0x42, 0x43, 0x77,
    0x55, 0x41, 0x41, 0x34, 0x49, 0x42, 0x41, 0x51, 0x42, 0x6f, 0x47, 0x77,
    0x57, 0x52, 0x30, 0x70, 0x4c, 0x4d, 0x31, 0x69, 0x63, 0x58, 0x34, 0x62,
    0x49, 0x4a, 0x36, 0x79, 0x64, 0x75, 0x46, 0x55, 0x2f, 0x41, 0x34, 0x6a,
    0x53, 0x69, 0x71, 0x45, 0x54, 0x36, 0x0a, 0x67, 0x76, 0x4a, 0x68, 0x77,
    0x67, 0x45, 0x72, 0x69, 0x6c, 0x71, 0x54, 0x4b, 0x66, 0x48, 0x36, 0x59,
    0x38, 0x39, 0x72, 0x71, 0x74, 0x7a, 0x57, 0x38, 0x6b, 0x34, 0x55, 0x75,
    0x72, 0x41, 0x4f, 0x43, 0x73, 0x45, 0x34, 0x46, 0x41, 0x36, 0x77, 0x62,
    0x6b, 0x48, 0x57, 0x77, 0x72, 0x55, 0x4d, 0x6e, 0x43, 0x6c, 0x59, 0x34,
    0x6c, 0x6b, 0x48, 0x4a, 0x68, 0x2b, 0x4d, 0x75, 0x4e, 0x61, 0x4a, 0x0a,
    0x6e, 0x43, 0x47, 0x72, 0x4b, 0x38, 0x77, 0x52, 0x4b, 0x47, 0x62, 0x2f,
    0x6d, 0x71, 0x57, 0x39, 0x64, 0x35, 0x70, 0x50, 0x37, 0x32, 0x45, 0x74,
    0x31, 0x51, 0x36, 0x4f, 0x57, 0x36, 0x44, 0x41, 0x4b, 0x71, 0x47, 0x66,
    0x6a, 0x44, 0x57, 0x68, 0x32, 0x4d, 0x7a, 0x53, 0x50, 0x48, 0x42, 0x78,
    0x63, 0x43, 0x4c, 0x65, 0x79, 0x69, 0x67, 0x4f, 0x31, 0x77, 0x71, 0x64,
    0x34, 0x57, 0x31, 0x54, 0x0a, 0x6e, 0x76, 0x76, 0x71, 0x6c, 0x36, 0x6c,
    0x34, 0x4c, 0x2b, 0x42, 0x35, 0x49, 0x54, 0x2f, 0x63, 0x2b, 0x2f, 0x45,
    0x48, 0x4f, 0x33, 0x50, 0x77, 0x62, 0x49, 0x39, 0x76, 0x36, 0x4d, 0x47,
    0x54, 0x74, 0x4c, 0x6a, 0x73, 0x5a, 0x67, 0x6b, 0x52, 0x4b, 0x49, 0x74,
    0x61, 0x50, 0x68, 0x2b, 0x59, 0x65, 0x4a, 0x64, 0x6d, 0x42, 0x59, 0x68,
    0x52, 0x44, 0x31, 0x42, 0x76, 0x57, 0x62, 0x36, 0x73, 0x0a, 0x56, 0x77,
    0x45, 0x62, 0x37, 0x61, 0x51, 0x31, 0x6f, 0x53, 0x46, 0x2b, 0x65, 0x73,
    0x55, 0x76, 0x4d, 0x6d, 0x6a, 0x47, 0x56, 0x75, 0x48, 0x58, 0x75, 0x51,
    0x76, 0x57, 0x4a, 0x61, 0x68, 0x6e, 0x6a, 0x59, 0x64, 0x59, 0x54, 0x32,
    0x44, 0x69, 0x6b, 0x79, 0x71, 0x52, 0x2b, 0x41, 0x77, 0x61, 0x4b, 0x7a,
    0x72, 0x65, 0x34, 0x47, 0x4a, 0x4d, 0x48, 0x73, 0x58, 0x33, 0x2f, 0x43,
    0x66, 0x38, 0x0a, 0x71, 0x64, 0x78, 0x79, 0x49, 0x2b, 0x42, 0x31, 0x6a,
    0x55, 0x77, 0x4e, 0x72, 0x37, 0x73, 0x4c, 0x41, 0x32, 0x45, 0x59, 0x44,
    0x6a, 0x6e, 0x55, 0x52, 0x30, 0x6a, 0x45, 0x48, 0x63, 0x72, 0x4f, 0x42,
    0x53, 0x70, 0x49, 0x51, 0x79, 0x52, 0x4d, 0x47, 0x57, 0x64, 0x75, 0x6a,
    0x30, 0x50, 0x31, 0x36, 0x79, 0x62, 0x39, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d,
    0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43, 0x45, 0x52, 0x54, 0x49, 0x46, 0x49,
    0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a};

extern const char test_self_signed_client_key[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x50,
    0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x49, 0x49, 0x45, 0x76, 0x67, 0x49, 0x42,
    0x41, 0x44, 0x41, 0x4e, 0x42, 0x67, 0x6b, 0x71, 0x68, 0x6b, 0x69, 0x47,
    0x39, 0x77, 0x30, 0x42, 0x41, 0x51, 0x45, 0x46, 0x41, 0x41, 0x53, 0x43,
    0x42, 0x4b, 0x67, 0x77, 0x67, 0x67, 0x53, 0x6b, 0x41, 0x67, 0x45, 0x41,
    0x41, 0x6f, 0x49, 0x42, 0x41, 0x51, 0x44, 0x76, 0x64, 0x7a, 0x4b, 0x44,
    0x54, 0x59, 0x76, 0x52, 0x67, 0x6a, 0x42, 0x4f, 0x0a, 0x55, 0x4f, 0x72,
    0x7a, 0x44, 0x77, 0x6b, 0x41, 0x5a, 0x47, 0x77, 0x4e, 0x46, 0x48, 0x48,
    0x6c, 0x4d, 0x59, 0x79, 0x4d, 0x47, 0x49, 0x35, 0x74, 0x49, 0x74, 0x6a,
    0x33, 0x74, 0x43, 0x7a, 0x58, 0x6b, 0x62, 0x70, 0x4d, 0x30, 0x75, 0x7a,
    0x33, 0x5a, 0x6a, 0x48, 0x56, 0x61, 0x68, 0x75, 0x2b, 0x65, 0x59, 0x63,
    0x2b, 0x4b, 0x76, 0x59, 0x41, 0x70, 0x4d, 0x36, 0x34, 0x46, 0x32, 0x64,
    0x42, 0x0a, 0x62, 0x31, 0x36, 0x68, 0x73, 0x37, 0x31, 0x33, 0x46, 0x43,
    0x6b, 0x38, 0x6d, 0x69, 0x68, 0x59, 0x41, 0x42, 0x6a, 0x6e, 0x53, 0x6e,
    0x64, 0x72, 0x51, 0x73, 0x6c, 0x2f, 0x55, 0x32, 0x76, 0x38, 0x59, 0x46,
    0x54, 0x37, 0x44, 0x69, 0x70, 0x66, 0x4c, 0x52, 0x65, 0x71, 0x71, 0x61,
    0x4f, 0x47, 0x75, 0x32, 0x6f, 0x39, 0x48, 0x64, 0x76, 0x57, 0x66, 0x69,
    0x55, 0x6c, 0x61, 0x69, 0x43, 0x2f, 0x0a, 0x55, 0x47, 0x47, 0x66, 0x52,
    0x2b, 0x59, 0x62, 0x6c, 0x70, 0x4b, 0x37, 0x43, 0x47, 0x2b, 0x37, 0x2f,
    0x68, 0x76, 0x54, 0x58, 0x74, 0x55, 0x73, 0x4d, 0x77, 0x2b, 0x4f, 0x70,
    0x70, 0x6f, 0x65, 0x48, 0x39, 0x7a, 0x38, 0x37, 0x72, 0x68, 0x4f, 0x4a,
    0x4d, 0x78, 0x74, 0x69, 0x43, 0x37, 0x58, 0x77, 0x55, 0x35, 0x72, 0x68,
    0x45, 0x6d, 0x61, 0x62, 0x2f, 0x31, 0x66, 0x31, 0x58, 0x4d, 0x2f, 0x0a,
    0x6e, 0x4c, 0x6f, 0x5a, 0x72, 0x66, 0x44, 0x41, 0x63, 0x54, 0x62, 0x44,
    0x79, 0x77, 0x6f, 0x65, 0x75, 0x38, 0x32, 0x36, 0x53, 0x4a, 0x33, 0x6d,
    0x69, 0x66, 0x61, 0x6a, 0x71, 0x37, 0x6f, 0x4b, 0x33, 0x4c, 0x44, 0x64,
    0x4e, 0x4c, 0x6a, 0x57, 0x5a, 0x77, 0x66, 0x45, 0x73, 0x43, 0x4f, 0x31,
    0x71, 0x70, 0x32, 0x43, 0x34, 0x67, 0x4c, 0x76, 0x42, 0x6c, 0x4f, 0x4f,
    0x4b, 0x73, 0x57, 0x4f, 0x0a, 0x4c, 0x4e, 0x62, 0x79, 0x36, 0x42, 0x79,
    0x78, 0x43, 0x4f, 0x50, 0x6c, 0x43, 0x54, 0x61, 0x30, 0x55, 0x43, 0x61,
    0x56, 0x75, 0x6f, 0x4e, 0x63, 0x6c, 0x59, 0x6f, 0x6c, 0x37, 0x31, 0x6a,
    0x79, 0x69, 0x31, 0x37, 0x4b, 0x57, 0x2b, 0x4e, 0x6b, 0x30, 0x6e, 0x4e,
    0x65, 0x39, 0x79, 0x61, 0x56, 0x63, 0x79, 0x72, 0x36, 0x48, 0x30, 0x7a,
    0x33, 0x62, 0x49, 0x6d, 0x66, 0x4a, 0x68, 0x62, 0x53, 0x0a, 0x75, 0x34,
    0x72, 0x7a, 0x49, 0x39, 0x33, 0x6e, 0x41, 0x67, 0x4d, 0x42, 0x41, 0x41,
    0x45, 0x43, 0x67, 0x67, 0x45, 0x42, 0x41, 0x4f, 0x49, 0x50, 0x4f, 0x4a,
    0x52, 0x54, 0x70, 0x47, 0x61, 0x48, 0x37, 0x47, 0x70, 0x43, 0x59, 0x55,
    0x70, 0x4c, 0x4b, 0x30, 0x67, 0x2f, 0x68, 0x50, 0x46, 0x6b, 0x46, 0x35,
    0x45, 0x79, 0x45, 0x57, 0x67, 0x2f, 0x31, 0x6c, 0x53, 0x59, 0x7a, 0x52,
    0x49, 0x70, 0x0a, 0x2b, 0x52, 0x73, 0x58, 0x36, 0x7a, 0x4f, 0x53, 0x2b,
    0x7a, 0x6b, 0x69, 0x4e, 0x48, 0x45, 0x76, 0x31, 0x6a, 0x6b, 0x65, 0x4b,
    0x4e, 0x6f, 0x37, 0x58, 0x44, 0x69, 0x48, 0x58, 0x4d, 0x37, 0x55, 0x36,
    0x52, 0x6b, 0x51, 0x74, 0x64, 0x6b, 0x5a, 0x41, 0x51, 0x64, 0x6b, 0x39,
    0x50, 0x6a, 0x4d, 0x33, 0x73, 0x45, 0x55, 0x64, 0x6d, 0x34, 0x43, 0x45,
    0x6e, 0x49, 0x6a, 0x66, 0x6d, 0x7a, 0x41, 0x0a, 0x70, 0x2f, 0x52, 0x38,
    0x54, 0x44, 0x30, 0x6b, 0x78, 0x6b, 0x4e, 0x4c, 0x49, 0x6b, 0x68, 0x75,
    0x46, 0x48, 0x32, 0x67, 0x64, 0x30, 0x35, 0x79, 0x33, 0x5a, 0x48, 0x44,
    0x53, 0x2f, 0x58, 0x69, 0x46, 0x6b, 0x41, 0x45, 0x39, 0x65, 0x4f, 0x54,
    0x30, 0x46, 0x72, 0x43, 0x37, 0x6f, 0x6d, 0x36, 0x45, 0x53, 0x44, 0x37,
    0x5a, 0x66, 0x46, 0x49, 0x57, 0x52, 0x31, 0x38, 0x70, 0x6e, 0x63, 0x57,
    0x0a, 0x5a, 0x47, 0x71, 0x37, 0x74, 0x46, 0x41, 0x5a, 0x5a, 0x52, 0x6d,
    0x70, 0x6b, 0x75, 0x6d, 0x32, 0x44, 0x2b, 0x4d, 0x4a, 0x79, 0x31, 0x67,
    0x57, 0x78, 0x49, 0x58, 0x42, 0x78, 0x74, 0x35, 0x6d, 0x61, 0x64, 0x54,
    0x45, 0x70, 0x52, 0x78, 0x51, 0x64, 0x35, 0x36, 0x74, 0x6f, 0x45, 0x6e,
    0x66, 0x78, 0x33, 0x37, 0x32, 0x46, 0x30, 0x79, 0x34, 0x7a, 0x6b, 0x63,
    0x58, 0x33, 0x70, 0x6e, 0x45, 0x0a, 0x34, 0x48, 0x36, 0x46, 0x61, 0x4a,
    0x55, 0x42, 0x6a, 0x64, 0x76, 0x4b, 0x6c, 0x32, 0x51, 0x7a, 0x46, 0x35,
    0x63, 0x30, 0x6a, 0x42, 0x71, 0x67, 0x78, 0x4d, 0x52, 0x76, 0x57, 0x50,
    0x35, 0x59, 0x66, 0x4e, 0x75, 0x38, 0x2b, 0x64, 0x6d, 0x61, 0x51, 0x4f,
    0x52, 0x50, 0x6b, 0x70, 0x7a, 0x53, 0x70, 0x74, 0x4f, 0x50, 0x6d, 0x5a,
    0x4d, 0x39, 0x56, 0x4b, 0x56, 0x2b, 0x74, 0x4a, 0x56, 0x53, 0x0a, 0x31,
    0x78, 0x6e, 0x4f, 0x49, 0x36, 0x44, 0x74, 0x72, 0x6e, 0x4e, 0x5a, 0x52,
    0x6f, 0x6a, 0x65, 0x67, 0x52, 0x2f, 0x45, 0x36, 0x4b, 0x68, 0x4e, 0x79,
    0x69, 0x50, 0x54, 0x59, 0x79, 0x39, 0x37, 0x55, 0x67, 0x59, 0x7a, 0x64,
    0x4b, 0x53, 0x2b, 0x53, 0x53, 0x45, 0x43, 0x67, 0x59, 0x45, 0x41, 0x2b,
    0x77, 0x67, 0x53, 0x49, 0x71, 0x72, 0x66, 0x6b, 0x65, 0x71, 0x71, 0x6f,
    0x74, 0x4a, 0x78, 0x0a, 0x63, 0x47, 0x78, 0x46, 0x34, 0x78, 0x39, 0x76,
    0x2f, 0x6c, 0x64, 0x4b, 0x72, 0x35, 0x68, 0x6c, 0x68, 0x4a, 0x4e, 0x6f,
    0x4b, 0x58, 0x4c, 0x6b, 0x65, 0x70, 0x6b, 0x63, 0x72, 0x76, 0x68, 0x68,
    0x78, 0x66, 0x48, 0x4b, 0x67, 0x6a, 0x57, 0x7a, 0x31, 0x6e, 0x5a, 0x59,
    0x2f, 0x2b, 0x52, 0x70, 0x67, 0x34, 0x32, 0x47, 0x46, 0x4d, 0x76, 0x78,
    0x57, 0x52, 0x72, 0x47, 0x54, 0x4d, 0x49, 0x4a, 0x0a, 0x64, 0x64, 0x69,
    0x4f, 0x72, 0x32, 0x34, 0x70, 0x30, 0x48, 0x43, 0x6b, 0x75, 0x73, 0x57,
    0x52, 0x4d, 0x4b, 0x51, 0x4c, 0x37, 0x58, 0x78, 0x76, 0x75, 0x48, 0x44,
    0x71, 0x30, 0x72, 0x6f, 0x38, 0x53, 0x47, 0x71, 0x58, 0x7a, 0x71, 0x57,
    0x47, 0x75, 0x48, 0x33, 0x31, 0x52, 0x2b, 0x59, 0x4e, 0x50, 0x38, 0x64,
    0x79, 0x32, 0x70, 0x71, 0x64, 0x33, 0x4f, 0x6c, 0x77, 0x7a, 0x54, 0x67,
    0x67, 0x0a, 0x38, 0x76, 0x30, 0x77, 0x77, 0x7a, 0x78, 0x38, 0x41, 0x75,
    0x79, 0x50, 0x35, 0x59, 0x73, 0x34, 0x4d, 0x32, 0x30, 0x45, 0x77, 0x76,
    0x37, 0x58, 0x75, 0x79, 0x30, 0x43, 0x67, 0x59, 0x45, 0x41, 0x39, 0x44,
    0x53, 0x47, 0x4d, 0x55, 0x38, 0x6a, 0x6d, 0x6a, 0x78, 0x4a, 0x2f, 0x75,
    0x50, 0x44, 0x43, 0x58, 0x57, 0x4f, 0x45, 0x41, 0x71, 0x74, 0x45, 0x37,
    0x38, 0x77, 0x54, 0x74, 0x49, 0x77, 0x0a, 0x75, 0x4d, 0x42, 0x76, 0x2b,
    0x67, 0x65, 0x30, 0x69, 0x6e, 0x63, 0x33, 0x37, 0x78, 0x66, 0x2b, 0x66,
    0x4e, 0x36, 0x44, 0x2f, 0x7a, 0x69, 0x54, 0x72, 0x4a, 0x76, 0x67, 0x77,
    0x2f, 0x58, 0x79, 0x54, 0x31, 0x35, 0x70, 0x6d, 0x51, 0x64, 0x4f, 0x6c,
    0x58, 0x78, 0x33, 0x53, 0x67, 0x31, 0x68, 0x39, 0x58, 0x42, 0x5a, 0x65,
    0x49, 0x6c, 0x61, 0x65, 0x43, 0x64, 0x46, 0x57, 0x72, 0x46, 0x42, 0x0a,
    0x6f, 0x59, 0x72, 0x56, 0x73, 0x69, 0x75, 0x6f, 0x58, 0x52, 0x73, 0x77,
    0x66, 0x6b, 0x46, 0x77, 0x41, 0x30, 0x79, 0x4f, 0x6b, 0x43, 0x73, 0x48,
    0x79, 0x47, 0x69, 0x49, 0x34, 0x54, 0x45, 0x30, 0x57, 0x31, 0x72, 0x47,
    0x62, 0x71, 0x50, 0x31, 0x35, 0x38, 0x49, 0x6a, 0x77, 0x58, 0x50, 0x63,
    0x7a, 0x42, 0x73, 0x77, 0x57, 0x49, 0x37, 0x69, 0x2f, 0x44, 0x36, 0x4c,
    0x70, 0x49, 0x4e, 0x4c, 0x0a, 0x42, 0x44, 0x37, 0x59, 0x59, 0x70, 0x66,
    0x48, 0x6d, 0x65, 0x4d, 0x43, 0x67, 0x59, 0x42, 0x30, 0x38, 0x41, 0x69,
    0x4b, 0x72, 0x37, 0x43, 0x66, 0x35, 0x34, 0x48, 0x2f, 0x67, 0x53, 0x71,
    0x6f, 0x35, 0x54, 0x63, 0x56, 0x47, 0x7a, 0x4c, 0x76, 0x64, 0x7a, 0x68,
    0x71, 0x58, 0x67, 0x4b, 0x45, 0x5a, 0x4b, 0x70, 0x30, 0x44, 0x48, 0x70,
    0x55, 0x68, 0x66, 0x69, 0x76, 0x70, 0x54, 0x4c, 0x65, 0x0a, 0x6f, 0x38,
    0x6a, 0x6a, 0x4b, 0x53, 0x4d, 0x53, 0x4e, 0x32, 0x55, 0x30, 0x4a, 0x76,
    0x48, 0x6a, 0x2f, 0x30, 0x78, 0x44, 0x61, 0x64, 0x47, 0x4f, 0x34, 0x59,
    0x4d, 0x59, 0x68, 0x4a, 0x63, 0x6c, 0x6c, 0x33, 0x43, 0x34, 0x56, 0x67,
    0x67, 0x53, 0x65, 0x6a, 0x61, 0x79, 0x62, 0x70, 0x41, 0x34, 0x36, 0x57,
    0x4a, 0x4a, 0x43, 0x64, 0x74, 0x39, 0x50, 0x74, 0x53, 0x55, 0x76, 0x33,
    0x36, 0x50, 0x0a, 0x65, 0x57, 0x41, 0x6f, 0x4f, 0x6b, 0x46, 0x73, 0x74,
    0x66, 0x68, 0x4a, 0x75, 0x75, 0x66, 0x58, 0x47, 0x78, 0x44, 0x73, 0x74,
    0x6e, 0x50, 0x74, 0x55, 0x61, 0x31, 0x6a, 0x57, 0x38, 0x38, 0x31, 0x67,
    0x69, 0x35, 0x78, 0x39, 0x44, 0x34, 0x4d, 0x6d, 0x71, 0x68, 0x5a, 0x6c,
    0x4b, 0x58, 0x6b, 0x68, 0x74, 0x64, 0x65, 0x41, 0x70, 0x72, 0x36, 0x4c,
    0x51, 0x4b, 0x42, 0x67, 0x51, 0x44, 0x64, 0x0a, 0x49, 0x74, 0x73, 0x4a,
    0x74, 0x39, 0x4a, 0x54, 0x6a, 0x70, 0x69, 0x72, 0x47, 0x66, 0x43, 0x35,
    0x6c, 0x68, 0x77, 0x49, 0x35, 0x73, 0x49, 0x49, 0x43, 0x61, 0x39, 0x6a,
    0x45, 0x4f, 0x39, 0x52, 0x76, 0x65, 0x45, 0x6f, 0x6c, 0x75, 0x57, 0x6b,
    0x4a, 0x59, 0x55, 0x66, 0x47, 0x36, 0x6b, 0x31, 0x78, 0x67, 0x48, 0x64,
    0x6b, 0x59, 0x77, 0x59, 0x57, 0x43, 0x64, 0x58, 0x44, 0x46, 0x5a, 0x61,
    0x0a, 0x44, 0x50, 0x4b, 0x75, 0x77, 0x6e, 0x45, 0x6b, 0x36, 0x4d, 0x72,
    0x55, 0x34, 0x66, 0x31, 0x38, 0x31, 0x6a, 0x6f, 0x4f, 0x37, 0x73, 0x4a,
    0x66, 0x33, 0x35, 0x2f, 0x73, 0x47, 0x6d, 0x75, 0x47, 0x4c, 0x30, 0x53,
    0x48, 0x7a, 0x51, 0x54, 0x76, 0x47, 0x76, 0x6e, 0x30, 0x75, 0x71, 0x6b,
    0x47, 0x4d, 0x38, 0x4d, 0x39, 0x52, 0x64, 0x6f, 0x4d, 0x58, 0x71, 0x7a,
    0x6b, 0x7a, 0x7a, 0x76, 0x4d, 0x0a, 0x4a, 0x67, 0x31, 0x65, 0x6a, 0x31,
    0x62, 0x55, 0x67, 0x58, 0x63, 0x44, 0x62, 0x54, 0x6e, 0x61, 0x45, 0x68,
    0x7a, 0x62, 0x64, 0x4c, 0x69, 0x54, 0x46, 0x73, 0x67, 0x35, 0x4e, 0x7a,
    0x4d, 0x74, 0x4b, 0x77, 0x4f, 0x6a, 0x64, 0x44, 0x49, 0x70, 0x5a, 0x51,
    0x4b, 0x42, 0x67, 0x45, 0x49, 0x48, 0x65, 0x4a, 0x49, 0x71, 0x69, 0x47,
    0x6a, 0x59, 0x67, 0x66, 0x37, 0x6d, 0x55, 0x6c, 0x58, 0x32, 0x0a, 0x76,
    0x4e, 0x57, 0x67, 0x46, 0x4e, 0x6c, 0x7a, 0x41, 0x70, 0x6b, 0x46, 0x53,
    0x43, 0x51, 0x38, 0x54, 0x6b, 0x7a, 0x6b, 0x44, 0x4f, 0x6a, 0x74, 0x43,
    0x64, 0x53, 0x48, 0x66, 0x64, 0x52, 0x44, 0x4a, 0x36, 0x2b, 0x71, 0x38,
    0x63, 0x53, 0x32, 0x54, 0x53, 0x51, 0x37, 0x51, 0x50, 0x6f, 0x41, 0x6c,
    0x49, 0x31, 0x77, 0x6f, 0x53, 0x30, 0x47, 0x34, 0x38, 0x54, 0x4e, 0x62,
    0x56, 0x53, 0x6f, 0x0a, 0x77, 0x44, 0x30, 0x6a, 0x4e, 0x56, 0x52, 0x54,
    0x64, 0x70, 0x41, 0x36, 0x52, 0x35, 0x46, 0x50, 0x73, 0x67, 0x30, 0x39,
    0x6f, 0x68, 0x42, 0x2f, 0x63, 0x61, 0x53, 0x6e, 0x30, 0x7a, 0x6c, 0x47,
    0x56, 0x68, 0x61, 0x32, 0x47, 0x53, 0x30, 0x38, 0x63, 0x65, 0x59, 0x72,
    0x6e, 0x37, 0x6e, 0x6e, 0x34, 0x50, 0x53, 0x5a, 0x2f, 0x55, 0x49, 0x59,
    0x54, 0x6d, 0x33, 0x70, 0x6a, 0x55, 0x6c, 0x56, 0x0a, 0x48, 0x35, 0x74,
    0x76, 0x48, 0x76, 0x30, 0x67, 0x47, 0x32, 0x43, 0x35, 0x76, 0x79, 0x33,
    0x74, 0x49, 0x59, 0x51, 0x74, 0x53, 0x51, 0x43, 0x6b, 0x0a, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x50, 0x52, 0x49, 0x56, 0x41,
    0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a};

extern const char test_signed_client_cert[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43,
    0x45, 0x52, 0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x49, 0x49, 0x44, 0x4e, 0x7a, 0x43, 0x43,
    0x41, 0x68, 0x38, 0x43, 0x46, 0x47, 0x79, 0x58, 0x30, 0x30, 0x52, 0x43,
    0x65, 0x70, 0x4f, 0x76, 0x2f, 0x71, 0x43, 0x4a, 0x31, 0x6f, 0x56, 0x64,
    0x54, 0x74, 0x59, 0x39, 0x32, 0x55, 0x38, 0x33, 0x4d, 0x41, 0x30, 0x47,
    0x43, 0x53, 0x71, 0x47, 0x53, 0x49, 0x62, 0x33, 0x44, 0x51, 0x45, 0x42,
    0x43, 0x77, 0x55, 0x41, 0x4d, 0x46, 0x59, 0x78, 0x0a, 0x43, 0x7a, 0x41,
    0x4a, 0x42, 0x67, 0x4e, 0x56, 0x42, 0x41, 0x59, 0x54, 0x41, 0x6b, 0x46,
    0x56, 0x4d, 0x52, 0x4d, 0x77, 0x45, 0x51, 0x59, 0x44, 0x56, 0x51, 0x51,
    0x49, 0x44, 0x41, 0x70, 0x54, 0x62, 0x32, 0x31, 0x6c, 0x4c, 0x56, 0x4e,
    0x30, 0x59, 0x58, 0x52, 0x6c, 0x4d, 0x53, 0x45, 0x77, 0x48, 0x77, 0x59,
    0x44, 0x56, 0x51, 0x51, 0x4b, 0x44, 0x42, 0x68, 0x4a, 0x62, 0x6e, 0x52,
    0x6c, 0x0a, 0x63, 0x6d, 0x35, 0x6c, 0x64, 0x43, 0x42, 0x58, 0x61, 0x57,
    0x52, 0x6e, 0x61, 0x58, 0x52, 0x7a, 0x49, 0x46, 0x42, 0x30, 0x65, 0x53,
    0x42, 0x4d, 0x64, 0x47, 0x51, 0x78, 0x44, 0x7a, 0x41, 0x4e, 0x42, 0x67,
    0x4e, 0x56, 0x42, 0x41, 0x4d, 0x4d, 0x42, 0x6e, 0x52, 0x6c, 0x63, 0x33,
    0x52, 0x6a, 0x59, 0x54, 0x41, 0x65, 0x46, 0x77, 0x30, 0x79, 0x4d, 0x44,
    0x41, 0x7a, 0x4d, 0x54, 0x67, 0x77, 0x0a, 0x4d, 0x54, 0x41, 0x32, 0x4d,
    0x54, 0x42, 0x61, 0x46, 0x77, 0x30, 0x7a, 0x4d, 0x44, 0x41, 0x7a, 0x4d,
    0x54, 0x59, 0x77, 0x4d, 0x54, 0x41, 0x32, 0x4d, 0x54, 0x42, 0x61, 0x4d,
    0x46, 0x6f, 0x78, 0x43, 0x7a, 0x41, 0x4a, 0x42, 0x67, 0x4e, 0x56, 0x42,
    0x41, 0x59, 0x54, 0x41, 0x6b, 0x46, 0x56, 0x4d, 0x52, 0x4d, 0x77, 0x45,
    0x51, 0x59, 0x44, 0x56, 0x51, 0x51, 0x49, 0x44, 0x41, 0x70, 0x54, 0x0a,
    0x62, 0x32, 0x31, 0x6c, 0x4c, 0x56, 0x4e, 0x30, 0x59, 0x58, 0x52, 0x6c,
    0x4d, 0x53, 0x45, 0x77, 0x48, 0x77, 0x59, 0x44, 0x56, 0x51, 0x51, 0x4b,
    0x44, 0x42, 0x68, 0x4a, 0x62, 0x6e, 0x52, 0x6c, 0x63, 0x6d, 0x35, 0x6c,
    0x64, 0x43, 0x42, 0x58, 0x61, 0x57, 0x52, 0x6e, 0x61, 0x58, 0x52, 0x7a,
    0x49, 0x46, 0x42, 0x30, 0x65, 0x53, 0x42, 0x4d, 0x64, 0x47, 0x51, 0x78,
    0x45, 0x7a, 0x41, 0x52, 0x0a, 0x42, 0x67, 0x4e, 0x56, 0x42, 0x41, 0x4d,
    0x4d, 0x43, 0x6e, 0x52, 0x6c, 0x63, 0x33, 0x52, 0x6a, 0x62, 0x47, 0x6c,
    0x6c, 0x62, 0x6e, 0x51, 0x77, 0x67, 0x67, 0x45, 0x69, 0x4d, 0x41, 0x30,
    0x47, 0x43, 0x53, 0x71, 0x47, 0x53, 0x49, 0x62, 0x33, 0x44, 0x51, 0x45,
    0x42, 0x41, 0x51, 0x55, 0x41, 0x41, 0x34, 0x49, 0x42, 0x44, 0x77, 0x41,
    0x77, 0x67, 0x67, 0x45, 0x4b, 0x41, 0x6f, 0x49, 0x42, 0x0a, 0x41, 0x51,
    0x43, 0x79, 0x71, 0x59, 0x52, 0x70, 0x2b, 0x44, 0x58, 0x56, 0x70, 0x37,
    0x32, 0x4e, 0x46, 0x62, 0x51, 0x48, 0x38, 0x68, 0x64, 0x68, 0x54, 0x5a,
    0x4c, 0x79, 0x63, 0x5a, 0x58, 0x4f, 0x6c, 0x4a, 0x68, 0x6d, 0x4d, 0x73,
    0x72, 0x4a, 0x6d, 0x72, 0x6a, 0x6e, 0x32, 0x70, 0x37, 0x70, 0x49, 0x2f,
    0x38, 0x6d, 0x54, 0x5a, 0x2f, 0x30, 0x46, 0x43, 0x2b, 0x53, 0x47, 0x57,
    0x42, 0x47, 0x0a, 0x5a, 0x56, 0x2b, 0x45, 0x4c, 0x69, 0x48, 0x72, 0x6d,
    0x43, 0x58, 0x35, 0x7a, 0x66, 0x61, 0x49, 0x4c, 0x72, 0x39, 0x49, 0x75,
    0x77, 0x37, 0x47, 0x68, 0x72, 0x33, 0x56, 0x7a, 0x6f, 0x65, 0x66, 0x69,
    0x38, 0x72, 0x36, 0x32, 0x72, 0x4c, 0x75, 0x70, 0x56, 0x50, 0x4e, 0x69,
    0x2f, 0x71, 0x64, 0x71, 0x79, 0x6a, 0x57, 0x6b, 0x32, 0x64, 0x45, 0x43,
    0x48, 0x43, 0x39, 0x5a, 0x33, 0x2b, 0x41, 0x0a, 0x67, 0x33, 0x4b, 0x7a,
    0x4b, 0x54, 0x79, 0x65, 0x72, 0x58, 0x57, 0x6a, 0x4b, 0x63, 0x76, 0x79,
    0x4b, 0x56, 0x6d, 0x4d, 0x30, 0x5a, 0x78, 0x45, 0x30, 0x52, 0x58, 0x68,
    0x44, 0x57, 0x2f, 0x52, 0x6f, 0x51, 0x62, 0x71, 0x5a, 0x73, 0x55, 0x32,
    0x47, 0x4b, 0x67, 0x31, 0x42, 0x32, 0x72, 0x68, 0x55, 0x55, 0x38, 0x4b,
    0x4e, 0x30, 0x67, 0x56, 0x6d, 0x4b, 0x6e, 0x30, 0x72, 0x4a, 0x48, 0x4f,
    0x0a, 0x78, 0x7a, 0x52, 0x56, 0x53, 0x59, 0x65, 0x59, 0x4c, 0x59, 0x70,
    0x35, 0x59, 0x6e, 0x37, 0x4b, 0x72, 0x74, 0x50, 0x4a, 0x63, 0x4b, 0x79,
    0x6f, 0x39, 0x61, 0x56, 0x75, 0x45, 0x72, 0x37, 0x64, 0x47, 0x41, 0x4e,
    0x7a, 0x70, 0x79, 0x46, 0x36, 0x6c, 0x67, 0x2f, 0x6e, 0x59, 0x42, 0x57,
    0x63, 0x2b, 0x39, 0x53, 0x47, 0x77, 0x6b, 0x6f, 0x4c, 0x64, 0x46, 0x76,
    0x4b, 0x76, 0x41, 0x42, 0x59, 0x0a, 0x4a, 0x4d, 0x79, 0x72, 0x62, 0x4e,
    0x68, 0x48, 0x55, 0x51, 0x66, 0x76, 0x30, 0x66, 0x7a, 0x61, 0x5a, 0x30,
    0x50, 0x38, 0x36, 0x64, 0x66, 0x54, 0x45, 0x4e, 0x72, 0x44, 0x78, 0x7a,
    0x41, 0x4c, 0x72, 0x7a, 0x47, 0x6e, 0x71, 0x63, 0x78, 0x33, 0x4b, 0x54,
    0x72, 0x77, 0x4a, 0x6a, 0x6b, 0x5a, 0x2f, 0x61, 0x53, 0x72, 0x31, 0x74,
    0x79, 0x44, 0x30, 0x2f, 0x74, 0x58, 0x76, 0x75, 0x6b, 0x52, 0x0a, 0x46,
    0x69, 0x50, 0x78, 0x57, 0x42, 0x4a, 0x68, 0x6a, 0x48, 0x51, 0x37, 0x30,
    0x47, 0x71, 0x54, 0x46, 0x51, 0x59, 0x31, 0x39, 0x52, 0x62, 0x68, 0x41,
    0x67, 0x4d, 0x42, 0x41, 0x41, 0x45, 0x77, 0x44, 0x51, 0x59, 0x4a, 0x4b,
    0x6f, 0x5a, 0x49, 0x68, 0x76, 0x63, 0x4e, 0x41, 0x51, 0x45, 0x4c, 0x42,
    0x51, 0x41, 0x44, 0x67, 0x67, 0x45, 0x42, 0x41, 0x46, 0x58, 0x43, 0x65,
    0x77, 0x4b, 0x38, 0x0a, 0x63, 0x57, 0x54, 0x2b, 0x7a, 0x57, 0x78, 0x58,
    0x79, 0x47, 0x46, 0x6e, 0x6f, 0x75, 0x46, 0x53, 0x42, 0x7a, 0x54, 0x69,
    0x30, 0x42, 0x4d, 0x42, 0x4a, 0x52, 0x72, 0x68, 0x73, 0x69, 0x4e, 0x6f,
    0x69, 0x51, 0x78, 0x6b, 0x71, 0x69, 0x74, 0x79, 0x4a, 0x48, 0x57, 0x46,
    0x45, 0x78, 0x69, 0x51, 0x5a, 0x69, 0x65, 0x2b, 0x37, 0x43, 0x41, 0x2b,
    0x45, 0x61, 0x62, 0x58, 0x43, 0x51, 0x55, 0x42, 0x0a, 0x2b, 0x4a, 0x77,
    0x4d, 0x53, 0x57, 0x4d, 0x32, 0x39, 0x6a, 0x33, 0x6d, 0x53, 0x77, 0x31,
    0x30, 0x44, 0x54, 0x66, 0x6d, 0x43, 0x33, 0x72, 0x68, 0x68, 0x65, 0x51,
    0x71, 0x47, 0x78, 0x79, 0x33, 0x30, 0x34, 0x42, 0x5a, 0x79, 0x55, 0x70,
    0x64, 0x70, 0x76, 0x49, 0x32, 0x64, 0x74, 0x33, 0x70, 0x2f, 0x6d, 0x63,
    0x73, 0x45, 0x37, 0x4f, 0x2b, 0x70, 0x34, 0x73, 0x51, 0x72, 0x53, 0x65,
    0x70, 0x0a, 0x67, 0x69, 0x6a, 0x69, 0x44, 0x73, 0x73, 0x4b, 0x41, 0x66,
    0x78, 0x54, 0x41, 0x6d, 0x55, 0x4d, 0x39, 0x33, 0x4e, 0x36, 0x2b, 0x51,
    0x38, 0x79, 0x4a, 0x4b, 0x35, 0x69, 0x6d, 0x6d, 0x78, 0x6c, 0x62, 0x65,
    0x59, 0x66, 0x69, 0x6a, 0x6f, 0x42, 0x76, 0x6d, 0x6b, 0x7a, 0x79, 0x42,
    0x2f, 0x42, 0x2b, 0x71, 0x4e, 0x52, 0x50, 0x73, 0x78, 0x30, 0x6e, 0x37,
    0x61, 0x46, 0x47, 0x6e, 0x66, 0x76, 0x0a, 0x6f, 0x57, 0x66, 0x6b, 0x57,
    0x32, 0x39, 0x36, 0x69, 0x50, 0x68, 0x57, 0x4c, 0x69, 0x77, 0x6b, 0x6e,
    0x70, 0x43, 0x33, 0x78, 0x42, 0x36, 0x6f, 0x4b, 0x33, 0x76, 0x52, 0x62,
    0x4b, 0x34, 0x5a, 0x6a, 0x31, 0x4f, 0x61, 0x47, 0x62, 0x30, 0x67, 0x72,
    0x4b, 0x37, 0x56, 0x4e, 0x38, 0x45, 0x79, 0x68, 0x42, 0x69, 0x78, 0x32,
    0x78, 0x56, 0x46, 0x36, 0x31, 0x69, 0x34, 0x64, 0x7a, 0x43, 0x4b, 0x0a,
    0x6b, 0x4d, 0x49, 0x70, 0x6c, 0x37, 0x43, 0x55, 0x70, 0x77, 0x31, 0x4d,
    0x62, 0x32, 0x7a, 0x38, 0x71, 0x33, 0x46, 0x32, 0x62, 0x48, 0x42, 0x53,
    0x37, 0x69, 0x46, 0x37, 0x67, 0x31, 0x43, 0x63, 0x6e, 0x35, 0x56, 0x47,
    0x63, 0x4f, 0x2b, 0x61, 0x4a, 0x2b, 0x36, 0x50, 0x57, 0x79, 0x64, 0x61,
    0x65, 0x71, 0x4a, 0x36, 0x56, 0x45, 0x42, 0x46, 0x30, 0x4e, 0x77, 0x76,
    0x39, 0x77, 0x6f, 0x65, 0x0a, 0x6d, 0x4c, 0x35, 0x41, 0x6c, 0x75, 0x4e,
    0x52, 0x4c, 0x61, 0x71, 0x6a, 0x5a, 0x76, 0x45, 0x3d, 0x0a, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43, 0x45, 0x52, 0x54, 0x49,
    0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a};

extern const char test_signed_client_key[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x50,
    0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x49, 0x49, 0x45, 0x76, 0x67, 0x49, 0x42,
    0x41, 0x44, 0x41, 0x4e, 0x42, 0x67, 0x6b, 0x71, 0x68, 0x6b, 0x69, 0x47,
    0x39, 0x77, 0x30, 0x42, 0x41, 0x51, 0x45, 0x46, 0x41, 0x41, 0x53, 0x43,
    0x42, 0x4b, 0x67, 0x77, 0x67, 0x67, 0x53, 0x6b, 0x41, 0x67, 0x45, 0x41,
    0x41, 0x6f, 0x49, 0x42, 0x41, 0x51, 0x43, 0x79, 0x71, 0x59, 0x52, 0x70,
    0x2b, 0x44, 0x58, 0x56, 0x70, 0x37, 0x32, 0x4e, 0x0a, 0x46, 0x62, 0x51,
    0x48, 0x38, 0x68, 0x64, 0x68, 0x54, 0x5a, 0x4c, 0x79, 0x63, 0x5a, 0x58,
    0x4f, 0x6c, 0x4a, 0x68, 0x6d, 0x4d, 0x73, 0x72, 0x4a, 0x6d, 0x72, 0x6a,
    0x6e, 0x32, 0x70, 0x37, 0x70, 0x49, 0x2f, 0x38, 0x6d, 0x54, 0x5a, 0x2f,
    0x30, 0x46, 0x43, 0x2b, 0x53, 0x47, 0x57, 0x42, 0x47, 0x5a, 0x56, 0x2b,
    0x45, 0x4c, 0x69, 0x48, 0x72, 0x6d, 0x43, 0x58, 0x35, 0x7a, 0x66, 0x61,
    0x49, 0x0a, 0x4c, 0x72, 0x39, 0x49, 0x75, 0x77, 0x37, 0x47, 0x68, 0x72,
    0x33, 0x56, 0x7a, 0x6f, 0x65, 0x66, 0x69, 0x38, 0x72, 0x36, 0x32, 0x72,
    0x4c, 0x75, 0x70, 0x56, 0x50, 0x4e, 0x69, 0x2f, 0x71, 0x64, 0x71, 0x79,
    0x6a, 0x57, 0x6b, 0x32, 0x64, 0x45, 0x43, 0x48, 0x43, 0x39, 0x5a, 0x33,
    0x2b, 0x41, 0x67, 0x33, 0x4b, 0x7a, 0x4b, 0x54, 0x79, 0x65, 0x72, 0x58,
    0x57, 0x6a, 0x4b, 0x63, 0x76, 0x79, 0x0a, 0x4b, 0x56, 0x6d, 0x4d, 0x30,
    0x5a, 0x78, 0x45, 0x30, 0x52, 0x58, 0x68, 0x44, 0x57, 0x2f, 0x52, 0x6f,
    0x51, 0x62, 0x71, 0x5a, 0x73, 0x55, 0x32, 0x47, 0x4b, 0x67, 0x31, 0x42,
    0x32, 0x72, 0x68, 0x55, 0x55, 0x38, 0x4b, 0x4e, 0x30, 0x67, 0x56, 0x6d,
    0x4b, 0x6e, 0x30, 0x72, 0x4a, 0x48, 0x4f, 0x78, 0x7a, 0x52, 0x56, 0x53,
    0x59, 0x65, 0x59, 0x4c, 0x59, 0x70, 0x35, 0x59, 0x6e, 0x37, 0x4b, 0x0a,
    0x72, 0x74, 0x50, 0x4a, 0x63, 0x4b, 0x79, 0x6f, 0x39, 0x61, 0x56, 0x75,
    0x45, 0x72, 0x37, 0x64, 0x47, 0x41, 0x4e, 0x7a, 0x70, 0x79, 0x46, 0x36,
    0x6c, 0x67, 0x2f, 0x6e, 0x59, 0x42, 0x57, 0x63, 0x2b, 0x39, 0x53, 0x47,
    0x77, 0x6b, 0x6f, 0x4c, 0x64, 0x46, 0x76, 0x4b, 0x76, 0x41, 0x42, 0x59,
    0x4a, 0x4d, 0x79, 0x72, 0x62, 0x4e, 0x68, 0x48, 0x55, 0x51, 0x66, 0x76,
    0x30, 0x66, 0x7a, 0x61, 0x0a, 0x5a, 0x30, 0x50, 0x38, 0x36, 0x64, 0x66,
    0x54, 0x45, 0x4e, 0x72, 0x44, 0x78, 0x7a, 0x41, 0x4c, 0x72, 0x7a, 0x47,
    0x6e, 0x71, 0x63, 0x78, 0x33, 0x4b, 0x54, 0x72, 0x77, 0x4a, 0x6a, 0x6b,
    0x5a, 0x2f, 0x61, 0x53, 0x72, 0x31, 0x74, 0x79, 0x44, 0x30, 0x2f, 0x74,
    0x58, 0x76, 0x75, 0x6b, 0x52, 0x46, 0x69, 0x50, 0x78, 0x57, 0x42, 0x4a,
    0x68, 0x6a, 0x48, 0x51, 0x37, 0x30, 0x47, 0x71, 0x54, 0x0a, 0x46, 0x51,
    0x59, 0x31, 0x39, 0x52, 0x62, 0x68, 0x41, 0x67, 0x4d, 0x42, 0x41, 0x41,
    0x45, 0x43, 0x67, 0x67, 0x45, 0x41, 0x49, 0x4c, 0x38, 0x4a, 0x55, 0x68,
    0x4c, 0x34, 0x61, 0x77, 0x79, 0x76, 0x70, 0x57, 0x68, 0x51, 0x38, 0x78,
    0x50, 0x67, 0x54, 0x53, 0x6c, 0x57, 0x77, 0x62, 0x45, 0x6e, 0x38, 0x42,
    0x45, 0x30, 0x54, 0x61, 0x63, 0x4a, 0x6e, 0x43, 0x49, 0x4c, 0x75, 0x68,
    0x4e, 0x4d, 0x0a, 0x42, 0x52, 0x64, 0x66, 0x38, 0x4c, 0x6c, 0x52, 0x6b,
    0x2f, 0x38, 0x50, 0x4b, 0x51, 0x77, 0x56, 0x70, 0x56, 0x46, 0x33, 0x54,
    0x46, 0x62, 0x59, 0x53, 0x4d, 0x49, 0x2b, 0x55, 0x36, 0x62, 0x34, 0x68,
    0x4d, 0x56, 0x73, 0x73, 0x66, 0x76, 0x33, 0x48, 0x56, 0x51, 0x63, 0x2f,
    0x30, 0x38, 0x33, 0x64, 0x48, 0x71, 0x2b, 0x33, 0x58, 0x4f, 0x77, 0x55,
    0x43, 0x56, 0x6c, 0x55, 0x73, 0x74, 0x52, 0x0a, 0x53, 0x41, 0x7a, 0x54,
    0x45, 0x32, 0x45, 0x35, 0x45, 0x44, 0x4d, 0x72, 0x31, 0x73, 0x74, 0x64,
    0x68, 0x30, 0x53, 0x51, 0x68, 0x56, 0x34, 0x4e, 0x69, 0x6c, 0x66, 0x6f,
    0x73, 0x39, 0x73, 0x35, 0x55, 0x6b, 0x31, 0x5a, 0x36, 0x49, 0x47, 0x53,
    0x7a, 0x74, 0x6f, 0x7a, 0x31, 0x47, 0x67, 0x4f, 0x45, 0x72, 0x49, 0x63,
    0x2f, 0x6d, 0x47, 0x50, 0x79, 0x2f, 0x61, 0x41, 0x2f, 0x68, 0x62, 0x72,
    0x0a, 0x66, 0x52, 0x57, 0x48, 0x76, 0x54, 0x70, 0x33, 0x35, 0x2b, 0x4d,
    0x62, 0x43, 0x4a, 0x53, 0x76, 0x5a, 0x75, 0x4f, 0x65, 0x65, 0x76, 0x58,
    0x32, 0x69, 0x4c, 0x73, 0x30, 0x64, 0x4e, 0x7a, 0x71, 0x64, 0x6b, 0x36,
    0x44, 0x69, 0x4f, 0x57, 0x49, 0x48, 0x2f, 0x42, 0x56, 0x47, 0x69, 0x72,
    0x56, 0x50, 0x74, 0x4f, 0x36, 0x79, 0x6b, 0x72, 0x6b, 0x75, 0x54, 0x6a,
    0x31, 0x46, 0x57, 0x69, 0x4e, 0x0a, 0x68, 0x79, 0x5a, 0x33, 0x4d, 0x42,
    0x43, 0x68, 0x53, 0x68, 0x6c, 0x4e, 0x48, 0x32, 0x70, 0x6f, 0x4e, 0x58,
    0x34, 0x36, 0x6e, 0x74, 0x4f, 0x63, 0x37, 0x6e, 0x45, 0x75, 0x73, 0x30,
    0x71, 0x74, 0x65, 0x4f, 0x67, 0x78, 0x42, 0x4b, 0x38, 0x6c, 0x75, 0x6d,
    0x6d, 0x46, 0x45, 0x74, 0x6c, 0x65, 0x68, 0x43, 0x41, 0x37, 0x68, 0x64,
    0x2f, 0x38, 0x78, 0x75, 0x76, 0x59, 0x6c, 0x50, 0x30, 0x6b, 0x0a, 0x37,
    0x61, 0x4e, 0x36, 0x38, 0x34, 0x4c, 0x43, 0x52, 0x44, 0x61, 0x6a, 0x6d,
    0x41, 0x47, 0x70, 0x6f, 0x5a, 0x4f, 0x35, 0x37, 0x4e, 0x53, 0x44, 0x59,
    0x51, 0x68, 0x41, 0x46, 0x47, 0x5a, 0x65, 0x55, 0x5a, 0x39, 0x33, 0x53,
    0x4d, 0x46, 0x75, 0x63, 0x51, 0x4b, 0x42, 0x67, 0x51, 0x44, 0x65, 0x37,
    0x47, 0x47, 0x6b, 0x7a, 0x5a, 0x46, 0x45, 0x69, 0x76, 0x39, 0x31, 0x75,
    0x31, 0x71, 0x39, 0x0a, 0x6c, 0x67, 0x4d, 0x79, 0x31, 0x68, 0x35, 0x64,
    0x5a, 0x6a, 0x49, 0x5a, 0x4b, 0x67, 0x51, 0x61, 0x4f, 0x61, 0x72, 0x50,
    0x43, 0x36, 0x77, 0x43, 0x51, 0x4d, 0x55, 0x64, 0x71, 0x43, 0x66, 0x36,
    0x63, 0x53, 0x4c, 0x73, 0x41, 0x50, 0x72, 0x34, 0x54, 0x38, 0x45, 0x44,
    0x6f, 0x57, 0x73, 0x6e, 0x59, 0x37, 0x64, 0x53, 0x6e, 0x72, 0x54, 0x5a,
    0x36, 0x59, 0x43, 0x49, 0x46, 0x4c, 0x31, 0x54, 0x0a, 0x69, 0x64, 0x67,
    0x38, 0x4d, 0x33, 0x42, 0x51, 0x58, 0x69, 0x70, 0x49, 0x43, 0x43, 0x4a,
    0x6b, 0x46, 0x4f, 0x52, 0x53, 0x37, 0x36, 0x70, 0x4b, 0x4b, 0x5a, 0x30,
    0x77, 0x4d, 0x6e, 0x33, 0x2f, 0x4e, 0x67, 0x6b, 0x53, 0x65, 0x70, 0x73,
    0x6d, 0x4e, 0x63, 0x74, 0x39, 0x31, 0x57, 0x48, 0x72, 0x36, 0x6f, 0x6b,
    0x76, 0x78, 0x34, 0x74, 0x4f, 0x61, 0x6f, 0x52, 0x43, 0x74, 0x64, 0x7a,
    0x55, 0x0a, 0x67, 0x37, 0x6a, 0x74, 0x34, 0x4d, 0x72, 0x33, 0x73, 0x66,
    0x4c, 0x43, 0x69, 0x5a, 0x74, 0x71, 0x54, 0x51, 0x79, 0x79, 0x53, 0x64,
    0x4d, 0x55, 0x45, 0x77, 0x4b, 0x42, 0x67, 0x51, 0x44, 0x4e, 0x4b, 0x2b,
    0x5a, 0x46, 0x4b, 0x4c, 0x30, 0x58, 0x68, 0x6b, 0x57, 0x5a, 0x50, 0x2b,
    0x50, 0x47, 0x4b, 0x6a, 0x57, 0x47, 0x38, 0x4c, 0x57, 0x70, 0x50, 0x69,
    0x4b, 0x33, 0x64, 0x37, 0x38, 0x2f, 0x0a, 0x77, 0x59, 0x42, 0x46, 0x58,
    0x7a, 0x53, 0x54, 0x47, 0x6c, 0x6b, 0x72, 0x36, 0x46, 0x76, 0x52, 0x6d,
    0x59, 0x74, 0x5a, 0x65, 0x4e, 0x77, 0x58, 0x57, 0x52, 0x59, 0x4c, 0x42,
    0x34, 0x55, 0x78, 0x5a, 0x39, 0x41, 0x74, 0x34, 0x68, 0x62, 0x4a, 0x56,
    0x45, 0x64, 0x69, 0x2f, 0x32, 0x64, 0x49, 0x54, 0x4f, 0x7a, 0x2f, 0x73,
    0x65, 0x68, 0x56, 0x44, 0x79, 0x43, 0x41, 0x6a, 0x6a, 0x73, 0x33, 0x0a,
    0x67, 0x79, 0x63, 0x73, 0x63, 0x33, 0x55, 0x4a, 0x71, 0x69, 0x5a, 0x62,
    0x63, 0x77, 0x35, 0x58, 0x4b, 0x68, 0x49, 0x35, 0x54, 0x57, 0x42, 0x75,
    0x57, 0x78, 0x6b, 0x4b, 0x45, 0x4e, 0x64, 0x62, 0x4d, 0x53, 0x61, 0x79,
    0x6f, 0x67, 0x56, 0x62, 0x70, 0x32, 0x61, 0x53, 0x59, 0x6f, 0x52, 0x62,
    0x6c, 0x48, 0x37, 0x36, 0x34, 0x2f, 0x2f, 0x74, 0x30, 0x41, 0x43, 0x6d,
    0x62, 0x66, 0x54, 0x57, 0x0a, 0x4b, 0x55, 0x51, 0x52, 0x51, 0x50, 0x42,
    0x2f, 0x75, 0x77, 0x4b, 0x42, 0x67, 0x51, 0x43, 0x35, 0x51, 0x6a, 0x6a,
    0x6a, 0x66, 0x50, 0x4c, 0x38, 0x77, 0x34, 0x63, 0x4a, 0x6b, 0x47, 0x6f,
    0x59, 0x70, 0x46, 0x4b, 0x45, 0x4c, 0x4f, 0x32, 0x50, 0x4d, 0x52, 0x37,
    0x78, 0x53, 0x72, 0x6d, 0x65, 0x45, 0x63, 0x36, 0x68, 0x77, 0x6c, 0x46,
    0x77, 0x6a, 0x65, 0x4e, 0x43, 0x67, 0x6a, 0x79, 0x33, 0x0a, 0x4a, 0x4d,
    0x36, 0x67, 0x30, 0x79, 0x2b, 0x2b, 0x72, 0x49, 0x6a, 0x37, 0x4f, 0x32,
    0x71, 0x52, 0x6b, 0x59, 0x30, 0x49, 0x58, 0x46, 0x78, 0x76, 0x76, 0x46,
    0x33, 0x55, 0x75, 0x57, 0x65, 0x64, 0x78, 0x54, 0x43, 0x75, 0x31, 0x78,
    0x43, 0x2f, 0x75, 0x59, 0x48, 0x70, 0x32, 0x74, 0x69, 0x35, 0x30, 0x36,
    0x4c, 0x73, 0x53, 0x63, 0x42, 0x37, 0x59, 0x5a, 0x6f, 0x41, 0x4d, 0x2f,
    0x59, 0x42, 0x0a, 0x34, 0x69, 0x59, 0x6e, 0x39, 0x54, 0x78, 0x36, 0x78,
    0x4c, 0x6f, 0x59, 0x47, 0x50, 0x30, 0x48, 0x30, 0x69, 0x47, 0x77, 0x55,
    0x32, 0x53, 0x79, 0x42, 0x6c, 0x4e, 0x6b, 0x48, 0x54, 0x38, 0x6f, 0x58,
    0x55, 0x2b, 0x53, 0x59, 0x50, 0x35, 0x4d, 0x57, 0x74, 0x59, 0x6b, 0x56,
    0x62, 0x65, 0x53, 0x33, 0x2f, 0x56, 0x74, 0x4e, 0x57, 0x7a, 0x31, 0x67,
    0x51, 0x4b, 0x42, 0x67, 0x51, 0x43, 0x41, 0x0a, 0x36, 0x4e, 0x6b, 0x34,
    0x6b, 0x4e, 0x30, 0x6d, 0x48, 0x37, 0x59, 0x78, 0x45, 0x4b, 0x52, 0x7a,
    0x53, 0x4f, 0x66, 0x79, 0x7a, 0x65, 0x44, 0x46, 0x34, 0x6f, 0x56, 0x37,
    0x6b, 0x75, 0x42, 0x32, 0x46, 0x59, 0x55, 0x62, 0x6b, 0x54, 0x4c, 0x2b,
    0x54, 0x69, 0x72, 0x43, 0x33, 0x4b, 0x35, 0x38, 0x4a, 0x69, 0x59, 0x59,
    0x35, 0x45, 0x67, 0x63, 0x33, 0x31, 0x74, 0x72, 0x4f, 0x4b, 0x46, 0x6d,
    0x0a, 0x4a, 0x6c, 0x7a, 0x31, 0x78, 0x7a, 0x30, 0x62, 0x36, 0x44, 0x6b,
    0x6d, 0x4b, 0x57, 0x54, 0x69, 0x56, 0x33, 0x72, 0x39, 0x4f, 0x50, 0x48,
    0x4b, 0x4a, 0x38, 0x50, 0x37, 0x49, 0x65, 0x4a, 0x78, 0x41, 0x5a, 0x57,
    0x6d, 0x5a, 0x7a, 0x43, 0x64, 0x44, 0x75, 0x77, 0x6b, 0x76, 0x30, 0x69,
    0x2b, 0x57, 0x57, 0x2b, 0x7a, 0x30, 0x7a, 0x73, 0x49, 0x65, 0x33, 0x4a,
    0x6a, 0x45, 0x61, 0x76, 0x4e, 0x0a, 0x33, 0x7a, 0x62, 0x36, 0x4f, 0x37,
    0x52, 0x30, 0x48, 0x74, 0x7a, 0x69, 0x6b, 0x73, 0x57, 0x6f, 0x71, 0x4d,
    0x65, 0x54, 0x71, 0x5a, 0x65, 0x4f, 0x2b, 0x77, 0x61, 0x39, 0x69, 0x77,
    0x36, 0x76, 0x56, 0x4b, 0x51, 0x77, 0x31, 0x77, 0x57, 0x45, 0x71, 0x77,
    0x4b, 0x42, 0x67, 0x46, 0x48, 0x66, 0x61, 0x68, 0x46, 0x73, 0x30, 0x44,
    0x5a, 0x35, 0x63, 0x55, 0x54, 0x70, 0x47, 0x70, 0x42, 0x74, 0x0a, 0x46,
    0x2f, 0x41, 0x51, 0x47, 0x37, 0x75, 0x6b, 0x67, 0x69, 0x70, 0x42, 0x36,
    0x4e, 0x36, 0x41, 0x6b, 0x42, 0x39, 0x6b, 0x44, 0x62, 0x67, 0x43, 0x73,
    0x31, 0x46, 0x4c, 0x67, 0x64, 0x31, 0x39, 0x39, 0x4d, 0x51, 0x72, 0x45,
    0x6e, 0x63, 0x75, 0x67, 0x35, 0x68, 0x66, 0x70, 0x71, 0x38, 0x51, 0x65,
    0x72, 0x62, 0x79, 0x4d, 0x61, 0x74, 0x6d, 0x41, 0x2b, 0x47, 0x58, 0x6f,
    0x47, 0x4d, 0x62, 0x0a, 0x37, 0x76, 0x7a, 0x74, 0x4b, 0x45, 0x48, 0x38,
    0x35, 0x79, 0x7a, 0x70, 0x34, 0x6e, 0x30, 0x32, 0x46, 0x4e, 0x4c, 0x36,
    0x48, 0x37, 0x78, 0x4c, 0x34, 0x56, 0x56, 0x49, 0x4c, 0x76, 0x79, 0x5a,
    0x48, 0x64, 0x6f, 0x6c, 0x6d, 0x69, 0x4f, 0x52, 0x4a, 0x34, 0x71, 0x54,
    0x32, 0x68, 0x5a, 0x6e, 0x6c, 0x38, 0x70, 0x45, 0x51, 0x32, 0x54, 0x59,
    0x75, 0x46, 0x34, 0x52, 0x6c, 0x48, 0x55, 0x64, 0x0a, 0x6e, 0x53, 0x77,
    0x58, 0x58, 0x2b, 0x32, 0x6f, 0x30, 0x4a, 0x2f, 0x6e, 0x46, 0x38, 0x35,
    0x66, 0x6d, 0x34, 0x41, 0x77, 0x57, 0x4b, 0x41, 0x63, 0x0a, 0x2d, 0x2d,
    0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x50, 0x52, 0x49, 0x56, 0x41,
    0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a};
