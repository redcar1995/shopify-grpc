#!/bin/bash
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

. "$HOME/.nvm/nvm.sh"

nvm use 10

set -ex

fixture=$1

shift

# Enter repo root
cd "$(dirname "$0")/../../.."

# Enter the grpc-node repo root (expected to be next to grpc repo root)
cd ../grpc-node

node -r "./test/fixtures/$fixture" test/performance/worker.js "$@"
