// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message_tracethreat.proto

#ifndef PROTOBUF_message_5ftracethreat_2eproto__INCLUDED
#define PROTOBUF_message_5ftracethreat_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace message_tracethreat {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_message_5ftracethreat_2eproto();
void protobuf_AssignDesc_message_5ftracethreat_2eproto();
void protobuf_ShutdownFile_message_5ftracethreat_2eproto();

class InfectedFileInfo;
class InfectedFileInfo_InfectedFileInfoValue;

enum InfectedFileInfo_EncodeType {
  InfectedFileInfo_EncodeType_MD5 = 1,
  InfectedFileInfo_EncodeType_MD5_HEADER = 2,
  InfectedFileInfo_EncodeType_MD5_BODY = 3,
  InfectedFileInfo_EncodeType_SHA_1 = 4,
  InfectedFileInfo_EncodeType_SHA_256 = 5,
  InfectedFileInfo_EncodeType_SSDEEP = 6
};
bool InfectedFileInfo_EncodeType_IsValid(int value);
const InfectedFileInfo_EncodeType InfectedFileInfo_EncodeType_EncodeType_MIN = InfectedFileInfo_EncodeType_MD5;
const InfectedFileInfo_EncodeType InfectedFileInfo_EncodeType_EncodeType_MAX = InfectedFileInfo_EncodeType_SSDEEP;
const int InfectedFileInfo_EncodeType_EncodeType_ARRAYSIZE = InfectedFileInfo_EncodeType_EncodeType_MAX + 1;

const ::google::protobuf::EnumDescriptor* InfectedFileInfo_EncodeType_descriptor();
inline const ::std::string& InfectedFileInfo_EncodeType_Name(InfectedFileInfo_EncodeType value) {
  return ::google::protobuf::internal::NameOfEnum(
    InfectedFileInfo_EncodeType_descriptor(), value);
}
inline bool InfectedFileInfo_EncodeType_Parse(
    const ::std::string& name, InfectedFileInfo_EncodeType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<InfectedFileInfo_EncodeType>(
    InfectedFileInfo_EncodeType_descriptor(), name, value);
}
enum InfectedFileInfo_FileType {
  InfectedFileInfo_FileType_PE = 1,
  InfectedFileInfo_FileType_ELF = 2,
  InfectedFileInfo_FileType_PROCESS = 3
};
bool InfectedFileInfo_FileType_IsValid(int value);
const InfectedFileInfo_FileType InfectedFileInfo_FileType_FileType_MIN = InfectedFileInfo_FileType_PE;
const InfectedFileInfo_FileType InfectedFileInfo_FileType_FileType_MAX = InfectedFileInfo_FileType_PROCESS;
const int InfectedFileInfo_FileType_FileType_ARRAYSIZE = InfectedFileInfo_FileType_FileType_MAX + 1;

const ::google::protobuf::EnumDescriptor* InfectedFileInfo_FileType_descriptor();
inline const ::std::string& InfectedFileInfo_FileType_Name(InfectedFileInfo_FileType value) {
  return ::google::protobuf::internal::NameOfEnum(
    InfectedFileInfo_FileType_descriptor(), value);
}
inline bool InfectedFileInfo_FileType_Parse(
    const ::std::string& name, InfectedFileInfo_FileType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<InfectedFileInfo_FileType>(
    InfectedFileInfo_FileType_descriptor(), name, value);
}
enum InfectedFileInfo_StatusResult {
  InfectedFileInfo_StatusResult_POSITIVE = 1,
  InfectedFileInfo_StatusResult_NEGATIVE = 2
};
bool InfectedFileInfo_StatusResult_IsValid(int value);
const InfectedFileInfo_StatusResult InfectedFileInfo_StatusResult_StatusResult_MIN = InfectedFileInfo_StatusResult_POSITIVE;
const InfectedFileInfo_StatusResult InfectedFileInfo_StatusResult_StatusResult_MAX = InfectedFileInfo_StatusResult_NEGATIVE;
const int InfectedFileInfo_StatusResult_StatusResult_ARRAYSIZE = InfectedFileInfo_StatusResult_StatusResult_MAX + 1;

