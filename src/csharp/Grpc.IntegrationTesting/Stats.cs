// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: src/proto/grpc/testing/stats.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Grpc.Testing {

  /// <summary>Holder for reflection information generated from src/proto/grpc/testing/stats.proto</summary>
  public static partial class StatsReflection {

    #region Descriptor
    /// <summary>File descriptor for src/proto/grpc/testing/stats.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static StatsReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "CiJzcmMvcHJvdG8vZ3JwYy90ZXN0aW5nL3N0YXRzLnByb3RvEgxncnBjLnRl",
            "c3RpbmciegoLU2VydmVyU3RhdHMSFAoMdGltZV9lbGFwc2VkGAEgASgBEhEK",
            "CXRpbWVfdXNlchgCIAEoARITCgt0aW1lX3N5c3RlbRgDIAEoARIWCg50b3Rh",
            "bF9jcHVfdGltZRgEIAEoBBIVCg1pZGxlX2NwdV90aW1lGAUgASgEIjsKD0hp",
            "c3RvZ3JhbVBhcmFtcxISCgpyZXNvbHV0aW9uGAEgASgBEhQKDG1heF9wb3Nz",
            "aWJsZRgCIAEoASJ3Cg1IaXN0b2dyYW1EYXRhEg4KBmJ1Y2tldBgBIAMoDRIQ",
            "CghtaW5fc2VlbhgCIAEoARIQCghtYXhfc2VlbhgDIAEoARILCgNzdW0YBCAB",
            "KAESFgoOc3VtX29mX3NxdWFyZXMYBSABKAESDQoFY291bnQYBiABKAEiOAoS",
            "UmVxdWVzdFJlc3VsdENvdW50EhMKC3N0YXR1c19jb2RlGAEgASgFEg0KBWNv",
            "dW50GAIgASgDIrYBCgtDbGllbnRTdGF0cxIuCglsYXRlbmNpZXMYASABKAsy",
            "Gy5ncnBjLnRlc3RpbmcuSGlzdG9ncmFtRGF0YRIUCgx0aW1lX2VsYXBzZWQY",
            "AiABKAESEQoJdGltZV91c2VyGAMgASgBEhMKC3RpbWVfc3lzdGVtGAQgASgB",
            "EjkKD3JlcXVlc3RfcmVzdWx0cxgFIAMoCzIgLmdycGMudGVzdGluZy5SZXF1",
            "ZXN0UmVzdWx0Q291bnRiBnByb3RvMw=="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Grpc.Testing.ServerStats), global::Grpc.Testing.ServerStats.Parser, new[]{ "TimeElapsed", "TimeUser", "TimeSystem", "TotalCpuTime", "IdleCpuTime" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Grpc.Testing.HistogramParams), global::Grpc.Testing.HistogramParams.Parser, new[]{ "Resolution", "MaxPossible" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Grpc.Testing.HistogramData), global::Grpc.Testing.HistogramData.Parser, new[]{ "Bucket", "MinSeen", "MaxSeen", "Sum", "SumOfSquares", "Count" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Grpc.Testing.RequestResultCount), global::Grpc.Testing.RequestResultCount.Parser, new[]{ "StatusCode", "Count" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Grpc.Testing.ClientStats), global::Grpc.Testing.ClientStats.Parser, new[]{ "Latencies", "TimeElapsed", "TimeUser", "TimeSystem", "RequestResults" }, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  public sealed partial class ServerStats : pb::IMessage<ServerStats> {
    private static readonly pb::MessageParser<ServerStats> _parser = new pb::MessageParser<ServerStats>(() => new ServerStats());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ServerStats> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Grpc.Testing.StatsReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ServerStats() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ServerStats(ServerStats other) : this() {
      timeElapsed_ = other.timeElapsed_;
      timeUser_ = other.timeUser_;
      timeSystem_ = other.timeSystem_;
      totalCpuTime_ = other.totalCpuTime_;
      idleCpuTime_ = other.idleCpuTime_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ServerStats Clone() {
      return new ServerStats(this);
    }

    /// <summary>Field number for the "time_elapsed" field.</summary>
    public const int TimeElapsedFieldNumber = 1;
    private double timeElapsed_;
    /// <summary>
    ///  wall clock time change in seconds since last reset
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double TimeElapsed {
      get { return timeElapsed_; }
      set {
        timeElapsed_ = value;
      }
    }

    /// <summary>Field number for the "time_user" field.</summary>
    public const int TimeUserFieldNumber = 2;
    private double timeUser_;
    /// <summary>
    ///  change in user time (in seconds) used by the server since last reset
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double TimeUser {
      get { return timeUser_; }
      set {
        timeUser_ = value;
      }
    }

    /// <summary>Field number for the "time_system" field.</summary>
    public const int TimeSystemFieldNumber = 3;
    private double timeSystem_;
    /// <summary>
    ///  change in server time (in seconds) used by the server process and all
    ///  threads since last reset
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double TimeSystem {
      get { return timeSystem_; }
      set {
        timeSystem_ = value;
      }
    }

    /// <summary>Field number for the "total_cpu_time" field.</summary>
    public const int TotalCpuTimeFieldNumber = 4;
    private ulong totalCpuTime_;
    /// <summary>
    ///  change in total cpu time of the server (data from proc/stat)
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ulong TotalCpuTime {
      get { return totalCpuTime_; }
      set {
        totalCpuTime_ = value;
      }
    }

    /// <summary>Field number for the "idle_cpu_time" field.</summary>
    public const int IdleCpuTimeFieldNumber = 5;
    private ulong idleCpuTime_;
    /// <summary>
    ///  change in idle time of the server (data from proc/stat)
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ulong IdleCpuTime {
      get { return idleCpuTime_; }
      set {
        idleCpuTime_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ServerStats);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ServerStats other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (TimeElapsed != other.TimeElapsed) return false;
      if (TimeUser != other.TimeUser) return false;
      if (TimeSystem != other.TimeSystem) return false;
      if (TotalCpuTime != other.TotalCpuTime) return false;
      if (IdleCpuTime != other.IdleCpuTime) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (TimeElapsed != 0D) hash ^= TimeElapsed.GetHashCode();
      if (TimeUser != 0D) hash ^= TimeUser.GetHashCode();
      if (TimeSystem != 0D) hash ^= TimeSystem.GetHashCode();
      if (TotalCpuTime != 0UL) hash ^= TotalCpuTime.GetHashCode();
      if (IdleCpuTime != 0UL) hash ^= IdleCpuTime.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (TimeElapsed != 0D) {
        output.WriteRawTag(9);
        output.WriteDouble(TimeElapsed);
      }
      if (TimeUser != 0D) {
        output.WriteRawTag(17);
        output.WriteDouble(TimeUser);
      }
      if (TimeSystem != 0D) {
        output.WriteRawTag(25);
        output.WriteDouble(TimeSystem);
      }
      if (TotalCpuTime != 0UL) {
        output.WriteRawTag(32);
        output.WriteUInt64(TotalCpuTime);
      }
      if (IdleCpuTime != 0UL) {
        output.WriteRawTag(40);
        output.WriteUInt64(IdleCpuTime);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (TimeElapsed != 0D) {
        size += 1 + 8;
      }
      if (TimeUser != 0D) {
        size += 1 + 8;
      }
      if (TimeSystem != 0D) {
        size += 1 + 8;
      }
      if (TotalCpuTime != 0UL) {
        size += 1 + pb::CodedOutputStream.ComputeUInt64Size(TotalCpuTime);
      }
      if (IdleCpuTime != 0UL) {
        size += 1 + pb::CodedOutputStream.ComputeUInt64Size(IdleCpuTime);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ServerStats other) {
      if (other == null) {
        return;
      }
      if (other.TimeElapsed != 0D) {
        TimeElapsed = other.TimeElapsed;
      }
      if (other.TimeUser != 0D) {
        TimeUser = other.TimeUser;
      }
      if (other.TimeSystem != 0D) {
        TimeSystem = other.TimeSystem;
      }
      if (other.TotalCpuTime != 0UL) {
        TotalCpuTime = other.TotalCpuTime;
      }
      if (other.IdleCpuTime != 0UL) {
        IdleCpuTime = other.IdleCpuTime;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 9: {
            TimeElapsed = input.ReadDouble();
            break;
          }
          case 17: {
            TimeUser = input.ReadDouble();
            break;
          }
          case 25: {
            TimeSystem = input.ReadDouble();
            break;
          }
          case 32: {
            TotalCpuTime = input.ReadUInt64();
            break;
          }
          case 40: {
            IdleCpuTime = input.ReadUInt64();
            break;
          }
        }
      }
    }

  }

  /// <summary>
  ///  Histogram params based on grpc/support/histogram.c
  /// </summary>
  public sealed partial class HistogramParams : pb::IMessage<HistogramParams> {
    private static readonly pb::MessageParser<HistogramParams> _parser = new pb::MessageParser<HistogramParams>(() => new HistogramParams());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<HistogramParams> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Grpc.Testing.StatsReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public HistogramParams() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public HistogramParams(HistogramParams other) : this() {
      resolution_ = other.resolution_;
      maxPossible_ = other.maxPossible_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public HistogramParams Clone() {
      return new HistogramParams(this);
    }

    /// <summary>Field number for the "resolution" field.</summary>
    public const int ResolutionFieldNumber = 1;
    private double resolution_;
    /// <summary>
    ///  first bucket is [0, 1 + resolution)
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double Resolution {
      get { return resolution_; }
      set {
        resolution_ = value;
      }
    }

    /// <summary>Field number for the "max_possible" field.</summary>
    public const int MaxPossibleFieldNumber = 2;
    private double maxPossible_;
    /// <summary>
    ///  use enough buckets to allow this value
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double MaxPossible {
      get { return maxPossible_; }
      set {
        maxPossible_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as HistogramParams);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(HistogramParams other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Resolution != other.Resolution) return false;
      if (MaxPossible != other.MaxPossible) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Resolution != 0D) hash ^= Resolution.GetHashCode();
      if (MaxPossible != 0D) hash ^= MaxPossible.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Resolution != 0D) {
        output.WriteRawTag(9);
        output.WriteDouble(Resolution);
      }
      if (MaxPossible != 0D) {
        output.WriteRawTag(17);
        output.WriteDouble(MaxPossible);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Resolution != 0D) {
        size += 1 + 8;
      }
      if (MaxPossible != 0D) {
        size += 1 + 8;
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(HistogramParams other) {
      if (other == null) {
        return;
      }
      if (other.Resolution != 0D) {
        Resolution = other.Resolution;
      }
      if (other.MaxPossible != 0D) {
        MaxPossible = other.MaxPossible;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 9: {
            Resolution = input.ReadDouble();
            break;
          }
          case 17: {
            MaxPossible = input.ReadDouble();
            break;
          }
        }
      }
    }

  }

  /// <summary>
  ///  Histogram data based on grpc/support/histogram.c
  /// </summary>
  public sealed partial class HistogramData : pb::IMessage<HistogramData> {
    private static readonly pb::MessageParser<HistogramData> _parser = new pb::MessageParser<HistogramData>(() => new HistogramData());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<HistogramData> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Grpc.Testing.StatsReflection.Descriptor.MessageTypes[2]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public HistogramData() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public HistogramData(HistogramData other) : this() {
      bucket_ = other.bucket_.Clone();
      minSeen_ = other.minSeen_;
      maxSeen_ = other.maxSeen_;
      sum_ = other.sum_;
      sumOfSquares_ = other.sumOfSquares_;
      count_ = other.count_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public HistogramData Clone() {
      return new HistogramData(this);
    }

    /// <summary>Field number for the "bucket" field.</summary>
    public const int BucketFieldNumber = 1;
    private static readonly pb::FieldCodec<uint> _repeated_bucket_codec
        = pb::FieldCodec.ForUInt32(10);
    private readonly pbc::RepeatedField<uint> bucket_ = new pbc::RepeatedField<uint>();
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<uint> Bucket {
      get { return bucket_; }
    }

    /// <summary>Field number for the "min_seen" field.</summary>
    public const int MinSeenFieldNumber = 2;
    private double minSeen_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double MinSeen {
      get { return minSeen_; }
      set {
        minSeen_ = value;
      }
    }

    /// <summary>Field number for the "max_seen" field.</summary>
    public const int MaxSeenFieldNumber = 3;
    private double maxSeen_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double MaxSeen {
      get { return maxSeen_; }
      set {
        maxSeen_ = value;
      }
    }

    /// <summary>Field number for the "sum" field.</summary>
    public const int SumFieldNumber = 4;
    private double sum_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double Sum {
      get { return sum_; }
      set {
        sum_ = value;
      }
    }

    /// <summary>Field number for the "sum_of_squares" field.</summary>
    public const int SumOfSquaresFieldNumber = 5;
    private double sumOfSquares_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double SumOfSquares {
      get { return sumOfSquares_; }
      set {
        sumOfSquares_ = value;
      }
    }

    /// <summary>Field number for the "count" field.</summary>
    public const int CountFieldNumber = 6;
    private double count_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double Count {
      get { return count_; }
      set {
        count_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as HistogramData);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(HistogramData other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if(!bucket_.Equals(other.bucket_)) return false;
      if (MinSeen != other.MinSeen) return false;
      if (MaxSeen != other.MaxSeen) return false;
      if (Sum != other.Sum) return false;
      if (SumOfSquares != other.SumOfSquares) return false;
      if (Count != other.Count) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      hash ^= bucket_.GetHashCode();
      if (MinSeen != 0D) hash ^= MinSeen.GetHashCode();
      if (MaxSeen != 0D) hash ^= MaxSeen.GetHashCode();
      if (Sum != 0D) hash ^= Sum.GetHashCode();
      if (SumOfSquares != 0D) hash ^= SumOfSquares.GetHashCode();
      if (Count != 0D) hash ^= Count.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      bucket_.WriteTo(output, _repeated_bucket_codec);
      if (MinSeen != 0D) {
        output.WriteRawTag(17);
        output.WriteDouble(MinSeen);
      }
      if (MaxSeen != 0D) {
        output.WriteRawTag(25);
        output.WriteDouble(MaxSeen);
      }
      if (Sum != 0D) {
        output.WriteRawTag(33);
        output.WriteDouble(Sum);
      }
      if (SumOfSquares != 0D) {
        output.WriteRawTag(41);
        output.WriteDouble(SumOfSquares);
      }
      if (Count != 0D) {
        output.WriteRawTag(49);
        output.WriteDouble(Count);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      size += bucket_.CalculateSize(_repeated_bucket_codec);
      if (MinSeen != 0D) {
        size += 1 + 8;
      }
      if (MaxSeen != 0D) {
        size += 1 + 8;
      }
      if (Sum != 0D) {
        size += 1 + 8;
      }
      if (SumOfSquares != 0D) {
        size += 1 + 8;
      }
      if (Count != 0D) {
        size += 1 + 8;
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(HistogramData other) {
      if (other == null) {
        return;
      }
      bucket_.Add(other.bucket_);
      if (other.MinSeen != 0D) {
        MinSeen = other.MinSeen;
      }
      if (other.MaxSeen != 0D) {
        MaxSeen = other.MaxSeen;
      }
      if (other.Sum != 0D) {
        Sum = other.Sum;
      }
      if (other.SumOfSquares != 0D) {
        SumOfSquares = other.SumOfSquares;
      }
      if (other.Count != 0D) {
        Count = other.Count;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 10:
          case 8: {
            bucket_.AddEntriesFrom(input, _repeated_bucket_codec);
            break;
          }
          case 17: {
            MinSeen = input.ReadDouble();
            break;
          }
          case 25: {
            MaxSeen = input.ReadDouble();
            break;
          }
          case 33: {
            Sum = input.ReadDouble();
            break;
          }
          case 41: {
            SumOfSquares = input.ReadDouble();
            break;
          }
          case 49: {
            Count = input.ReadDouble();
            break;
          }
        }
      }
    }

  }

  public sealed partial class RequestResultCount : pb::IMessage<RequestResultCount> {
    private static readonly pb::MessageParser<RequestResultCount> _parser = new pb::MessageParser<RequestResultCount>(() => new RequestResultCount());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<RequestResultCount> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Grpc.Testing.StatsReflection.Descriptor.MessageTypes[3]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RequestResultCount() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RequestResultCount(RequestResultCount other) : this() {
      statusCode_ = other.statusCode_;
      count_ = other.count_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RequestResultCount Clone() {
      return new RequestResultCount(this);
    }

    /// <summary>Field number for the "status_code" field.</summary>
    public const int StatusCodeFieldNumber = 1;
    private int statusCode_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int StatusCode {
      get { return statusCode_; }
      set {
        statusCode_ = value;
      }
    }

    /// <summary>Field number for the "count" field.</summary>
    public const int CountFieldNumber = 2;
    private long count_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public long Count {
      get { return count_; }
      set {
        count_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as RequestResultCount);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(RequestResultCount other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (StatusCode != other.StatusCode) return false;
      if (Count != other.Count) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (StatusCode != 0) hash ^= StatusCode.GetHashCode();
      if (Count != 0L) hash ^= Count.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (StatusCode != 0) {
        output.WriteRawTag(8);
        output.WriteInt32(StatusCode);
      }
      if (Count != 0L) {
        output.WriteRawTag(16);
        output.WriteInt64(Count);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (StatusCode != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(StatusCode);
      }
      if (Count != 0L) {
        size += 1 + pb::CodedOutputStream.ComputeInt64Size(Count);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(RequestResultCount other) {
      if (other == null) {
        return;
      }
      if (other.StatusCode != 0) {
        StatusCode = other.StatusCode;
      }
      if (other.Count != 0L) {
        Count = other.Count;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            StatusCode = input.ReadInt32();
            break;
          }
          case 16: {
            Count = input.ReadInt64();
            break;
          }
        }
      }
    }

  }

  public sealed partial class ClientStats : pb::IMessage<ClientStats> {
    private static readonly pb::MessageParser<ClientStats> _parser = new pb::MessageParser<ClientStats>(() => new ClientStats());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ClientStats> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Grpc.Testing.StatsReflection.Descriptor.MessageTypes[4]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ClientStats() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ClientStats(ClientStats other) : this() {
      Latencies = other.latencies_ != null ? other.Latencies.Clone() : null;
      timeElapsed_ = other.timeElapsed_;
      timeUser_ = other.timeUser_;
      timeSystem_ = other.timeSystem_;
      requestResults_ = other.requestResults_.Clone();
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ClientStats Clone() {
      return new ClientStats(this);
    }

    /// <summary>Field number for the "latencies" field.</summary>
    public const int LatenciesFieldNumber = 1;
    private global::Grpc.Testing.HistogramData latencies_;
    /// <summary>
    ///  Latency histogram. Data points are in nanoseconds.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Grpc.Testing.HistogramData Latencies {
      get { return latencies_; }
      set {
        latencies_ = value;
      }
    }

    /// <summary>Field number for the "time_elapsed" field.</summary>
    public const int TimeElapsedFieldNumber = 2;
    private double timeElapsed_;
    /// <summary>
    ///  See ServerStats for details.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double TimeElapsed {
      get { return timeElapsed_; }
      set {
        timeElapsed_ = value;
      }
    }

    /// <summary>Field number for the "time_user" field.</summary>
    public const int TimeUserFieldNumber = 3;
    private double timeUser_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double TimeUser {
      get { return timeUser_; }
      set {
        timeUser_ = value;
      }
    }

    /// <summary>Field number for the "time_system" field.</summary>
    public const int TimeSystemFieldNumber = 4;
    private double timeSystem_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public double TimeSystem {
      get { return timeSystem_; }
      set {
        timeSystem_ = value;
      }
    }

    /// <summary>Field number for the "request_results" field.</summary>
    public const int RequestResultsFieldNumber = 5;
    private static readonly pb::FieldCodec<global::Grpc.Testing.RequestResultCount> _repeated_requestResults_codec
        = pb::FieldCodec.ForMessage(42, global::Grpc.Testing.RequestResultCount.Parser);
    private readonly pbc::RepeatedField<global::Grpc.Testing.RequestResultCount> requestResults_ = new pbc::RepeatedField<global::Grpc.Testing.RequestResultCount>();
    /// <summary>
    ///  Number of failed requests (one row per status code seen)
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pbc::RepeatedField<global::Grpc.Testing.RequestResultCount> RequestResults {
      get { return requestResults_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ClientStats);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ClientStats other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (!object.Equals(Latencies, other.Latencies)) return false;
      if (TimeElapsed != other.TimeElapsed) return false;
      if (TimeUser != other.TimeUser) return false;
      if (TimeSystem != other.TimeSystem) return false;
      if(!requestResults_.Equals(other.requestResults_)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (latencies_ != null) hash ^= Latencies.GetHashCode();
      if (TimeElapsed != 0D) hash ^= TimeElapsed.GetHashCode();
      if (TimeUser != 0D) hash ^= TimeUser.GetHashCode();
      if (TimeSystem != 0D) hash ^= TimeSystem.GetHashCode();
      hash ^= requestResults_.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (latencies_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(Latencies);
      }
      if (TimeElapsed != 0D) {
        output.WriteRawTag(17);
        output.WriteDouble(TimeElapsed);
      }
      if (TimeUser != 0D) {
        output.WriteRawTag(25);
        output.WriteDouble(TimeUser);
      }
      if (TimeSystem != 0D) {
        output.WriteRawTag(33);
        output.WriteDouble(TimeSystem);
      }
      requestResults_.WriteTo(output, _repeated_requestResults_codec);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (latencies_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Latencies);
      }
      if (TimeElapsed != 0D) {
        size += 1 + 8;
      }
      if (TimeUser != 0D) {
        size += 1 + 8;
      }
      if (TimeSystem != 0D) {
        size += 1 + 8;
      }
      size += requestResults_.CalculateSize(_repeated_requestResults_codec);
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ClientStats other) {
      if (other == null) {
        return;
      }
      if (other.latencies_ != null) {
        if (latencies_ == null) {
          latencies_ = new global::Grpc.Testing.HistogramData();
        }
        Latencies.MergeFrom(other.Latencies);
      }
      if (other.TimeElapsed != 0D) {
        TimeElapsed = other.TimeElapsed;
      }
      if (other.TimeUser != 0D) {
        TimeUser = other.TimeUser;
      }
      if (other.TimeSystem != 0D) {
        TimeSystem = other.TimeSystem;
      }
      requestResults_.Add(other.requestResults_);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 10: {
            if (latencies_ == null) {
              latencies_ = new global::Grpc.Testing.HistogramData();
            }
            input.ReadMessage(latencies_);
            break;
          }
          case 17: {
            TimeElapsed = input.ReadDouble();
            break;
          }
          case 25: {
            TimeUser = input.ReadDouble();
            break;
          }
          case 33: {
            TimeSystem = input.ReadDouble();
            break;
          }
          case 42: {
            requestResults_.AddEntriesFrom(input, _repeated_requestResults_codec);
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
