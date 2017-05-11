// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: route_guide.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Routeguide {

  /// <summary>Holder for reflection information generated from route_guide.proto</summary>
  public static partial class RouteGuideReflection {

    #region Descriptor
    /// <summary>File descriptor for route_guide.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static RouteGuideReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "ChFyb3V0ZV9ndWlkZS5wcm90bxIKcm91dGVndWlkZSIsCgVQb2ludBIQCghs",
            "YXRpdHVkZRgBIAEoBRIRCglsb25naXR1ZGUYAiABKAUiSQoJUmVjdGFuZ2xl",
            "Eh0KAmxvGAEgASgLMhEucm91dGVndWlkZS5Qb2ludBIdCgJoaRgCIAEoCzIR",
            "LnJvdXRlZ3VpZGUuUG9pbnQiPAoHRmVhdHVyZRIMCgRuYW1lGAEgASgJEiMK",
            "CGxvY2F0aW9uGAIgASgLMhEucm91dGVndWlkZS5Qb2ludCJBCglSb3V0ZU5v",
            "dGUSIwoIbG9jYXRpb24YASABKAsyES5yb3V0ZWd1aWRlLlBvaW50Eg8KB21l",
            "c3NhZ2UYAiABKAkiYgoMUm91dGVTdW1tYXJ5EhMKC3BvaW50X2NvdW50GAEg",
            "ASgFEhUKDWZlYXR1cmVfY291bnQYAiABKAUSEAoIZGlzdGFuY2UYAyABKAUS",
            "FAoMZWxhcHNlZF90aW1lGAQgASgFMoUCCgpSb3V0ZUd1aWRlEjYKCkdldEZl",
            "YXR1cmUSES5yb3V0ZWd1aWRlLlBvaW50GhMucm91dGVndWlkZS5GZWF0dXJl",
            "IgASPgoMTGlzdEZlYXR1cmVzEhUucm91dGVndWlkZS5SZWN0YW5nbGUaEy5y",
            "b3V0ZWd1aWRlLkZlYXR1cmUiADABEj4KC1JlY29yZFJvdXRlEhEucm91dGVn",
            "dWlkZS5Qb2ludBoYLnJvdXRlZ3VpZGUuUm91dGVTdW1tYXJ5IgAoARI/CglS",
            "b3V0ZUNoYXQSFS5yb3V0ZWd1aWRlLlJvdXRlTm90ZRoVLnJvdXRlZ3VpZGUu",
            "Um91dGVOb3RlIgAoATABQjYKG2lvLmdycGMuZXhhbXBsZXMucm91dGVndWlk",
            "ZUIPUm91dGVHdWlkZVByb3RvUAGiAgNSVEdiBnByb3RvMw=="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Routeguide.Point), global::Routeguide.Point.Parser, new[]{ "Latitude", "Longitude" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Routeguide.Rectangle), global::Routeguide.Rectangle.Parser, new[]{ "Lo", "Hi" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Routeguide.Feature), global::Routeguide.Feature.Parser, new[]{ "Name", "Location" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Routeguide.RouteNote), global::Routeguide.RouteNote.Parser, new[]{ "Location", "Message" }, null, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Routeguide.RouteSummary), global::Routeguide.RouteSummary.Parser, new[]{ "PointCount", "FeatureCount", "Distance", "ElapsedTime" }, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  /// <summary>
  /// Points are represented as latitude-longitude pairs in the E7 representation
  /// (degrees multiplied by 10**7 and rounded to the nearest integer).
  /// Latitudes should be in the range +/- 90 degrees and longitude should be in
  /// the range +/- 180 degrees (inclusive).
  /// </summary>
  public sealed partial class Point : pb::IMessage<Point> {
    private static readonly pb::MessageParser<Point> _parser = new pb::MessageParser<Point>(() => new Point());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Point> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Routeguide.RouteGuideReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point(Point other) : this() {
      latitude_ = other.latitude_;
      longitude_ = other.longitude_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point Clone() {
      return new Point(this);
    }

    /// <summary>Field number for the "latitude" field.</summary>
    public const int LatitudeFieldNumber = 1;
    private int latitude_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Latitude {
      get { return latitude_; }
      set {
        latitude_ = value;
      }
    }

    /// <summary>Field number for the "longitude" field.</summary>
    public const int LongitudeFieldNumber = 2;
    private int longitude_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Longitude {
      get { return longitude_; }
      set {
        longitude_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Point);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Point other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Latitude != other.Latitude) return false;
      if (Longitude != other.Longitude) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Latitude != 0) hash ^= Latitude.GetHashCode();
      if (Longitude != 0) hash ^= Longitude.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Latitude != 0) {
        output.WriteRawTag(8);
        output.WriteInt32(Latitude);
      }
      if (Longitude != 0) {
        output.WriteRawTag(16);
        output.WriteInt32(Longitude);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Latitude != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Latitude);
      }
      if (Longitude != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Longitude);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Point other) {
      if (other == null) {
        return;
      }
      if (other.Latitude != 0) {
        Latitude = other.Latitude;
      }
      if (other.Longitude != 0) {
        Longitude = other.Longitude;
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
            Latitude = input.ReadInt32();
            break;
          }
          case 16: {
            Longitude = input.ReadInt32();
            break;
          }
        }
      }
    }

  }

  /// <summary>
  /// A latitude-longitude rectangle, represented as two diagonally opposite
  /// points "lo" and "hi".
  /// </summary>
  public sealed partial class Rectangle : pb::IMessage<Rectangle> {
    private static readonly pb::MessageParser<Rectangle> _parser = new pb::MessageParser<Rectangle>(() => new Rectangle());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Rectangle> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Routeguide.RouteGuideReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Rectangle() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Rectangle(Rectangle other) : this() {
      Lo = other.lo_ != null ? other.Lo.Clone() : null;
      Hi = other.hi_ != null ? other.Hi.Clone() : null;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Rectangle Clone() {
      return new Rectangle(this);
    }

    /// <summary>Field number for the "lo" field.</summary>
    public const int LoFieldNumber = 1;
    private global::Routeguide.Point lo_;
    /// <summary>
    /// One corner of the rectangle.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Routeguide.Point Lo {
      get { return lo_; }
      set {
        lo_ = value;
      }
    }

    /// <summary>Field number for the "hi" field.</summary>
    public const int HiFieldNumber = 2;
    private global::Routeguide.Point hi_;
    /// <summary>
    /// The other corner of the rectangle.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Routeguide.Point Hi {
      get { return hi_; }
      set {
        hi_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Rectangle);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Rectangle other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (!object.Equals(Lo, other.Lo)) return false;
      if (!object.Equals(Hi, other.Hi)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (lo_ != null) hash ^= Lo.GetHashCode();
      if (hi_ != null) hash ^= Hi.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (lo_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(Lo);
      }
      if (hi_ != null) {
        output.WriteRawTag(18);
        output.WriteMessage(Hi);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (lo_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Lo);
      }
      if (hi_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Hi);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Rectangle other) {
      if (other == null) {
        return;
      }
      if (other.lo_ != null) {
        if (lo_ == null) {
          lo_ = new global::Routeguide.Point();
        }
        Lo.MergeFrom(other.Lo);
      }
      if (other.hi_ != null) {
        if (hi_ == null) {
          hi_ = new global::Routeguide.Point();
        }
        Hi.MergeFrom(other.Hi);
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
          case 10: {
            if (lo_ == null) {
              lo_ = new global::Routeguide.Point();
            }
            input.ReadMessage(lo_);
            break;
          }
          case 18: {
            if (hi_ == null) {
              hi_ = new global::Routeguide.Point();
            }
            input.ReadMessage(hi_);
            break;
          }
        }
      }
    }

  }

  /// <summary>
  /// A feature names something at a given point.
  ///
  /// If a feature could not be named, the name is empty.
  /// </summary>
  public sealed partial class Feature : pb::IMessage<Feature> {
    private static readonly pb::MessageParser<Feature> _parser = new pb::MessageParser<Feature>(() => new Feature());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Feature> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Routeguide.RouteGuideReflection.Descriptor.MessageTypes[2]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Feature() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Feature(Feature other) : this() {
      name_ = other.name_;
      Location = other.location_ != null ? other.Location.Clone() : null;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Feature Clone() {
      return new Feature(this);
    }

    /// <summary>Field number for the "name" field.</summary>
    public const int NameFieldNumber = 1;
    private string name_ = "";
    /// <summary>
    /// The name of the feature.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Name {
      get { return name_; }
      set {
        name_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    /// <summary>Field number for the "location" field.</summary>
    public const int LocationFieldNumber = 2;
    private global::Routeguide.Point location_;
    /// <summary>
    /// The point where the feature is detected.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Routeguide.Point Location {
      get { return location_; }
      set {
        location_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Feature);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Feature other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Name != other.Name) return false;
      if (!object.Equals(Location, other.Location)) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Name.Length != 0) hash ^= Name.GetHashCode();
      if (location_ != null) hash ^= Location.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Name.Length != 0) {
        output.WriteRawTag(10);
        output.WriteString(Name);
      }
      if (location_ != null) {
        output.WriteRawTag(18);
        output.WriteMessage(Location);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Name.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Name);
      }
      if (location_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Location);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Feature other) {
      if (other == null) {
        return;
      }
      if (other.Name.Length != 0) {
        Name = other.Name;
      }
      if (other.location_ != null) {
        if (location_ == null) {
          location_ = new global::Routeguide.Point();
        }
        Location.MergeFrom(other.Location);
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
          case 10: {
            Name = input.ReadString();
            break;
          }
          case 18: {
            if (location_ == null) {
              location_ = new global::Routeguide.Point();
            }
            input.ReadMessage(location_);
            break;
          }
        }
      }
    }

  }

  /// <summary>
  /// A RouteNote is a message sent while at a given point.
  /// </summary>
  public sealed partial class RouteNote : pb::IMessage<RouteNote> {
    private static readonly pb::MessageParser<RouteNote> _parser = new pb::MessageParser<RouteNote>(() => new RouteNote());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<RouteNote> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Routeguide.RouteGuideReflection.Descriptor.MessageTypes[3]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RouteNote() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RouteNote(RouteNote other) : this() {
      Location = other.location_ != null ? other.Location.Clone() : null;
      message_ = other.message_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RouteNote Clone() {
      return new RouteNote(this);
    }

    /// <summary>Field number for the "location" field.</summary>
    public const int LocationFieldNumber = 1;
    private global::Routeguide.Point location_;
    /// <summary>
    /// The location from which the message is sent.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Routeguide.Point Location {
      get { return location_; }
      set {
        location_ = value;
      }
    }

    /// <summary>Field number for the "message" field.</summary>
    public const int MessageFieldNumber = 2;
    private string message_ = "";
    /// <summary>
    /// The message to be sent.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Message {
      get { return message_; }
      set {
        message_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as RouteNote);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(RouteNote other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (!object.Equals(Location, other.Location)) return false;
      if (Message != other.Message) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (location_ != null) hash ^= Location.GetHashCode();
      if (Message.Length != 0) hash ^= Message.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (location_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(Location);
      }
      if (Message.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(Message);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (location_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Location);
      }
      if (Message.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Message);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(RouteNote other) {
      if (other == null) {
        return;
      }
      if (other.location_ != null) {
        if (location_ == null) {
          location_ = new global::Routeguide.Point();
        }
        Location.MergeFrom(other.Location);
      }
      if (other.Message.Length != 0) {
        Message = other.Message;
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
          case 10: {
            if (location_ == null) {
              location_ = new global::Routeguide.Point();
            }
            input.ReadMessage(location_);
            break;
          }
          case 18: {
            Message = input.ReadString();
            break;
          }
        }
      }
    }

  }

  /// <summary>
  /// A RouteSummary is received in response to a RecordRoute rpc.
  ///
  /// It contains the number of individual points received, the number of
  /// detected features, and the total distance covered as the cumulative sum of
  /// the distance between each point.
  /// </summary>
  public sealed partial class RouteSummary : pb::IMessage<RouteSummary> {
    private static readonly pb::MessageParser<RouteSummary> _parser = new pb::MessageParser<RouteSummary>(() => new RouteSummary());
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<RouteSummary> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Routeguide.RouteGuideReflection.Descriptor.MessageTypes[4]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RouteSummary() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RouteSummary(RouteSummary other) : this() {
      pointCount_ = other.pointCount_;
      featureCount_ = other.featureCount_;
      distance_ = other.distance_;
      elapsedTime_ = other.elapsedTime_;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public RouteSummary Clone() {
      return new RouteSummary(this);
    }

    /// <summary>Field number for the "point_count" field.</summary>
    public const int PointCountFieldNumber = 1;
    private int pointCount_;
    /// <summary>
    /// The number of points received.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int PointCount {
      get { return pointCount_; }
      set {
        pointCount_ = value;
      }
    }

    /// <summary>Field number for the "feature_count" field.</summary>
    public const int FeatureCountFieldNumber = 2;
    private int featureCount_;
    /// <summary>
    /// The number of known features passed while traversing the route.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int FeatureCount {
      get { return featureCount_; }
      set {
        featureCount_ = value;
      }
    }

    /// <summary>Field number for the "distance" field.</summary>
    public const int DistanceFieldNumber = 3;
    private int distance_;
    /// <summary>
    /// The distance covered in metres.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Distance {
      get { return distance_; }
      set {
        distance_ = value;
      }
    }

    /// <summary>Field number for the "elapsed_time" field.</summary>
    public const int ElapsedTimeFieldNumber = 4;
    private int elapsedTime_;
    /// <summary>
    /// The duration of the traversal in seconds.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int ElapsedTime {
      get { return elapsedTime_; }
      set {
        elapsedTime_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as RouteSummary);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(RouteSummary other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (PointCount != other.PointCount) return false;
      if (FeatureCount != other.FeatureCount) return false;
      if (Distance != other.Distance) return false;
      if (ElapsedTime != other.ElapsedTime) return false;
      return true;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (PointCount != 0) hash ^= PointCount.GetHashCode();
      if (FeatureCount != 0) hash ^= FeatureCount.GetHashCode();
      if (Distance != 0) hash ^= Distance.GetHashCode();
      if (ElapsedTime != 0) hash ^= ElapsedTime.GetHashCode();
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (PointCount != 0) {
        output.WriteRawTag(8);
        output.WriteInt32(PointCount);
      }
      if (FeatureCount != 0) {
        output.WriteRawTag(16);
        output.WriteInt32(FeatureCount);
      }
      if (Distance != 0) {
        output.WriteRawTag(24);
        output.WriteInt32(Distance);
      }
      if (ElapsedTime != 0) {
        output.WriteRawTag(32);
        output.WriteInt32(ElapsedTime);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (PointCount != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(PointCount);
      }
      if (FeatureCount != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(FeatureCount);
      }
      if (Distance != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Distance);
      }
      if (ElapsedTime != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(ElapsedTime);
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(RouteSummary other) {
      if (other == null) {
        return;
      }
      if (other.PointCount != 0) {
        PointCount = other.PointCount;
      }
      if (other.FeatureCount != 0) {
        FeatureCount = other.FeatureCount;
      }
      if (other.Distance != 0) {
        Distance = other.Distance;
      }
      if (other.ElapsedTime != 0) {
        ElapsedTime = other.ElapsedTime;
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
            PointCount = input.ReadInt32();
            break;
          }
          case 16: {
            FeatureCount = input.ReadInt32();
            break;
          }
          case 24: {
            Distance = input.ReadInt32();
            break;
          }
          case 32: {
            ElapsedTime = input.ReadInt32();
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
