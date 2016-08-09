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

"""A setup module for the GRPC Python package."""

from distutils import extension as _extension
from distutils import util
import os
import os.path
import pkg_resources
import platform
import re
import shlex
import shutil
import sys
import sysconfig

import setuptools
from setuptools.command import egg_info

# Redirect the manifest template from MANIFEST.in to PYTHON-MANIFEST.in.
egg_info.manifest_maker.template = 'PYTHON-MANIFEST.in'

PY3 = sys.version_info.major == 3
PYTHON_STEM = os.path.join('src', 'python', 'grpcio')
CORE_INCLUDE = ('include', '.',)
BORINGSSL_INCLUDE = (os.path.join('third_party', 'boringssl', 'include'),)
ZLIB_INCLUDE = (os.path.join('third_party', 'zlib'),)

# Ensure we're in the proper directory whether or not we're being used by pip.
os.chdir(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.abspath(PYTHON_STEM))

# Break import-style to ensure we can actually find our in-repo dependencies.
import _unixccompiler_patch
import commands
import grpc_core_dependencies
import grpc_version

if 'win32' in sys.platform:
  _unixccompiler_patch.monkeypatch_unix_compiler()


LICENSE = '3-clause BSD'

# Environment variable to determine whether or not the Cython extension should
# *use* Cython or use the generated C files. Note that this requires the C files
# to have been generated by building first *with* Cython support. Even if this
# is set to false, if the script detects that the generated `.c` file isn't
# present, then it will still attempt to use Cython.
BUILD_WITH_CYTHON = os.environ.get('GRPC_PYTHON_BUILD_WITH_CYTHON', False)

# Environment variable to determine whether or not to enable coverage analysis
# in Cython modules.
ENABLE_CYTHON_TRACING = os.environ.get(
    'GRPC_PYTHON_ENABLE_CYTHON_TRACING', False)

# There are some situations (like on Windows) where CC, CFLAGS, and LDFLAGS are
# entirely ignored/dropped/forgotten by distutils and its Cygwin/MinGW support.
# We use these environment variables to thus get around that without locking
# ourselves in w.r.t. the multitude of operating systems this ought to build on.
# We can also use these variables as a way to inject environment-specific
# compiler/linker flags. We assume GCC-like compilers and/or MinGW as a
# reasonable default.
EXTRA_ENV_COMPILE_ARGS = os.environ.get('GRPC_PYTHON_CFLAGS', None)
EXTRA_ENV_LINK_ARGS = os.environ.get('GRPC_PYTHON_LDFLAGS', None)
if EXTRA_ENV_COMPILE_ARGS is None:
  EXTRA_ENV_COMPILE_ARGS = '-fno-wrapv'
  if 'win32' in sys.platform:
    # We use define flags here and don't directly add to DEFINE_MACROS below to
    # ensure that the expert user/builder has a way of turning it off (via the
    # envvars) without adding yet more GRPC-specific envvars.
    # See https://sourceforge.net/p/mingw-w64/bugs/363/
    if '32' in platform.architecture()[0]:
      EXTRA_ENV_COMPILE_ARGS += ' -D_ftime=_ftime32 -D_timeb=__timeb32 -D_ftime_s=_ftime32_s'
    else:
      EXTRA_ENV_COMPILE_ARGS += ' -D_ftime=_ftime64 -D_timeb=__timeb64'
  elif "linux" in sys.platform or "darwin" in sys.platform:
    EXTRA_ENV_COMPILE_ARGS += ' -fvisibility=hidden'
if EXTRA_ENV_LINK_ARGS is None:
  EXTRA_ENV_LINK_ARGS = '-lpthread'
  if 'win32' in sys.platform:
    # TODO(atash) check if this is actually safe to just import and call on
    # non-Windows (to avoid breaking import style)
    from distutils.cygwinccompiler import get_msvcr
    msvcr = get_msvcr()[0]
    # TODO(atash) sift through the GCC specs to see if libstdc++ can have any
    # influence on the linkage outcome on MinGW for non-C++ programs.
    EXTRA_ENV_LINK_ARGS += (
        ' -static-libgcc -static-libstdc++ -mcrtdll={msvcr} '
        '-static'.format(msvcr=msvcr))
  elif "linux" in sys.platform:
    EXTRA_ENV_LINK_ARGS += ' -Wl,-wrap,memcpy'
EXTRA_COMPILE_ARGS = shlex.split(EXTRA_ENV_COMPILE_ARGS)
EXTRA_LINK_ARGS = shlex.split(EXTRA_ENV_LINK_ARGS)

CYTHON_EXTENSION_PACKAGE_NAMES = ()

CYTHON_EXTENSION_MODULE_NAMES = ('grpc._cython.cygrpc',)

CYTHON_HELPER_C_FILES = ()

CORE_C_FILES = tuple(grpc_core_dependencies.CORE_SOURCE_FILES)

EXTENSION_INCLUDE_DIRECTORIES = (
    (PYTHON_STEM,) + CORE_INCLUDE + BORINGSSL_INCLUDE + ZLIB_INCLUDE)

EXTENSION_LIBRARIES = ()
if "linux" in sys.platform:
  EXTENSION_LIBRARIES += ('rt',)
if not "win32" in sys.platform:
  EXTENSION_LIBRARIES += ('m',)
if "win32" in sys.platform:
  EXTENSION_LIBRARIES += ('ws2_32',)

