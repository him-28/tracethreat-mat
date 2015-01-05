// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message_network.proto

#ifndef PROTOBUF_message_5fnetwork_2eproto__INCLUDED
#define PROTOBUF_message_5fnetwork_2eproto__INCLUDED

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

namespace message_network {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_message_5fnetwork_2eproto();
void protobuf_AssignDesc_message_5fnetwork_2eproto();
void protobuf_ShutdownFile_message_5fnetwork_2eproto();

class ScanVirusInfo;

enum ScanVirusInfo_ScanStatusType {
  ScanVirusInfo_ScanStatusType_REGISTER = 1,
  ScanVirusInfo_ScanStatusType_SCAN = 2,
  ScanVirusInfo_ScanStatusType_CLOSE_CONNECTION = 3,
  ScanVirusInfo_ScanStatusType_REGISTER_SUCCESS = 4,
  ScanVirusInfo_ScanStatusType_REGISTER_UNSUCCESS = 5,
  ScanVirusInfo_ScanStatusType_SCAN_SUCCESS = 6,
  ScanVirusInfo_ScanStatusType_SCAN_UNSUCCESS = 7,
  ScanVirusInfo_ScanStatusType_RESULT = 8
};
bool ScanVirusInfo_ScanStatusType_IsValid(int value);
const ScanVirusInfo_ScanStatusType ScanVirusInfo_ScanStatusType_ScanStatusType_MIN = ScanVirusInfo_ScanStatusType_REGISTER;
const ScanVirusInfo_ScanStatusType ScanVirusInfo_ScanStatusType_ScanStatusType_MAX = ScanVirusInfo_ScanStatusType_RESULT;
const int ScanVirusInfo_ScanStatusType_ScanStatusType_ARRAYSIZE = ScanVirusInfo_ScanStatusType_ScanStatusType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ScanVirusInfo_ScanStatusType_descriptor();
inline const ::std::string& ScanVirusInfo_ScanStatusType_Name(ScanVirusInfo_ScanStatusType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ScanVirusInfo_ScanStatusType_descriptor(), value);
}
inline bool ScanVirusInfo_ScanStatusType_Parse(
    const ::std::string& name, ScanVirusInfo_ScanStatusType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ScanVirusInfo_ScanStatusType>(
    ScanVirusInfo_ScanStatusType_descriptor(), name, value);
}
// ===================================================================

class ScanVirusInfo : public ::google::protobuf::Message {
 public:
  ScanVirusInfo();
  virtual ~ScanVirusInfo();

  ScanVirusInfo(const ScanVirusInfo& from);

  inline ScanVirusInfo& operator=(const ScanVirusInfo& from) {
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
  static const ScanVirusInfo& default_instance();

  void Swap(ScanVirusInfo* other);

  // implements Message ----------------------------------------------

  ScanVirusInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ScanVirusInfo& from);
  void MergeFrom(const ScanVirusInfo& from);
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

