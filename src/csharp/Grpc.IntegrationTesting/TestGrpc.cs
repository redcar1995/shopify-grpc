// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto
#region Designer generated code

using System;
using System.Threading;
using System.Threading.Tasks;
using Grpc.Core;

namespace grpc.testing {
  public static class TestService
  {
    static readonly string __ServiceName = "grpc.testing.TestService";

    static readonly Marshaller<global::grpc.testing.Empty> __Marshaller_Empty = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.Empty.ParseFrom);
    static readonly Marshaller<global::grpc.testing.SimpleRequest> __Marshaller_SimpleRequest = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.SimpleRequest.ParseFrom);
    static readonly Marshaller<global::grpc.testing.SimpleResponse> __Marshaller_SimpleResponse = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.SimpleResponse.ParseFrom);
    static readonly Marshaller<global::grpc.testing.StreamingOutputCallRequest> __Marshaller_StreamingOutputCallRequest = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.StreamingOutputCallRequest.ParseFrom);
    static readonly Marshaller<global::grpc.testing.StreamingOutputCallResponse> __Marshaller_StreamingOutputCallResponse = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.StreamingOutputCallResponse.ParseFrom);
    static readonly Marshaller<global::grpc.testing.StreamingInputCallRequest> __Marshaller_StreamingInputCallRequest = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.StreamingInputCallRequest.ParseFrom);
    static readonly Marshaller<global::grpc.testing.StreamingInputCallResponse> __Marshaller_StreamingInputCallResponse = Marshallers.Create((arg) => arg.ToByteArray(), global::grpc.testing.StreamingInputCallResponse.ParseFrom);

    static readonly Method<global::grpc.testing.Empty, global::grpc.testing.Empty> __Method_EmptyCall = new Method<global::grpc.testing.Empty, global::grpc.testing.Empty>(
        MethodType.Unary,
        "EmptyCall",
        __Marshaller_Empty,
        __Marshaller_Empty);

    static readonly Method<global::grpc.testing.SimpleRequest, global::grpc.testing.SimpleResponse> __Method_UnaryCall = new Method<global::grpc.testing.SimpleRequest, global::grpc.testing.SimpleResponse>(
        MethodType.Unary,
        "UnaryCall",
        __Marshaller_SimpleRequest,
        __Marshaller_SimpleResponse);

    static readonly Method<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> __Method_StreamingOutputCall = new Method<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse>(
        MethodType.ServerStreaming,
        "StreamingOutputCall",
        __Marshaller_StreamingOutputCallRequest,
        __Marshaller_StreamingOutputCallResponse);

    static readonly Method<global::grpc.testing.StreamingInputCallRequest, global::grpc.testing.StreamingInputCallResponse> __Method_StreamingInputCall = new Method<global::grpc.testing.StreamingInputCallRequest, global::grpc.testing.StreamingInputCallResponse>(
        MethodType.ClientStreaming,
        "StreamingInputCall",
        __Marshaller_StreamingInputCallRequest,
        __Marshaller_StreamingInputCallResponse);

    static readonly Method<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> __Method_FullDuplexCall = new Method<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse>(
        MethodType.DuplexStreaming,
        "FullDuplexCall",
        __Marshaller_StreamingOutputCallRequest,
        __Marshaller_StreamingOutputCallResponse);

    static readonly Method<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> __Method_HalfDuplexCall = new Method<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse>(
        MethodType.DuplexStreaming,
        "HalfDuplexCall",
        __Marshaller_StreamingOutputCallRequest,
        __Marshaller_StreamingOutputCallResponse);

    // client-side stub interface
    public interface ITestServiceClient
    {
      global::grpc.testing.Empty EmptyCall(global::grpc.testing.Empty request, CancellationToken token = default(CancellationToken));
      Task<global::grpc.testing.Empty> EmptyCallAsync(global::grpc.testing.Empty request, CancellationToken token = default(CancellationToken));
      global::grpc.testing.SimpleResponse UnaryCall(global::grpc.testing.SimpleRequest request, CancellationToken token = default(CancellationToken));
      Task<global::grpc.testing.SimpleResponse> UnaryCallAsync(global::grpc.testing.SimpleRequest request, CancellationToken token = default(CancellationToken));
      AsyncServerStreamingCall<global::grpc.testing.StreamingOutputCallResponse> StreamingOutputCall(global::grpc.testing.StreamingOutputCallRequest request, CancellationToken token = default(CancellationToken));
      AsyncClientStreamingCall<global::grpc.testing.StreamingInputCallRequest, global::grpc.testing.StreamingInputCallResponse> StreamingInputCall(CancellationToken token = default(CancellationToken));
      AsyncDuplexStreamingCall<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> FullDuplexCall(CancellationToken token = default(CancellationToken));
      AsyncDuplexStreamingCall<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> HalfDuplexCall(CancellationToken token = default(CancellationToken));
    }

    // server-side interface
    public interface ITestService
    {
      Task<global::grpc.testing.Empty> EmptyCall(ServerCallContext context, global::grpc.testing.Empty request);
      Task<global::grpc.testing.SimpleResponse> UnaryCall(ServerCallContext context, global::grpc.testing.SimpleRequest request);
      Task StreamingOutputCall(ServerCallContext context, global::grpc.testing.StreamingOutputCallRequest request, IServerStreamWriter<global::grpc.testing.StreamingOutputCallResponse> responseStream);
      Task<global::grpc.testing.StreamingInputCallResponse> StreamingInputCall(ServerCallContext context, IAsyncStreamReader<global::grpc.testing.StreamingInputCallRequest> requestStream);
      Task FullDuplexCall(ServerCallContext context, IAsyncStreamReader<global::grpc.testing.StreamingOutputCallRequest> requestStream, IServerStreamWriter<global::grpc.testing.StreamingOutputCallResponse> responseStream);
      Task HalfDuplexCall(ServerCallContext context, IAsyncStreamReader<global::grpc.testing.StreamingOutputCallRequest> requestStream, IServerStreamWriter<global::grpc.testing.StreamingOutputCallResponse> responseStream);
    }

    // client stub
    public class TestServiceClient : ClientBase<TestServiceClient, StubConfiguration>, ITestServiceClient
    {
      public TestServiceClient(Channel channel) : this(channel, StubConfiguration.Default)
      {
      }
      public TestServiceClient(Channel channel, StubConfiguration config) : base(channel, config)
      {
      }
      public global::grpc.testing.Empty EmptyCall(global::grpc.testing.Empty request, CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_EmptyCall);
        return Calls.BlockingUnaryCall(call, request, token);
      }
      public Task<global::grpc.testing.Empty> EmptyCallAsync(global::grpc.testing.Empty request, CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_EmptyCall);
        return Calls.AsyncUnaryCall(call, request, token);
      }
      public global::grpc.testing.SimpleResponse UnaryCall(global::grpc.testing.SimpleRequest request, CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_UnaryCall);
        return Calls.BlockingUnaryCall(call, request, token);
      }
      public Task<global::grpc.testing.SimpleResponse> UnaryCallAsync(global::grpc.testing.SimpleRequest request, CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_UnaryCall);
        return Calls.AsyncUnaryCall(call, request, token);
      }
      public AsyncServerStreamingCall<global::grpc.testing.StreamingOutputCallResponse> StreamingOutputCall(global::grpc.testing.StreamingOutputCallRequest request, CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_StreamingOutputCall);
        return Calls.AsyncServerStreamingCall(call, request, token);
      }
      public AsyncClientStreamingCall<global::grpc.testing.StreamingInputCallRequest, global::grpc.testing.StreamingInputCallResponse> StreamingInputCall(CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_StreamingInputCall);
        return Calls.AsyncClientStreamingCall(call, token);
      }
      public AsyncDuplexStreamingCall<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> FullDuplexCall(CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_FullDuplexCall);
        return Calls.AsyncDuplexStreamingCall(call, token);
      }
      public AsyncDuplexStreamingCall<global::grpc.testing.StreamingOutputCallRequest, global::grpc.testing.StreamingOutputCallResponse> HalfDuplexCall(CancellationToken token = default(CancellationToken))
      {
        var call = CreateCall(__ServiceName, __Method_HalfDuplexCall);
        return Calls.AsyncDuplexStreamingCall(call, token);
      }
    }

    // creates service definition that can be registered with a server
    public static ServerServiceDefinition BindService(ITestService serviceImpl)
    {
      return ServerServiceDefinition.CreateBuilder(__ServiceName)
          .AddMethod(__Method_EmptyCall, serviceImpl.EmptyCall)
          .AddMethod(__Method_UnaryCall, serviceImpl.UnaryCall)
          .AddMethod(__Method_StreamingOutputCall, serviceImpl.StreamingOutputCall)
          .AddMethod(__Method_StreamingInputCall, serviceImpl.StreamingInputCall)
          .AddMethod(__Method_FullDuplexCall, serviceImpl.FullDuplexCall)
          .AddMethod(__Method_HalfDuplexCall, serviceImpl.HalfDuplexCall).Build();
    }

    // creates a new client stub
    public static ITestServiceClient NewStub(Channel channel)
    {
      return new TestServiceClient(channel);
    }

    // creates a new client stub
    public static ITestServiceClient NewStub(Channel channel, StubConfiguration config)
    {
      return new TestServiceClient(channel, config);
    }
  }
}
#endregion
