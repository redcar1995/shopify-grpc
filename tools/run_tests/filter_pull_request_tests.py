#!/usr/bin/env python2.7
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

"""Filter out tests based on file differences compared to merge target branch"""

import re
from subprocess import call, check_output


class TestSuite:
  """
  Contains tag to identify job as belonging to this test suite and
  triggers to identify if changed files are relevant
  """
  def __init__(self, tags):
    """
    Build TestSuite to group tests by their tags
    :param tag: string used to identify if a job belongs to this TestSuite
    todo(mattkwong): Change the use of tag because do not want to depend on
    job.shortname to identify what suite a test belongs to
    """
    self.triggers = []
    self.tags = tags

  def add_trigger(self, trigger):
    """
    Add a regex to list of triggers that determine if a changed file should run tests
    :param trigger: regex matching file relevant to tests
    """
    self.triggers.append(trigger)

# Create test suites
_core_test_suite = TestSuite(['_c_'])
_cpp_test_suite = TestSuite(['_c++_'])
_csharp_test_suite = TestSuite(['_csharp_'])
_node_test_suite = TestSuite(['_node_'])
_objc_test_suite = TestSuite(['_objc_'])
_php_test_suite = TestSuite(['_php_', '_php7_'])
_python_test_suite = TestSuite(['_python_'])
_ruby_test_suite = TestSuite(['_ruby'])
_all_test_suites = [_core_test_suite, _cpp_test_suite, _csharp_test_suite,
                    _node_test_suite, _objc_test_suite, _php_test_suite,
                    _python_test_suite, _ruby_test_suite]

# Dictionary of whitelistable files where the key is a regex matching changed files
# and the value is a list of tests that should be run. An empty list means that
# the changed files should not trigger any tests. Any changed file that does not
# match any of these regexes will trigger all tests
_WHITELIST_DICT = {
  '^templates/.*': [],
  '^doc/.*': [],
  '^examples/.*': [],
  '^summerofcode/.*': [],
  '.*README.md$': [],
  '.*LICENSE$': [],
  '^src/cpp.*': [_cpp_test_suite],
  '^src/csharp.*': [_csharp_test_suite],
  '^src/node.*': [_node_test_suite],
  '^src/objective-c.*': [_objc_test_suite],
  '^src/php.*': [_php_test_suite],
  '^src/python.*': [_python_test_suite],
  '^src/ruby.*': [_ruby_test_suite],
  '^test/core.*': [_core_test_suite],
  '^test/cpp.*': [_cpp_test_suite],
  '^test/distrib/cpp.*': [_cpp_test_suite],
  '^test/distrib/csharp.*': [_csharp_test_suite],
  '^test/distrib/node.*': [_node_test_suite],
  '^test/distrib/php.*': [_php_test_suite],
  '^test/distrib/python.*': [_python_test_suite],
  '^test/distrib/ruby.*': [_ruby_test_suite]
}
# Add all triggers to their respective test suites
for trigger, test_suites in _WHITELIST_DICT.iteritems():
  for test_suite in test_suites:
    test_suite.add_trigger(trigger)


def _get_changed_files(base_branch):
  """
  Get list of changed files between current branch and base of target merge branch
  """
  # git fetch might need to be called on Jenkins slave
  # todo(mattkwong): remove or uncomment below after seeing if Jenkins needs this
  # call(['git', 'fetch'])

  # Get file changes between branch and merge-base of specified branch
  # Not combined to be Windows friendly
  base_commit = check_output(["git", "merge-base", base_branch, "HEAD"]).rstrip()
  return check_output(["git", "diff", base_commit, "--name-only"]).splitlines()


def _can_skip_tests(file_names, triggers):
  """
  Determines if tests are skippable based on if all files do not match list of regexes
  :param file_names: list of changed files generated by _get_changed_files()
  :param triggers: list of regexes matching file name that indicates tests should be run
  :return: safe to skip tests
  """
  for file_name in file_names:
    if any(re.match(trigger, file_name) for trigger in triggers):
      return False
  return True


def _remove_irrelevant_tests(tests, tag):
  """
  Filters out tests by config or language - will not remove sanitizer tests
  :param tests: list of all tests generated by run_tests_matrix.py
  :param tag: string representing language or config to filter - "_(language)_" or "_(config)"
  :return: list of relevant tests
  """
  # todo(mattkwong): find a more reliable way to filter tests - don't use shortname
  return [test for test in tests if tag not in test.shortname or
          any(san_tag in test.shortname for san_tag in ['_asan', '_tsan', '_msan'])]


def _remove_sanitizer_tests(tests):
  """
  Filters out sanitizer tests
  :param tests: list of all tests generated by run_tests_matrix.py
  :return: list of relevant tests
  """
  # todo(mattkwong): find a more reliable way to filter tests - don't use shortname
  return [test for test in tests if
          all(san_tag not in test.shortname for san_tag in ['_asan', '_tsan', '_msan'])]


def filter_tests(tests, base_branch):
  """
  Filters out tests that are safe to ignore
  :param tests: list of all tests generated by run_tests_matrix.py
  :return: list of relevant tests
  """
  print("Finding file differences between %s repo and current branch...\n" % base_branch)
  changed_files = _get_changed_files(base_branch)
  for changed_file in changed_files:
    print(changed_file)
  print

  # Regex that combines all keys in _WHITELIST_DICT
  all_triggers = "(" + ")|(".join(_WHITELIST_DICT.keys()) + ")"
  # Check if all tests have to be run
  for changed_file in changed_files:
    if not re.match(all_triggers, changed_file):
      return(tests)
  # Filter out tests by language
  for test_suite in _all_test_suites:
    if _can_skip_tests(changed_files, test_suite.triggers):
      for tag in test_suite.tags:
        print("  Filtering %s tests" % tag)
        tests = _remove_irrelevant_tests(tests, tag)
  # Sanitizer tests skipped if core and c++ are skipped
  if _can_skip_tests(changed_files, _cpp_test_suite.triggers + _core_test_suite.triggers):
    print("  Filtering Sanitizer tests")
    tests = _remove_sanitizer_tests(tests)

  return tests
