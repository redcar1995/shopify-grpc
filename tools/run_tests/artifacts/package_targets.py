#!/usr/bin/env python
# Copyright 2016 gRPC authors.
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

"""Definition of targets to build distribution packages."""

import os.path
import sys

sys.path.insert(0, os.path.abspath('..'))
import python_utils.jobset as jobset


def create_docker_jobspec(name, dockerfile_dir, shell_command, environ={},
                   flake_retries=0, timeout_retries=0):
  """Creates jobspec for a task running under docker."""
  environ = environ.copy()
  environ['RUN_COMMAND'] = shell_command

  docker_args=[]
  for k,v in environ.items():
    docker_args += ['-e', '%s=%s' % (k, v)]
  docker_env = {'DOCKERFILE_DIR': dockerfile_dir,
                'DOCKER_RUN_SCRIPT': 'tools/run_tests/dockerize/docker_run.sh',
                'OUTPUT_DIR': 'artifacts'}
  jobspec = jobset.JobSpec(
          cmdline=['tools/run_tests/dockerize/build_and_run_docker.sh'] + docker_args,
          environ=docker_env,
          shortname='build_package.%s' % (name),
          timeout_seconds=30*60,
          flake_retries=flake_retries,
          timeout_retries=timeout_retries)
  return jobspec

def create_jobspec(name, cmdline, environ=None, cwd=None, shell=False,
                   flake_retries=0, timeout_retries=0):
  """Creates jobspec."""
  jobspec = jobset.JobSpec(
          cmdline=cmdline,
          environ=environ,
          cwd=cwd,
          shortname='build_package.%s' % (name),
          timeout_seconds=10*60,
          flake_retries=flake_retries,
          timeout_retries=timeout_retries,
          shell=shell)
  return jobspec


class CSharpPackage:
  """Builds C# nuget packages."""

  def __init__(self, linux=False):
    self.linux = linux
    self.labels = ['package', 'csharp']
    if linux:
      self.name = 'csharp_package_dotnetcli_linux'
      self.labels += ['linux']
    else:
      self.name = 'csharp_package_dotnetcli_windows'
      self.labels += ['windows']

  def pre_build_jobspecs(self):
    return []

  def build_jobspec(self):
    if self.linux:
      return create_docker_jobspec(
          self.name,
          'tools/dockerfile/test/csharp_coreclr_x64',
          'src/csharp/build_packages_dotnetcli.sh')
    else:
      return create_jobspec(self.name,
                            ['build_packages_dotnetcli.bat'],
                            cwd='src\\csharp',
                            shell=True)

  def __str__(self):
    return self.name


class NodePackage:
  """Builds Node NPM package and collects precompiled binaries"""

  def __init__(self):
    self.name = 'node_package'
    self.labels = ['package', 'node', 'linux']

  def pre_build_jobspecs(self):
    return []

  def build_jobspec(self):
    return create_docker_jobspec(
        self.name,
        'tools/dockerfile/grpc_artifact_linux_x64',
        'tools/run_tests/artifacts/build_package_node.sh')


class RubyPackage:
  """Collects ruby gems created in the artifact phase"""

  def __init__(self):
    self.name = 'ruby_package'
    self.labels = ['package', 'ruby', 'linux']

  def pre_build_jobspecs(self):
    return []

  def build_jobspec(self):
    return create_docker_jobspec(
        self.name,
        'tools/dockerfile/grpc_artifact_linux_x64',
        'tools/run_tests/artifacts/build_package_ruby.sh')


class PythonPackage:
  """Collects python eggs and wheels created in the artifact phase"""

  def __init__(self):
    self.name = 'python_package'
    self.labels = ['package', 'python', 'linux']

  def pre_build_jobspecs(self):
    return []

  def build_jobspec(self):
    return create_docker_jobspec(
        self.name,
        'tools/dockerfile/grpc_artifact_linux_x64',
        'tools/run_tests/artifacts/build_package_python.sh')


class PHPPackage:
  """Copy PHP PECL package artifact"""

  def __init__(self):
    self.name = 'php_package'
    self.labels = ['package', 'php', 'linux']

  def pre_build_jobspecs(self):
    return []

  def build_jobspec(self):
    return create_docker_jobspec(
        self.name,
        'tools/dockerfile/grpc_artifact_linux_x64',
        'tools/run_tests/artifacts/build_package_php.sh')


def targets():
  """Gets list of supported targets"""
  return [CSharpPackage(),
          CSharpPackage(linux=True),
          NodePackage(),
          RubyPackage(),
          PythonPackage(),
          PHPPackage()]