const ::google::protobuf::EnumDescriptor* InfectedFileInfo_StatusResult_descriptor();
inline const ::std::string& InfectedFileInfo_StatusResult_Name(InfectedFileInfo_StatusResult value) {
  return ::google::protobuf::internal::NameOfEnum(
    InfectedFileInfo_StatusResult_descriptor(), value);
}
inline bool InfectedFileInfo_StatusResult_Parse(
    const ::std::string& name, InfectedFileInfo_StatusResult* value) {
  return ::google::protobuf::internal::ParseNamedEnum<InfectedFileInfo_StatusResult>(
    InfectedFileInfo_StatusResult_descriptor(), name, value);
}
// ===================================================================

class InfectedFileInfo_InfectedFileInfoValue : public ::google::protobuf::Message {
 public:
  InfectedFileInfo_InfectedFileInfoValue();
  virtual ~InfectedFileInfo_InfectedFileInfoValue();

  InfectedFileInfo_InfectedFileInfoValue(const InfectedFileInfo_InfectedFileInfoValue& from);

  inline InfectedFileInfo_InfectedFileInfoValue& operator=(const InfectedFileInfo_InfectedFileInfoValue& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const InfectedFileInfo_InfectedFileInfoValue& default_instance();

  void Swap(InfectedFileInfo_InfectedFileInfoValue* other);

  // implements Message ----------------------------------------------

  InfectedFileInfo_InfectedFileInfoValue* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const InfectedFileInfo_InfectedFileInfoValue& from);
  void MergeFrom(const InfectedFileInfo_InfectedFileInfoValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required .message_tracethreat.InfectedFileInfo.EncodeType encode_sign_type = 1;
  inline bool has_encode_sign_type() const;
  inline void clear_encode_sign_type();
  static const int kEncodeSignTypeFieldNumber = 1;
  inline ::message_tracethreat::InfectedFileInfo_EncodeType encode_sign_type() const;
  inline void set_encode_sign_type(::message_tracethreat::InfectedFileInfo_EncodeType value);

  // required .message_tracethreat.InfectedFileInfo.FileType file_type = 2;
  inline bool has_file_type() const;
  inline void clear_file_type();
  static const int kFileTypeFieldNumber = 2;
  inline ::message_tracethreat::InfectedFileInfo_FileType file_type() const;
  inline void set_file_type(::message_tracethreat::InfectedFileInfo_FileType value);

  // required .message_tracethreat.InfectedFileInfo.StatusResult status_result = 3;
  inline bool has_status_result() const;
  inline void clear_status_result();
  static const int kStatusResultFieldNumber = 3;
  inline ::message_tracethreat::InfectedFileInfo_StatusResult status_result() const;
  inline void set_status_result(::message_tracethreat::InfectedFileInfo_StatusResult value);

  // required bytes file_name = 4;
  inline bool has_file_name() const;
  inline void clear_file_name();
  static const int kFileNameFieldNumber = 4;
  inline const ::std::string& file_name() const;
  inline void set_file_name(const ::std::string& value);
  inline void set_file_name(const char* value);
  inline void set_file_name(const void* value, size_t size);
  inline ::std::string* mutable_file_name();
  inline ::std::string* release_file_name();
  inline void set_allocated_file_name(::std::string* file_name);

  // required bytes binary = 5;
  inline bool has_binary() const;
  inline void clear_binary();
  static const int kBinaryFieldNumber = 5;
  inline const ::std::string& binary() const;
  inline void set_binary(const ::std::string& value);
  inline void set_binary(const char* value);
  inline void set_binary(const void* value, size_t size);
  inline ::std::string* mutable_binary();
  inline ::std::string* release_binary();
  inline void set_allocated_binary(::std::string* binary);

  // @@protoc_insertion_point(class_scope:message_tracethreat.InfectedFileInfo.InfectedFileInfoValue)
 private:
  inline void set_has_encode_sign_type();
  inline void clear_has_encode_sign_type();
  inline void set_has_file_type();
  inline void clear_has_file_type();
  inline void set_has_status_result();
  inline void clear_has_status_result();
  inline void set_has_file_name();
  inline void clear_has_file_name();
  inline void set_has_binary();
  inline void clear_has_binary();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  int encode_sign_type_;
  int file_type_;
  ::std::string* file_name_;
  ::std::string* binary_;
  int status_result_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_message_5ftracethreat_2eproto();
  friend void protobuf_AssignDesc_message_5ftracethreat_2eproto();
  friend void protobuf_ShutdownFile_message_5ftracethreat_2eproto();

  void InitAsDefaultInstance();
  static InfectedFileInfo_InfectedFileInfoValue* default_instance_;
};
// -------------------------------------------------------------------

class InfectedFileInfo : public ::google::protobuf::Message {
 public:
  InfectedFileInfo();
  virtual ~InfectedFileInfo();

