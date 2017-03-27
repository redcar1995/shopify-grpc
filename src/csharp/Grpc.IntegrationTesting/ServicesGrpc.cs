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
using grpc = global::Grpc.Core;

namespace Grpc.Testing {
  public static partial class BenchmarkService
  {
    static readonly string __ServiceName = "grpc.testing.BenchmarkService";

    static readonly grpc::Marshaller<global::Grpc.Testing.SimpleRequest> __Marshaller_SimpleRequest = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.SimpleRequest.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.SimpleResponse> __Marshaller_SimpleResponse = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.SimpleResponse.Parser.ParseFrom);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_UnaryCall = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.Unary,
        __ServiceName,
        "UnaryCall",
        __Marshaller_SimpleRequest,
        __Marshaller_SimpleResponse);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_StreamingCall = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.DuplexStreaming,
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
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="request">The request received from the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>The response to send back to the client (wrapped by a task).</returns>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.SimpleResponse> UnaryCall(global::Grpc.Testing.SimpleRequest request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="requestStream">Used for reading requests from the client.</param>
      /// <param name="responseStream">Used for sending responses back to the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>A task indicating completion of the handler.</returns>
      public virtual global::System.Threading.Tasks.Task StreamingCall(grpc::IAsyncStreamReader<global::Grpc.Testing.SimpleRequest> requestStream, grpc::IServerStreamWriter<global::Grpc.Testing.SimpleResponse> responseStream, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for BenchmarkService</summary>
    public partial class BenchmarkServiceClient : grpc::ClientBase<BenchmarkServiceClient>
    {
      /// <summary>Creates a new client for BenchmarkService</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public BenchmarkServiceClient(grpc::Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for BenchmarkService that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public BenchmarkServiceClient(grpc::CallInvoker callInvoker) : base(callInvoker)
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
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The response received from the server.</returns>
      public virtual global::Grpc.Testing.SimpleResponse UnaryCall(global::Grpc.Testing.SimpleRequest request, grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return UnaryCall(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The response received from the server.</returns>
      public virtual global::Grpc.Testing.SimpleResponse UnaryCall(global::Grpc.Testing.SimpleRequest request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_UnaryCall, null, options, request);
      }
      /// <summary>
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.SimpleResponse> UnaryCallAsync(global::Grpc.Testing.SimpleRequest request, grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return UnaryCallAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.SimpleResponse> UnaryCallAsync(global::Grpc.Testing.SimpleRequest request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_UnaryCall, null, options, request);
      }
      /// <summary>
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingCall(grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return StreamingCall(new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// One request followed by one response.
      /// The server returns the client payload as-is.
      /// </summary>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingCall(grpc::CallOptions options)
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
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static grpc::ServerServiceDefinition BindService(BenchmarkServiceBase serviceImpl)
    {
      return grpc::ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_UnaryCall, serviceImpl.UnaryCall)
          .AddMethod(__Method_StreamingCall, serviceImpl.StreamingCall).Build();
    }

  }
  public static partial class WorkerService
  {
    static readonly string __ServiceName = "grpc.testing.WorkerService";

    static readonly grpc::Marshaller<global::Grpc.Testing.ServerArgs> __Marshaller_ServerArgs = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ServerArgs.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.ServerStatus> __Marshaller_ServerStatus = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ServerStatus.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.ClientArgs> __Marshaller_ClientArgs = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ClientArgs.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.ClientStatus> __Marshaller_ClientStatus = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.ClientStatus.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.CoreRequest> __Marshaller_CoreRequest = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.CoreRequest.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.CoreResponse> __Marshaller_CoreResponse = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.CoreResponse.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.Void> __Marshaller_Void = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.Void.Parser.ParseFrom);

    static readonly grpc::Method<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus> __Method_RunServer = new grpc::Method<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus>(
        grpc::MethodType.DuplexStreaming,
        __ServiceName,
        "RunServer",
        __Marshaller_ServerArgs,
        __Marshaller_ServerStatus);

    static readonly grpc::Method<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus> __Method_RunClient = new grpc::Method<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus>(
        grpc::MethodType.DuplexStreaming,
        __ServiceName,
        "RunClient",
        __Marshaller_ClientArgs,
        __Marshaller_ClientStatus);

    static readonly grpc::Method<global::Grpc.Testing.CoreRequest, global::Grpc.Testing.CoreResponse> __Method_CoreCount = new grpc::Method<global::Grpc.Testing.CoreRequest, global::Grpc.Testing.CoreResponse>(
        grpc::MethodType.Unary,
        __ServiceName,
        "CoreCount",
        __Marshaller_CoreRequest,
        __Marshaller_CoreResponse);

    static readonly grpc::Method<global::Grpc.Testing.Void, global::Grpc.Testing.Void> __Method_QuitWorker = new grpc::Method<global::Grpc.Testing.Void, global::Grpc.Testing.Void>(
        grpc::MethodType.Unary,
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
      /// Start server with specified workload.
      /// First request sent specifies the ServerConfig followed by ServerStatus
      /// response. After that, a "Mark" can be sent anytime to request the latest
      /// stats. Closing the stream will initiate shutdown of the test server
      /// and once the shutdown has finished, the OK status is sent to terminate
      /// this RPC.
      /// </summary>
      /// <param name="requestStream">Used for reading requests from the client.</param>
      /// <param name="responseStream">Used for sending responses back to the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>A task indicating completion of the handler.</returns>
      public virtual global::System.Threading.Tasks.Task RunServer(grpc::IAsyncStreamReader<global::Grpc.Testing.ServerArgs> requestStream, grpc::IServerStreamWriter<global::Grpc.Testing.ServerStatus> responseStream, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// Start client with specified workload.
      /// First request sent specifies the ClientConfig followed by ClientStatus
      /// response. After that, a "Mark" can be sent anytime to request the latest
      /// stats. Closing the stream will initiate shutdown of the test client
      /// and once the shutdown has finished, the OK status is sent to terminate
      /// this RPC.
      /// </summary>
      /// <param name="requestStream">Used for reading requests from the client.</param>
      /// <param name="responseStream">Used for sending responses back to the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>A task indicating completion of the handler.</returns>
      public virtual global::System.Threading.Tasks.Task RunClient(grpc::IAsyncStreamReader<global::Grpc.Testing.ClientArgs> requestStream, grpc::IServerStreamWriter<global::Grpc.Testing.ClientStatus> responseStream, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// Just return the core count - unary call
      /// </summary>
      /// <param name="request">The request received from the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>The response to send back to the client (wrapped by a task).</returns>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.CoreResponse> CoreCount(global::Grpc.Testing.CoreRequest request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// Quit this worker
      /// </summary>
      /// <param name="request">The request received from the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>The response to send back to the client (wrapped by a task).</returns>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.Void> QuitWorker(global::Grpc.Testing.Void request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for WorkerService</summary>
    public partial class WorkerServiceClient : grpc::ClientBase<WorkerServiceClient>
    {
      /// <summary>Creates a new client for WorkerService</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      public WorkerServiceClient(grpc::Channel channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for WorkerService that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      public WorkerServiceClient(grpc::CallInvoker callInvoker) : base(callInvoker)
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
      /// Start server with specified workload.
      /// First request sent specifies the ServerConfig followed by ServerStatus
      /// response. After that, a "Mark" can be sent anytime to request the latest
      /// stats. Closing the stream will initiate shutdown of the test server
      /// and once the shutdown has finished, the OK status is sent to terminate
      /// this RPC.
      /// </summary>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus> RunServer(grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return RunServer(new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Start server with specified workload.
      /// First request sent specifies the ServerConfig followed by ServerStatus
      /// response. After that, a "Mark" can be sent anytime to request the latest
      /// stats. Closing the stream will initiate shutdown of the test server
      /// and once the shutdown has finished, the OK status is sent to terminate
      /// this RPC.
      /// </summary>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.ServerArgs, global::Grpc.Testing.ServerStatus> RunServer(grpc::CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_RunServer, null, options);
      }
      /// <summary>
      /// Start client with specified workload.
      /// First request sent specifies the ClientConfig followed by ClientStatus
      /// response. After that, a "Mark" can be sent anytime to request the latest
      /// stats. Closing the stream will initiate shutdown of the test client
      /// and once the shutdown has finished, the OK status is sent to terminate
      /// this RPC.
      /// </summary>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus> RunClient(grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return RunClient(new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Start client with specified workload.
      /// First request sent specifies the ClientConfig followed by ClientStatus
      /// response. After that, a "Mark" can be sent anytime to request the latest
      /// stats. Closing the stream will initiate shutdown of the test client
      /// and once the shutdown has finished, the OK status is sent to terminate
      /// this RPC.
      /// </summary>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.ClientArgs, global::Grpc.Testing.ClientStatus> RunClient(grpc::CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_RunClient, null, options);
      }
      /// <summary>
      /// Just return the core count - unary call
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The response received from the server.</returns>
      public virtual global::Grpc.Testing.CoreResponse CoreCount(global::Grpc.Testing.CoreRequest request, grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return CoreCount(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Just return the core count - unary call
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The response received from the server.</returns>
      public virtual global::Grpc.Testing.CoreResponse CoreCount(global::Grpc.Testing.CoreRequest request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_CoreCount, null, options, request);
      }
      /// <summary>
      /// Just return the core count - unary call
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.CoreResponse> CoreCountAsync(global::Grpc.Testing.CoreRequest request, grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return CoreCountAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Just return the core count - unary call
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.CoreResponse> CoreCountAsync(global::Grpc.Testing.CoreRequest request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_CoreCount, null, options, request);
      }
      /// <summary>
      /// Quit this worker
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The response received from the server.</returns>
      public virtual global::Grpc.Testing.Void QuitWorker(global::Grpc.Testing.Void request, grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return QuitWorker(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Quit this worker
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The response received from the server.</returns>
      public virtual global::Grpc.Testing.Void QuitWorker(global::Grpc.Testing.Void request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_QuitWorker, null, options, request);
      }
      /// <summary>
      /// Quit this worker
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.Void> QuitWorkerAsync(global::Grpc.Testing.Void request, grpc::Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return QuitWorkerAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Quit this worker
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.Void> QuitWorkerAsync(global::Grpc.Testing.Void request, grpc::CallOptions options)
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
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static grpc::ServerServiceDefinition BindService(WorkerServiceBase serviceImpl)
    {
      return grpc::ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_RunServer, serviceImpl.RunServer)
          .AddMethod(__Method_RunClient, serviceImpl.RunClient)
          .AddMethod(__Method_CoreCount, serviceImpl.CoreCount)
          .AddMethod(__Method_QuitWorker, serviceImpl.QuitWorker).Build();
    }

  }
}
#endregion
