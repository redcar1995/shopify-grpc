#!/bin/sh
# Copyright 2015 gRPC authors.
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

export TEST=${TEST:-false}

# Upgrade Python's YAML library
python3 -m pip install --upgrade --ignore-installed PyYAML

echo "Generating build_autogenerated.yaml from bazel BUILD file"
rm -f build_autogenerated.yaml
python3 tools/buildgen/extract_metadata_from_bazel_xml.py

cd `dirname $0`/../..
mako_renderer=tools/buildgen/mako_renderer.py

tools/buildgen/build_cleaner.py build_handwritten.yaml

# check build_autogenerated.yaml is already in its "clean" form
TEST=true tools/buildgen/build_cleaner.py build_autogenerated.yaml

. tools/buildgen/generate_build_additions.sh

# Instead of generating from a single build.yaml, we've split it into
# - build_handwritten.yaml: manually written metadata
# - build_autogenerated.yaml: generated from bazel BUILD file
python3 tools/buildgen/generate_projects.py build_handwritten.yaml build_autogenerated.yaml $gen_build_files $*

rm $gen_build_files
