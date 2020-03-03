#!/usr/bin/env python
# Copyright 2020 gRPC authors.
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
"""Run xDS integration tests on GCP using Traffic Director."""

import argparse
import googleapiclient.discovery
import grpc
import logging
import os
import random
import shlex
import socket
import subprocess
import sys
import tempfile
import time

from oauth2client.client import GoogleCredentials

from src.proto.grpc.testing import messages_pb2
from src.proto.grpc.testing import test_pb2_grpc

logger = logging.getLogger()
console_handler = logging.StreamHandler()
logger.addHandler(console_handler)


def parse_port_range(port_arg):
    try:
        port = int(port_arg)
        return range(port, port + 1)
    except:
        port_min, port_max = port_arg.split(':')
        return range(int(port_min), int(port_max) + 1)


argp = argparse.ArgumentParser(description='Run xDS interop tests on GCP')
argp.add_argument('--project_id', help='GCP project id')
argp.add_argument(
    '--gcp_suffix',
    default='',
    help='Optional suffix for all generated GCP resource names. Useful to '
    'ensure distinct names across test runs.')
argp.add_argument(
    '--test_case',
    default=None,
    choices=[
        'all',
        'backends_restart',
        'change_backend_service',
        'new_instance_group_receives_traffic',
        'ping_pong',
        'remove_instance_group',
        'round_robin',
        'secondary_locality_gets_requests_on_primary_failure',
        'secondary_locality_gets_no_requests_on_partial_primary_failure',
    ])
argp.add_argument(
    '--client_cmd',
    default=None,
    help='Command to launch xDS test client. This script will fill in '
    '{service_host}, {service_port},{stats_port} and {qps} parameters using '
    'str.format(), and generate the GRPC_XDS_BOOTSTRAP file.')
argp.add_argument('--zone', default='us-central1-a')
argp.add_argument('--secondary_zone',
                  default='us-west1-b',
                  help='Zone to use for secondary TD locality tests')
argp.add_argument('--qps', default=10, help='Client QPS')
argp.add_argument(
    '--wait_for_backend_sec',
    default=600,
    help='Time limit for waiting for created backend services to report '
    'healthy when launching or updated GCP resources')
argp.add_argument(
    '--keep_gcp_resources',
    default=False,
    action='store_true',
    help=
    'Leave GCP VMs and configuration running after test. Default behavior is '
    'to delete when tests complete.')
argp.add_argument(
    '--compute_discovery_document',
    default=None,
    type=str,
    help=
    'If provided, uses this file instead of retrieving via the GCP discovery '
    'API')
argp.add_argument('--network',
                  default='global/networks/default',
                  help='GCP network to use')
argp.add_argument('--service_port_range',
                  default='8080:8100',
                  type=parse_port_range,
                  help='Listening port for created gRPC backends. Specified as '
                  'either a single int or as a range in the format min:max, in '
                  'which case an available port p will be chosen s.t. min <= p '
                  '<= max')
argp.add_argument(
    '--stats_port',
    default=8079,
    type=int,
    help='Local port for the client process to expose the LB stats service')
argp.add_argument('--xds_server',
                  default='trafficdirector.googleapis.com:443',
                  help='xDS server')
argp.add_argument('--source_image',
                  default='projects/debian-cloud/global/images/family/debian-9',
                  help='Source image for VMs created during the test')
argp.add_argument(
    '--tolerate_gcp_errors',
    default=False,
    action='store_true',
    help=
    'Continue with test even when an error occurs during setup. Intended for '
    'manual testing, where attempts to recreate any GCP resources already '
    'existing will result in an error')
argp.add_argument('--verbose',
                  help='verbose log output',
                  default=False,
                  action='store_true')
args = argp.parse_args()

if args.verbose:
    logger.setLevel(logging.DEBUG)

_WAIT_FOR_BACKEND_SEC = args.wait_for_backend_sec
_WAIT_FOR_OPERATION_SEC = 60
_INSTANCE_GROUP_SIZE = 2
_NUM_TEST_RPCS = 10 * args.qps
_WAIT_FOR_STATS_SEC = 60
_BOOTSTRAP_TEMPLATE = """
{{
  "node": {{
    "id": "{node_id}"
  }},
  "xds_servers": [{{
    "server_uri": "%s",
    "channel_creds": [
      {{
        "type": "google_default",
        "config": {{}}
      }}
    ]
  }}]
}}""" % args.xds_server
_PATH_MATCHER_NAME = 'path-matcher'
_BASE_TEMPLATE_NAME = 'test-template'
_BASE_INSTANCE_GROUP_NAME = 'test-ig'
_BASE_HEALTH_CHECK_NAME = 'test-hc'
_BASE_FIREWALL_RULE_NAME = 'test-fw-rule'
_BASE_BACKEND_SERVICE_NAME = 'test-backend-service'
_BASE_URL_MAP_NAME = 'test-map'
_BASE_SERVICE_HOST = 'grpc-test'
_BASE_TARGET_PROXY_NAME = 'test-target-proxy'
_BASE_FORWARDING_RULE_NAME = 'test-forwarding-rule'


