#!/usr/bin/env python
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
"""Helper to upload Jenkins test results to BQ"""

from __future__ import print_function

import os
import six
import sys
import time
import uuid

gcp_utils_dir = os.path.abspath(
    os.path.join(os.path.dirname(__file__), '../../gcp/utils'))
sys.path.append(gcp_utils_dir)
import big_query_utils

_DATASET_ID = 'jenkins_test_results'
_DESCRIPTION = 'Test results from master job run on Jenkins'
# 90 days in milliseconds
_EXPIRATION_MS = 90 * 24 * 60 * 60 * 1000
_PARTITION_TYPE = 'DAY'
_PROJECT_ID = 'grpc-testing'
_RESULTS_SCHEMA = [
    ('job_name', 'STRING', 'Name of Jenkins job'),
    ('build_id', 'INTEGER', 'Build ID of Jenkins job'),
    ('build_url', 'STRING', 'URL of Jenkins job'),
    ('test_name', 'STRING', 'Individual test name'),
    ('language', 'STRING', 'Language of test'),
    ('platform', 'STRING', 'Platform used for test'),
    ('config', 'STRING', 'Config used for test'),
    ('compiler', 'STRING', 'Compiler used for test'),
    ('iomgr_platform', 'STRING', 'Iomgr used for test'),
    ('result', 'STRING', 'Test result: PASSED, TIMEOUT, FAILED, or SKIPPED'),
    ('timestamp', 'TIMESTAMP', 'Timestamp of test run'),
    ('elapsed_time', 'FLOAT', 'How long test took to run'),
    ('cpu_estimated', 'FLOAT', 'Estimated CPU usage of test'),
    ('cpu_measured', 'FLOAT', 'Actual CPU usage of test'),
    ('return_code', 'INTEGER', 'Exit code of test'),
]
_INTEROP_RESULTS_SCHEMA = [
    ('job_name', 'STRING', 'Name of Jenkins/Kokoro job'),
    ('build_id', 'INTEGER', 'Build ID of Jenkins/Kokoro job'),
    ('build_url', 'STRING', 'URL of Jenkins/Kokoro job'),
    ('test_name', 'STRING',
     'Unique test name combining client, server, and test_name'),
    ('suite', 'STRING',
     'Test suite: cloud_to_cloud, cloud_to_prod, or cloud_to_prod_auth'),
    ('client', 'STRING', 'Client language'),
    ('server', 'STRING', 'Server host name'),
    ('test_case', 'STRING', 'Name of test case'),
    ('result', 'STRING', 'Test result: PASSED, TIMEOUT, FAILED, or SKIPPED'),
    ('timestamp', 'TIMESTAMP', 'Timestamp of test run'),
    ('elapsed_time', 'FLOAT', 'How long test took to run'),
]


def _get_build_metadata(test_results):
    """Add Jenkins/Kokoro build metadata to test_results based on environment
  variables set by Jenkins/Kokoro.
  """
    build_id = os.getenv('BUILD_ID') or os.getenv('KOKORO_BUILD_NUMBER')
    build_url = os.getenv('BUILD_URL') or os.getenv('KOKORO_BUILD_URL')
    job_name = os.getenv('JOB_BASE_NAME') or os.getenv('KOKORO_JOB_NAME')

    if build_id:
        test_results['build_id'] = build_id
    if build_url:
        test_results['build_url'] = build_url
    if job_name:
        test_results['job_name'] = job_name


def upload_results_to_bq(resultset, bq_table, args, platform):
    """Upload test results to a BQ table.

  Args:
      resultset: dictionary generated by jobset.run
      bq_table: string name of table to create/upload results to in BQ
      args: args in run_tests.py, generated by argparse
      platform: string name of platform tests were run on
  """
    bq = big_query_utils.create_big_query()
    big_query_utils.create_partitioned_table(
        bq,
        _PROJECT_ID,
        _DATASET_ID,
        bq_table,
        _RESULTS_SCHEMA,
        _DESCRIPTION,
        partition_type=_PARTITION_TYPE,
        expiration_ms=_EXPIRATION_MS)

    for shortname, results in six.iteritems(resultset):
        for result in results:
            test_results = {}
            _get_build_metadata(test_results)
            test_results['compiler'] = args.compiler
            test_results['config'] = args.config
            test_results['cpu_estimated'] = result.cpu_estimated
            test_results['cpu_measured'] = result.cpu_measured
            test_results['elapsed_time'] = '%.2f' % result.elapsed_time
            test_results['iomgr_platform'] = args.iomgr_platform
            # args.language is a list, but will always have one element in the contexts
            # this function is used.
            test_results['language'] = args.language[0]
            test_results['platform'] = platform
            test_results['result'] = result.state
            test_results['return_code'] = result.returncode
            test_results['test_name'] = shortname
            test_results['timestamp'] = time.strftime('%Y-%m-%d %H:%M:%S')

            row = big_query_utils.make_row(str(uuid.uuid4()), test_results)

            # TODO(jtattermusch): rows are inserted one by one, very inefficient
            max_retries = 3
            for attempt in range(max_retries):
                if big_query_utils.insert_rows(bq, _PROJECT_ID, _DATASET_ID,
                                               bq_table, [row]):
                    break
                else:
                    if attempt < max_retries - 1:
                        print('Error uploading result to bigquery, will retry.')
                    else:
                        print(
                            'Error uploading result to bigquery, all attempts failed.'
                        )
                        sys.exit(1)


def upload_interop_results_to_bq(resultset, bq_table, args):
    """Upload interop test results to a BQ table.

  Args:
      resultset: dictionary generated by jobset.run
      bq_table: string name of table to create/upload results to in BQ
      args: args in run_interop_tests.py, generated by argparse
  """
    bq = big_query_utils.create_big_query()
    big_query_utils.create_partitioned_table(
        bq,
        _PROJECT_ID,
        _DATASET_ID,
        bq_table,
        _INTEROP_RESULTS_SCHEMA,
        _DESCRIPTION,
        partition_type=_PARTITION_TYPE,
        expiration_ms=_EXPIRATION_MS)

    for shortname, results in six.iteritems(resultset):
        for result in results:
            test_results = {}
            _get_build_metadata(test_results)
            test_results['elapsed_time'] = '%.2f' % result.elapsed_time
            test_results['result'] = result.state
            test_results['test_name'] = shortname
            test_results['suite'] = shortname.split(':')[0]
            test_results['client'] = shortname.split(':')[1]
            test_results['server'] = shortname.split(':')[2]
            test_results['test_case'] = shortname.split(':')[3]
            test_results['timestamp'] = time.strftime('%Y-%m-%d %H:%M:%S')
            row = big_query_utils.make_row(str(uuid.uuid4()), test_results)
            # TODO(jtattermusch): rows are inserted one by one, very inefficient
            max_retries = 3
            for attempt in range(max_retries):
                if big_query_utils.insert_rows(bq, _PROJECT_ID, _DATASET_ID,
                                               bq_table, [row]):
                    break
                else:
                    if attempt < max_retries - 1:
                        print('Error uploading result to bigquery, will retry.')
                    else:
                        print(
                            'Error uploading result to bigquery, all attempts failed.'
                        )
                        sys.exit(1)
