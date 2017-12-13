#!/usr/bin/env python2.7

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

import collections
import perfection
import sys

_MAX_HEADER_LIST_SIZE = 16 * 1024 * 1024

Setting = collections.namedtuple('Setting', 'id default min max on_error')
OnError = collections.namedtuple('OnError', 'behavior code')
clamp_invalid_value = OnError('CLAMP_INVALID_VALUE', 'PROTOCOL_ERROR')
disconnect_on_invalid_value = lambda e: OnError('DISCONNECT_ON_INVALID_VALUE', e)
DecoratedSetting = collections.namedtuple('DecoratedSetting',
                                          'enum name setting')

_SETTINGS = {
    'HEADER_TABLE_SIZE':
    Setting(1, 4096, 0, 0xffffffff, clamp_invalid_value),
    'ENABLE_PUSH':
    Setting(2, 1, 0, 1, disconnect_on_invalid_value('PROTOCOL_ERROR')),
    'MAX_CONCURRENT_STREAMS':
    Setting(3, 0xffffffff, 0, 0xffffffff,
            disconnect_on_invalid_value('PROTOCOL_ERROR')),
    'INITIAL_WINDOW_SIZE':
    Setting(4, 65535, 0, 0x7fffffff,
            disconnect_on_invalid_value('FLOW_CONTROL_ERROR')),
    'MAX_FRAME_SIZE':
    Setting(5, 16384, 16384, 16777215,
            disconnect_on_invalid_value('PROTOCOL_ERROR')),
    'MAX_HEADER_LIST_SIZE':
    Setting(6, _MAX_HEADER_LIST_SIZE, 0, _MAX_HEADER_LIST_SIZE,
            clamp_invalid_value),
    'GRPC_ALLOW_TRUE_BINARY_METADATA':
    Setting(0xfe03, 0, 0, 1, clamp_invalid_value),
}

H = open('src/core/ext/transport/chttp2/transport/http2_settings.h', 'w')
C = open('src/core/ext/transport/chttp2/transport/http2_settings.c', 'w')


# utility: print a big comment block into a set of files
def put_banner(files, banner):
    for f in files:
        print >> f, '/*'
        for line in banner:
            print >> f, ' * %s' % line
        print >> f, ' */'
        print >> f


# copy-paste copyright notice from this file
with open(sys.argv[0]) as my_source:
    copyright = []
    for line in my_source:
        if line[0] != '#': break
    for line in my_source:
        if line[0] == '#':
            copyright.append(line)
            break
    for line in my_source:
        if line[0] != '#':
            break
        copyright.append(line)
    put_banner([H, C], [line[2:].rstrip() for line in copyright])

put_banner(
    [H, C],
    ["Automatically generated by tools/codegen/core/gen_settings_ids.py"])

print >> H, "#ifndef GRPC_CORE_EXT_TRANSPORT_CHTTP2_TRANSPORT_HTTP2_SETTINGS_H"
print >> H, "#define GRPC_CORE_EXT_TRANSPORT_CHTTP2_TRANSPORT_HTTP2_SETTINGS_H"
print >> H
print >> H, "#include <stdint.h>"
print >> H, "#include <stdbool.h>"
print >> H

print >> C, "#include \"src/core/ext/transport/chttp2/transport/http2_settings.h\""
print >> C
print >> C, "#include <grpc/support/useful.h>"
print >> C, "#include \"src/core/lib/transport/http2_errors.h\""
print >> C

p = perfection.hash_parameters(sorted(x.id for x in _SETTINGS.values()))
print p


def hash(i):
    i += p.offset
    x = i % p.t
    y = i / p.t
    return x + p.r[y]


decorated_settings = [
    DecoratedSetting(hash(setting.id), name, setting)
    for name, setting in _SETTINGS.iteritems()
]

print >> H, 'typedef enum {'
for decorated_setting in sorted(decorated_settings):
    print >> H, '  GRPC_CHTTP2_SETTINGS_%s = %d, /* wire id %d */' % (
        decorated_setting.name, decorated_setting.enum,
        decorated_setting.setting.id)
print >> H, '} grpc_chttp2_setting_id;'
print >> H
print >> H, '#define GRPC_CHTTP2_NUM_SETTINGS %d' % (
    max(x.enum for x in decorated_settings) + 1)

print >> H, 'extern const uint16_t grpc_setting_id_to_wire_id[];'
print >> C, 'const uint16_t grpc_setting_id_to_wire_id[] = {%s};' % ','.join(
    '%d' % s for s in p.slots)
print >> H
print >> H, "bool grpc_wire_id_to_setting_id(uint32_t wire_id, grpc_chttp2_setting_id *out);"
cgargs = {
    'r': ','.join('%d' % (r if r is not None else 0) for r in p.r),
    't': p.t,
    'offset': abs(p.offset),
    'offset_sign': '+' if p.offset > 0 else '-'
}
print >> C, """
bool grpc_wire_id_to_setting_id(uint32_t wire_id, grpc_chttp2_setting_id *out) {
  uint32_t i = wire_id %(offset_sign)s %(offset)d;
  uint32_t x = i %% %(t)d;
  uint32_t y = i / %(t)d;
  uint32_t h = x;
  switch (y) {
""" % cgargs
for i, r in enumerate(p.r):
    if not r: continue
    if r < 0: print >> C, 'case %d: h -= %d; break;' % (i, -r)
    else: print >> C, 'case %d: h += %d; break;' % (i, r)
print >> C, """
  }
  *out = (grpc_chttp2_setting_id)h;
  return h < GPR_ARRAY_SIZE(grpc_setting_id_to_wire_id) && grpc_setting_id_to_wire_id[h] == wire_id;
}
""" % cgargs

print >> H, """
typedef enum {
  GRPC_CHTTP2_CLAMP_INVALID_VALUE,
  GRPC_CHTTP2_DISCONNECT_ON_INVALID_VALUE
} grpc_chttp2_invalid_value_behavior;

typedef struct {
  const char *name;
  uint32_t default_value;
  uint32_t min_value;
  uint32_t max_value;
  grpc_chttp2_invalid_value_behavior invalid_value_behavior;
  uint32_t error_value;
} grpc_chttp2_setting_parameters;

extern const grpc_chttp2_setting_parameters grpc_chttp2_settings_parameters[GRPC_CHTTP2_NUM_SETTINGS];
"""
print >> C, "const grpc_chttp2_setting_parameters grpc_chttp2_settings_parameters[GRPC_CHTTP2_NUM_SETTINGS] = {"
i = 0
for decorated_setting in sorted(decorated_settings):
    while i < decorated_setting.enum:
        print >> C, "{NULL, 0, 0, 0, GRPC_CHTTP2_DISCONNECT_ON_INVALID_VALUE, GRPC_HTTP2_PROTOCOL_ERROR},"
        i += 1
    print >> C, "{\"%s\", %du, %du, %du, GRPC_CHTTP2_%s, GRPC_HTTP2_%s}," % (
        decorated_setting.name, decorated_setting.setting.default,
        decorated_setting.setting.min, decorated_setting.setting.max,
        decorated_setting.setting.on_error.behavior,
        decorated_setting.setting.on_error.code,)
    i += 1
print >> C, "};"

print >> H
print >> H, "#endif /* GRPC_CORE_EXT_TRANSPORT_CHTTP2_TRANSPORT_HTTP2_SETTINGS_H */"

H.close()
C.close()
