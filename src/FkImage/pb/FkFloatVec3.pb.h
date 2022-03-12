// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FkFloatVec3.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_FkFloatVec3_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_FkFloatVec3_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_FkFloatVec3_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_FkFloatVec3_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_FkFloatVec3_2eproto;
namespace fk_pb {
class FkFloatVec3;
struct FkFloatVec3DefaultTypeInternal;
extern FkFloatVec3DefaultTypeInternal _FkFloatVec3_default_instance_;
}  // namespace fk_pb
PROTOBUF_NAMESPACE_OPEN
template<> ::fk_pb::FkFloatVec3* Arena::CreateMaybeMessage<::fk_pb::FkFloatVec3>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace fk_pb {

// ===================================================================

class FkFloatVec3 final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:fk_pb.FkFloatVec3) */ {
 public:
  inline FkFloatVec3() : FkFloatVec3(nullptr) {}
  ~FkFloatVec3() override;
  explicit constexpr FkFloatVec3(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  FkFloatVec3(const FkFloatVec3& from);
  FkFloatVec3(FkFloatVec3&& from) noexcept
    : FkFloatVec3() {
    *this = ::std::move(from);
  }

  inline FkFloatVec3& operator=(const FkFloatVec3& from) {
    CopyFrom(from);
    return *this;
  }
  inline FkFloatVec3& operator=(FkFloatVec3&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const FkFloatVec3& default_instance() {
    return *internal_default_instance();
  }
  static inline const FkFloatVec3* internal_default_instance() {
    return reinterpret_cast<const FkFloatVec3*>(
               &_FkFloatVec3_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(FkFloatVec3& a, FkFloatVec3& b) {
    a.Swap(&b);
  }
  inline void Swap(FkFloatVec3* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(FkFloatVec3* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  FkFloatVec3* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<FkFloatVec3>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const FkFloatVec3& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const FkFloatVec3& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(FkFloatVec3* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "fk_pb.FkFloatVec3";
  }
  protected:
  explicit FkFloatVec3(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kXFieldNumber = 1,
    kYFieldNumber = 2,
    kZFieldNumber = 3,
  };
  // float x = 1;
  void clear_x();
  float x() const;
  void set_x(float value);
  private:
  float _internal_x() const;
  void _internal_set_x(float value);
  public:

  // float y = 2;
  void clear_y();
  float y() const;
  void set_y(float value);
  private:
  float _internal_y() const;
  void _internal_set_y(float value);
  public:

  // float z = 3;
  void clear_z();
  float z() const;
  void set_z(float value);
  private:
  float _internal_z() const;
  void _internal_set_z(float value);
  public:

  // @@protoc_insertion_point(class_scope:fk_pb.FkFloatVec3)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  float x_;
  float y_;
  float z_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_FkFloatVec3_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// FkFloatVec3

// float x = 1;
inline void FkFloatVec3::clear_x() {
  x_ = 0;
}
inline float FkFloatVec3::_internal_x() const {
  return x_;
}
inline float FkFloatVec3::x() const {
  // @@protoc_insertion_point(field_get:fk_pb.FkFloatVec3.x)
  return _internal_x();
}
inline void FkFloatVec3::_internal_set_x(float value) {
  
  x_ = value;
}
inline void FkFloatVec3::set_x(float value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:fk_pb.FkFloatVec3.x)
}

// float y = 2;
inline void FkFloatVec3::clear_y() {
  y_ = 0;
}
inline float FkFloatVec3::_internal_y() const {
  return y_;
}
inline float FkFloatVec3::y() const {
  // @@protoc_insertion_point(field_get:fk_pb.FkFloatVec3.y)
  return _internal_y();
}
inline void FkFloatVec3::_internal_set_y(float value) {
  
  y_ = value;
}
inline void FkFloatVec3::set_y(float value) {
  _internal_set_y(value);
  // @@protoc_insertion_point(field_set:fk_pb.FkFloatVec3.y)
}

// float z = 3;
inline void FkFloatVec3::clear_z() {
  z_ = 0;
}
inline float FkFloatVec3::_internal_z() const {
  return z_;
}
inline float FkFloatVec3::z() const {
  // @@protoc_insertion_point(field_get:fk_pb.FkFloatVec3.z)
  return _internal_z();
}
inline void FkFloatVec3::_internal_set_z(float value) {
  
  z_ = value;
}
inline void FkFloatVec3::set_z(float value) {
  _internal_set_z(value);
  // @@protoc_insertion_point(field_set:fk_pb.FkFloatVec3.z)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace fk_pb

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_FkFloatVec3_2eproto