def get_client_stats(num_rpcs, timeout_sec):
    with grpc.insecure_channel('localhost:%d' % args.stats_port) as channel:
        stub = test_pb2_grpc.LoadBalancerStatsServiceStub(channel)
        request = messages_pb2.LoadBalancerStatsRequest()
        request.num_rpcs = num_rpcs
        request.timeout_sec = timeout_sec
        rpc_timeout = timeout_sec * 2  # Allow time for connection establishment
        try:
            response = stub.GetClientStats(request,
                                           wait_for_ready=True,
                                           timeout=rpc_timeout)
            logger.debug('Invoked GetClientStats RPC: %s', response)
            return response
        except grpc.RpcError as rpc_error:
            raise Exception('GetClientStats RPC failed')


def wait_until_only_given_instances_receive_load(backends,
                                                 timeout_sec,
                                                 num_rpcs=100,
                                                 allow_failures=False):
    start_time = time.time()
    error_msg = None
    logger.debug('Waiting for %d sec until backends %s  receive load' %
                 (timeout_sec, backends))
    while time.time() - start_time <= timeout_sec:
        error_msg = None
        stats = get_client_stats(num_rpcs, timeout_sec)
        rpcs_by_peer = stats.rpcs_by_peer
        for backend in backends:
            if backend not in rpcs_by_peer:
                error_msg = 'Backend %s did not receive load' % backend
                break
        if not error_msg and len(rpcs_by_peer) > len(backends):
            error_msg = 'Unexpected backend received load: %s' % rpcs_by_peer
        if not allow_failures and stats.num_failures > 0:
            error_msg = '%d RPCs failed' % stats.num_failures
        if not error_msg:
            return
    raise Exception(error_msg)


def test_backends_restart(gcp, backend_service, instance_group):
    instance_names = get_instance_names(gcp, instance_group)
    num_instances = len(instance_names)
    start_time = time.time()
    wait_until_only_given_instances_receive_load(instance_names,
                                                 _WAIT_FOR_STATS_SEC)
    stats = get_client_stats(_NUM_TEST_RPCS, _WAIT_FOR_STATS_SEC)
    try:
        resize_instance_group(gcp, instance_group, 0)
        wait_until_only_given_instances_receive_load([],
                                                     _WAIT_FOR_BACKEND_SEC,
                                                     allow_failures=True)
    finally:
        resize_instance_group(gcp, instance_group, num_instances)
    wait_for_healthy_backends(gcp, backend_service, instance_group)
    new_instance_names = get_instance_names(gcp, instance_group)
    wait_until_only_given_instances_receive_load(new_instance_names,
                                                 _WAIT_FOR_BACKEND_SEC)
    new_stats = get_client_stats(_NUM_TEST_RPCS, _WAIT_FOR_STATS_SEC)
    original_distribution = list(stats.rpcs_by_peer.values())
    original_distribution.sort()
    new_distribution = list(new_stats.rpcs_by_peer.values())
    new_distribution.sort()
    if original_distribution != new_distribution:
        raise Exception('Distributions do not match: ', stats, new_stats)


def test_change_backend_service(gcp, original_backend_service, instance_group,
                                alternate_backend_service,
                                same_zone_instance_group):
    original_backend_instances = get_instance_names(gcp, instance_group)
    alternate_backend_instances = get_instance_names(gcp,
                                                     same_zone_instance_group)
    patch_backend_instances(gcp, alternate_backend_service,
                            [same_zone_instance_group])
    wait_for_healthy_backends(gcp, original_backend_service, instance_group)
    wait_for_healthy_backends(gcp, alternate_backend_service,
                              same_zone_instance_group)
    wait_until_only_given_instances_receive_load(original_backend_instances,
                                                 _WAIT_FOR_STATS_SEC)
    try:
        patch_url_map_backend_service(gcp, alternate_backend_service)
        stats = get_client_stats(_NUM_TEST_RPCS, _WAIT_FOR_STATS_SEC)
        if stats.num_failures > 0:
            raise Exception('Unexpected failure: %s', stats)
        wait_until_only_given_instances_receive_load(
            alternate_backend_instances, _WAIT_FOR_STATS_SEC)
    finally:
        patch_url_map_backend_service(gcp, original_backend_service)
        patch_backend_instances(gcp, alternate_backend_service, [])


