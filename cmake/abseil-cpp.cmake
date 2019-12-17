# Copyright 2019 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(gRPC_ABSL_PROVIDER STREQUAL "module")
  if(NOT ABSL_ROOT_DIR)
    set(ABSL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/abseil-cpp)
  endif()
  if(EXISTS "${ABSL_ROOT_DIR}/CMakeLists.txt")
    add_subdirectory(${ABSL_ROOT_DIR} third_party/abseil-cpp)
  else()
    message(WARNING "gRPC_ABSL_PROVIDER is \"module\" but ABSL_ROOT_DIR is wrong")
  endif()
elseif(gRPC_ABSL_PROVIDER STREQUAL "package")
  # Use "CONFIG" as there is no built-in cmake module for absl.
  find_package(absl REQUIRED CONFIG)
  set(_gRPC_FIND_ABSL "if(NOT absl_FOUND)\n  find_package(absl CONFIG)\nendif()")
endif()
