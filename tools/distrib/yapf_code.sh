#!/bin/bash
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

set -ex

# change to root directory
cd "$(dirname "${0}")/../.."

DIRS=(
    'examples/python'
    'src/python'
    'tools'
)

VIRTUALENV=yapf_virtual_environment

python3 -m virtualenv $VIRTUALENV -p $(which python3)
PYTHON=${VIRTUALENV}/bin/python
"$PYTHON" -m pip install --upgrade pip==19.3.1
"$PYTHON" -m pip install --upgrade futures
"$PYTHON" -m pip install yapf==0.28.0

yapf() {
    $PYTHON -m yapf -i -r --style=setup.cfg "${1}"
}

if [[ -z "${TEST}" ]]; then
    for dir in "${DIRS[@]}"; do
	yapf "${dir}"
    done
else
    ok=yes
    for dir in "${DIRS[@]}"; do
	tempdir=$(mktemp -d)
	cp -RT "${dir}" "${tempdir}"
	yapf "${tempdir}"
	diff -x '*.pyc' -ru "${dir}" "${tempdir}" || ok=no
	rm -rf "${tempdir}"
    done
    if [[ ${ok} == no ]]; then
	false
    fi
fi