DEFINE_MACROS = (
    ('OPENSSL_NO_ASM', 1), ('_WIN32_WINNT', 0x600),
    ('GPR_BACKWARDS_COMPATIBILITY_MODE', 1),)
if "win32" in sys.platform:
  DEFINE_MACROS += (('OPENSSL_WINDOWS', 1), ('WIN32_LEAN_AND_MEAN', 1),)
  if '64bit' in platform.architecture()[0]:
    DEFINE_MACROS += (('MS_WIN64', 1),)

LDFLAGS = tuple(EXTRA_LINK_ARGS)
CFLAGS = tuple(EXTRA_COMPILE_ARGS)
if "linux" in sys.platform or "darwin" in sys.platform:
  pymodinit_type = 'PyObject*' if PY3 else 'void'
  pymodinit = '__attribute__((visibility ("default"))) {}'.format(pymodinit_type)
  DEFINE_MACROS += (('PyMODINIT_FUNC', pymodinit),)


# By default, Python3 distutils enforces compatibility of
# c plugins (.so files) with the OSX version Python3 was built with.
# For Python3.4, this is OSX 10.6, but we need Thread Local Support (__thread)
if 'darwin' in sys.platform and PY3:
  mac_target = sysconfig.get_config_var('MACOSX_DEPLOYMENT_TARGET')
  if mac_target and (pkg_resources.parse_version(mac_target) <
                     pkg_resources.parse_version('10.7.0')):
    os.environ['MACOSX_DEPLOYMENT_TARGET'] = '10.7'
    os.environ['_PYTHON_HOST_PLATFORM'] = re.sub(
        r'macosx-[0-9]+\.[0-9]+-(.+)',
        r'macosx-10.7-\1',
        util.get_platform())

def cython_extensions_and_necessity():
  cython_module_files = [os.path.join(PYTHON_STEM,
                               name.replace('.', '/') + '.pyx')
                  for name in CYTHON_EXTENSION_MODULE_NAMES]
  extensions = [
      _extension.Extension(
          name=module_name,
          sources=[module_file] + list(CYTHON_HELPER_C_FILES) + list(CORE_C_FILES),
          include_dirs=list(EXTENSION_INCLUDE_DIRECTORIES),
          libraries=list(EXTENSION_LIBRARIES),
          define_macros=list(DEFINE_MACROS),
          extra_compile_args=list(CFLAGS),
          extra_link_args=list(LDFLAGS),
      ) for (module_name, module_file) in zip(list(CYTHON_EXTENSION_MODULE_NAMES), cython_module_files)
  ]
  need_cython = BUILD_WITH_CYTHON
  if not BUILD_WITH_CYTHON:
    need_cython = need_cython or not commands.check_and_update_cythonization(extensions)
  return commands.try_cythonize(extensions, linetracing=ENABLE_CYTHON_TRACING, mandatory=BUILD_WITH_CYTHON), need_cython

CYTHON_EXTENSION_MODULES, need_cython = cython_extensions_and_necessity()

PACKAGE_DIRECTORIES = {
    '': PYTHON_STEM,
}

INSTALL_REQUIRES = (
    'six>=1.5.2',
    'enum34>=1.0.4',
    'futures>=2.2.0',
    # TODO(atash): eventually split the grpcio package into a metapackage
    # depending on protobuf and the runtime component (independent of protobuf)
    'protobuf>=3.0.0a3',
)

SETUP_REQUIRES = INSTALL_REQUIRES + (
    'sphinx>=1.3',
    'sphinx_rtd_theme>=0.1.8',
    'six>=1.10',
)
if BUILD_WITH_CYTHON:
  sys.stderr.write(
    "You requested a Cython build via GRPC_PYTHON_BUILD_WITH_CYTHON, "
    "but do not have Cython installed. We won't stop you from using "
    "other commands, but the extension files will fail to build.\n")
elif need_cython:
  sys.stderr.write(
      'We could not find Cython. Setup may take 10-20 minutes.\n')
  SETUP_REQUIRES += ('cython>=0.23',)

COMMAND_CLASS = {
    'doc': commands.SphinxDocumentation,
    'build_project_metadata': commands.BuildProjectMetadata,
    'build_py': commands.BuildPy,
    'build_ext': commands.BuildExt,
    'gather': commands.Gather,
}

# Ensure that package data is copied over before any commands have been run:
credentials_dir = os.path.join(PYTHON_STEM, 'grpc', '_cython', '_credentials')
try:
  os.mkdir(credentials_dir)
except OSError:
  pass
shutil.copyfile(os.path.join('etc', 'roots.pem'),
                os.path.join(credentials_dir, 'roots.pem'))

PACKAGE_DATA = {
    # Binaries that may or may not be present in the final installation, but are
    # mentioned here for completeness.
    'grpc._cython': [
        '_credentials/roots.pem',
        '_windows/grpc_c.32.python',
        '_windows/grpc_c.64.python',
    ],
}
PACKAGES = setuptools.find_packages(PYTHON_STEM)

setuptools.setup(
  name='grpcio',
  version=grpc_version.VERSION,
  license=LICENSE,
  ext_modules=CYTHON_EXTENSION_MODULES,
  packages=list(PACKAGES),
  package_dir=PACKAGE_DIRECTORIES,
  package_data=PACKAGE_DATA,
  install_requires=INSTALL_REQUIRES,
  setup_requires=SETUP_REQUIRES,
  cmdclass=COMMAND_CLASS,
)
