// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "commweb.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace commweb {

namespace {

const ::google::protobuf::Descriptor* AxleInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AxleInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* VehicleInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  VehicleInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* CmdInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CmdInfo_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_commweb_2eproto() {
  protobuf_AddDesc_commweb_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "commweb.proto");
  GOOGLE_CHECK(file != NULL);
  AxleInfo_descriptor_ = file->message_type(0);
  static const int AxleInfo_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AxleInfo, fw_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AxleInfo, fv_),
  };
  AxleInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AxleInfo_descriptor_,
      AxleInfo::default_instance_,
      AxleInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AxleInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AxleInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AxleInfo));
  VehicleInfo_descriptor_ = file->message_type(1);
  static const int VehicleInfo_offsets_[8] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, cdir_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, caxlecount_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, clevel_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, ntype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, fw_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, fv_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, ainfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, ccolumn_),
  };
  VehicleInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      VehicleInfo_descriptor_,
      VehicleInfo::default_instance_,
      VehicleInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(VehicleInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(VehicleInfo));
  CmdInfo_descriptor_ = file->message_type(2);
  static const int CmdInfo_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CmdInfo, type_),
  };
  CmdInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CmdInfo_descriptor_,
      CmdInfo::default_instance_,
      CmdInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CmdInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CmdInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CmdInfo));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_commweb_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AxleInfo_descriptor_, &AxleInfo::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    VehicleInfo_descriptor_, &VehicleInfo::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CmdInfo_descriptor_, &CmdInfo::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_commweb_2eproto() {
  delete AxleInfo::default_instance_;
  delete AxleInfo_reflection_;
  delete VehicleInfo::default_instance_;
  delete VehicleInfo_reflection_;
  delete CmdInfo::default_instance_;
  delete CmdInfo_reflection_;
}

void protobuf_AddDesc_commweb_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rcommweb.proto\022\007commweb\"\"\n\010AxleInfo\022\n\n\002"
    "fW\030\001 \002(\002\022\n\n\002fV\030\002 \002(\002\"\231\001\n\013VehicleInfo\022\014\n\004"
    "cDir\030\001 \001(\005\022\022\n\ncAxleCount\030\002 \002(\005\022\016\n\006cLevel"
    "\030\003 \001(\005\022\r\n\005nType\030\004 \002(\005\022\n\n\002fW\030\005 \002(\002\022\n\n\002fV\030"
    "\006 \002(\002\022 \n\005aInfo\030\007 \003(\0132\021.commweb.AxleInfo\022"
    "\017\n\007cColumn\030\010 \001(\t\"\027\n\007CmdInfo\022\014\n\004type\030\001 \002("
    "\005", 241);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "commweb.proto", &protobuf_RegisterTypes);
  AxleInfo::default_instance_ = new AxleInfo();
  VehicleInfo::default_instance_ = new VehicleInfo();
  CmdInfo::default_instance_ = new CmdInfo();
  AxleInfo::default_instance_->InitAsDefaultInstance();
  VehicleInfo::default_instance_->InitAsDefaultInstance();
  CmdInfo::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_commweb_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_commweb_2eproto {
  StaticDescriptorInitializer_commweb_2eproto() {
    protobuf_AddDesc_commweb_2eproto();
  }
} static_descriptor_initializer_commweb_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int AxleInfo::kFWFieldNumber;
const int AxleInfo::kFVFieldNumber;
#endif  // !_MSC_VER

AxleInfo::AxleInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void AxleInfo::InitAsDefaultInstance() {
}

AxleInfo::AxleInfo(const AxleInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void AxleInfo::SharedCtor() {
  _cached_size_ = 0;
  fw_ = 0;
  fv_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AxleInfo::~AxleInfo() {
  SharedDtor();
}

void AxleInfo::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AxleInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AxleInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AxleInfo_descriptor_;
}

const AxleInfo& AxleInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_commweb_2eproto();  return *default_instance_;
}

AxleInfo* AxleInfo::default_instance_ = NULL;

AxleInfo* AxleInfo::New() const {
  return new AxleInfo;
}

void AxleInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    fw_ = 0;
    fv_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AxleInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required float fW = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &fw_)));
          set_has_fw();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(21)) goto parse_fV;
        break;
      }
      
      // required float fV = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_fV:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &fv_)));
          set_has_fv();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void AxleInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required float fW = 1;
  if (has_fw()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(1, this->fw(), output);
  }
  
  // required float fV = 2;
  if (has_fv()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(2, this->fv(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* AxleInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required float fW = 1;
  if (has_fw()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(1, this->fw(), target);
  }
  
  // required float fV = 2;
  if (has_fv()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(2, this->fv(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int AxleInfo::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required float fW = 1;
    if (has_fw()) {
      total_size += 1 + 4;
    }
    
    // required float fV = 2;
    if (has_fv()) {
      total_size += 1 + 4;
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AxleInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AxleInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AxleInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AxleInfo::MergeFrom(const AxleInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_fw()) {
      set_fw(from.fw());
    }
    if (from.has_fv()) {
      set_fv(from.fv());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AxleInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AxleInfo::CopyFrom(const AxleInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AxleInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void AxleInfo::Swap(AxleInfo* other) {
  if (other != this) {
    std::swap(fw_, other->fw_);
    std::swap(fv_, other->fv_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AxleInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AxleInfo_descriptor_;
  metadata.reflection = AxleInfo_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int VehicleInfo::kCDirFieldNumber;
const int VehicleInfo::kCAxleCountFieldNumber;
const int VehicleInfo::kCLevelFieldNumber;
const int VehicleInfo::kNTypeFieldNumber;
const int VehicleInfo::kFWFieldNumber;
const int VehicleInfo::kFVFieldNumber;
const int VehicleInfo::kAInfoFieldNumber;
const int VehicleInfo::kCColumnFieldNumber;
#endif  // !_MSC_VER

VehicleInfo::VehicleInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void VehicleInfo::InitAsDefaultInstance() {
}

VehicleInfo::VehicleInfo(const VehicleInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void VehicleInfo::SharedCtor() {
  _cached_size_ = 0;
  cdir_ = 0;
  caxlecount_ = 0;
  clevel_ = 0;
  ntype_ = 0;
  fw_ = 0;
  fv_ = 0;
  ccolumn_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

VehicleInfo::~VehicleInfo() {
  SharedDtor();
}

void VehicleInfo::SharedDtor() {
  if (ccolumn_ != &::google::protobuf::internal::kEmptyString) {
    delete ccolumn_;
  }
  if (this != default_instance_) {
  }
}

void VehicleInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* VehicleInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return VehicleInfo_descriptor_;
}

const VehicleInfo& VehicleInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_commweb_2eproto();  return *default_instance_;
}

VehicleInfo* VehicleInfo::default_instance_ = NULL;

VehicleInfo* VehicleInfo::New() const {
  return new VehicleInfo;
}

void VehicleInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    cdir_ = 0;
    caxlecount_ = 0;
    clevel_ = 0;
    ntype_ = 0;
    fw_ = 0;
    fv_ = 0;
    if (has_ccolumn()) {
      if (ccolumn_ != &::google::protobuf::internal::kEmptyString) {
        ccolumn_->clear();
      }
    }
  }
  ainfo_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool VehicleInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 cDir = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cdir_)));
          set_has_cdir();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_cAxleCount;
        break;
      }
      
      // required int32 cAxleCount = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_cAxleCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &caxlecount_)));
          set_has_caxlecount();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_cLevel;
        break;
      }
      
      // optional int32 cLevel = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_cLevel:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &clevel_)));
          set_has_clevel();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_nType;
        break;
      }
      
      // required int32 nType = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ntype_)));
          set_has_ntype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(45)) goto parse_fW;
        break;
      }
      
      // required float fW = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_fW:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &fw_)));
          set_has_fw();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(53)) goto parse_fV;
        break;
      }
      
      // required float fV = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_fV:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &fv_)));
          set_has_fv();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_aInfo;
        break;
      }
      
      // repeated .commweb.AxleInfo aInfo = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_aInfo:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_ainfo()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_aInfo;
        if (input->ExpectTag(66)) goto parse_cColumn;
        break;
      }
      
      // optional string cColumn = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_cColumn:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ccolumn()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->ccolumn().data(), this->ccolumn().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void VehicleInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 cDir = 1;
  if (has_cdir()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cdir(), output);
  }
  
  // required int32 cAxleCount = 2;
  if (has_caxlecount()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->caxlecount(), output);
  }
  
  // optional int32 cLevel = 3;
  if (has_clevel()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->clevel(), output);
  }
  
  // required int32 nType = 4;
  if (has_ntype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->ntype(), output);
  }
  
  // required float fW = 5;
  if (has_fw()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(5, this->fw(), output);
  }
  
  // required float fV = 6;
  if (has_fv()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(6, this->fv(), output);
  }
  
  // repeated .commweb.AxleInfo aInfo = 7;
  for (int i = 0; i < this->ainfo_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      7, this->ainfo(i), output);
  }
  
  // optional string cColumn = 8;
  if (has_ccolumn()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ccolumn().data(), this->ccolumn().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->ccolumn(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* VehicleInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 cDir = 1;
  if (has_cdir()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->cdir(), target);
  }
  
  // required int32 cAxleCount = 2;
  if (has_caxlecount()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->caxlecount(), target);
  }
  
  // optional int32 cLevel = 3;
  if (has_clevel()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->clevel(), target);
  }
  
  // required int32 nType = 4;
  if (has_ntype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->ntype(), target);
  }
  
  // required float fW = 5;
  if (has_fw()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(5, this->fw(), target);
  }
  
  // required float fV = 6;
  if (has_fv()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(6, this->fv(), target);
  }
  
  // repeated .commweb.AxleInfo aInfo = 7;
  for (int i = 0; i < this->ainfo_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        7, this->ainfo(i), target);
  }
  
  // optional string cColumn = 8;
  if (has_ccolumn()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ccolumn().data(), this->ccolumn().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->ccolumn(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int VehicleInfo::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 cDir = 1;
    if (has_cdir()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cdir());
    }
    
    // required int32 cAxleCount = 2;
    if (has_caxlecount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->caxlecount());
    }
    
    // optional int32 cLevel = 3;
    if (has_clevel()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->clevel());
    }
    
    // required int32 nType = 4;
    if (has_ntype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ntype());
    }
    
    // required float fW = 5;
    if (has_fw()) {
      total_size += 1 + 4;
    }
    
    // required float fV = 6;
    if (has_fv()) {
      total_size += 1 + 4;
    }
    
    // optional string cColumn = 8;
    if (has_ccolumn()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ccolumn());
    }
    
  }
  // repeated .commweb.AxleInfo aInfo = 7;
  total_size += 1 * this->ainfo_size();
  for (int i = 0; i < this->ainfo_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->ainfo(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void VehicleInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const VehicleInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const VehicleInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void VehicleInfo::MergeFrom(const VehicleInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  ainfo_.MergeFrom(from.ainfo_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cdir()) {
      set_cdir(from.cdir());
    }
    if (from.has_caxlecount()) {
      set_caxlecount(from.caxlecount());
    }
    if (from.has_clevel()) {
      set_clevel(from.clevel());
    }
    if (from.has_ntype()) {
      set_ntype(from.ntype());
    }
    if (from.has_fw()) {
      set_fw(from.fw());
    }
    if (from.has_fv()) {
      set_fv(from.fv());
    }
    if (from.has_ccolumn()) {
      set_ccolumn(from.ccolumn());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void VehicleInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void VehicleInfo::CopyFrom(const VehicleInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool VehicleInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000003a) != 0x0000003a) return false;
  
  for (int i = 0; i < ainfo_size(); i++) {
    if (!this->ainfo(i).IsInitialized()) return false;
  }
  return true;
}

void VehicleInfo::Swap(VehicleInfo* other) {
  if (other != this) {
    std::swap(cdir_, other->cdir_);
    std::swap(caxlecount_, other->caxlecount_);
    std::swap(clevel_, other->clevel_);
    std::swap(ntype_, other->ntype_);
    std::swap(fw_, other->fw_);
    std::swap(fv_, other->fv_);
    ainfo_.Swap(&other->ainfo_);
    std::swap(ccolumn_, other->ccolumn_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata VehicleInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = VehicleInfo_descriptor_;
  metadata.reflection = VehicleInfo_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int CmdInfo::kTypeFieldNumber;
#endif  // !_MSC_VER

CmdInfo::CmdInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CmdInfo::InitAsDefaultInstance() {
}

CmdInfo::CmdInfo(const CmdInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CmdInfo::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CmdInfo::~CmdInfo() {
  SharedDtor();
}

void CmdInfo::SharedDtor() {
  if (this != default_instance_) {
  }
}

void CmdInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CmdInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CmdInfo_descriptor_;
}

const CmdInfo& CmdInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_commweb_2eproto();  return *default_instance_;
}

CmdInfo* CmdInfo::default_instance_ = NULL;

CmdInfo* CmdInfo::New() const {
  return new CmdInfo;
}

void CmdInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CmdInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void CmdInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->type(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CmdInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->type(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CmdInfo::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->type());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CmdInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CmdInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CmdInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CmdInfo::MergeFrom(const CmdInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CmdInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CmdInfo::CopyFrom(const CmdInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CmdInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void CmdInfo::Swap(CmdInfo* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CmdInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CmdInfo_descriptor_;
  metadata.reflection = CmdInfo_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace commweb

// @@protoc_insertion_point(global_scope)
