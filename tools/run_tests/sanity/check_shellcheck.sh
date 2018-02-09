#!/usr/bin/env bash
# Copyright 2017 gRPC authors.
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


set -e

ROOT="$(dirname "$0")/../../.."

DIRS=(
    'test'
    'tools/run_tests'
)

for dir in "${DIRS[@]}"; do
  find "$ROOT/$dir/" -name "*.sh" -type f -print0 | xargs -n1 -0 shellcheck
done