  InfectedFileInfo(const InfectedFileInfo& from);

  inline InfectedFileInfo& operator=(const InfectedFileInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const InfectedFileInfo& default_instance();

  void Swap(InfectedFileInfo* other);

  // implements Message ----------------------------------------------

  InfectedFileInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const InfectedFileInfo& from);
  void MergeFrom(const InfectedFileInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef InfectedFileInfo_InfectedFileInfoValue InfectedFileInfoValue;

  typedef InfectedFileInfo_EncodeType EncodeType;
  static const EncodeType MD5 = InfectedFileInfo_EncodeType_MD5;
  static const EncodeType MD5_HEADER = InfectedFileInfo_EncodeType_MD5_HEADER;
  static const EncodeType MD5_BODY = InfectedFileInfo_EncodeType_MD5_BODY;
  static const EncodeType SHA_1 = InfectedFileInfo_EncodeType_SHA_1;
  static const EncodeType SHA_256 = InfectedFileInfo_EncodeType_SHA_256;
  static const EncodeType SSDEEP = InfectedFileInfo_EncodeType_SSDEEP;
  static inline bool EncodeType_IsValid(int value) {
    return InfectedFileInfo_EncodeType_IsValid(value);
  }
  static const EncodeType EncodeType_MIN =
    InfectedFileInfo_EncodeType_EncodeType_MIN;
  static const EncodeType EncodeType_MAX =
    InfectedFileInfo_EncodeType_EncodeType_MAX;
  static const int EncodeType_ARRAYSIZE =
    InfectedFileInfo_EncodeType_EncodeType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  EncodeType_descriptor() {
    return InfectedFileInfo_EncodeType_descriptor();
  }
  static inline const ::std::string& EncodeType_Name(EncodeType value) {
    return InfectedFileInfo_EncodeType_Name(value);
  }
  static inline bool EncodeType_Parse(const ::std::string& name,
      EncodeType* value) {
    return InfectedFileInfo_EncodeType_Parse(name, value);
  }

  typedef InfectedFileInfo_FileType FileType;
  static const FileType PE = InfectedFileInfo_FileType_PE;
  static const FileType ELF = InfectedFileInfo_FileType_ELF;
  static const FileType PROCESS = InfectedFileInfo_FileType_PROCESS;
  static inline bool FileType_IsValid(int value) {
    return InfectedFileInfo_FileType_IsValid(value);
  }
  static const FileType FileType_MIN =
    InfectedFileInfo_FileType_FileType_MIN;
  static const FileType FileType_MAX =
    InfectedFileInfo_FileType_FileType_MAX;
  static const int FileType_ARRAYSIZE =
    InfectedFileInfo_FileType_FileType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  FileType_descriptor() {
    return InfectedFileInfo_FileType_descriptor();
  }
  static inline const ::std::string& FileType_Name(FileType value) {
    return InfectedFileInfo_FileType_Name(value);
  }
  static inline bool FileType_Parse(const ::std::string& name,
      FileType* value) {
    return InfectedFileInfo_FileType_Parse(name, value);
  }

  typedef InfectedFileInfo_StatusResult StatusResult;
  static const StatusResult POSITIVE = InfectedFileInfo_StatusResult_POSITIVE;
  static const StatusResult NEGATIVE = InfectedFileInfo_StatusResult_NEGATIVE;
  static inline bool StatusResult_IsValid(int value) {
    return InfectedFileInfo_StatusResult_IsValid(value);
  }
  static const StatusResult StatusResult_MIN =
    InfectedFileInfo_StatusResult_StatusResult_MIN;
  static const StatusResult StatusResult_MAX =
    InfectedFileInfo_StatusResult_StatusResult_MAX;
  static const int StatusResult_ARRAYSIZE =
    InfectedFileInfo_StatusResult_StatusResult_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  StatusResult_descriptor() {
    return InfectedFileInfo_StatusResult_descriptor();
  }
  static inline const ::std::string& StatusResult_Name(StatusResult value) {
    return InfectedFileInfo_StatusResult_Name(value);
  }
  static inline bool StatusResult_Parse(const ::std::string& name,
      StatusResult* value) {
    return InfectedFileInfo_StatusResult_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required bytes uuid = 1;
  inline bool has_uuid() const;
  inline void clear_uuid();
  static const int kUuidFieldNumber = 1;
  inline const ::std::string& uuid() const;
  inline void set_uuid(const ::std::string& value);
  inline void set_uuid(const char* value);
  inline void set_uuid(const void* value, size_t size);
  inline ::std::string* mutable_uuid();
  inline ::std::string* release_uuid();
  inline void set_allocated_uuid(::std::string* uuid);

  // required bytes machine_name = 2;
  inline bool has_machine_name() const;
  inline void clear_machine_name();
  static const int kMachineNameFieldNumber = 2;
  inline const ::std::string& machine_name() const;
  inline void set_machine_name(const ::std::string& value);
  inline void set_machine_name(const char* value);
  inline void set_machine_name(const void* value, size_t size);
  inline ::std::string* mutable_machine_name();
  inline ::std::string* release_machine_name();
  inline void set_allocated_machine_name(::std::string* machine_name);

  // required bytes ip = 3;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 3;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const void* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  inline void set_allocated_ip(::std::string* ip);

  // repeated .message_tracethreat.InfectedFileInfo.InfectedFileInfoValue infected_file_info_value = 4;
  inline int infected_file_info_value_size() const;
  inline void clear_infected_file_info_value();
  static const int kInfectedFileInfoValueFieldNumber = 4;
  inline const ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue& infected_file_info_value(int index) const;
  inline ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue* mutable_infected_file_info_value(int index);
  inline ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue* add_infected_file_info_value();
  inline const ::google::protobuf::RepeatedPtrField< ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue >&
      infected_file_info_value() const;
  inline ::google::protobuf::RepeatedPtrField< ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue >*
      mutable_infected_file_info_value();

  // @@protoc_insertion_point(class_scope:message_tracethreat.InfectedFileInfo)
 private:
  inline void set_has_uuid();
  inline void clear_has_uuid();
  inline void set_has_machine_name();
  inline void clear_has_machine_name();
  inline void set_has_ip();
  inline void clear_has_ip();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* uuid_;
  ::std::string* machine_name_;
  ::std::string* ip_;
  ::google::protobuf::RepeatedPtrField< ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue > infected_file_info_value_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_message_5ftracethreat_2eproto();
  friend void protobuf_AssignDesc_message_5ftracethreat_2eproto();
  friend void protobuf_ShutdownFile_message_5ftracethreat_2eproto();

  void InitAsDefaultInstance();
  static InfectedFileInfo* default_instance_;
};
// ===================================================================


// ===================================================================

// InfectedFileInfo_InfectedFileInfoValue

// required .message_tracethreat.InfectedFileInfo.EncodeType encode_sign_type = 1;
inline bool InfectedFileInfo_InfectedFileInfoValue::has_encode_sign_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_has_encode_sign_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_has_encode_sign_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_encode_sign_type() {
  encode_sign_type_ = 1;
  clear_has_encode_sign_type();
}
inline ::message_tracethreat::InfectedFileInfo_EncodeType InfectedFileInfo_InfectedFileInfoValue::encode_sign_type() const {
  return static_cast< ::message_tracethreat::InfectedFileInfo_EncodeType >(encode_sign_type_);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_encode_sign_type(::message_tracethreat::InfectedFileInfo_EncodeType value) {
  assert(::message_tracethreat::InfectedFileInfo_EncodeType_IsValid(value));
  set_has_encode_sign_type();
  encode_sign_type_ = value;
}

// required .message_tracethreat.InfectedFileInfo.FileType file_type = 2;
inline bool InfectedFileInfo_InfectedFileInfoValue::has_file_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_has_file_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_has_file_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_file_type() {
  file_type_ = 1;
  clear_has_file_type();
}
inline ::message_tracethreat::InfectedFileInfo_FileType InfectedFileInfo_InfectedFileInfoValue::file_type() const {
  return static_cast< ::message_tracethreat::InfectedFileInfo_FileType >(file_type_);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_file_type(::message_tracethreat::InfectedFileInfo_FileType value) {
  assert(::message_tracethreat::InfectedFileInfo_FileType_IsValid(value));
  set_has_file_type();
  file_type_ = value;
}

// required .message_tracethreat.InfectedFileInfo.StatusResult status_result = 3;
inline bool InfectedFileInfo_InfectedFileInfoValue::has_status_result() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_has_status_result() {
  _has_bits_[0] |= 0x00000004u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_has_status_result() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_status_result() {
  status_result_ = 1;
  clear_has_status_result();
}
inline ::message_tracethreat::InfectedFileInfo_StatusResult InfectedFileInfo_InfectedFileInfoValue::status_result() const {
  return static_cast< ::message_tracethreat::InfectedFileInfo_StatusResult >(status_result_);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_status_result(::message_tracethreat::InfectedFileInfo_StatusResult value) {
  assert(::message_tracethreat::InfectedFileInfo_StatusResult_IsValid(value));
  set_has_status_result();
  status_result_ = value;
}

// required bytes file_name = 4;
inline bool InfectedFileInfo_InfectedFileInfoValue::has_file_name() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_has_file_name() {
  _has_bits_[0] |= 0x00000008u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_has_file_name() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_file_name() {
  if (file_name_ != &::google::protobuf::internal::kEmptyString) {
    file_name_->clear();
  }
  clear_has_file_name();
}
inline const ::std::string& InfectedFileInfo_InfectedFileInfoValue::file_name() const {
  return *file_name_;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_file_name(const ::std::string& value) {
  set_has_file_name();
  if (file_name_ == &::google::protobuf::internal::kEmptyString) {
    file_name_ = new ::std::string;
  }
  file_name_->assign(value);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_file_name(const char* value) {
  set_has_file_name();
  if (file_name_ == &::google::protobuf::internal::kEmptyString) {
    file_name_ = new ::std::string;
  }
  file_name_->assign(value);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_file_name(const void* value, size_t size) {
  set_has_file_name();
  if (file_name_ == &::google::protobuf::internal::kEmptyString) {
    file_name_ = new ::std::string;
  }
  file_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* InfectedFileInfo_InfectedFileInfoValue::mutable_file_name() {
  set_has_file_name();
  if (file_name_ == &::google::protobuf::internal::kEmptyString) {
    file_name_ = new ::std::string;
  }
  return file_name_;
}
inline ::std::string* InfectedFileInfo_InfectedFileInfoValue::release_file_name() {
  clear_has_file_name();
  if (file_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = file_name_;
    file_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_allocated_file_name(::std::string* file_name) {
  if (file_name_ != &::google::protobuf::internal::kEmptyString) {
    delete file_name_;
  }
  if (file_name) {
    set_has_file_name();
    file_name_ = file_name;
  } else {
    clear_has_file_name();
    file_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required bytes binary = 5;
inline bool InfectedFileInfo_InfectedFileInfoValue::has_binary() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_has_binary() {
  _has_bits_[0] |= 0x00000010u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_has_binary() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void InfectedFileInfo_InfectedFileInfoValue::clear_binary() {
  if (binary_ != &::google::protobuf::internal::kEmptyString) {
    binary_->clear();
  }
  clear_has_binary();
}
inline const ::std::string& InfectedFileInfo_InfectedFileInfoValue::binary() const {
  return *binary_;
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_binary(const ::std::string& value) {
  set_has_binary();
  if (binary_ == &::google::protobuf::internal::kEmptyString) {
    binary_ = new ::std::string;
  }
  binary_->assign(value);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_binary(const char* value) {
  set_has_binary();
  if (binary_ == &::google::protobuf::internal::kEmptyString) {
    binary_ = new ::std::string;
  }
  binary_->assign(value);
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_binary(const void* value, size_t size) {
  set_has_binary();
  if (binary_ == &::google::protobuf::internal::kEmptyString) {
    binary_ = new ::std::string;
  }
  binary_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* InfectedFileInfo_InfectedFileInfoValue::mutable_binary() {
  set_has_binary();
  if (binary_ == &::google::protobuf::internal::kEmptyString) {
    binary_ = new ::std::string;
  }
  return binary_;
}
inline ::std::string* InfectedFileInfo_InfectedFileInfoValue::release_binary() {
  clear_has_binary();
  if (binary_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = binary_;
    binary_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void InfectedFileInfo_InfectedFileInfoValue::set_allocated_binary(::std::string* binary) {
  if (binary_ != &::google::protobuf::internal::kEmptyString) {
    delete binary_;
  }
  if (binary) {
    set_has_binary();
    binary_ = binary;
  } else {
    clear_has_binary();
    binary_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// InfectedFileInfo

// required bytes uuid = 1;
inline bool InfectedFileInfo::has_uuid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void InfectedFileInfo::set_has_uuid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void InfectedFileInfo::clear_has_uuid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void InfectedFileInfo::clear_uuid() {
  if (uuid_ != &::google::protobuf::internal::kEmptyString) {
    uuid_->clear();
  }
  clear_has_uuid();
}
inline const ::std::string& InfectedFileInfo::uuid() const {
  return *uuid_;
}
inline void InfectedFileInfo::set_uuid(const ::std::string& value) {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(value);
}
inline void InfectedFileInfo::set_uuid(const char* value) {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(value);
}
inline void InfectedFileInfo::set_uuid(const void* value, size_t size) {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* InfectedFileInfo::mutable_uuid() {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  return uuid_;
}
inline ::std::string* InfectedFileInfo::release_uuid() {
  clear_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = uuid_;
    uuid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void InfectedFileInfo::set_allocated_uuid(::std::string* uuid) {
  if (uuid_ != &::google::protobuf::internal::kEmptyString) {
    delete uuid_;
  }
  if (uuid) {
    set_has_uuid();
    uuid_ = uuid;
  } else {
    clear_has_uuid();
    uuid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required bytes machine_name = 2;
inline bool InfectedFileInfo::has_machine_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void InfectedFileInfo::set_has_machine_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void InfectedFileInfo::clear_has_machine_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void InfectedFileInfo::clear_machine_name() {
  if (machine_name_ != &::google::protobuf::internal::kEmptyString) {
    machine_name_->clear();
  }
  clear_has_machine_name();
}
inline const ::std::string& InfectedFileInfo::machine_name() const {
  return *machine_name_;
}
inline void InfectedFileInfo::set_machine_name(const ::std::string& value) {
  set_has_machine_name();
  if (machine_name_ == &::google::protobuf::internal::kEmptyString) {
    machine_name_ = new ::std::string;
  }
  machine_name_->assign(value);
}
inline void InfectedFileInfo::set_machine_name(const char* value) {
  set_has_machine_name();
  if (machine_name_ == &::google::protobuf::internal::kEmptyString) {
    machine_name_ = new ::std::string;
  }
  machine_name_->assign(value);
}
inline void InfectedFileInfo::set_machine_name(const void* value, size_t size) {
  set_has_machine_name();
  if (machine_name_ == &::google::protobuf::internal::kEmptyString) {
    machine_name_ = new ::std::string;
  }
  machine_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* InfectedFileInfo::mutable_machine_name() {
  set_has_machine_name();
  if (machine_name_ == &::google::protobuf::internal::kEmptyString) {
    machine_name_ = new ::std::string;
  }
  return machine_name_;
}
inline ::std::string* InfectedFileInfo::release_machine_name() {
  clear_has_machine_name();
  if (machine_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = machine_name_;
    machine_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void InfectedFileInfo::set_allocated_machine_name(::std::string* machine_name) {
  if (machine_name_ != &::google::protobuf::internal::kEmptyString) {
    delete machine_name_;
  }
  if (machine_name) {
    set_has_machine_name();
    machine_name_ = machine_name;
  } else {
    clear_has_machine_name();
    machine_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required bytes ip = 3;
inline bool InfectedFileInfo::has_ip() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void InfectedFileInfo::set_has_ip() {
  _has_bits_[0] |= 0x00000004u;
}
inline void InfectedFileInfo::clear_has_ip() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void InfectedFileInfo::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& InfectedFileInfo::ip() const {
  return *ip_;
}
inline void InfectedFileInfo::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void InfectedFileInfo::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void InfectedFileInfo::set_ip(const void* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* InfectedFileInfo::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* InfectedFileInfo::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void InfectedFileInfo::set_allocated_ip(::std::string* ip) {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    delete ip_;
  }
  if (ip) {
    set_has_ip();
    ip_ = ip;
  } else {
    clear_has_ip();
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// repeated .message_tracethreat.InfectedFileInfo.InfectedFileInfoValue infected_file_info_value = 4;
inline int InfectedFileInfo::infected_file_info_value_size() const {
  return infected_file_info_value_.size();
}
inline void InfectedFileInfo::clear_infected_file_info_value() {
  infected_file_info_value_.Clear();
}
inline const ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue& InfectedFileInfo::infected_file_info_value(int index) const {
  return infected_file_info_value_.Get(index);
}
inline ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue* InfectedFileInfo::mutable_infected_file_info_value(int index) {
  return infected_file_info_value_.Mutable(index);
}
inline ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue* InfectedFileInfo::add_infected_file_info_value() {
  return infected_file_info_value_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue >&
InfectedFileInfo::infected_file_info_value() const {
  return infected_file_info_value_;
}
inline ::google::protobuf::RepeatedPtrField< ::message_tracethreat::InfectedFileInfo_InfectedFileInfoValue >*
InfectedFileInfo::mutable_infected_file_info_value() {
  return &infected_file_info_value_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace message_tracethreat

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::message_tracethreat::InfectedFileInfo_EncodeType>() {
  return ::message_tracethreat::InfectedFileInfo_EncodeType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::message_tracethreat::InfectedFileInfo_FileType>() {
  return ::message_tracethreat::InfectedFileInfo_FileType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::message_tracethreat::InfectedFileInfo_StatusResult>() {
  return ::message_tracethreat::InfectedFileInfo_StatusResult_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_message_5ftracethreat_2eproto__INCLUDED
