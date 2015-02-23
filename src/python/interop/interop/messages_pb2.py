# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: test/cpp/interop/messages.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='test/cpp/interop/messages.proto',
  package='grpc.testing',
  serialized_pb=_b('\n\x1ftest/cpp/interop/messages.proto\x12\x0cgrpc.testing\"@\n\x07Payload\x12\'\n\x04type\x18\x01 \x01(\x0e\x32\x19.grpc.testing.PayloadType\x12\x0c\n\x04\x62ody\x18\x02 \x01(\x0c\"\xb1\x01\n\rSimpleRequest\x12\x30\n\rresponse_type\x18\x01 \x01(\x0e\x32\x19.grpc.testing.PayloadType\x12\x15\n\rresponse_size\x18\x02 \x01(\x05\x12&\n\x07payload\x18\x03 \x01(\x0b\x32\x15.grpc.testing.Payload\x12\x15\n\rfill_username\x18\x04 \x01(\x08\x12\x18\n\x10\x66ill_oauth_scope\x18\x05 \x01(\x08\"_\n\x0eSimpleResponse\x12&\n\x07payload\x18\x01 \x01(\x0b\x32\x15.grpc.testing.Payload\x12\x10\n\x08username\x18\x02 \x01(\t\x12\x13\n\x0boauth_scope\x18\x03 \x01(\t\"C\n\x19StreamingInputCallRequest\x12&\n\x07payload\x18\x01 \x01(\x0b\x32\x15.grpc.testing.Payload\"=\n\x1aStreamingInputCallResponse\x12\x1f\n\x17\x61ggregated_payload_size\x18\x01 \x01(\x05\"7\n\x12ResponseParameters\x12\x0c\n\x04size\x18\x01 \x01(\x05\x12\x13\n\x0binterval_us\x18\x02 \x01(\x05\"\xb5\x01\n\x1aStreamingOutputCallRequest\x12\x30\n\rresponse_type\x18\x01 \x01(\x0e\x32\x19.grpc.testing.PayloadType\x12=\n\x13response_parameters\x18\x02 \x03(\x0b\x32 .grpc.testing.ResponseParameters\x12&\n\x07payload\x18\x03 \x01(\x0b\x32\x15.grpc.testing.Payload\"E\n\x1bStreamingOutputCallResponse\x12&\n\x07payload\x18\x01 \x01(\x0b\x32\x15.grpc.testing.Payload*?\n\x0bPayloadType\x12\x10\n\x0c\x43OMPRESSABLE\x10\x00\x12\x12\n\x0eUNCOMPRESSABLE\x10\x01\x12\n\n\x06RANDOM\x10\x02')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

_PAYLOADTYPE = _descriptor.EnumDescriptor(
  name='PayloadType',
  full_name='grpc.testing.PayloadType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='COMPRESSABLE', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='UNCOMPRESSABLE', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='RANDOM', index=2, number=2,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=836,
  serialized_end=899,
)
_sym_db.RegisterEnumDescriptor(_PAYLOADTYPE)

PayloadType = enum_type_wrapper.EnumTypeWrapper(_PAYLOADTYPE)
COMPRESSABLE = 0
UNCOMPRESSABLE = 1
RANDOM = 2



_PAYLOAD = _descriptor.Descriptor(
  name='Payload',
  full_name='grpc.testing.Payload',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='type', full_name='grpc.testing.Payload.type', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='body', full_name='grpc.testing.Payload.body', index=1,
      number=2, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=49,
  serialized_end=113,
)


_SIMPLEREQUEST = _descriptor.Descriptor(
  name='SimpleRequest',
  full_name='grpc.testing.SimpleRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='response_type', full_name='grpc.testing.SimpleRequest.response_type', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='response_size', full_name='grpc.testing.SimpleRequest.response_size', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='payload', full_name='grpc.testing.SimpleRequest.payload', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='fill_username', full_name='grpc.testing.SimpleRequest.fill_username', index=3,
      number=4, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='fill_oauth_scope', full_name='grpc.testing.SimpleRequest.fill_oauth_scope', index=4,
      number=5, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=116,
  serialized_end=293,
)


_SIMPLERESPONSE = _descriptor.Descriptor(
  name='SimpleResponse',
  full_name='grpc.testing.SimpleResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='payload', full_name='grpc.testing.SimpleResponse.payload', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='username', full_name='grpc.testing.SimpleResponse.username', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='oauth_scope', full_name='grpc.testing.SimpleResponse.oauth_scope', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=295,
  serialized_end=390,
)


_STREAMINGINPUTCALLREQUEST = _descriptor.Descriptor(
  name='StreamingInputCallRequest',
  full_name='grpc.testing.StreamingInputCallRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='payload', full_name='grpc.testing.StreamingInputCallRequest.payload', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=392,
  serialized_end=459,
)


_STREAMINGINPUTCALLRESPONSE = _descriptor.Descriptor(
  name='StreamingInputCallResponse',
  full_name='grpc.testing.StreamingInputCallResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='aggregated_payload_size', full_name='grpc.testing.StreamingInputCallResponse.aggregated_payload_size', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=461,
  serialized_end=522,
)


_RESPONSEPARAMETERS = _descriptor.Descriptor(
  name='ResponseParameters',
  full_name='grpc.testing.ResponseParameters',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='size', full_name='grpc.testing.ResponseParameters.size', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='interval_us', full_name='grpc.testing.ResponseParameters.interval_us', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=524,
  serialized_end=579,
)


