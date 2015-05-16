// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: route_guide.proto

#import "GPBProtocolBuffers_RuntimeSupport.h"

#import "RouteGuide.pbobjc.h"

#pragma mark - RGDRouteGuideRoot

@implementation RGDRouteGuideRoot

@end

static GPBFileDescriptor *RGDRouteGuideRoot_FileDescriptor(void) {
  // This is called by +initialize so there is no need to worry
  // about thread safety of the singleton.
  static GPBFileDescriptor *descriptor = NULL;
  if (!descriptor) {
    descriptor = [[GPBFileDescriptor alloc] initWithPackage:@"grpc.example.routeguide"
                                                     syntax:GPBFileSyntaxProto3];
  }
  return descriptor;
}

#pragma mark - RGDPoint

@implementation RGDPoint

@dynamic latitude;
@dynamic longitude;

typedef struct RGDPoint_Storage {
  uint32_t _has_storage_[1];
  int32_t latitude;
  int32_t longitude;
} RGDPoint_Storage;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = NULL;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "latitude",
        .number = RGDPoint_FieldNumber_Latitude,
        .hasIndex = 0,
        .flags = GPBFieldOptional,
        .type = GPBTypeInt32,
        .offset = offsetof(RGDPoint_Storage, latitude),
        .defaultValue.valueInt32 = 0,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
      {
        .name = "longitude",
        .number = RGDPoint_FieldNumber_Longitude,
        .hasIndex = 1,
        .flags = GPBFieldOptional,
        .type = GPBTypeInt32,
        .offset = offsetof(RGDPoint_Storage, longitude),
        .defaultValue.valueInt32 = 0,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
    };
    descriptor = [GPBDescriptor allocDescriptorForClass:[RGDPoint class]
                                              rootClass:[RGDRouteGuideRoot class]
                                                   file:RGDRouteGuideRoot_FileDescriptor()
                                                 fields:fields
                                             fieldCount:sizeof(fields) / sizeof(GPBMessageFieldDescription)
                                                 oneofs:NULL
                                             oneofCount:0
                                                  enums:NULL
                                              enumCount:0
                                                 ranges:NULL
                                             rangeCount:0
                                            storageSize:sizeof(RGDPoint_Storage)
                                             wireFormat:NO];
  }
  return descriptor;
}

@end

#pragma mark - RGDRectangle

@implementation RGDRectangle

@dynamic hasLo, lo;
@dynamic hasHi, hi;

typedef struct RGDRectangle_Storage {
  uint32_t _has_storage_[1];
  RGDPoint *lo;
  RGDPoint *hi;
} RGDRectangle_Storage;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = NULL;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "lo",
        .number = RGDRectangle_FieldNumber_Lo,
        .hasIndex = 0,
        .flags = GPBFieldOptional,
        .type = GPBTypeMessage,
        .offset = offsetof(RGDRectangle_Storage, lo),
        .defaultValue.valueMessage = nil,
        .typeSpecific.className = GPBStringifySymbol(RGDPoint),
        .fieldOptions = NULL,
      },
      {
        .name = "hi",
        .number = RGDRectangle_FieldNumber_Hi,
        .hasIndex = 1,
        .flags = GPBFieldOptional,
        .type = GPBTypeMessage,
        .offset = offsetof(RGDRectangle_Storage, hi),
        .defaultValue.valueMessage = nil,
        .typeSpecific.className = GPBStringifySymbol(RGDPoint),
        .fieldOptions = NULL,
      },
    };
    descriptor = [GPBDescriptor allocDescriptorForClass:[RGDRectangle class]
                                              rootClass:[RGDRouteGuideRoot class]
                                                   file:RGDRouteGuideRoot_FileDescriptor()
                                                 fields:fields
                                             fieldCount:sizeof(fields) / sizeof(GPBMessageFieldDescription)
                                                 oneofs:NULL
                                             oneofCount:0
                                                  enums:NULL
                                              enumCount:0
                                                 ranges:NULL
                                             rangeCount:0
                                            storageSize:sizeof(RGDRectangle_Storage)
                                             wireFormat:NO];
  }
  return descriptor;
}

@end

#pragma mark - RGDFeature

@implementation RGDFeature

@dynamic name;
@dynamic hasLocation, location;

