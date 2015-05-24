// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: route_guide.proto

#import "GPBProtocolBuffers.h"

#if GOOGLE_PROTOBUF_OBJC_GEN_VERSION != 30000
#error This file was generated by a different version of protoc-gen-objc which is incompatible with your Protocol Buffer sources.
#endif

CF_EXTERN_C_BEGIN

@class RGDFeature;
@class RGDPoint;
@class RGDRectangle;
@class RGDRouteNote;
@class RGDRouteSummary;


#pragma mark - RGDRouteGuideRoot

@interface RGDRouteGuideRoot : GPBRootObject
@end

#pragma mark - RGDPoint

typedef GPB_ENUM(RGDPoint_FieldNumber) {
  RGDPoint_FieldNumber_Latitude = 1,
  RGDPoint_FieldNumber_Longitude = 2,
};

// Points are represented as latitude-longitude pairs in the E7 representation
// (degrees multiplied by 10**7 and rounded to the nearest integer).
// Latitudes should be in the range +/- 90 degrees and longitude should be in
// the range +/- 180 degrees (inclusive).
@interface RGDPoint : GPBMessage

@property(nonatomic, readwrite) int32_t latitude;

@property(nonatomic, readwrite) int32_t longitude;

@end

#pragma mark - RGDRectangle

typedef GPB_ENUM(RGDRectangle_FieldNumber) {
  RGDRectangle_FieldNumber_Lo = 1,
  RGDRectangle_FieldNumber_Hi = 2,
};

// A latitude-longitude rectangle, represented as two diagonally opposite
// points "lo" and "hi".
@interface RGDRectangle : GPBMessage

// One corner of the rectangle.
@property(nonatomic, readwrite) BOOL hasLo;
@property(nonatomic, readwrite, strong) RGDPoint *lo;

// The other corner of the rectangle.
@property(nonatomic, readwrite) BOOL hasHi;
@property(nonatomic, readwrite, strong) RGDPoint *hi;

@end

#pragma mark - RGDFeature

typedef GPB_ENUM(RGDFeature_FieldNumber) {
  RGDFeature_FieldNumber_Name = 1,
  RGDFeature_FieldNumber_Location = 2,
};

// A feature names something at a given point.
//
// If a feature could not be named, the name is empty.
@interface RGDFeature : GPBMessage

// The name of the feature.
@property(nonatomic, readwrite, copy) NSString *name;

// The point where the feature is detected.
@property(nonatomic, readwrite) BOOL hasLocation;
@property(nonatomic, readwrite, strong) RGDPoint *location;

@end

#pragma mark - RGDRouteNote

typedef GPB_ENUM(RGDRouteNote_FieldNumber) {
  RGDRouteNote_FieldNumber_Location = 1,
  RGDRouteNote_FieldNumber_Message = 2,
};

// A RouteNote is a message sent while at a given point.
@interface RGDRouteNote : GPBMessage

// The location from which the message is sent.
@property(nonatomic, readwrite) BOOL hasLocation;
@property(nonatomic, readwrite, strong) RGDPoint *location;

// The message to be sent.
@property(nonatomic, readwrite, copy) NSString *message;

@end

#pragma mark - RGDRouteSummary

typedef GPB_ENUM(RGDRouteSummary_FieldNumber) {
  RGDRouteSummary_FieldNumber_PointCount = 1,
  RGDRouteSummary_FieldNumber_FeatureCount = 2,
  RGDRouteSummary_FieldNumber_Distance = 3,
  RGDRouteSummary_FieldNumber_ElapsedTime = 4,
};

// A RouteSummary is received in response to a RecordRoute rpc.
//
// It contains the number of individual points received, the number of
// detected features, and the total distance covered as the cumulative sum of
// the distance between each point.
@interface RGDRouteSummary : GPBMessage

// The number of points received.
@property(nonatomic, readwrite) int32_t pointCount;

// The number of known features passed while traversing the route.
@property(nonatomic, readwrite) int32_t featureCount;

// The distance covered in metres.
@property(nonatomic, readwrite) int32_t distance;

// The duration of the traversal in seconds.
@property(nonatomic, readwrite) int32_t elapsedTime;

@end

CF_EXTERN_C_END
