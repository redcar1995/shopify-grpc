#!/usr/bin/env python
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

"""Run tests in parallel."""

import argparse
import glob
import itertools
import json
import multiprocessing
import os
import re
import sys
import time
import platform

import jobset
import watch_dirs


ROOT = os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), '../..'))
os.chdir(ROOT)


# SimpleConfig: just compile with CONFIG=config, and run the binary to test
class SimpleConfig(object):

  def __init__(self, config, environ=None):
    if environ is None:
      environ = {}
    self.build_config = config
    self.maxjobs = 2 * multiprocessing.cpu_count()
    self.allow_hashing = (config != 'gcov')
    self.environ = environ
    self.environ['CONFIG'] = config

  def job_spec(self, cmdline, hash_targets, shortname=None):
    """Construct a jobset.JobSpec for a test under this config

       Args:
         cmdline:      a list of strings specifying the command line the test
                       would like to run
         hash_targets: either None (don't do caching of test results), or
                       a list of strings specifying files to include in a
                       binary hash to check if a test has changed
                       -- if used, all artifacts needed to run the test must
                          be listed
    """
    return jobset.JobSpec(cmdline=cmdline,
                          shortname=shortname,
                          environ=self.environ,
                          hash_targets=hash_targets
                              if self.allow_hashing else None)


# ValgrindConfig: compile with some CONFIG=config, but use valgrind to run
class ValgrindConfig(object):

  def __init__(self, config, tool, args=None):
    if args is None:
      args = []
    self.build_config = config
    self.tool = tool
    self.args = args
    self.maxjobs = 2 * multiprocessing.cpu_count()
    self.allow_hashing = False

  def job_spec(self, cmdline, hash_targets):
    return jobset.JobSpec(cmdline=['valgrind', '--tool=%s' % self.tool] +
                          self.args + cmdline,
                          shortname='valgrind %s' % binary,
                          hash_targets=None)


class CLanguage(object):

  def __init__(self, make_target, test_lang):
    self.make_target = make_target
    if platform.system() == 'Windows':
      plat = 'windows'
    else:
      plat = 'posix'
    self.platform = plat
    with open('tools/run_tests/tests.json') as f:
      js = json.load(f)
      self.binaries = [tgt
                       for tgt in js
                       if tgt['language'] == test_lang and
                          plat in tgt['platforms']]

  def test_specs(self, config, travis):
    out = []
    for target in self.binaries:
      if travis and target['flaky']:
        continue
      if self.platform == 'windows':
        binary = 'vsprojects\\test_bin\\%s.exe' % (target['name'])
      else:
        binary = 'bins/%s/%s' % (config.build_config, target['name'])
      out.append(config.job_spec([binary], [binary]))
    return sorted(out)

  def make_targets(self):
    return ['buildtests_%s' % self.make_target]

  def build_steps(self):
    return []

  def supports_multi_config(self):
    return True

  def __str__(self):
    return self.make_target