def test_new_instance_group_receives_traffic(gcp, backend_service,
                                             instance_group,
                                             same_zone_instance_group):
    instance_names = get_instance_names(gcp, instance_group)
    wait_until_only_given_instances_receive_load(instance_names,
                                                 _WAIT_FOR_STATS_SEC)
    try:
        patch_backend_instances(gcp,
                                backend_service,
                                [instance_group, same_zone_instance_group],
                                balancing_mode='RATE')
        wait_for_healthy_backends(gcp, backend_service, instance_group)
        wait_for_healthy_backends(gcp, backend_service,
                                  same_zone_instance_group)
        combined_instance_names = instance_names + get_instance_names(
            gcp, same_zone_instance_group)
        wait_until_only_given_instances_receive_load(combined_instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)
    finally:
        patch_backend_instances(gcp, backend_service, [instance_group])


def test_ping_pong(gcp, backend_service, instance_group):
    wait_for_healthy_backends(gcp, backend_service, instance_group)
    instance_names = get_instance_names(gcp, instance_group)
    start_time = time.time()
    error_msg = None
    while time.time() - start_time <= _WAIT_FOR_STATS_SEC:
        error_msg = None
        stats = get_client_stats(_NUM_TEST_RPCS, _WAIT_FOR_STATS_SEC)
        rpcs_by_peer = stats.rpcs_by_peer
        for instance in instance_names:
            if instance not in rpcs_by_peer:
                error_msg = 'Instance %s did not receive load' % instance
                break
        if not error_msg and len(rpcs_by_peer) > len(instance_names):
            error_msg = 'Unexpected instance received load: %s' % rpcs_by_peer
        if not error_msg:
            return
    raise Exception(error_msg)


def test_remove_instance_group(gcp, backend_service, instance_group,
                               same_zone_instance_group):
    try:
        patch_backend_instances(gcp,
                                backend_service,
                                [instance_group, same_zone_instance_group],
                                balancing_mode='RATE')
        wait_for_healthy_backends(gcp, backend_service, instance_group)
        wait_for_healthy_backends(gcp, backend_service,
                                  same_zone_instance_group)
        instance_names = get_instance_names(gcp, instance_group)
        same_zone_instance_names = get_instance_names(gcp,
                                                      same_zone_instance_group)
        wait_until_only_given_instances_receive_load(
            instance_names + same_zone_instance_names, _WAIT_FOR_BACKEND_SEC)
        patch_backend_instances(gcp,
                                backend_service, [same_zone_instance_group],
                                balancing_mode='RATE')
        wait_until_only_given_instances_receive_load(same_zone_instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)
    finally:
        patch_backend_instances(gcp, backend_service, [instance_group])
        wait_until_only_given_instances_receive_load(instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)


def test_round_robin(gcp, backend_service, instance_group):
    wait_for_healthy_backends(gcp, backend_service, instance_group)
    instance_names = get_instance_names(gcp, instance_group)
    threshold = 1
    wait_until_only_given_instances_receive_load(instance_names,
                                                 _WAIT_FOR_STATS_SEC)
    stats = get_client_stats(_NUM_TEST_RPCS, _WAIT_FOR_STATS_SEC)
    requests_received = [stats.rpcs_by_peer[x] for x in stats.rpcs_by_peer]
    total_requests_received = sum(
        [stats.rpcs_by_peer[x] for x in stats.rpcs_by_peer])
    if total_requests_received != _NUM_TEST_RPCS:
        raise Exception('Unexpected RPC failures', stats)
    expected_requests = total_requests_received / len(instance_names)
    for instance in instance_names:
        if abs(stats.rpcs_by_peer[instance] - expected_requests) > threshold:
            raise Exception(
                'RPC peer distribution differs from expected by more than %d '
                'for instance %s (%s)', threshold, instance, stats)


def test_secondary_locality_gets_no_requests_on_partial_primary_failure(
        gcp, backend_service, primary_instance_group,
        secondary_zone_instance_group):
    try:
        patch_backend_instances(
            gcp, backend_service,
            [primary_instance_group, secondary_zone_instance_group])
        wait_for_healthy_backends(gcp, backend_service, primary_instance_group)
        wait_for_healthy_backends(gcp, backend_service,
                                  secondary_zone_instance_group)
        primary_instance_names = get_instance_names(gcp, instance_group)
        secondary_instance_names = get_instance_names(
            gcp, secondary_zone_instance_group)
        wait_until_only_given_instances_receive_load(primary_instance_names,
                                                     _WAIT_FOR_STATS_SEC)
        original_size = len(primary_instance_names)
        resize_instance_group(gcp, primary_instance_group, original_size - 1)
        remaining_instance_names = get_instance_names(gcp,
                                                      primary_instance_group)
        wait_until_only_given_instances_receive_load(remaining_instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)
    finally:
        patch_backend_instances(gcp, backend_service, [primary_instance_group])
        resize_instance_group(gcp, primary_instance_group, original_size)


