// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: src/proto/grpc/testing/benchmark_service.proto
// </auto-generated>
// Original file comments:
// Copyright 2015 gRPC authors.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An integration test service that covers all the method signature permutations
// of unary/streaming requests/responses.
#pragma warning disable 0414, 1591
#region Designer generated code

using grpc = global::Grpc.Core;

namespace Grpc.Testing {
  public static partial class BenchmarkService
  {
    static readonly string __ServiceName = "grpc.testing.BenchmarkService";

    static readonly grpc::Marshaller<global::Grpc.Testing.SimpleRequest> __Marshaller_grpc_testing_SimpleRequest = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.SimpleRequest.Parser.ParseFrom);
    static readonly grpc::Marshaller<global::Grpc.Testing.SimpleResponse> __Marshaller_grpc_testing_SimpleResponse = grpc::Marshallers.Create((arg) => global::Google.Protobuf.MessageExtensions.ToByteArray(arg), global::Grpc.Testing.SimpleResponse.Parser.ParseFrom);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_UnaryCall = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.Unary,
        __ServiceName,
        "UnaryCall",
        __Marshaller_grpc_testing_SimpleRequest,
        __Marshaller_grpc_testing_SimpleResponse);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_StreamingCall = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.DuplexStreaming,
        __ServiceName,
        "StreamingCall",
        __Marshaller_grpc_testing_SimpleRequest,
        __Marshaller_grpc_testing_SimpleResponse);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_StreamingFromClient = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.ClientStreaming,
        __ServiceName,
        "StreamingFromClient",
        __Marshaller_grpc_testing_SimpleRequest,
        __Marshaller_grpc_testing_SimpleResponse);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_StreamingFromServer = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.ServerStreaming,
        __ServiceName,
        "StreamingFromServer",
        __Marshaller_grpc_testing_SimpleRequest,
        __Marshaller_grpc_testing_SimpleResponse);

    static readonly grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> __Method_StreamingBothWays = new grpc::Method<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(
        grpc::MethodType.DuplexStreaming,
        __ServiceName,
        "StreamingBothWays",
        __Marshaller_grpc_testing_SimpleRequest,
        __Marshaller_grpc_testing_SimpleResponse);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::Grpc.Testing.BenchmarkServiceReflection.Descriptor.Services[0]; }
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
      /// Repeated sequence of one request followed by one response.
      /// Should be called streaming ping-pong
      /// The server returns the client payload as-is on each response
      /// </summary>
      /// <param name="requestStream">Used for reading requests from the client.</param>
      /// <param name="responseStream">Used for sending responses back to the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>A task indicating completion of the handler.</returns>
      public virtual global::System.Threading.Tasks.Task StreamingCall(grpc::IAsyncStreamReader<global::Grpc.Testing.SimpleRequest> requestStream, grpc::IServerStreamWriter<global::Grpc.Testing.SimpleResponse> responseStream, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// Single-sided unbounded streaming from client to server
      /// The server returns the client payload as-is once the client does WritesDone
      /// </summary>
      /// <param name="requestStream">Used for reading requests from the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>The response to send back to the client (wrapped by a task).</returns>
      public virtual global::System.Threading.Tasks.Task<global::Grpc.Testing.SimpleResponse> StreamingFromClient(grpc::IAsyncStreamReader<global::Grpc.Testing.SimpleRequest> requestStream, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// Single-sided unbounded streaming from server to client
      /// The server repeatedly returns the client payload as-is
      /// </summary>
      /// <param name="request">The request received from the client.</param>
      /// <param name="responseStream">Used for sending responses back to the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>A task indicating completion of the handler.</returns>
      public virtual global::System.Threading.Tasks.Task StreamingFromServer(global::Grpc.Testing.SimpleRequest request, grpc::IServerStreamWriter<global::Grpc.Testing.SimpleResponse> responseStream, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      /// <summary>
      /// Two-sided unbounded streaming between server to client
      /// Both sides send the content of their own choice to the other
      /// </summary>
      /// <param name="requestStream">Used for reading requests from the client.</param>
      /// <param name="responseStream">Used for sending responses back to the client.</param>
      /// <param name="context">The context of the server-side call handler being invoked.</param>
      /// <returns>A task indicating completion of the handler.</returns>
      public virtual global::System.Threading.Tasks.Task StreamingBothWays(grpc::IAsyncStreamReader<global::Grpc.Testing.SimpleRequest> requestStream, grpc::IServerStreamWriter<global::Grpc.Testing.SimpleResponse> responseStream, grpc::ServerCallContext context)
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
      public virtual global::Grpc.Testing.SimpleResponse UnaryCall(global::Grpc.Testing.SimpleRequest request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
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
      public virtual grpc::AsyncUnaryCall<global::Grpc.Testing.SimpleResponse> UnaryCallAsync(global::Grpc.Testing.SimpleRequest request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
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
      /// Repeated sequence of one request followed by one response.
      /// Should be called streaming ping-pong
      /// The server returns the client payload as-is on each response
      /// </summary>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingCall(grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return StreamingCall(new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Repeated sequence of one request followed by one response.
      /// Should be called streaming ping-pong
      /// The server returns the client payload as-is on each response
      /// </summary>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingCall(grpc::CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_StreamingCall, null, options);
      }
      /// <summary>
      /// Single-sided unbounded streaming from client to server
      /// The server returns the client payload as-is once the client does WritesDone
      /// </summary>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncClientStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingFromClient(grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return StreamingFromClient(new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Single-sided unbounded streaming from client to server
      /// The server returns the client payload as-is once the client does WritesDone
      /// </summary>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncClientStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingFromClient(grpc::CallOptions options)
      {
        return CallInvoker.AsyncClientStreamingCall(__Method_StreamingFromClient, null, options);
      }
      /// <summary>
      /// Single-sided unbounded streaming from server to client
      /// The server repeatedly returns the client payload as-is
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncServerStreamingCall<global::Grpc.Testing.SimpleResponse> StreamingFromServer(global::Grpc.Testing.SimpleRequest request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return StreamingFromServer(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Single-sided unbounded streaming from server to client
      /// The server repeatedly returns the client payload as-is
      /// </summary>
      /// <param name="request">The request to send to the server.</param>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncServerStreamingCall<global::Grpc.Testing.SimpleResponse> StreamingFromServer(global::Grpc.Testing.SimpleRequest request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncServerStreamingCall(__Method_StreamingFromServer, null, options, request);
      }
      /// <summary>
      /// Two-sided unbounded streaming between server to client
      /// Both sides send the content of their own choice to the other
      /// </summary>
      /// <param name="headers">The initial metadata to send with the call. This parameter is optional.</param>
      /// <param name="deadline">An optional deadline for the call. The call will be cancelled if deadline is hit.</param>
      /// <param name="cancellationToken">An optional token for canceling the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingBothWays(grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return StreamingBothWays(new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      /// <summary>
      /// Two-sided unbounded streaming between server to client
      /// Both sides send the content of their own choice to the other
      /// </summary>
      /// <param name="options">The options for the call.</param>
      /// <returns>The call object.</returns>
      public virtual grpc::AsyncDuplexStreamingCall<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse> StreamingBothWays(grpc::CallOptions options)
      {
        return CallInvoker.AsyncDuplexStreamingCall(__Method_StreamingBothWays, null, options);
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
          .AddMethod(__Method_StreamingCall, serviceImpl.StreamingCall)
          .AddMethod(__Method_StreamingFromClient, serviceImpl.StreamingFromClient)
          .AddMethod(__Method_StreamingFromServer, serviceImpl.StreamingFromServer)
          .AddMethod(__Method_StreamingBothWays, serviceImpl.StreamingBothWays).Build();
    }

    /// <summary>Register service method with a service binder with or without implementation. Useful when customizing the  service binding logic.
    /// Note: this method is part of an experimental API that can change or be removed without any prior notice.</summary>
    /// <param name="serviceBinder">Service methods will be bound by calling <c>AddMethod</c> on this object.</param>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    public static void BindService(grpc::ServiceBinderBase serviceBinder, BenchmarkServiceBase serviceImpl)
    {
      serviceBinder.AddMethod(__Method_UnaryCall, serviceImpl == null ? null : new grpc::UnaryServerMethod<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(serviceImpl.UnaryCall));
      serviceBinder.AddMethod(__Method_StreamingCall, serviceImpl == null ? null : new grpc::DuplexStreamingServerMethod<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(serviceImpl.StreamingCall));
      serviceBinder.AddMethod(__Method_StreamingFromClient, serviceImpl == null ? null : new grpc::ClientStreamingServerMethod<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(serviceImpl.StreamingFromClient));
      serviceBinder.AddMethod(__Method_StreamingFromServer, serviceImpl == null ? null : new grpc::ServerStreamingServerMethod<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(serviceImpl.StreamingFromServer));
      serviceBinder.AddMethod(__Method_StreamingBothWays, serviceImpl == null ? null : new grpc::DuplexStreamingServerMethod<global::Grpc.Testing.SimpleRequest, global::Grpc.Testing.SimpleResponse>(serviceImpl.StreamingBothWays));
    }

  }
}
#endregion
