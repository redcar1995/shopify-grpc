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
import platform
import random
import re
import subprocess
import sys
import time
import xml.etree.cElementTree as ET

import jobset
import watch_dirs

ROOT = os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), '../..'))
os.chdir(ROOT)


_FORCE_ENVIRON_FOR_WRAPPERS = {}


# SimpleConfig: just compile with CONFIG=config, and run the binary to test
class SimpleConfig(object):

  def __init__(self, config, environ=None):
    if environ is None:
      environ = {}
    self.build_config = config
    self.allow_hashing = (config != 'gcov')
    self.environ = environ
    self.environ['CONFIG'] = config

  def job_spec(self, cmdline, hash_targets, shortname=None, environ={}):
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
    actual_environ = self.environ.copy()
    for k, v in environ.iteritems():
      actual_environ[k] = v
    return jobset.JobSpec(cmdline=cmdline,
                          shortname=shortname,
                          environ=actual_environ,
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
    self.allow_hashing = False

  def job_spec(self, cmdline, hash_targets):
    return jobset.JobSpec(cmdline=['valgrind', '--tool=%s' % self.tool] +
                          self.args + cmdline,
                          shortname='valgrind %s' % cmdline[0],
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
        binary = 'vsprojects/test_bin/%s.exe' % (target['name'])
      else:
        binary = 'bins/%s/%s' % (config.build_config, target['name'])
      out.append(config.job_spec([binary], [binary]))
    return sorted(out)

  def make_targets(self):
    return ['buildtests_%s' % self.make_target, 'tools_%s' % self.make_target]

  def build_steps(self):
    return []

  def supports_multi_config(self):
    return True

  def __str__(self):
    return self.make_target


class NodeLanguage(object):

  def test_specs(self, config, travis):
    return [config.job_spec(['tools/run_tests/run_node.sh'], None,
                            environ=_FORCE_ENVIRON_FOR_WRAPPERS)]

  def make_targets(self):
    return ['static_c', 'shared_c']

  def build_steps(self):
    return [['tools/run_tests/build_node.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'node'


class PhpLanguage(object):

  def test_specs(self, config, travis):
    return [config.job_spec(['src/php/bin/run_tests.sh'], None,
                            environ=_FORCE_ENVIRON_FOR_WRAPPERS)]

  def make_targets(self):
    return ['static_c', 'shared_c']

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
    self._build_python_versions = set([
        python_version
        for test in self._tests
        for python_version in test['pythonVersions']])
    self._has_python_versions = []

  def test_specs(self, config, travis):
    job_specifications = []
    for test in self._tests:
      command = None
      short_name = None
      if 'module' in test:
        command = ['tools/run_tests/run_python.sh', '-m', test['module']]
        short_name = test['module']
      elif 'file' in test:
        command = ['tools/run_tests/run_python.sh', test['file']]
        short_name = test['file']
      else:
        raise ValueError('expected input to be a module or file to run '
                         'unittests from')
      for python_version in test['pythonVersions']:
        if python_version in self._has_python_versions:
          environment = dict(_FORCE_ENVIRON_FOR_WRAPPERS)
          environment['PYVER'] = python_version
          job_specifications.append(config.job_spec(
              command, None, environ=environment, shortname=short_name))
        else:
          jobset.message(
              'WARNING',
              'Could not find Python {}; skipping test'.format(python_version),
              '{}\n'.format(command), do_newline=True)
    return job_specifications

  def make_targets(self):
    return ['static_c', 'grpc_python_plugin', 'shared_c']

  def build_steps(self):
    commands = []
    for python_version in self._build_python_versions:
      try:
        with open(os.devnull, 'w') as output:
          subprocess.check_call(['which', 'python' + python_version],
                                stdout=output, stderr=output)
        commands.append(['tools/run_tests/build_python.sh', python_version])
        self._has_python_versions.append(python_version)
      except:
        jobset.message('WARNING', 'Missing Python ' + python_version,
                       do_newline=True)
    return commands

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'python'


class RubyLanguage(object):

  def test_specs(self, config, travis):
    return [config.job_spec(['tools/run_tests/run_ruby.sh'], None,
                            environ=_FORCE_ENVIRON_FOR_WRAPPERS)]

  def make_targets(self):
    return ['run_dep_checks']

  def build_steps(self):
    return [['tools/run_tests/build_ruby.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'ruby'


class CSharpLanguage(object):
  def __init__(self):
    if platform.system() == 'Windows':
      plat = 'windows'
    else:
      plat = 'posix'
    self.platform = plat

  def test_specs(self, config, travis):
    assemblies = ['Grpc.Core.Tests',
                  'Grpc.Examples.Tests',
                  'Grpc.IntegrationTesting']
    if self.platform == 'windows':
      cmd = 'tools\\run_tests\\run_csharp.bat'
    else:
      cmd = 'tools/run_tests/run_csharp.sh'
    return [config.job_spec([cmd, assembly],
            None, shortname=assembly,
            environ=_FORCE_ENVIRON_FOR_WRAPPERS)
            for assembly in assemblies ]

  def make_targets(self):
    # For Windows, this target doesn't really build anything,
    # everything is build by buildall script later.
    return ['grpc_csharp_ext']

  def build_steps(self):
    if self.platform == 'windows':
      return [['src\\csharp\\buildall.bat']]
    else:
      return [['tools/run_tests/build_csharp.sh']]

  def supports_multi_config(self):
    return False

  def __str__(self):
    return 'csharp'


class Sanity(object):

  def test_specs(self, config, travis):
    return [config.job_spec('tools/run_tests/run_sanity.sh', None),
            config.job_spec('tools/run_tests/check_sources_and_headers.py', None)]

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
        'TSAN_OPTIONS': 'suppressions=tools/tsan_suppressions.txt:halt_on_error=1:second_deadlock_stack=1'}),
    'msan': SimpleConfig('msan'),
    'ubsan': SimpleConfig('ubsan'),
    'asan': SimpleConfig('asan', environ={
        'ASAN_OPTIONS': 'detect_leaks=1:color=always:suppressions=tools/tsan_suppressions.txt',
        'LSAN_OPTIONS': 'report_objects=1'}),
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

def runs_per_test_type(arg_str):
    """Auxilary function to parse the "runs_per_test" flag.

       Returns:
           A positive integer or 0, the latter indicating an infinite number of
           runs.

       Raises:
           argparse.ArgumentTypeError: Upon invalid input.
    """
    if arg_str == 'inf':
        return 0
    try:
        n = int(arg_str)
        if n <= 0: raise ValueError
        return n
    except:
        msg = "'{}' isn't a positive integer or 'inf'".format(arg_str)
        raise argparse.ArgumentTypeError(msg)
argp.add_argument('-n', '--runs_per_test', default=1, type=runs_per_test_type,
        help='A positive integer or "inf". If "inf", all tests will run in an '
             'infinite loop. Especially useful in combination with "-f"')
argp.add_argument('-r', '--regex', default='.*', type=str)
argp.add_argument('-j', '--jobs', default=2 * multiprocessing.cpu_count(), type=int)
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
argp.add_argument('-S', '--stop_on_failure',
                  default=False,
                  action='store_const',
                  const=True)
argp.add_argument('-a', '--antagonists', default=0, type=int)
argp.add_argument('-x', '--xml_report', default=None, type=str,
        help='Generates a JUnit-compatible XML report')
args = argp.parse_args()

# grab config
run_configs = set(_CONFIGS[cfg]
                  for cfg in itertools.chain.from_iterable(
                      _CONFIGS.iterkeys() if x == 'all' else [x]
                      for x in args.config))
build_configs = set(cfg.build_config for cfg in run_configs)

if args.travis:
  _FORCE_ENVIRON_FOR_WRAPPERS = {'GRPC_TRACE': 'surface,batch'}

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
    self._last_save = time.time()

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
    if time.time() - self._last_save > 1:
      self.save()

  def dump(self):
    return [{'cmdline': k, 'hash': v}
            for k, v in self._last_successful_run.iteritems()]

  def parse(self, exdump):
    self._last_successful_run = dict((o['cmdline'], o['hash']) for o in exdump)

  def save(self):
    with open('.run_tests_cache', 'w') as f:
      f.write(json.dumps(self.dump()))
    self._last_save = time.time()

  def maybe_load(self):
    if os.path.exists('.run_tests_cache'):
      with open('.run_tests_cache') as f:
        self.parse(json.loads(f.read()))


def _build_and_run(check_cancelled, newline_on_success, travis, cache, xml_report=None):
  """Do one pass of building & running tests."""
  # build latest sequentially
  if not jobset.run(build_steps, maxjobs=1,
                    newline_on_success=newline_on_success, travis=travis):
    return 1

  # start antagonists
  antagonists = [subprocess.Popen(['tools/run_tests/antagonist.py'])
                 for _ in range(0, args.antagonists)]
  try:
    infinite_runs = runs_per_test == 0
    # When running on travis, we want out test runs to be as similar as possible
    # for reproducibility purposes.
    if travis:
      massaged_one_run = sorted(one_run, key=lambda x: x.shortname)
    else:
      # whereas otherwise, we want to shuffle things up to give all tests a
      # chance to run.
      massaged_one_run = list(one_run)  # random.shuffle needs an indexable seq.
      random.shuffle(massaged_one_run)  # which it modifies in-place.
    if infinite_runs:
      assert len(massaged_one_run) > 0, 'Must have at least one test for a -n inf run'
    runs_sequence = (itertools.repeat(massaged_one_run) if infinite_runs
                     else itertools.repeat(massaged_one_run, runs_per_test))
    all_runs = itertools.chain.from_iterable(runs_sequence)

    root = ET.Element('testsuites') if xml_report else None
    testsuite = ET.SubElement(root, 'testsuite', id='1', package='grpc', name='tests') if xml_report else None

    if not jobset.run(all_runs, check_cancelled,
                      newline_on_success=newline_on_success, travis=travis,
                      infinite_runs=infinite_runs,
                      maxjobs=args.jobs,
                      stop_on_failure=args.stop_on_failure,
                      cache=cache if not xml_report else None,
                      xml_report=testsuite):
      return 2
  finally:
    for antagonist in antagonists:
      antagonist.kill()
    if xml_report:
      tree = ET.ElementTree(root)
      tree.write(xml_report, encoding='UTF-8')

  if cache: cache.save()

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
                          cache=test_cache,
                          xml_report=args.xml_report)
  if result == 0:
    jobset.message('SUCCESS', 'All tests passed', do_newline=True)
  else:
    jobset.message('FAILED', 'Some tests failed', do_newline=True)
  sys.exit(result)
