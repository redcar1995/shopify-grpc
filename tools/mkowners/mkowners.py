#!/usr/bin/env python3
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

import argparse
import collections
import operator
import os
import re
import subprocess

#
# Find the root of the git tree
#

git_root = (subprocess
            .check_output(['git', 'rev-parse', '--show-toplevel'])
            .decode('utf-8')
            .strip())

#
# Parse command line arguments
#

default_out = os.path.join(git_root, '.github', 'CODEOWNERS')

argp = argparse.ArgumentParser('Generate .github/CODEOWNERS file')
argp.add_argument('--out', '-o',
                  type=str,
                  default=default_out,
                  help='Output file (default %s)' % default_out)
args = argp.parse_args()

#
# Walk git tree to locate all OWNERS files
#

owners_files = [os.path.join(root, 'OWNERS')
                for root, dirs, files in os.walk(git_root)
                if 'OWNERS' in files]

#
# Parse owners files
#

Owners = collections.namedtuple('Owners', 'parent directives dir')
Directive = collections.namedtuple('Directive', 'who globs')

def parse_owners(filename):
  with open(filename) as f:
    src = f.read().splitlines()
  parent = True
  directives = []
  for line in src:
    line = line.strip()
    # line := directive | comment
    if not line: continue
    if line[0] == '#': continue
    # it's a directive
    directive = None
    if line == 'set noparent':
      parent = False
    elif line == '*':
      directive = Directive(who='*', globs=[])
    elif ' ' in line:
      (who, globs) = line.split(' ', 1)
      globs_list = [glob
                    for glob in globs.split(' ')
                    if glob]
      directive = Directive(who=who, globs=globs_list)
    else:
      directive = Directive(who=line, globs=[])
    if directive:
      directives.append(directive)
  return Owners(parent=parent,
                directives=directives,
                dir=os.path.relpath(os.path.dirname(filename), git_root))

owners_data = sorted([parse_owners(filename)
                      for filename in owners_files],
                     key=operator.attrgetter('dir'))

#
# Modify owners so that parented OWNERS files point to the actual
# Owners tuple with their parent field
#

new_owners_data = []
for owners in owners_data:
  if owners.parent == True:
    best_parent = None
    best_parent_score = None
    for possible_parent in owners_data:
      if possible_parent is owners: continue
      rel = os.path.relpath(owners.dir, possible_parent.dir)
      # '..' ==> we had to walk up from possible_parent to get to owners
      #      ==> not a parent
      if '..' in rel: continue
      depth = len(rel.split(os.sep))
      if not best_parent or depth < best_parent_score:
        best_parent = possible_parent
        best_parent_score = depth
    if best_parent:
      owners = owners._replace(parent = best_parent.dir)
    else:
      owners = owners._replace(parent = None)
  new_owners_data.append(owners)
owners_data = new_owners_data

#
# In bottom to top order, process owners data structures to build up
# a CODEOWNERS file for GitHub
#

def full_dir(rules_dir, sub_path):
  return os.path.join(rules_dir, sub_path) if rules_dir != '.' else sub_path

def glob_intersect(g1, g2):
  if not g2:
    return all(c == '*' for c in g1)
  if not g1:
    return all(c == '*' for c in g2)
  c1, *t1 = g1
  c2, *t2 = g2
  if c1 == '*':
    return glob_intersect(g1, t2) or glob_intersect(t1, g2)
  if c2 == '*':
    return glob_intersect(t1, g2) or glob_intersect(g1, t2)
  return c1 == c2 and glob_intersect(t1, t2)

def add_parent_to_globs(parent, globs, globs_dir):
  if not parent: return
  for owners in owners_data:
    if owners.dir == parent:
      for directive in owners.directives:
        for dglob in directive.globs or ['**']:
          for gglob, glob in globs.items():
            if glob_intersect(full_dir(globs_dir, gglob),
                              full_dir(owners.dir, dglob)):
              if directive.who not in glob:
                glob.append(directive.who)
      add_parent_to_globs(owners.parent, globs, globs_dir)
      return
  assert(False)

todo = owners_data.copy()
done = set()
with open(args.out, 'w') as out:
  out.write('# Auto-generated by the tools/mkowners/mkowners.py tool\n')
  out.write('# Uses OWNERS files in different modules throughout the\n')
  out.write('# repository as the source of truth for module ownership.\n')
  while todo:
    head, *todo = todo
    if head.parent and not head.parent in done:
      todo.append(head)
      continue
    globs = collections.OrderedDict()
    for directive in head.directives:
      for glob in directive.globs or ['**']:
        if glob not in globs:
          globs[glob] = []
        globs[glob].append(directive.who)
    add_parent_to_globs(head.parent, globs, head.dir)
    for glob, owners in globs.items():
      out.write('/%s %s\n' % (
          full_dir(head.dir, glob), ' '.join(owners)))
    done.add(head.dir)
