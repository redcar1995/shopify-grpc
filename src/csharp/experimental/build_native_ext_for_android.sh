#!/bin/sh
# Copyright 2018 gRPC authors.
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

# Helper script to crosscompile grpc_csharp_ext native extension for Android.

set -ex

cd "$(dirname "$0")/../../../cmake"

mkdir -p build
cd build

# set to the location where Android SDK is installed
ANDROID_SDK_PATH="$HOME/Android/Sdk"

# set to location where Android NDK is installed, usually a subfolder of Android SDK
# install the Android NDK through the Android SDK Manager
ANDROID_NDK_PATH=${ANDROID_SDK_PATH}/ndk-bundle

# set to location of the cmake executable
# by default, use cmake binary from the Android SDK
CMAKE_PATH="${ANDROID_SDK_PATH}/cmake/3.6.4111459/bin/cmake"

# ANDROID_ABI in ('arm64-v8a', 'armeabi-v7a')
${CMAKE_PATH} ../.. \
  -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK_PATH}/build/cmake/android.toolchain.cmake" \
  -DCMAKE_ANDROID_NDK="${ANDROID_NDK_PATH}" \
  -DCMAKE_ANDROID_STL_TYPE=c++_static \
  -DRUN_HAVE_POSIX_REGEX=0 \
  -DRUN_HAVE_STD_REGEX=0 \
  -DRUN_HAVE_STEADY_CLOCK=0 \
  -DCMAKE_BUILD_TYPE=Release \
  -DANDROID_PLATFORM=android-28 \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_NDK="${ANDROID_NDK_PATH}"

make -j4 grpc_csharp_ext

