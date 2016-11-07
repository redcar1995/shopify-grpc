// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: src/proto/grpc/testing/metrics.proto
// Original file comments:
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
//
// Contains the definitions for a metrics service and the type of metrics
// exposed by the service.
//
// Currently, 'Gauge' (i.e a metric that represents the measured value of
// something at an instant of time) is the only metric type supported by the
// service.
#region Designer generated code

using System;
using System.Threading;
using System.Threading.Tasks;
using Grpc.Core;

namespace Grpc.Testing {
  public static partial class MetricsService
  {
    static readonly string __ServiceName = "grpc.testing.MetricsService";

    static readonly Marshaller<global::Grpc.Testing.EmptyMessage> __Marshaller_EmptyMessage = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.EmptyMessage.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.GaugeResponse> __Marshaller_GaugeResponse = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.GaugeResponse.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.GaugeRequest> __Marshaller_GaugeRequest = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.GaugeRequest.Parser.ParseFrom);

    static readonly Method<global::Grpc.Testing.EmptyMessage, global::Grpc.Testing.GaugeResponse> __Method_GetAllGauges = new Method<global::Grpc.Testing.EmptyMessage, global::Grpc.Testing.GaugeResponse>(
        MethodType.ServerStreaming,
        __ServiceName,
        "GetAllGauges",
        __Marshaller_EmptyMessage,
        __Marshaller_GaugeResponse);

    static readonly Method<global::Grpc.Testing.GaugeRequest, global::Grpc.Testing.GaugeResponse> __Method_GetGauge = new Method<global::Grpc.Testing.GaugeRequest, global::Grpc.Testing.GaugeResponse>(
        MethodType.Unary,
        __ServiceName,
        "GetGauge",
        __Marshaller_GaugeRequest,
        __Marshaller_GaugeResponse);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::Grpc.Testing.MetricsReflection.Descriptor.Services[0]; }
    }

    /// <summary>Base class for server-side implementations of MetricsService</summary>
    public abstract partial class MetricsServiceBase
    {
      /// <summary>
      ///  Returns the values of all the gauges that are currently being maintained by
      ///  the service
      /// </summary>
      public virtual global::System.Threading.Tasks.Task GetAllGauges(global::Grpc.Testing.EmptyMessage request, IServerStreamWriter<global::Grpc.Testing.GaugeResponse> responseStream, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      /// <summary>
      ///  Returns the value of one gauge
      /// </summary>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.GaugeResponse> GetGauge(global::Grpc.Testing.GaugeRequest request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for MetricsService</summary>
    public partial class MetricsServiceClient : ClientBase<MetricsServiceClient>
    {
      /// <summary>Creates a new client for MetricsService</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public MetricsServiceClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for MetricsService that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public MetricsServiceClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected MetricsServiceClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected MetricsServiceClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      /// <summary>
      ///  Returns the values of all the gauges that are currently being maintained by
      ///  the service
      /// </summary>
      public virtual AsyncServerStreamingCall<global::Grpc.Testing.GaugeResponse> GetAllGauges(global::Grpc.Testing.EmptyMessage request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return GetAllGauges(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Returns the values of all the gauges that are currently being maintained by
      ///  the service
      /// </summary>
      public virtual AsyncServerStreamingCall<global::Grpc.Testing.GaugeResponse> GetAllGauges(global::Grpc.Testing.EmptyMessage request, CallOptions options)
      {
        return CallInvoker.AsyncServerStreamingCall(__Method_GetAllGauges, null, options, request);
      }
      /// <summary>
      ///  Returns the value of one gauge
      /// </summary>
      public virtual global::Grpc.Testing.GaugeResponse GetGauge(global::Grpc.Testing.GaugeRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return GetGauge(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Returns the value of one gauge
      /// </summary>
      public virtual global::Grpc.Testing.GaugeResponse GetGauge(global::Grpc.Testing.GaugeRequest request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_GetGauge, null, options, request);
      }
      /// <summary>
      ///  Returns the value of one gauge
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.GaugeResponse> GetGaugeAsync(global::Grpc.Testing.GaugeRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return GetGaugeAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Returns the value of one gauge
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.GaugeResponse> GetGaugeAsync(global::Grpc.Testing.GaugeRequest request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_GetGauge, null, options, request);
      }
      protected override MetricsServiceClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new MetricsServiceClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    public static ServerServiceDefinition BindService(MetricsServiceBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_GetAllGauges, serviceImpl.GetAllGauges)
          .AddMethod(__Method_GetGauge, serviceImpl.GetGauge).Build();
    }

  }
}
#endregion
