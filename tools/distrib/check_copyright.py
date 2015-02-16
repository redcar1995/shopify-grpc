#!/usr/bin/python2.7

# Copyright 2015, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


import os
import sys
import subprocess

# find our home
ROOT = os.path.abspath(
    os.path.join(os.path.dirname(sys.argv[0]), '../..'))
os.chdir(ROOT)

# open the license text
with open('LICENSE') as f:
  LICENSE = f.read().splitlines()

# license format by file extension
# key is the file extension, value is a format string
# that given a line of license text, returns what should
# be in the file
LICENSE_FMT = {
  '.c': ' * %s',
  '.cc': ' * %s',
  '.h': ' * %s',
  '.py': '# %s',
  '.rb': '# %s',
}

# pregenerate the actual text that we should have
LICENSE_TEXT = dict(
    (k, '\n'.join((v % line).rstrip() for line in LICENSE))
    for k, v in LICENSE_FMT.iteritems())

OLD_LICENSE_TEXT = dict(
    (k, v.replace('2015', '2014')) for k, v in LICENSE_TEXT.iteritems())

# scan files, validate the text
for filename in subprocess.check_output('git ls-tree -r --name-only -r HEAD',
                                        shell=True).splitlines():
  ext = os.path.splitext(filename)[1]
  if ext not in LICENSE_TEXT: continue
  license = LICENSE_TEXT[ext]
  old_license = OLD_LICENSE_TEXT[ext]
  with open(filename) as f:
    text = '\n'.join(line.rstrip() for line in f.read().splitlines())
  if license in text:
    pass
  elif old_license in text:
    pass
    #print 'old license in: %s' % filename
  else:
    print 'no license in: %s' % filename