class NodeLanguage(object):

  def test_specs(self, config, travis):
    return [config.job_spec(['tools/run_tests/run_node.sh'], None)]

  def make_targets(self):
    return ['static_c']

  def build_steps(self):
    return [['tools/run_tests/build_node.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'node'


class PhpLanguage(object):

  def test_specs(self, config, travis):
    return [config.job_spec(['src/php/bin/run_tests.sh'], None)]

  def make_targets(self):
    return ['static_c']

  def build_steps(self):
    return [['tools/run_tests/build_php.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'php'


class PythonLanguage(object):

  def __init__(self):
    with open('tools/run_tests/python_tests.json') as f:
      self._tests = json.load(f)

  def test_specs(self, config, travis):
    modules = [config.job_spec(['tools/run_tests/run_python.sh', '-m',
                                test['module']], 
                               None, 
                               shortname=test['module'])
               for test in self._tests if 'module' in test]
    files = [config.job_spec(['tools/run_tests/run_python.sh',
                              test['file']], 
                             None, 
                             shortname=test['file'])
             for test in self._tests if 'file' in test]
    return files + modules

  def make_targets(self):
    return ['static_c', 'grpc_python_plugin']

  def build_steps(self):
    return [['tools/run_tests/build_python.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'python'


class RubyLanguage(object):

  def test_specs(self, config, travis):
    return [config.job_spec(['tools/run_tests/run_ruby.sh'], None)]

  def make_targets(self):
    return ['run_dep_checks']

  def build_steps(self):
    return [['tools/run_tests/build_ruby.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'ruby'


class CSharpLanguage(object):
  def test_specs(self, config, travis):
    assemblies = ['Grpc.Core.Tests',
                  'Grpc.Examples.Tests',
                  'Grpc.IntegrationTesting']
    return [config.job_spec(['tools/run_tests/run_csharp.sh', assembly],
            None, shortname=assembly)
            for assembly in assemblies ]

  def make_targets(self):
    return ['grpc_csharp_ext']

  def build_steps(self):
    return [['tools/run_tests/build_csharp.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'csharp'


class Sanity(object):

  def test_specs(self, config, travis):
    return [config.job_spec('tools/run_tests/run_sanity.sh', None)]

  def make_targets(self):
    return ['run_dep_checks']

  def build_steps(self):
    return []

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'sanity'


class Build(object):

  def test_specs(self, config, travis):
    return []

  def make_targets(self):
    return ['static']

  def build_steps(self):
    return []

  def supports_multi_config(self):
    return True

  def __str__(self):
    return self.make_target


# different configurations we can run under
_CONFIGS = {
    'dbg': SimpleConfig('dbg'),
    'opt': SimpleConfig('opt'),
    'tsan': SimpleConfig('tsan', environ={
        'TSAN_OPTIONS': 'suppressions=tools/tsan_suppressions.txt'}),
    'msan': SimpleConfig('msan'),
    'ubsan': SimpleConfig('ubsan'),
    'asan': SimpleConfig('asan', environ={
        'ASAN_OPTIONS': 'detect_leaks=1:color=always:suppressions=tools/tsan_suppressions.txt'}),
    'asan-noleaks': SimpleConfig('asan', environ={
        'ASAN_OPTIONS': 'detect_leaks=0:color=always:suppressions=tools/tsan_suppressions.txt'}),
    'gcov': SimpleConfig('gcov'),
    'memcheck': ValgrindConfig('valgrind', 'memcheck', ['--leak-check=full']),
    'helgrind': ValgrindConfig('dbg', 'helgrind')
    }


_DEFAULT = ['opt']
_LANGUAGES = {
    'c++': CLanguage('cxx', 'c++'),
    'c': CLanguage('c', 'c'),
    'node': NodeLanguage(),
    'php': PhpLanguage(),
    'python': PythonLanguage(),
    'ruby': RubyLanguage(),
    'csharp': CSharpLanguage(),
    'sanity': Sanity(),
    'build': Build(),
    }

# parse command line
argp = argparse.ArgumentParser(description='Run grpc tests.')
argp.add_argument('-c', '--config',
                  choices=['all'] + sorted(_CONFIGS.keys()),
                  nargs='+',
                  default=_DEFAULT)
argp.add_argument('-n', '--runs_per_test', default=1, type=int)
argp.add_argument('-r', '--regex', default='.*', type=str)
argp.add_argument('-j', '--jobs', default=1000, type=int)
argp.add_argument('-s', '--slowdown', default=1.0, type=float)
argp.add_argument('-f', '--forever',
                  default=False,
                  action='store_const',
                  const=True)
argp.add_argument('-t', '--travis',
                  default=False,
                  action='store_const',
                  const=True)
argp.add_argument('--newline_on_success',
                  default=False,
                  action='store_const',
                  const=True)
argp.add_argument('-l', '--language',
                  choices=['all'] + sorted(_LANGUAGES.keys()),
                  nargs='+',
                  default=['all'])
args = argp.parse_args()

# grab config
run_configs = set(_CONFIGS[cfg]
                  for cfg in itertools.chain.from_iterable(
                      _CONFIGS.iterkeys() if x == 'all' else [x]
                      for x in args.config))
build_configs = set(cfg.build_config for cfg in run_configs)

make_targets = []
languages = set(_LANGUAGES[l]
                for l in itertools.chain.from_iterable(
                      _LANGUAGES.iterkeys() if x == 'all' else [x]
                      for x in args.language))

if len(build_configs) > 1:
  for language in languages:
    if not language.supports_multi_config():
      print language, 'does not support multiple build configurations'
      sys.exit(1)

if platform.system() == 'Windows':
  def make_jobspec(cfg, targets):
    return jobset.JobSpec(['make.bat', 'CONFIG=%s' % cfg] + targets,
                          cwd='vsprojects', shell=True)
else:
  def make_jobspec(cfg, targets):
    return jobset.JobSpec(['make',
                           '-j', '%d' % (multiprocessing.cpu_count() + 1),
                           'EXTRA_DEFINES=GRPC_TEST_SLOWDOWN_MACHINE_FACTOR=%f' % 
                               args.slowdown,
                           'CONFIG=%s' % cfg] + targets)

build_steps = [make_jobspec(cfg, 
                            list(set(itertools.chain.from_iterable(
                                         l.make_targets() for l in languages))))
               for cfg in build_configs]
build_steps.extend(set(
                   jobset.JobSpec(cmdline, environ={'CONFIG': cfg})
                   for cfg in build_configs
                   for l in languages
                   for cmdline in l.build_steps()))               
one_run = set(
    spec
    for config in run_configs
    for language in languages
    for spec in language.test_specs(config, args.travis)
    if re.search(args.regex, spec.shortname))

runs_per_test = args.runs_per_test
forever = args.forever


class TestCache(object):
  """Cache for running tests."""

  def __init__(self, use_cache_results):
    self._last_successful_run = {}
    self._use_cache_results = use_cache_results

  def should_run(self, cmdline, bin_hash):
    if cmdline not in self._last_successful_run:
      return True
    if self._last_successful_run[cmdline] != bin_hash:
      return True
    if not self._use_cache_results:
      return True
    return False

  def finished(self, cmdline, bin_hash):
    self._last_successful_run[cmdline] = bin_hash
    self.save()

  def dump(self):
    return [{'cmdline': k, 'hash': v}
            for k, v in self._last_successful_run.iteritems()]

  def parse(self, exdump):
    self._last_successful_run = dict((o['cmdline'], o['hash']) for o in exdump)

  def save(self):
    with open('.run_tests_cache', 'w') as f:
      f.write(json.dumps(self.dump()))

  def maybe_load(self):
    if os.path.exists('.run_tests_cache'):
      with open('.run_tests_cache') as f:
        self.parse(json.loads(f.read()))


def _build_and_run(check_cancelled, newline_on_success, travis, cache):
  """Do one pass of building & running tests."""
  # build latest sequentially
  if not jobset.run(build_steps, maxjobs=1,
                    newline_on_success=newline_on_success, travis=travis):
    return 1

  # run all the tests
  all_runs = itertools.chain.from_iterable(
      itertools.repeat(one_run, runs_per_test))
  if not jobset.run(all_runs, check_cancelled,
                    newline_on_success=newline_on_success, travis=travis,
                    maxjobs=min(args.jobs, min(c.maxjobs for c in run_configs)),
                    cache=cache):
    return 2

  return 0


test_cache = TestCache(runs_per_test == 1)
test_cache.maybe_load()

if forever:
  success = True
  while True:
    dw = watch_dirs.DirWatcher(['src', 'include', 'test', 'examples'])
    initial_time = dw.most_recent_change()
    have_files_changed = lambda: dw.most_recent_change() != initial_time
    previous_success = success
    success = _build_and_run(check_cancelled=have_files_changed,
                             newline_on_success=False,
                             travis=args.travis,
                             cache=test_cache) == 0
    if not previous_success and success:
      jobset.message('SUCCESS',
                     'All tests are now passing properly',
                     do_newline=True)
    jobset.message('IDLE', 'No change detected')
    while not have_files_changed():
      time.sleep(1)
else:
  result = _build_and_run(check_cancelled=lambda: False,
                          newline_on_success=args.newline_on_success,
                          travis=args.travis,
                          cache=test_cache)
  if result == 0:
    jobset.message('SUCCESS', 'All tests passed', do_newline=True)
  else:
    jobset.message('FAILED', 'Some tests failed', do_newline=True)
  sys.exit(result)
