// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: src/proto/grpc/testing/metrics.proto
#region Designer generated code

using System;
using System.Threading;
using System.Threading.Tasks;
using Grpc.Core;

namespace Grpc.Testing {
  public static class MetricsService
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

    // service descriptor
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::Grpc.Testing.MetricsReflection.Descriptor.Services[0]; }
    }

    // client interface
    [System.Obsolete("Client side interfaced will be removed in the next release. Use client class directly.")]
    public interface IMetricsServiceClient
    {
      AsyncServerStreamingCall<global::Grpc.Testing.GaugeResponse> GetAllGauges(global::Grpc.Testing.EmptyMessage request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken));
      AsyncServerStreamingCall<global::Grpc.Testing.GaugeResponse> GetAllGauges(global::Grpc.Testing.EmptyMessage request, CallOptions options);
      global::Grpc.Testing.GaugeResponse GetGauge(global::Grpc.Testing.GaugeRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken));
      global::Grpc.Testing.GaugeResponse GetGauge(global::Grpc.Testing.GaugeRequest request, CallOptions options);
      AsyncUnaryCall<global::Grpc.Testing.GaugeResponse> GetGaugeAsync(global::Grpc.Testing.GaugeRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken));
      AsyncUnaryCall<global::Grpc.Testing.GaugeResponse> GetGaugeAsync(global::Grpc.Testing.GaugeRequest request, CallOptions options);
    }

    // server-side interface
    [System.Obsolete("Service implementations should inherit from the generated abstract base class instead.")]
    public interface IMetricsService
    {
      Task GetAllGauges(global::Grpc.Testing.EmptyMessage request, IServerStreamWriter<global::Grpc.Testing.GaugeResponse> responseStream, ServerCallContext context);
      Task<global::Grpc.Testing.GaugeResponse> GetGauge(global::Grpc.Testing.GaugeRequest request, ServerCallContext context);
    }

    // server-side abstract class
    public abstract class MetricsServiceBase
    {
      public virtual Task GetAllGauges(global::Grpc.Testing.EmptyMessage request, IServerStreamWriter<global::Grpc.Testing.GaugeResponse> responseStream, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

      public virtual Task<global::Grpc.Testing.GaugeResponse> GetGauge(global::Grpc.Testing.GaugeRequest request, ServerCallContext context)
      {
        throw new RpcException(new Status(StatusCode.Unimplemented, ""));
      }

    }

    // client stub
    #pragma warning disable 0618
    public class MetricsServiceClient : ClientBase<MetricsServiceClient>, IMetricsServiceClient
    #pragma warning restore 0618
    {
      public MetricsServiceClient(Channel channel) : base(channel)
      {
      }
      public MetricsServiceClient(CallInvoker callInvoker) : base(callInvoker)
      {
      }
      ///<summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      protected MetricsServiceClient() : base()
      {
      }
      ///<summary>Protected constructor to allow creation of configured clients.</summary>
      protected MetricsServiceClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      public virtual AsyncServerStreamingCall<global::Grpc.Testing.GaugeResponse> GetAllGauges(global::Grpc.Testing.EmptyMessage request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return GetAllGauges(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncServerStreamingCall<global::Grpc.Testing.GaugeResponse> GetAllGauges(global::Grpc.Testing.EmptyMessage request, CallOptions options)
      {
        return CallInvoker.AsyncServerStreamingCall(__Method_GetAllGauges, null, options, request);
      }
      public virtual global::Grpc.Testing.GaugeResponse GetGauge(global::Grpc.Testing.GaugeRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return GetGauge(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual global::Grpc.Testing.GaugeResponse GetGauge(global::Grpc.Testing.GaugeRequest request, CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_GetGauge, null, options, request);
      }
      public virtual AsyncUnaryCall<global::Grpc.Testing.GaugeResponse> GetGaugeAsync(global::Grpc.Testing.GaugeRequest request, Metadata headers = null, DateTime? deadline = null, CancellationToken cancellationToken = default(CancellationToken))
      {
        return GetGaugeAsync(request, new CallOptions(headers, deadline, cancellationToken));
      }
      public virtual AsyncUnaryCall<global::Grpc.Testing.GaugeResponse> GetGaugeAsync(global::Grpc.Testing.GaugeRequest request, CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_GetGauge, null, options, request);
      }
      protected override MetricsServiceClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new MetricsServiceClient(configuration);
      }
    }

    // creates service definition that can be registered with a server
    #pragma warning disable 0618
    public static ServerServiceDefinition BindService(IMetricsService serviceImpl)
    #pragma warning restore 0618
    {
      return ServerServiceDefinition.CreateBuilder(__ServiceName)
          .AddMethod(__Method_GetAllGauges, serviceImpl.GetAllGauges)
          .AddMethod(__Method_GetGauge, serviceImpl.GetGauge).Build();
    }

    // creates service definition that can be registered with a server
    #pragma warning disable 0618
    public static ServerServiceDefinition BindService(MetricsServiceBase serviceImpl)
    #pragma warning restore 0618
    {
      return ServerServiceDefinition.CreateBuilder(__ServiceName)
          .AddMethod(__Method_GetAllGauges, serviceImpl.GetAllGauges)
          .AddMethod(__Method_GetGauge, serviceImpl.GetGauge).Build();
    }

    // creates a new client
    public static MetricsServiceClient NewClient(Channel channel)
    {
      return new MetricsServiceClient(channel);
    }

  }
}
#endregion
