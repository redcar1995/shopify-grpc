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

set -e

# directories to run against
DIRS="src/core/lib src/core/tsi src/core/ext src/cpp test/core test/cpp include src/compiler src/node src/csharp src/ruby"

# file matching patterns to check
GLOB="*.h *.c *.cc"

# clang format command
CLANG_FORMAT=${CLANG_FORMAT:-clang-format-5.0}

files=
for dir in $DIRS
do
  for glob in $GLOB
  do
    files="$files `find ${CLANG_FORMAT_ROOT}/$dir -name $glob -and -not -name *.generated.* -and -not -name *.pb.h -and -not -name *.pb.c -and -not -name *.pb.cc -and -not -name end2end_tests.c -and -not -name end2end_nosec_tests.c -and -not -name public_headers_must_be_c89.c`"
  done
done

# The CHANGED_FILES variable is used to restrict the set of files to check.
# Here we set files to the intersection of files and CHANGED_FILES
if [ -n "$CHANGED_FILES" ]; then
  files=$(comm -12 <(echo $files | tr ' ' '\n' | sort -u) <(echo $CHANGED_FILES | tr ' ' '\n' | sort -u))
fi

if [ "$TEST" == "" ]
then
  echo $files | xargs $CLANG_FORMAT -i
else
  ok=yes
  for file in $files
  do
    tmp=`mktemp`
    $CLANG_FORMAT $file > $tmp
    diff -u $file $tmp || ok=no
    rm $tmp
  done
  if [ $ok == no ]
  then
    false
  fi
fi
