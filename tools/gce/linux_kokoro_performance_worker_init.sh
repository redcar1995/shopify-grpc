#!/bin/bash
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

# Initializes a fresh GCE VM to become a Kokoro Linux performance worker.
# You shouldn't run this script on your own,
# use create_linux_kokoro_performance_worker.sh instead.

set -ex

sudo apt-get update

# Install Java 8 JDK (to build gRPC Java)
sudo apt-get install -y openjdk-8-jdk
sudo apt-get install -y unzip lsof

sudo apt-get install -y \
  autoconf \
  autotools-dev \
  build-essential \
  bzip2 \
  ccache \
  curl \
  gcc \
  gcc-multilib \
  git \
  gyp \
  lcov \
  libc6 \
  libc6-dbg \
  libc6-dev \
  libcurl4-openssl-dev \
  libgtest-dev \
  libreadline-dev \
  libssl-dev \
  libtool \
  make \
  strace \
  pypy \
  python-dev \
  python-pip \
  python-setuptools \
  python-yaml \
  python3-dev \
  python3-pip \
  python3-setuptools \
  python3-yaml \
  telnet \
  unzip \
  wget \
  zip \
  zlib1g-dev

# perftools
sudo apt-get install -y google-perftools libgoogle-perftools-dev

# netperf
sudo apt-get install -y netperf

# required to run kokoro_log_reader.py
sudo apt-get install -y python-psutil python3-psutil

# C++ dependencies
sudo apt-get install -y libgflags-dev libgtest-dev libc++-dev clang

# Python dependencies
sudo pip install --upgrade pip==10.0.1
sudo pip install tabulate
sudo pip install google-api-python-client
sudo pip install virtualenv

# TODO(jtattermusch): revisit python installation
# Building gRPC Python depends on python3.4 being installed, but python3.4
# is not available on Ubuntu 16.10, so install from source
curl -O https://www.python.org/ftp/python/3.4.6/Python-3.4.6.tgz
tar xzvf Python-3.4.6.tgz
(
cd Python-3.4.6 || exit
./configure --enable-shared --prefix=/usr/local LDFLAGS="-Wl,--rpath=/usr/local/lib"
sudo make altinstall
)
rm Python-3.4.6.tgz

curl -O https://bootstrap.pypa.io/get-pip.py
sudo pypy get-pip.py
sudo pypy -m pip install tabulate
# TODO(jtattermusch): make it clear this already uses pypy's pip
sudo pip install google-api-python-client

# Node dependencies (nvm has to be installed under user kbuilder)
touch .profile
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.25.4/install.sh | bash
# silence shellcheck as it cannot follow the following `source` path statically:
# shellcheck disable=SC1090
source ~/.nvm/nvm.sh
nvm install 0.12 && npm config set cache /tmp/npm-cache
nvm install 4 && npm config set cache /tmp/npm-cache
nvm install 5 && npm config set cache /tmp/npm-cache
nvm alias default 4

# C# dependencies
sudo apt-get install -y cmake

# C# mono dependencies (http://www.mono-project.com/docs/getting-started/install/linux/#debian-ubuntu-and-derivatives)
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb https://download.mono-project.com/repo/ubuntu stable-bionic main" | sudo tee /etc/apt/sources.list.d/mono-official-stable.list
sudo apt-get update
sudo apt-get install -y mono-devel

# C# .NET Core dependencies (https://www.microsoft.com/net/download)
wget -q https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb

sudo apt-get install -y apt-transport-https
sudo apt-get update
sudo apt-get install -y dotnet-sdk-2.1

# Ruby dependencies
gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
curl -sSL https://get.rvm.io | bash -s stable --ruby
# silence shellcheck as it cannot follow the following `source` path statically:
# shellcheck disable=SC1090
source ~/.rvm/scripts/rvm

git clone https://github.com/rbenv/rbenv.git ~/.rbenv
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(rbenv init -)"

git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
export PATH="$HOME/.rbenv/plugins/ruby-build/bin:$PATH"

rbenv install 2.4.0
rbenv global 2.4.0
ruby -v

# Install bundler (prerequisite for gRPC Ruby)
gem install bundler

# PHP dependencies
sudo apt-get install -y php php-dev phpunit php-pear unzip zlib1g-dev
curl -sS https://getcomposer.org/installer | php
sudo mv composer.phar /usr/local/bin/composer

# Java dependencies - nothing as we already have Java JDK 8

# Go dependencies
# Currently, the golang package available via apt-get doesn't have the latest go.
# Significant performance improvements with grpc-go have been observed after
# upgrading from go 1.5 to a later version, so a later go version is preferred.
# Following go install instructions from https://golang.org/doc/install
GO_VERSION=1.10
OS=linux
ARCH=amd64
curl -O https://storage.googleapis.com/golang/go${GO_VERSION}.${OS}-${ARCH}.tar.gz
sudo tar -C /usr/local -xzf go$GO_VERSION.$OS-$ARCH.tar.gz
# Put go on the PATH, keep the usual installation dir
sudo ln -s /usr/local/go/bin/go /usr/bin/go
rm go$GO_VERSION.$OS-$ARCH.tar.gz

# Install perf, to profile benchmarks. (need to get the right linux-tools-<> for kernel version)
sudo apt-get install -y linux-tools-common linux-tools-generic "linux-tools-$(uname -r)"
# see http://unix.stackexchange.com/questions/14227/do-i-need-root-admin-permissions-to-run-userspace-perf-tool-perf-events-ar
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid
# see http://stackoverflow.com/questions/21284906/perf-couldnt-record-kernel-reference-relocation-symbol
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict

# qps workers under perf appear to need a lot of mmap pages under certain scenarios and perf args in
# order to not lose perf events or time out
echo 4096 | sudo tee /proc/sys/kernel/perf_event_mlock_kb

# Fetch scripts to generate flame graphs from perf data collected
# on benchmarks
git clone -v https://github.com/brendangregg/FlameGraph ~/FlameGraph

# Install scipy and numpy for benchmarking scripts
sudo apt-get install -y python-scipy python-numpy

# Install docker
curl -sSL https://get.docker.com/ | sh
# Enable kbuilder to use docker without sudo:
sudo usermod -aG docker kbuilder

# Add pubkey of Kokoro driver VM to allow SSH
# silence false-positive shellcheck warning ("< redirect does not affect sudo")
# shellcheck disable=SC2024
sudo tee --append ~kbuilder/.ssh/authorized_keys < kokoro_performance.pub

# Kokoro requires /tmpfs/READY file to exist the directory and file itself should
# be owned by kbuilder.
sudo mkdir /tmpfs
sudo chown kbuilder /tmpfs
touch /tmpfs/READY

# Restart for VM to pick up kernel update
echo 'Successfully initialized the linux worker, going for reboot in 10 seconds'
sleep 10
sudo reboot
