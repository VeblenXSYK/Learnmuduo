# Generated by the protocol buffer compiler.  DO NOT EDIT!

from google.protobuf import descriptor
from google.protobuf import message
from google.protobuf import reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)



DESCRIPTOR = descriptor.FileDescriptor(
  name='commweb.proto',
  package='commweb',
  serialized_pb='\n\rcommweb.proto\x12\x07\x63ommweb\"\"\n\x08\x41xleInfo\x12\n\n\x02\x66W\x18\x01 \x02(\x02\x12\n\n\x02\x66V\x18\x02 \x02(\x02\"\x99\x01\n\x0bVehicleInfo\x12\x0c\n\x04\x63\x44ir\x18\x01 \x01(\x05\x12\x12\n\ncAxleCount\x18\x02 \x02(\x05\x12\x0e\n\x06\x63Level\x18\x03 \x01(\x05\x12\r\n\x05nType\x18\x04 \x02(\x05\x12\n\n\x02\x66W\x18\x05 \x02(\x02\x12\n\n\x02\x66V\x18\x06 \x02(\x02\x12 \n\x05\x61Info\x18\x07 \x03(\x0b\x32\x11.commweb.AxleInfo\x12\x0f\n\x07\x63\x43olumn\x18\x08 \x01(\t\"\x17\n\x07\x43mdInfo\x12\x0c\n\x04type\x18\x01 \x02(\x05')




_AXLEINFO = descriptor.Descriptor(
  name='AxleInfo',
  full_name='commweb.AxleInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='fW', full_name='commweb.AxleInfo.fW', index=0,
      number=1, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='fV', full_name='commweb.AxleInfo.fV', index=1,
      number=2, type=2, cpp_type=6, label=2,
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
  serialized_start=26,
  serialized_end=60,
)


_VEHICLEINFO = descriptor.Descriptor(
  name='VehicleInfo',
  full_name='commweb.VehicleInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='cDir', full_name='commweb.VehicleInfo.cDir', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='cAxleCount', full_name='commweb.VehicleInfo.cAxleCount', index=1,
      number=2, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='cLevel', full_name='commweb.VehicleInfo.cLevel', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='nType', full_name='commweb.VehicleInfo.nType', index=3,
      number=4, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='fW', full_name='commweb.VehicleInfo.fW', index=4,
      number=5, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='fV', full_name='commweb.VehicleInfo.fV', index=5,
      number=6, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='aInfo', full_name='commweb.VehicleInfo.aInfo', index=6,
      number=7, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='cColumn', full_name='commweb.VehicleInfo.cColumn', index=7,
      number=8, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=unicode("", "utf-8"),
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
  serialized_start=63,
  serialized_end=216,
)


_CMDINFO = descriptor.Descriptor(
  name='CmdInfo',
  full_name='commweb.CmdInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='type', full_name='commweb.CmdInfo.type', index=0,
      number=1, type=5, cpp_type=1, label=2,
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
  serialized_start=218,
  serialized_end=241,
)

_VEHICLEINFO.fields_by_name['aInfo'].message_type = _AXLEINFO
DESCRIPTOR.message_types_by_name['AxleInfo'] = _AXLEINFO
DESCRIPTOR.message_types_by_name['VehicleInfo'] = _VEHICLEINFO
DESCRIPTOR.message_types_by_name['CmdInfo'] = _CMDINFO

class AxleInfo(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _AXLEINFO
  
  # @@protoc_insertion_point(class_scope:commweb.AxleInfo)

class VehicleInfo(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _VEHICLEINFO
  
  # @@protoc_insertion_point(class_scope:commweb.VehicleInfo)

class CmdInfo(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _CMDINFO
  
  # @@protoc_insertion_point(class_scope:commweb.CmdInfo)

# @@protoc_insertion_point(module_scope)