def test_secondary_locality_gets_requests_on_primary_failure(
        gcp, backend_service, primary_instance_group,
        secondary_zone_instance_group):
    try:
        patch_backend_instances(
            gcp, backend_service,
            [primary_instance_group, secondary_zone_instance_group])
        wait_for_healthy_backends(gcp, backend_service, primary_instance_group)
        wait_for_healthy_backends(gcp, backend_service,
                                  secondary_zone_instance_group)
        primary_instance_names = get_instance_names(gcp, instance_group)
        secondary_instance_names = get_instance_names(
            gcp, secondary_zone_instance_group)
        wait_until_only_given_instances_receive_load(primary_instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)
        original_size = len(primary_instance_names)
        resize_instance_group(gcp, primary_instance_group, 0)
        wait_until_only_given_instances_receive_load(secondary_instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)

        resize_instance_group(gcp, primary_instance_group, original_size)
        new_instance_names = get_instance_names(gcp, primary_instance_group)
        wait_for_healthy_backends(gcp, backend_service, primary_instance_group)
        wait_until_only_given_instances_receive_load(new_instance_names,
                                                     _WAIT_FOR_BACKEND_SEC)
    finally:
        patch_backend_instances(gcp, backend_service, [primary_instance_group])


def create_instance_template(gcp, name, network, source_image):
    config = {
        'name': name,
        'properties': {
            'tags': {
                'items': ['allow-health-checks']
            },
            'machineType': 'e2-standard-2',
            'serviceAccounts': [{
                'email': 'default',
                'scopes': ['https://www.googleapis.com/auth/cloud-platform',]
            }],
            'networkInterfaces': [{
                'accessConfigs': [{
                    'type': 'ONE_TO_ONE_NAT'
                }],
                'network': network
            }],
            'disks': [{
                'boot': True,
                'initializeParams': {
                    'sourceImage': source_image
                }
            }],
            'metadata': {
                'items': [{
                    'key':
                        'startup-script',
                    'value':
                        """#!/bin/bash
sudo apt update
sudo apt install -y git default-jdk
mkdir java_server
pushd java_server
git clone https://github.com/grpc/grpc-java.git
pushd grpc-java
pushd interop-testing
../gradlew installDist -x test -PskipCodegen=true -PskipAndroid=true

nohup build/install/grpc-interop-testing/bin/xds-test-server --port=%d 1>/dev/null &"""
                        % gcp.service_port
                }]
            }
        }
    }

    result = gcp.compute.instanceTemplates().insert(project=gcp.project,
                                                    body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    gcp.instance_template = GcpResource(config['name'], result['targetLink'])


def add_instance_group(gcp, zone, name, size):
    config = {
        'name': name,
        'instanceTemplate': gcp.instance_template.url,
        'targetSize': size,
        'namedPorts': [{
            'name': 'grpc',
            'port': gcp.service_port
        }]
    }

    result = gcp.compute.instanceGroupManagers().insert(project=gcp.project,
                                                        zone=zone,
                                                        body=config).execute()
    wait_for_zone_operation(gcp, zone, result['name'])
    result = gcp.compute.instanceGroupManagers().get(
        project=gcp.project, zone=zone,
        instanceGroupManager=config['name']).execute()
    instance_group = InstanceGroup(config['name'], result['instanceGroup'],
                                   zone)
    gcp.instance_groups.append(instance_group)
    return instance_group


def create_health_check(gcp, name):
    config = {
        'name': name,
        'type': 'TCP',
        'tcpHealthCheck': {
            'portName': 'grpc'
        }
    }
    result = gcp.compute.healthChecks().insert(project=gcp.project,
                                               body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    gcp.health_check = GcpResource(config['name'], result['targetLink'])


def create_health_check_firewall_rule(gcp, name):
    config = {
        'name': name,
        'direction': 'INGRESS',
        'allowed': [{
            'IPProtocol': 'tcp'
        }],
        'sourceRanges': ['35.191.0.0/16', '130.211.0.0/22'],
        'targetTags': ['allow-health-checks'],
    }
    result = gcp.compute.firewalls().insert(project=gcp.project,
                                            body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    gcp.health_check_firewall_rule = GcpResource(config['name'],
                                                 result['targetLink'])


def add_backend_service(gcp, name):
    config = {
        'name': name,
        'loadBalancingScheme': 'INTERNAL_SELF_MANAGED',
        'healthChecks': [gcp.health_check.url],
        'portName': 'grpc',
        'protocol': 'HTTP2'
    }
    result = gcp.compute.backendServices().insert(project=gcp.project,
                                                  body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    backend_service = GcpResource(config['name'], result['targetLink'])
    gcp.backend_services.append(backend_service)
    return backend_service


def create_url_map(gcp, name, backend_service, host_name):
    config = {
        'name': name,
        'defaultService': backend_service.url,
        'pathMatchers': [{
            'name': _PATH_MATCHER_NAME,
            'defaultService': backend_service.url,
        }],
        'hostRules': [{
            'hosts': [host_name],
            'pathMatcher': _PATH_MATCHER_NAME
        }]
    }
    result = gcp.compute.urlMaps().insert(project=gcp.project,
                                          body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    gcp.url_map = GcpResource(config['name'], result['targetLink'])


def create_target_http_proxy(gcp, name):
    config = {
        'name': name,
        'url_map': gcp.url_map.url,
    }
    result = gcp.compute.targetHttpProxies().insert(project=gcp.project,
                                                    body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    gcp.target_http_proxy = GcpResource(config['name'], result['targetLink'])


def create_global_forwarding_rule(gcp, name, port):
    config = {
        'name': name,
        'loadBalancingScheme': 'INTERNAL_SELF_MANAGED',
        'portRange': str(port),
        'IPAddress': '0.0.0.0',
        'network': args.network,
        'target': gcp.target_http_proxy.url,
    }
    result = gcp.compute.globalForwardingRules().insert(project=gcp.project,
                                                        body=config).execute()
    wait_for_global_operation(gcp, result['name'])
    gcp.global_forwarding_rule = GcpResource(config['name'],
                                             result['targetLink'])


def delete_global_forwarding_rule(gcp):
    try:
        result = gcp.compute.globalForwardingRules().delete(
            project=gcp.project,
            forwardingRule=gcp.global_forwarding_rule.name).execute()
        wait_for_global_operation(gcp, result['name'])
    except googleapiclient.errors.HttpError as http_error:
        logger.info('Delete failed: %s', http_error)


def delete_target_http_proxy(gcp):
    try:
        result = gcp.compute.targetHttpProxies().delete(
            project=gcp.project,
            targetHttpProxy=gcp.target_http_proxy.name).execute()
        wait_for_global_operation(gcp, result['name'])
    except googleapiclient.errors.HttpError as http_error:
        logger.info('Delete failed: %s', http_error)


def delete_url_map(gcp):
    try:
        result = gcp.compute.urlMaps().delete(
            project=gcp.project, urlMap=gcp.url_map.name).execute()
        wait_for_global_operation(gcp, result['name'])
    except googleapiclient.errors.HttpError as http_error:
        logger.info('Delete failed: %s', http_error)


def delete_backend_services(gcp):
    for backend_service in gcp.backend_services:
        try:
            result = gcp.compute.backendServices().delete(
                project=gcp.project,
                backendService=gcp.backend_service.name).execute()
            wait_for_global_operation(gcp, result['name'])
        except googleapiclient.errors.HttpError as http_error:
            logger.info('Delete failed: %s', http_error)


def delete_firewall(gcp):
    try:
        result = gcp.compute.firewalls().delete(
            project=gcp.project,
            firewall=gcp.health_check_firewall_rule.name).execute()
        wait_for_global_operation(gcp, result['name'])
    except googleapiclient.errors.HttpError as http_error:
        logger.info('Delete failed: %s', http_error)


def delete_health_check(gcp):
    try:
        result = gcp.compute.healthChecks().delete(
            project=gcp.project, healthCheck=gcp.health_check.name).execute()
        wait_for_global_operation(gcp, result['name'])
    except googleapiclient.errors.HttpError as http_error:
        logger.info('Delete failed: %s', http_error)


def delete_instance_groups(gcp):
    for instance_group in gcp.instance_groups:
        try:
            result = gcp.compute.instanceGroupManagers().delete(
                project=gcp.project,
                zone=instance_group.zone,
                instanceGroupManager=instance_group.name).execute()
            wait_for_zone_operation(gcp,
                                    instance_group.zone,
                                    result['name'],
                                    timeout_sec=_WAIT_FOR_BACKEND_SEC)
        except googleapiclient.errors.HttpError as http_error:
            logger.info('Delete failed: %s', http_error)


def delete_instance_template(gcp):
    try:
        result = gcp.compute.instanceTemplates().delete(
            project=gcp.project,
            instanceTemplate=gcp.instance_template.name).execute()
        wait_for_global_operation(gcp, result['name'])
    except googleapiclient.errors.HttpError as http_error:
        logger.info('Delete failed: %s', http_error)


def patch_backend_instances(gcp,
                            backend_service,
                            instance_groups,
                            balancing_mode='UTILIZATION'):
    config = {
        'backends': [{
            'group': instance_group.url,
            'balancingMode': balancing_mode,
            'maxRate': 1 if balancing_mode == 'RATE' else None
        } for instance_group in instance_groups],
    }
    result = gcp.compute.backendServices().patch(
        project=gcp.project, backendService=backend_service.name,
        body=config).execute()
    wait_for_global_operation(gcp, result['name'])


def resize_instance_group(gcp, instance_group, new_size, timeout_sec=120):
    result = gcp.compute.instanceGroupManagers().resize(
        project=gcp.project,
        zone=instance_group.zone,
        instanceGroupManager=instance_group.name,
        size=new_size).execute()
    wait_for_zone_operation(gcp,
                            instance_group.zone,
                            result['name'],
                            timeout_sec=360)
    start_time = time.time()
    while True:
        current_size = len(get_instance_names(gcp, instance_group))
        if current_size == new_size:
            break
        if time.time() - start_time > timeout_sec:
            raise Exception('Failed to resize primary instance group')
        time.sleep(1)


def patch_url_map_backend_service(gcp, backend_service):
    config = {
        'defaultService':
            backend_service.url,
        'pathMatchers': [{
            'name': _PATH_MATCHER_NAME,
            'defaultService': backend_service.url,
        }]
    }
    result = gcp.compute.urlMaps().patch(project=gcp.project,
                                         urlMap=gcp.url_map.name,
                                         body=config).execute()
    wait_for_global_operation(gcp, result['name'])


def wait_for_global_operation(gcp,
                              operation,
                              timeout_sec=_WAIT_FOR_OPERATION_SEC):
    start_time = time.time()
    while time.time() - start_time <= timeout_sec:
        result = gcp.compute.globalOperations().get(
            project=gcp.project, operation=operation).execute()
        if result['status'] == 'DONE':
            if 'error' in result:
                raise Exception(result['error'])
            return
        time.sleep(1)
    raise Exception('Operation %s did not complete within %d', operation,
                    timeout_sec)


def wait_for_zone_operation(gcp,
                            zone,
                            operation,
                            timeout_sec=_WAIT_FOR_OPERATION_SEC):
    start_time = time.time()
    while time.time() - start_time <= timeout_sec:
        result = gcp.compute.zoneOperations().get(
            project=gcp.project, zone=zone, operation=operation).execute()
        if result['status'] == 'DONE':
            if 'error' in result:
                raise Exception(result['error'])
            return
        time.sleep(1)
    raise Exception('Operation %s did not complete within %d', operation,
                    timeout_sec)


def wait_for_healthy_backends(gcp,
                              backend_service,
                              instance_group,
                              timeout_sec=_WAIT_FOR_BACKEND_SEC):
    start_time = time.time()
    config = {'group': instance_group.url}
    while time.time() - start_time <= timeout_sec:
        result = gcp.compute.backendServices().getHealth(
            project=gcp.project,
            backendService=backend_service.name,
            body=config).execute()
        if 'healthStatus' in result:
            healthy = True
            for instance in result['healthStatus']:
                if instance['healthState'] != 'HEALTHY':
                    healthy = False
                    break
            if healthy:
                return
        time.sleep(1)
    raise Exception('Not all backends became healthy within %d seconds: %s' %
                    (timeout_sec, result))


def get_instance_names(gcp, instance_group):
    instance_names = []
    result = gcp.compute.instanceGroups().listInstances(
        project=gcp.project,
        zone=instance_group.zone,
        instanceGroup=instance_group.name,
        body={
            'instanceState': 'ALL'
        }).execute()
    if 'items' not in result:
        return []
    for item in result['items']:
        # listInstances() returns the full URL of the instance, which ends with
        # the instance name. compute.instances().get() requires using the
        # instance name (not the full URL) to look up instance details, so we
        # just extract the name manually.
        instance_name = item['instance'].split('/')[-1]
        instance_names.append(instance_name)
    return instance_names


def start_xds_client(cmd, service_port):
    bootstrap_path = None
    with tempfile.NamedTemporaryFile(delete=False) as bootstrap_file:
        bootstrap_file.write(
            _BOOTSTRAP_TEMPLATE.format(
                node_id=socket.gethostname()).encode('utf-8'))
        bootstrap_path = bootstrap_file.name

    client_process = subprocess.Popen(shlex.split(cmd),
                                      env=dict(
                                          os.environ,
                                          GRPC_XDS_BOOTSTRAP=bootstrap_path))
    return client_process


class InstanceGroup(object):

    def __init__(self, name, url, zone):
        self.name = name
        self.url = url
        self.zone = zone


class GcpResource(object):

    def __init__(self, name, url):
        self.name = name
        self.url = url


class GcpState(object):

    def __init__(self, compute, project):
        self.compute = compute
        self.project = project
        self.health_check = None
        self.health_check_firewall_rule = None
        self.backend_services = []
        self.url_map = None
        self.target_http_proxy = None
        self.global_forwarding_rule = None
        self.service_port = None
        self.instance_template = None
        self.instance_groups = []

    def clean_up(self):
        if self.global_forwarding_rule:
            delete_global_forwarding_rule(self)
        if self.target_http_proxy:
            delete_target_http_proxy(self)
        if self.url_map:
            delete_url_map(self)
        delete_backend_services(self)
        if self.health_check_firewall_rule:
            delete_firewall(self)
        if self.health_check:
            delete_health_check(self)
        delete_instance_groups(self)
        if self.instance_template:
            delete_instance_template(self)


if args.compute_discovery_document:
    with open(args.compute_discovery_document, 'r') as discovery_doc:
        compute = googleapiclient.discovery.build_from_document(
            discovery_doc.read())
else:
    compute = googleapiclient.discovery.build('compute', 'v1')

client_process = None

try:
    gcp = GcpState(compute, args.project_id)
    health_check_name = _BASE_HEALTH_CHECK_NAME + args.gcp_suffix
    firewall_name = _BASE_FIREWALL_RULE_NAME + args.gcp_suffix
    backend_service_name = _BASE_BACKEND_SERVICE_NAME + args.gcp_suffix
    alternate_backend_service_name = _BASE_BACKEND_SERVICE_NAME + '-alternate' + args.gcp_suffix
    url_map_name = _BASE_URL_MAP_NAME + args.gcp_suffix
    service_host_name = _BASE_SERVICE_HOST + args.gcp_suffix
    target_http_proxy_name = _BASE_TARGET_PROXY_NAME + args.gcp_suffix
    forwarding_rule_name = _BASE_FORWARDING_RULE_NAME + args.gcp_suffix
    template_name = _BASE_TARGET_PROXY_NAME + args.gcp_suffix
    instance_group_name = _BASE_INSTANCE_GROUP_NAME + args.gcp_suffix
    same_zone_instance_group_name = _BASE_INSTANCE_GROUP_NAME + '-same-zone' + args.gcp_suffix
    secondary_zone_instance_group_name = _BASE_INSTANCE_GROUP_NAME + '-secondary-zone' + args.gcp_suffix
    try:
        create_health_check(gcp, health_check_name)
        create_health_check_firewall_rule(gcp, firewall_name)
        backend_service = add_backend_service(gcp, backend_service_name)
        alternate_backend_service = add_backend_service(
            gcp, alternate_backend_service_name)
        create_url_map(gcp, url_map_name, backend_service, service_host_name)
        create_target_http_proxy(gcp, target_http_proxy_name)
        potential_service_ports = list(args.service_port_range)
        random.shuffle(potential_service_ports)
        for port in potential_service_ports:
            try:
                create_global_forwarding_rule(gcp, forwarding_rule_name, port)
                gcp.service_port = port
                break
            except googleapiclient.errors.HttpError as http_error:
                logger.warning(
                    'Got error %s when attempting to create forwarding rule to '
                    'port %d. Retrying with another port.' % (http_error, port))
        if not gcp.service_port:
            raise Exception('Failed to pick a service port in the range %s' %
                            args.service_port_range)
        create_instance_template(gcp, template_name, args.network,
                                 args.source_image)
        instance_group = add_instance_group(gcp, args.zone, instance_group_name,
                                            _INSTANCE_GROUP_SIZE)
        patch_backend_instances(gcp, backend_service, [instance_group])
        same_zone_instance_group = add_instance_group(
            gcp, args.zone, same_zone_instance_group_name, _INSTANCE_GROUP_SIZE)
        secondary_zone_instance_group = add_instance_group(
            gcp, args.secondary_zone, secondary_zone_instance_group_name,
            _INSTANCE_GROUP_SIZE)
    except googleapiclient.errors.HttpError as http_error:
        if args.tolerate_gcp_errors:
            logger.warning(
                'Failed to set up backends: %s. Attempting to continue since '
                '--tolerate_gcp_errors=true', http_error)
            if not gcp.instance_template:
                result = compute.instanceTemplates().get(
                    project=args.project_id,
                    instanceTemplate=template_name).execute()
                gcp.instance_template = GcpResource(template_name,
                                                    result['selfLink'])
            if not gcp.backend_services:
                result = compute.backendServices().get(
                    project=args.project_id,
                    backendService=backend_service_name).execute()
                backend_service = GcpResource(backend_service_name,
                                              result['selfLink'])
                gcp.backend_services.append(backend_service)
                result = compute.backendServices().get(
                    project=args.project_id,
                    backendService=alternate_backend_service_name).execute()
                alternate_backend_service = GcpResource(
                    alternate_backend_service_name, result['selfLink'])
                gcp.backend_services.append(alternate_backend_service)
            if not gcp.instance_groups:
                result = compute.instanceGroups().get(
                    project=args.project_id,
                    zone=args.zone,
                    instanceGroup=instance_group_name).execute()
                instance_group = InstanceGroup(instance_group_name,
                                               result['selfLink'], args.zone)
                gcp.instance_groups.append(instance_group)
                result = compute.instanceGroups().get(
                    project=args.project_id,
                    zone=args.zone,
                    instanceGroup=same_zone_instance_group_name).execute()
                same_zone_instance_group = InstanceGroup(
                    same_zone_instance_group_name, result['selfLink'],
                    args.zone)
                gcp.instance_groups.append(same_zone_instance_group)
                result = compute.instanceGroups().get(
                    project=args.project_id,
                    zone=args.secondary_zone,
                    instanceGroup=secondary_zone_instance_group_name).execute()
                secondary_zone_instance_group = InstanceGroup(
                    secondary_zone_instance_group_name, result['selfLink'],
                    args.secondary_zone)
                gcp.instance_groups.append(secondary_zone_instance_group)
            if not gcp.health_check:
                result = compute.healthChecks().get(
                    project=args.project_id,
                    healthCheck=health_check_name).execute()
                gcp.health_check = GcpResource(health_check_name,
                                               result['selfLink'])
            if not gcp.url_map:
                result = compute.urlMaps().get(project=args.project_id,
                                               urlMap=url_map_name).execute()
                gcp.url_map = GcpResource(url_map_name, result['selfLink'])
            if not gcp.service_port:
                gcp.service_port = args.service_port_range[0]
                logger.warning('Using arbitrary service port in range: %d' %
                               gcp.service_port)
        else:
            raise http_error

    wait_for_healthy_backends(gcp, backend_service, instance_group)

    cmd = args.client_cmd.format(service_host=service_host_name,
                                 service_port=gcp.service_port,
                                 stats_port=args.stats_port,
                                 qps=args.qps)
    client_process = start_xds_client(cmd, gcp.service_port)

    if args.test_case == 'all':
        test_backends_restart(gcp, backend_service, instance_group)
        test_change_backend_service(gcp, backend_service, instance_group,
                                    alternate_backend_service,
                                    same_zone_instance_group)
        test_new_instance_group_receives_traffic(gcp, backend_service,
                                                 instance_group,
                                                 same_zone_instance_group)
        test_ping_pong(gcp, backend_service, instance_group)
        test_remove_instance_group(gcp, backend_service, instance_group,
                                   same_zone_instance_group)
        test_round_robin(gcp, backend_service, instance_group)
        test_secondary_locality_gets_no_requests_on_partial_primary_failure(
            gcp, backend_service, instance_group, secondary_zone_instance_group)
        test_secondary_locality_gets_requests_on_primary_failure(
            gcp, backend_service, instance_group, secondary_zone_instance_group)
    elif args.test_case == 'backends_restart':
        test_backends_restart(gcp, backend_service, instance_group)
    elif args.test_case == 'change_backend_service':
        test_change_backend_service(gcp, backend_service, instance_group,
                                    alternate_backend_service,
                                    same_zone_instance_group)
    elif args.test_case == 'new_instance_group_receives_traffic':
        test_new_instance_group_receives_traffic(gcp, backend_service,
                                                 instance_group,
                                                 same_zone_instance_group)
    elif args.test_case == 'ping_pong':
        test_ping_pong(gcp, backend_service, instance_group)
    elif args.test_case == 'remove_instance_group':
        test_remove_instance_group(gcp, backend_service, instance_group,
                                   same_zone_instance_group)
    elif args.test_case == 'round_robin':
        test_round_robin(gcp, backend_service, instance_group)
    elif args.test_case == 'secondary_locality_gets_no_requests_on_partial_primary_failure':
        test_secondary_locality_gets_no_requests_on_partial_primary_failure(
            gcp, backend_service, instance_group, secondary_zone_instance_group)
    elif args.test_case == 'secondary_locality_gets_requests_on_primary_failure':
        test_secondary_locality_gets_requests_on_primary_failure(
            gcp, backend_service, instance_group, secondary_zone_instance_group)
    else:
        logger.error('Unknown test case: %s', args.test_case)
        sys.exit(1)
finally:
    if client_process:
        client_process.terminate()
    if not args.keep_gcp_resources:
        logger.info('Cleaning up GCP resources. This may take some time.')
        gcp.clean_up()