typedef struct RGDFeature_Storage {
  uint32_t _has_storage_[1];
  NSString *name;
  RGDPoint *location;
} RGDFeature_Storage;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = NULL;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "name",
        .number = RGDFeature_FieldNumber_Name,
        .hasIndex = 0,
        .flags = GPBFieldOptional,
        .type = GPBTypeString,
        .offset = offsetof(RGDFeature_Storage, name),
        .defaultValue.valueString = nil,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
      {
        .name = "location",
        .number = RGDFeature_FieldNumber_Location,
        .hasIndex = 1,
        .flags = GPBFieldOptional,
        .type = GPBTypeMessage,
        .offset = offsetof(RGDFeature_Storage, location),
        .defaultValue.valueMessage = nil,
        .typeSpecific.className = GPBStringifySymbol(RGDPoint),
        .fieldOptions = NULL,
      },
    };
    descriptor = [GPBDescriptor allocDescriptorForClass:[RGDFeature class]
                                              rootClass:[RGDRouteGuideRoot class]
                                                   file:RGDRouteGuideRoot_FileDescriptor()
                                                 fields:fields
                                             fieldCount:sizeof(fields) / sizeof(GPBMessageFieldDescription)
                                                 oneofs:NULL
                                             oneofCount:0
                                                  enums:NULL
                                              enumCount:0
                                                 ranges:NULL
                                             rangeCount:0
                                            storageSize:sizeof(RGDFeature_Storage)
                                             wireFormat:NO];
  }
  return descriptor;
}

@end

#pragma mark - RGDRouteNote

@implementation RGDRouteNote

@dynamic hasLocation, location;
@dynamic message;

typedef struct RGDRouteNote_Storage {
  uint32_t _has_storage_[1];
  RGDPoint *location;
  NSString *message;
} RGDRouteNote_Storage;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = NULL;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "location",
        .number = RGDRouteNote_FieldNumber_Location,
        .hasIndex = 0,
        .flags = GPBFieldOptional,
        .type = GPBTypeMessage,
        .offset = offsetof(RGDRouteNote_Storage, location),
        .defaultValue.valueMessage = nil,
        .typeSpecific.className = GPBStringifySymbol(RGDPoint),
        .fieldOptions = NULL,
      },
      {
        .name = "message",
        .number = RGDRouteNote_FieldNumber_Message,
        .hasIndex = 1,
        .flags = GPBFieldOptional,
        .type = GPBTypeString,
        .offset = offsetof(RGDRouteNote_Storage, message),
        .defaultValue.valueString = nil,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
    };
    descriptor = [GPBDescriptor allocDescriptorForClass:[RGDRouteNote class]
                                              rootClass:[RGDRouteGuideRoot class]
                                                   file:RGDRouteGuideRoot_FileDescriptor()
                                                 fields:fields
                                             fieldCount:sizeof(fields) / sizeof(GPBMessageFieldDescription)
                                                 oneofs:NULL
                                             oneofCount:0
                                                  enums:NULL
                                              enumCount:0
                                                 ranges:NULL
                                             rangeCount:0
                                            storageSize:sizeof(RGDRouteNote_Storage)
                                             wireFormat:NO];
  }
  return descriptor;
}

@end

#pragma mark - RGDRouteSummary

@implementation RGDRouteSummary

@dynamic pointCount;
@dynamic featureCount;
@dynamic distance;
@dynamic elapsedTime;

typedef struct RGDRouteSummary_Storage {
  uint32_t _has_storage_[1];
  int32_t pointCount;
  int32_t featureCount;
  int32_t distance;
  int32_t elapsedTime;
} RGDRouteSummary_Storage;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = NULL;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "pointCount",
        .number = RGDRouteSummary_FieldNumber_PointCount,
        .hasIndex = 0,
        .flags = GPBFieldOptional,
        .type = GPBTypeInt32,
        .offset = offsetof(RGDRouteSummary_Storage, pointCount),
        .defaultValue.valueInt32 = 0,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
      {
        .name = "featureCount",
        .number = RGDRouteSummary_FieldNumber_FeatureCount,
        .hasIndex = 1,
        .flags = GPBFieldOptional,
        .type = GPBTypeInt32,
        .offset = offsetof(RGDRouteSummary_Storage, featureCount),
        .defaultValue.valueInt32 = 0,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
      {
        .name = "distance",
        .number = RGDRouteSummary_FieldNumber_Distance,
        .hasIndex = 2,
        .flags = GPBFieldOptional,
        .type = GPBTypeInt32,
        .offset = offsetof(RGDRouteSummary_Storage, distance),
        .defaultValue.valueInt32 = 0,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
      {
        .name = "elapsedTime",
        .number = RGDRouteSummary_FieldNumber_ElapsedTime,
        .hasIndex = 3,
        .flags = GPBFieldOptional,
        .type = GPBTypeInt32,
        .offset = offsetof(RGDRouteSummary_Storage, elapsedTime),
        .defaultValue.valueInt32 = 0,
        .typeSpecific.className = NULL,
        .fieldOptions = NULL,
      },
    };
    descriptor = [GPBDescriptor allocDescriptorForClass:[RGDRouteSummary class]
                                              rootClass:[RGDRouteGuideRoot class]
                                                   file:RGDRouteGuideRoot_FileDescriptor()
                                                 fields:fields
                                             fieldCount:sizeof(fields) / sizeof(GPBMessageFieldDescription)
                                                 oneofs:NULL
                                             oneofCount:0
                                                  enums:NULL
                                              enumCount:0
                                                 ranges:NULL
                                             rangeCount:0
                                            storageSize:sizeof(RGDRouteSummary_Storage)
                                             wireFormat:NO];
  }
  return descriptor;
}

@end

