// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: src/proto/grpc/testing/services.proto
// Original file comments:
// Copyright 2015, Google Inc.
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
// An integration test service that covers all the method signature permutations
// of unary/streaming requests/responses.
#region Designer generated code

using System;
using System.Threading;
using System.Threading.Tasks;
using Grpc.Core;

namespace Grpc.Testing {
  public static partial class BenchmarkService
  {
    static readonly string __ServiceName = "grpc.testing.BenchmarkService";

    static readonly Marshaller<global::Grpc.Testing.SimpleRequest> __Marshaller_SimpleRequest = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.SimpleRequest.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.SimpleResponse> __Marshaller_SimpleResponse = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.SimpleResponse.Parser.ParseFrom);

    static readonly Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_UnaryCall = new Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        MethodType.Unary,
        __ServiceName,
        "UnaryCall",
        __Marshaller_SimpleRequest,
        __Marshaller_SimpleResponse);

    static readonly Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_StreamingCall = new Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        MethodType.DuplexStreaming,
        __ServiceName,
        "StreamingCall",
        __Marshaller_SimpleRequest,
        __Marshaller_SimpleResponse);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::Grpc.Testing.ServicesReflection.Descriptor.Services[0]; }
    }

    /// <summary>Base class for server-side implementations of BenchmarkService</summary>
    public abstract partial class BenchmarkServiceBase
    {
      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.SimpleResponse> UnaryCall(global::Grpc.Testing.SimpleRequest request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual global::System.Threading.Tasks.Task StreamingCall(IAsyncStreamReader<global::Grpc.Testing.SimpleRequest> requestStream, IServerStreamWriter<global::Grpc.Testing.SimpleResponse> responseStream, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for BenchmarkService</summary>
    public partial class BenchmarkServiceClient : ClientBase<BenchmarkServiceClient>
    {
      /// <summary>Creates a new client for BenchmarkService</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public BenchmarkServiceClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for BenchmarkService that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public BenchmarkServiceClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected BenchmarkServiceClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected BenchmarkServiceClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual global::Grpc.Testing.SimpleResponse UnaryCall(global::Grpc.Testing.SimpleRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return UnaryCall(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual global::Grpc.Testing.SimpleResponse UnaryCall(global::Grpc.Testing.SimpleRequest request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_UnaryCall, null, options, request);
      }
      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.SimpleResponse> UnaryCallAsync(global::Grpc.Testing.SimpleRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return UnaryCallAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.SimpleResponse> UnaryCallAsync(global::Grpc.Testing.SimpleRequest request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_UnaryCall, null, options, request);
      }
      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingCall(Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return StreamingCall(new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  One request followed by one response.
      ///  The server returns the client payload as-is.
      /// </summary>
      public virtual AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingCall(CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_StreamingCall, null, options);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      protected override BenchmarkServiceClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new BenchmarkServiceClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    public static ServerServiceDefinition BindService(BenchmarkServiceBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_UnaryCall, serviceImpl.UnaryCall)
          .AddMethod(__Method_StreamingCall, serviceImpl.StreamingCall).Build();
    }

  }
  public static partial class WorkerService
  {
    static readonly string __ServiceName = "grpc.testing.WorkerService";

    static readonly Marshaller<global::Grpc.Testing.ServerArgs> __Marshaller_ServerArgs = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ServerArgs.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.ServerStatus> __Marshaller_ServerStatus = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ServerStatus.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.ClientArgs> __Marshaller_ClientArgs = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ClientArgs.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.ClientStatus> __Marshaller_ClientStatus = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ClientStatus.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.CoreRequest> __Marshaller_CoreRequest = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.CoreRequest.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.CoreResponse> __Marshaller_CoreResponse = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.CoreResponse.Parser.ParseFrom);
    static readonly Marshaller<global::Grpc.Testing.Void> __Marshaller_Void = Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.Void.Parser.ParseFrom);

    static readonly Method<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus> __Method_RunServer = new Method<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus>(
        MethodType.DuplexStreaming,
        __ServiceName,
        "RunServer",
        __Marshaller_ServerArgs,
        __Marshaller_ServerStatus);

    static readonly Method<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus> __Method_RunClient = new Method<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus>(
        MethodType.DuplexStreaming,
        __ServiceName,
        "RunClient",
        __Marshaller_ClientArgs,
        __Marshaller_ClientStatus);

    static readonly Method<global::Grpc.Testing.CoreRequest, global::Grpc.Testing.CoreResponse> __Method_CoreCount = new Method<global::Grpc.Testing.CoreRequest, global::Grpc.Testing.CoreResponse>(
        MethodType.Unary,
        __ServiceName,
        "CoreCount",
        __Marshaller_CoreRequest,
        __Marshaller_CoreResponse);

    static readonly Method<global::Grpc.Testing.Void, global::Grpc.Testing.Void> __Method_QuitWorker = new Method<global::Grpc.Testing.Void, global::Grpc.Testing.Void>(
        MethodType.Unary,
        __ServiceName,
        "QuitWorker",
        __Marshaller_Void,
        __Marshaller_Void);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::Grpc.Testing.ServicesReflection.Descriptor.Services[1]; }
    }

    /// <summary>Base class for server-side implementations of WorkerService</summary>
    public abstract partial class WorkerServiceBase
    {
      /// <summary>
      ///  Start server with specified workload.
      ///  First request sent specifies the ServerConfig followed by ServerStatus
      ///  response. After that, a "Mark" can be sent anytime to request the latest
      ///  stats. Closing the stream will initiate shutdown of the test server
      ///  and once the shutdown has finished, the OK status is sent to terminate
      ///  this RPC.
      /// </summary>
      public virtual global::System.Threading.Tasks.Task RunServer(IAsyncStreamReader<global::Grpc.Testing.ServerArgs> requestStream, IServerStreamWriter<global::Grpc.Testing.ServerStatus> responseStream, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      /// <summary>
      ///  Start client with specified workload.
      ///  First request sent specifies the ClientConfig followed by ClientStatus
      ///  response. After that, a "Mark" can be sent anytime to request the latest
      ///  stats. Closing the stream will initiate shutdown of the test client
      ///  and once the shutdown has finished, the OK status is sent to terminate
      ///  this RPC.
      /// </summary>
      public virtual global::System.Threading.Tasks.Task RunClient(IAsyncStreamReader<global::Grpc.Testing.ClientArgs> requestStream, IServerStreamWriter<global::Grpc.Testing.ClientStatus> responseStream, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      /// <summary>
      ///  Just return the core count - unary call
      /// </summary>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.CoreResponse> CoreCount(global::Grpc.Testing.CoreRequest request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      /// <summary>
      ///  Quit this worker
      /// </summary>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.Void> QuitWorker(global::Grpc.Testing.Void request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for WorkerService</summary>
    public partial class WorkerServiceClient : ClientBase<WorkerServiceClient>
    {
      /// <summary>Creates a new client for WorkerService</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public WorkerServiceClient(Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for WorkerService that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public WorkerServiceClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected WorkerServiceClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      protected WorkerServiceClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      /// <summary>
      ///  Start server with specified workload.
      ///  First request sent specifies the ServerConfig followed by ServerStatus
      ///  response. After that, a "Mark" can be sent anytime to request the latest
      ///  stats. Closing the stream will initiate shutdown of the test server
      ///  and once the shutdown has finished, the OK status is sent to terminate
      ///  this RPC.
      /// </summary>
      public virtual AsyncDuplexStreamingCall<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus> RunServer(Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return RunServer(new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Start server with specified workload.
      ///  First request sent specifies the ServerConfig followed by ServerStatus
      ///  response. After that, a "Mark" can be sent anytime to request the latest
      ///  stats. Closing the stream will initiate shutdown of the test server
      ///  and once the shutdown has finished, the OK status is sent to terminate
      ///  this RPC.
      /// </summary>
      public virtual AsyncDuplexStreamingCall<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus> RunServer(CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_RunServer, null, options);
      }
      /// <summary>
      ///  Start client with specified workload.
      ///  First request sent specifies the ClientConfig followed by ClientStatus
      ///  response. After that, a "Mark" can be sent anytime to request the latest
      ///  stats. Closing the stream will initiate shutdown of the test client
      ///  and once the shutdown has finished, the OK status is sent to terminate
      ///  this RPC.
      /// </summary>
      public virtual AsyncDuplexStreamingCall<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus> RunClient(Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return RunClient(new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Start client with specified workload.
      ///  First request sent specifies the ClientConfig followed by ClientStatus
      ///  response. After that, a "Mark" can be sent anytime to request the latest
      ///  stats. Closing the stream will initiate shutdown of the test client
      ///  and once the shutdown has finished, the OK status is sent to terminate
      ///  this RPC.
      /// </summary>
      public virtual AsyncDuplexStreamingCall<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus> RunClient(CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_RunClient, null, options);
      }
      /// <summary>
      ///  Just return the core count - unary call
      /// </summary>
      public virtual global::Grpc.Testing.CoreResponse CoreCount(global::Grpc.Testing.CoreRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return CoreCount(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Just return the core count - unary call
      /// </summary>
      public virtual global::Grpc.Testing.CoreResponse CoreCount(global::Grpc.Testing.CoreRequest request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_CoreCount, null, options, request);
      }
      /// <summary>
      ///  Just return the core count - unary call
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.CoreResponse> CoreCountAsync(global::Grpc.Testing.CoreRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return CoreCountAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Just return the core count - unary call
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.CoreResponse> CoreCountAsync(global::Grpc.Testing.CoreRequest request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_CoreCount, null, options, request);
      }
      /// <summary>
      ///  Quit this worker
      /// </summary>
      public virtual global::Grpc.Testing.Void QuitWorker(global::Grpc.Testing.Void request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return QuitWorker(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Quit this worker
      /// </summary>
      public virtual global::Grpc.Testing.Void QuitWorker(global::Grpc.Testing.Void request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_QuitWorker, null, options, request);
      }
      /// <summary>
      ///  Quit this worker
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.Void> QuitWorkerAsync(global::Grpc.Testing.Void request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return QuitWorkerAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      ///  Quit this worker
      /// </summary>
      public virtual AsyncUnaryCall<global::Grpc.Testing.Void> QuitWorkerAsync(global::Grpc.Testing.Void request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_QuitWorker, null, options, request);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      protected override WorkerServiceClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new WorkerServiceClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    public static ServerServiceDefinition BindService(WorkerServiceBase serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_RunServer, serviceImpl.RunServer)
          .AddMethod(__Method_RunClient, serviceImpl.RunClient)
          .AddMethod(__Method_CoreCount, serviceImpl.CoreCount)
          .AddMethod(__Method_QuitWorker, serviceImpl.QuitWorker).Build();
    }

  }
}
#endregion