_STREAMINGOUTPUTCALLREQUEST = _descriptor.Descriptor(
  name='StreamingOutputCallRequest',
  full_name='grpc.testing.StreamingOutputCallRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='response_type', full_name='grpc.testing.StreamingOutputCallRequest.response_type', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='response_parameters', full_name='grpc.testing.StreamingOutputCallRequest.response_parameters', index=1,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='payload', full_name='grpc.testing.StreamingOutputCallRequest.payload', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=582,
  serialized_end=763,
)


_STREAMINGOUTPUTCALLRESPONSE = _descriptor.Descriptor(
  name='StreamingOutputCallResponse',
  full_name='grpc.testing.StreamingOutputCallResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='payload', full_name='grpc.testing.StreamingOutputCallResponse.payload', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=765,
  serialized_end=834,
)

_PAYLOAD.fields_by_name['type'].enum_type = _PAYLOADTYPE
_SIMPLEREQUEST.fields_by_name['response_type'].enum_type = _PAYLOADTYPE
_SIMPLEREQUEST.fields_by_name['payload'].message_type = _PAYLOAD
_SIMPLERESPONSE.fields_by_name['payload'].message_type = _PAYLOAD
_STREAMINGINPUTCALLREQUEST.fields_by_name['payload'].message_type = _PAYLOAD
_STREAMINGOUTPUTCALLREQUEST.fields_by_name['response_type'].enum_type = _PAYLOADTYPE
_STREAMINGOUTPUTCALLREQUEST.fields_by_name['response_parameters'].message_type = _RESPONSEPARAMETERS
_STREAMINGOUTPUTCALLREQUEST.fields_by_name['payload'].message_type = _PAYLOAD
_STREAMINGOUTPUTCALLRESPONSE.fields_by_name['payload'].message_type = _PAYLOAD
DESCRIPTOR.message_types_by_name['Payload'] = _PAYLOAD
DESCRIPTOR.message_types_by_name['SimpleRequest'] = _SIMPLEREQUEST
DESCRIPTOR.message_types_by_name['SimpleResponse'] = _SIMPLERESPONSE
DESCRIPTOR.message_types_by_name['StreamingInputCallRequest'] = _STREAMINGINPUTCALLREQUEST
DESCRIPTOR.message_types_by_name['StreamingInputCallResponse'] = _STREAMINGINPUTCALLRESPONSE
DESCRIPTOR.message_types_by_name['ResponseParameters'] = _RESPONSEPARAMETERS
DESCRIPTOR.message_types_by_name['StreamingOutputCallRequest'] = _STREAMINGOUTPUTCALLREQUEST
DESCRIPTOR.message_types_by_name['StreamingOutputCallResponse'] = _STREAMINGOUTPUTCALLRESPONSE
DESCRIPTOR.enum_types_by_name['PayloadType'] = _PAYLOADTYPE

Payload = _reflection.GeneratedProtocolMessageType('Payload', (_message.Message,), dict(
  DESCRIPTOR = _PAYLOAD,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.Payload)
  ))
_sym_db.RegisterMessage(Payload)

SimpleRequest = _reflection.GeneratedProtocolMessageType('SimpleRequest', (_message.Message,), dict(
  DESCRIPTOR = _SIMPLEREQUEST,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.SimpleRequest)
  ))
_sym_db.RegisterMessage(SimpleRequest)

SimpleResponse = _reflection.GeneratedProtocolMessageType('SimpleResponse', (_message.Message,), dict(
  DESCRIPTOR = _SIMPLERESPONSE,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.SimpleResponse)
  ))
_sym_db.RegisterMessage(SimpleResponse)

StreamingInputCallRequest = _reflection.GeneratedProtocolMessageType('StreamingInputCallRequest', (_message.Message,), dict(
  DESCRIPTOR = _STREAMINGINPUTCALLREQUEST,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.StreamingInputCallRequest)
  ))
_sym_db.RegisterMessage(StreamingInputCallRequest)

StreamingInputCallResponse = _reflection.GeneratedProtocolMessageType('StreamingInputCallResponse', (_message.Message,), dict(
  DESCRIPTOR = _STREAMINGINPUTCALLRESPONSE,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.StreamingInputCallResponse)
  ))
_sym_db.RegisterMessage(StreamingInputCallResponse)

ResponseParameters = _reflection.GeneratedProtocolMessageType('ResponseParameters', (_message.Message,), dict(
  DESCRIPTOR = _RESPONSEPARAMETERS,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.ResponseParameters)
  ))
_sym_db.RegisterMessage(ResponseParameters)

StreamingOutputCallRequest = _reflection.GeneratedProtocolMessageType('StreamingOutputCallRequest', (_message.Message,), dict(
  DESCRIPTOR = _STREAMINGOUTPUTCALLREQUEST,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.StreamingOutputCallRequest)
  ))
_sym_db.RegisterMessage(StreamingOutputCallRequest)

StreamingOutputCallResponse = _reflection.GeneratedProtocolMessageType('StreamingOutputCallResponse', (_message.Message,), dict(
  DESCRIPTOR = _STREAMINGOUTPUTCALLRESPONSE,
  __module__ = 'test.cpp.interop.messages_pb2'
  # @@protoc_insertion_point(class_scope:grpc.testing.StreamingOutputCallResponse)
  ))
_sym_db.RegisterMessage(StreamingOutputCallResponse)


from grpc.framework.face import demonstration as _face_testing
from grpc.framework.face import interfaces as _face_interfaces
# @@protoc_insertion_point(module_scope)