  typedef ScanVirusInfo_ScanStatusType ScanStatusType;
  static const ScanStatusType REGISTER = ScanVirusInfo_ScanStatusType_REGISTER;
  static const ScanStatusType SCAN = ScanVirusInfo_ScanStatusType_SCAN;
  static const ScanStatusType CLOSE_CONNECTION = ScanVirusInfo_ScanStatusType_CLOSE_CONNECTION;
  static const ScanStatusType REGISTER_SUCCESS = ScanVirusInfo_ScanStatusType_REGISTER_SUCCESS;
  static const ScanStatusType REGISTER_UNSUCCESS = ScanVirusInfo_ScanStatusType_REGISTER_UNSUCCESS;
  static const ScanStatusType SCAN_SUCCESS = ScanVirusInfo_ScanStatusType_SCAN_SUCCESS;
  static const ScanStatusType SCAN_UNSUCCESS = ScanVirusInfo_ScanStatusType_SCAN_UNSUCCESS;
  static const ScanStatusType RESULT = ScanVirusInfo_ScanStatusType_RESULT;
  static inline bool ScanStatusType_IsValid(int value) {
    return ScanVirusInfo_ScanStatusType_IsValid(value);
  }
  static const ScanStatusType ScanStatusType_MIN =
    ScanVirusInfo_ScanStatusType_ScanStatusType_MIN;
  static const ScanStatusType ScanStatusType_MAX =
    ScanVirusInfo_ScanStatusType_ScanStatusType_MAX;
  static const int ScanStatusType_ARRAYSIZE =
    ScanVirusInfo_ScanStatusType_ScanStatusType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  ScanStatusType_descriptor() {
    return ScanVirusInfo_ScanStatusType_descriptor();
  }
  static inline const ::std::string& ScanStatusType_Name(ScanStatusType value) {
    return ScanVirusInfo_ScanStatusType_Name(value);
  }
  static inline bool ScanStatusType_Parse(const ::std::string& name,
      ScanStatusType* value) {
    return ScanVirusInfo_ScanStatusType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required bytes ip = 1;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 1;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const void* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  inline void set_allocated_ip(::std::string* ip);

  // required bytes uuid = 2;
  inline bool has_uuid() const;
  inline void clear_uuid();
  static const int kUuidFieldNumber = 2;
  inline const ::std::string& uuid() const;
  inline void set_uuid(const ::std::string& value);
  inline void set_uuid(const char* value);
  inline void set_uuid(const void* value, size_t size);
  inline ::std::string* mutable_uuid();
  inline ::std::string* release_uuid();
  inline void set_allocated_uuid(::std::string* uuid);

  // required bytes mac_addr = 3;
  inline bool has_mac_addr() const;
  inline void clear_mac_addr();
  static const int kMacAddrFieldNumber = 3;
  inline const ::std::string& mac_addr() const;
  inline void set_mac_addr(const ::std::string& value);
  inline void set_mac_addr(const char* value);
  inline void set_mac_addr(const void* value, size_t size);
  inline ::std::string* mutable_mac_addr();
  inline ::std::string* release_mac_addr();
  inline void set_allocated_mac_addr(::std::string* mac_addr);

  // optional .message_network.ScanVirusInfo.ScanStatusType scan_status = 4;
  inline bool has_scan_status() const;
  inline void clear_scan_status();
  static const int kScanStatusFieldNumber = 4;
  inline ::message_network::ScanVirusInfo_ScanStatusType scan_status() const;
  inline void set_scan_status(::message_network::ScanVirusInfo_ScanStatusType value);

  // @@protoc_insertion_point(class_scope:message_network.ScanVirusInfo)
 private:
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_uuid();
  inline void clear_has_uuid();
  inline void set_has_mac_addr();
  inline void clear_has_mac_addr();
  inline void set_has_scan_status();
  inline void clear_has_scan_status();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* ip_;
  ::std::string* uuid_;
  ::std::string* mac_addr_;
  int scan_status_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_message_5fnetwork_2eproto();
  friend void protobuf_AssignDesc_message_5fnetwork_2eproto();
  friend void protobuf_ShutdownFile_message_5fnetwork_2eproto();

  void InitAsDefaultInstance();
  static ScanVirusInfo* default_instance_;
};
// ===================================================================


// ===================================================================

// ScanVirusInfo

// required bytes ip = 1;
inline bool ScanVirusInfo::has_ip() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ScanVirusInfo::set_has_ip() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ScanVirusInfo::clear_has_ip() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ScanVirusInfo::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& ScanVirusInfo::ip() const {
  return *ip_;
}
inline void ScanVirusInfo::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void ScanVirusInfo::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void ScanVirusInfo::set_ip(const void* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScanVirusInfo::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* ScanVirusInfo::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ScanVirusInfo::set_allocated_ip(::std::string* ip) {
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

// required bytes uuid = 2;
inline bool ScanVirusInfo::has_uuid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ScanVirusInfo::set_has_uuid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ScanVirusInfo::clear_has_uuid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ScanVirusInfo::clear_uuid() {
  if (uuid_ != &::google::protobuf::internal::kEmptyString) {
    uuid_->clear();
  }
  clear_has_uuid();
}
inline const ::std::string& ScanVirusInfo::uuid() const {
  return *uuid_;
}
inline void ScanVirusInfo::set_uuid(const ::std::string& value) {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(value);
}
inline void ScanVirusInfo::set_uuid(const char* value) {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(value);
}
inline void ScanVirusInfo::set_uuid(const void* value, size_t size) {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScanVirusInfo::mutable_uuid() {
  set_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    uuid_ = new ::std::string;
  }
  return uuid_;
}
inline ::std::string* ScanVirusInfo::release_uuid() {
  clear_has_uuid();
  if (uuid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = uuid_;
    uuid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ScanVirusInfo::set_allocated_uuid(::std::string* uuid) {
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

// required bytes mac_addr = 3;
inline bool ScanVirusInfo::has_mac_addr() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ScanVirusInfo::set_has_mac_addr() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ScanVirusInfo::clear_has_mac_addr() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ScanVirusInfo::clear_mac_addr() {
  if (mac_addr_ != &::google::protobuf::internal::kEmptyString) {
    mac_addr_->clear();
  }
  clear_has_mac_addr();
}
inline const ::std::string& ScanVirusInfo::mac_addr() const {
  return *mac_addr_;
}
inline void ScanVirusInfo::set_mac_addr(const ::std::string& value) {
  set_has_mac_addr();
  if (mac_addr_ == &::google::protobuf::internal::kEmptyString) {
    mac_addr_ = new ::std::string;
  }
  mac_addr_->assign(value);
}
inline void ScanVirusInfo::set_mac_addr(const char* value) {
  set_has_mac_addr();
  if (mac_addr_ == &::google::protobuf::internal::kEmptyString) {
    mac_addr_ = new ::std::string;
  }
  mac_addr_->assign(value);
}
inline void ScanVirusInfo::set_mac_addr(const void* value, size_t size) {
  set_has_mac_addr();
  if (mac_addr_ == &::google::protobuf::internal::kEmptyString) {
    mac_addr_ = new ::std::string;
  }
  mac_addr_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScanVirusInfo::mutable_mac_addr() {
  set_has_mac_addr();
  if (mac_addr_ == &::google::protobuf::internal::kEmptyString) {
    mac_addr_ = new ::std::string;
  }
  return mac_addr_;
}
inline ::std::string* ScanVirusInfo::release_mac_addr() {
  clear_has_mac_addr();
  if (mac_addr_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = mac_addr_;
    mac_addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ScanVirusInfo::set_allocated_mac_addr(::std::string* mac_addr) {
  if (mac_addr_ != &::google::protobuf::internal::kEmptyString) {
    delete mac_addr_;
  }
  if (mac_addr) {
    set_has_mac_addr();
    mac_addr_ = mac_addr;
  } else {
    clear_has_mac_addr();
    mac_addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional .message_network.ScanVirusInfo.ScanStatusType scan_status = 4;
inline bool ScanVirusInfo::has_scan_status() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ScanVirusInfo::set_has_scan_status() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ScanVirusInfo::clear_has_scan_status() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ScanVirusInfo::clear_scan_status() {
  scan_status_ = 1;
  clear_has_scan_status();
}
inline ::message_network::ScanVirusInfo_ScanStatusType ScanVirusInfo::scan_status() const {
  return static_cast< ::message_network::ScanVirusInfo_ScanStatusType >(scan_status_);
}
inline void ScanVirusInfo::set_scan_status(::message_network::ScanVirusInfo_ScanStatusType value) {
  assert(::message_network::ScanVirusInfo_ScanStatusType_IsValid(value));
  set_has_scan_status();
  scan_status_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace message_network

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::message_network::ScanVirusInfo_ScanStatusType>() {
  return ::message_network::ScanVirusInfo_ScanStatusType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_message_5fnetwork_2eproto__INCLUDED
