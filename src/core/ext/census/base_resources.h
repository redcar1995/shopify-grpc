/*
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

#ifndef GRPC_CORE_EXT_CENSUS_BASE_RESOURCES_H
#define GRPC_CORE_EXT_CENSUS_BASE_RESOURCES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Define all base resources. This should be called by census initialization. */
void define_base_resources();

#ifdef __cplusplus
}
#endif

#endif /* GRPC_CORE_EXT_CENSUS_BASE_RESOURCES_H */