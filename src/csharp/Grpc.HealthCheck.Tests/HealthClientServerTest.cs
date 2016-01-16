﻿#region Copyright notice and license
// Copyright 2015-2016, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#endregion

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grpc.Core;
using Grpc.Health.V1Alpha;
using NUnit.Framework;

namespace Grpc.HealthCheck.Tests
{
    /// <summary>
    /// Health client talks to health server.
    /// </summary>
    public class HealthClientServerTest
    {
        const string Host = "localhost";
        Server server;
        Channel channel;
        Grpc.Health.V1Alpha.Health.IHealthClient client;
        Grpc.HealthCheck.HealthServiceImpl serviceImpl;

        [TestFixtureSetUp]
        public void Init()
        {
            serviceImpl = new HealthServiceImpl();

            server = new Server
            {
                Services = { Grpc.Health.V1Alpha.Health.BindService(serviceImpl) },
                Ports = { { Host, ServerPort.PickUnused, ServerCredentials.Insecure } }
            };
            server.Start();
            channel = new Channel(Host, server.Ports.Single().BoundPort, ChannelCredentials.Insecure);

            client = Grpc.Health.V1Alpha.Health.NewClient(channel);
        }

        [TestFixtureTearDown]
        public void Cleanup()
        {
            channel.ShutdownAsync().Wait();

            server.ShutdownAsync().Wait();
        }

        [Test]
        public void ServiceIsRunning()
        {
            serviceImpl.SetStatus("", "", HealthCheckResponse.Types.ServingStatus.SERVING);

            var response = client.Check(new HealthCheckRequest { Host = "", Service = "" });
            Assert.AreEqual(HealthCheckResponse.Types.ServingStatus.SERVING, response.Status);
        }

        [Test]
        public void ServiceDoesntExist()
        {
            Assert.Throws(Is.TypeOf(typeof(RpcException)).And.Property("Status").Property("StatusCode").EqualTo(StatusCode.NotFound), () => client.Check(new HealthCheckRequest { Host = "", Service = "nonexistent.service" }));
        }

        // TODO(jtattermusch): add test with timeout once timeouts are supported
    }
}
