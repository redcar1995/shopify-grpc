#!/usr/bin/env bash
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
#
# Test full Bazel
#
# NOTE: No empty lines should appear in this file before igncr is set!
set -ex -o igncr || set -ex

mkdir -p /var/local/git
git clone /var/local/jenkins/grpc /var/local/git/grpc
(cd /var/local/jenkins/grpc/ && git submodule foreach 'cd /var/local/git/grpc \
&& git submodule update --init --reference /var/local/jenkins/grpc/${name} \
${name}')
cd /var/local/git/grpc/test
bazel test --spawn_strategy=standalone --genrule_strategy=standalone --test_output=errors //src/python/...
bazel test --spawn_strategy=standalone --genrule_strategy=standalone --test_output=errors //examples/python/...
bazel clean --expunge
bazel test --config=python3 --spawn_strategy=standalone --genrule_strategy=standalone --test_output=errors //src/python/...
bazel test --config=python3 --spawn_strategy=standalone --genrule_strategy=standalone --test_output=errors //examples/python/...
(cd bazel/test/python_test_repo;
  bazel test --test_output=errors //...
)
