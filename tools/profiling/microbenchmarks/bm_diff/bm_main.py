#!/usr/bin/env python2.7
#
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

""" Runs the entire bm_*.py pipeline, and possible comments on the PR """

import bm_constants
import bm_build
import bm_run
import bm_diff

import sys
import os
import argparse
import multiprocessing
import subprocess

sys.path.append(
  os.path.join(
    os.path.dirname(sys.argv[0]), '..', '..', 'run_tests', 'python_utils'))
import comment_on_pr


def _args():
  argp = argparse.ArgumentParser(
    description='Perform diff on microbenchmarks')
  argp.add_argument(
    '-t',
    '--track',
    choices=sorted(bm_constants._INTERESTING),
    nargs='+',
    default=sorted(bm_constants._INTERESTING),
    help='Which metrics to track')
  argp.add_argument(
    '-b',
    '--benchmarks',
    nargs='+',
    choices=bm_constants._AVAILABLE_BENCHMARK_TESTS,
    default=bm_constants._AVAILABLE_BENCHMARK_TESTS,
    help='Which benchmarks to run')
  argp.add_argument(
    '-d',
    '--diff_base',
    type=str,
    help='Commit or branch to compare the current one to')
  argp.add_argument(
    '-o',
    '--old',
    default='old',
    type=str,
    help='Name of baseline run to compare to. Ususally just called "old"')
  argp.add_argument(
    '-r',
    '--repetitions',
    type=int,
    default=1,
    help='Number of repetitions to pass to the benchmarks')
  argp.add_argument(
    '-l',
    '--loops',
    type=int,
    default=20,
    help='Number of times to loops the benchmarks. More loops cuts down on noise'
  )
  argp.add_argument(
    '-j',
    '--jobs',
    type=int,
    default=multiprocessing.cpu_count(),
    help='Number of CPUs to use')
  args = argp.parse_args()
  assert args.diff_base or args.old, "One of diff_base or old must be set!"
  if args.loops < 3:
    print "WARNING: This run will likely be noisy. Increase loops."
  return args


def eintr_be_gone(fn):
  """Run fn until it doesn't stop because of EINTR"""

  def inner(*args):
    while True:
      try:
        return fn(*args)
      except IOError, e:
        if e.errno != errno.EINTR:
          raise

  return inner


def main(args):

  bm_build.build('new', args.benchmarks, args.jobs)

  old = args.old
  if args.diff_base:
    old = 'old'
    where_am_i = subprocess.check_output(
      ['git', 'rev-parse', '--abbrev-ref', 'HEAD']).strip()
    subprocess.check_call(['git', 'checkout', args.diff_base])
    try:
      bm_build.build('old', args.benchmarks, args.jobs)
    finally:
      subprocess.check_call(['git', 'checkout', where_am_i])
      subprocess.check_call(['git', 'submodule', 'update'])

  bm_run.run('new', args.benchmarks, args.jobs, args.loops, args.repetitions)
  bm_run.run(old, args.benchmarks, args.jobs, args.loops, args.repetitions)

  diff, note = bm_diff.diff(args.benchmarks, args.loops, args.track, old,
                'new')
  if diff:
    text = 'Performance differences noted:\n' + diff
  else:
    text = 'No significant performance differences'
  print('%s\n%s' % (note, text))
  comment_on_pr.comment_on_pr('```\n%s\n\n%s\n```' % (note, text))


if __name__ == '__main__':
  args = _args()
  main(args)
