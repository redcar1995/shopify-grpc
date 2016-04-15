#!/usr/bin/env python2.7
# Copyright 2016, Google Inc.
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

# Uploads performance benchmark result file to bigquery.

import argparse
import json
import os
import sys
import uuid


gcp_utils_dir = os.path.abspath(os.path.join(
    os.path.dirname(__file__), '../../gcp/utils'))
sys.path.append(gcp_utils_dir)
import big_query_utils


_PROJECT_ID='grpc-testing'


def _upload_scenario_result_to_bigquery(dataset_id, table_id, result_file):
  bq = big_query_utils.create_big_query()
  _create_results_table(bq, dataset_id, table_id)

  with open(result_file, 'r') as f:
    scenario_result = json.loads(f.read())

  if not _insert_result(bq, dataset_id, table_id, scenario_result):
    print 'Error uploading result to bigquery.'
    sys.exit(1)


def _insert_result(bq, dataset_id, table_id, scenario_result):
  _flatten_result_inplace(scenario_result)
  row = big_query_utils.make_row(str(uuid.uuid4()), scenario_result)
  return big_query_utils.insert_rows(bq,
                                     _PROJECT_ID,
                                     dataset_id,
                                     table_id,
                                     [row])


def _create_results_table(bq, dataset_id, table_id):
  with open(os.path.dirname(__file__) + '/scenario_result_schema.json', 'r') as f:
    table_schema = json.loads(f.read())
  desc = 'Results of performance benchmarks.'
  return big_query_utils.create_table2(bq, _PROJECT_ID, dataset_id,
                               table_id, table_schema, desc)


def _flatten_result_inplace(scenario_result):
  """Bigquery is not really great for handling deeply nested data
  and repeated fields. To maintain values of some fields while keeping
  the schema relatively simple, we artificially leave some of the fields
  as JSON strings.
  """
  scenario_result['scenario']['clientConfig'] = json.dumps(scenario_result['scenario']['clientConfig'])
  scenario_result['scenario']['serverConfig'] = json.dumps(scenario_result['scenario']['serverConfig'])
  scenario_result['latencies'] = json.dumps(scenario_result['latencies'])
  for stats in scenario_result['clientStats']:
    stats['latencies'] = json.dumps(stats['latencies'])
  scenario_result['serverCores'] = json.dumps(scenario_result['serverCores'])


argp = argparse.ArgumentParser(description='Upload result to big query.')
argp.add_argument('--bq_result_table', required=True, default=None, type=str,
                  help='Bigquery "dataset.table" to upload results to.')
argp.add_argument('--file_to_upload', default='scenario_result.json', type=str,
                  help='Report file to upload.')

args = argp.parse_args()

dataset_id, table_id = args.bq_result_table.split('.', 2)
_upload_scenario_result_to_bigquery(dataset_id, table_id, args.file_to_upload)
print 'Successfully uploaded %s to BigQuery.\n' % args.file_to_upload
