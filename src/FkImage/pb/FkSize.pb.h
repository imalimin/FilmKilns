// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FkSize.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_FkSize_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_FkSize_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_FkSize_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_FkSize_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_FkSize_2eproto;
namespace com {
namespace alimin {
namespace fk {
namespace pb {
class FkSize;
struct FkSizeDefaultTypeInternal;
extern FkSizeDefaultTypeInternal _FkSize_default_instance_;
}  // namespace pb
}  // namespace fk
}  // namespace alimin
}  // namespace com
PROTOBUF_NAMESPACE_OPEN
template<> ::com::alimin::fk::pb::FkSize* Arena::CreateMaybeMessage<::com::alimin::fk::pb::FkSize>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace com {
namespace alimin {
namespace fk {
namespace pb {

// ===================================================================

class FkSize final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:com.alimin.fk.pb.FkSize) */ {
 public:
  inline FkSize() : FkSize(nullptr) {}
  ~FkSize() override;
  explicit constexpr FkSize(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  FkSize(const FkSize& from);
  FkSize(FkSize&& from) noexcept
    : FkSize() {
    *this = ::std::move(from);
  }

  inline FkSize& operator=(const FkSize& from) {
    CopyFrom(from);
    return *this;
  }
  inline FkSize& operator=(FkSize&& from) noexcept {
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
  static const FkSize& default_instance() {
    return *internal_default_instance();
  }
  static inline const FkSize* internal_default_instance() {
    return reinterpret_cast<const FkSize*>(
               &_FkSize_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(FkSize& a, FkSize& b) {
    a.Swap(&b);
  }
  inline void Swap(FkSize* other) {
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
  void UnsafeArenaSwap(FkSize* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  FkSize* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<FkSize>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const FkSize& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const FkSize& from);
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
  void InternalSwap(FkSize* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "com.alimin.fk.pb.FkSize";
  }
  protected:
  explicit FkSize(::PROTOBUF_NAMESPACE_ID::Arena* arena,
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
    kWidthFieldNumber = 1,
    kHeightFieldNumber = 2,
  };
  // int32 width = 1;
  void clear_width();
  int32_t width() const;
  void set_width(int32_t value);
  private:
  int32_t _internal_width() const;
  void _internal_set_width(int32_t value);
  public:

  // int32 height = 2;
  void clear_height();
  int32_t height() const;
  void set_height(int32_t value);
  private:
  int32_t _internal_height() const;
  void _internal_set_height(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:com.alimin.fk.pb.FkSize)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t width_;
  int32_t height_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_FkSize_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// FkSize

// int32 width = 1;
inline void FkSize::clear_width() {
  width_ = 0;
}
inline int32_t FkSize::_internal_width() const {
  return width_;
}
inline int32_t FkSize::width() const {
  // @@protoc_insertion_point(field_get:com.alimin.fk.pb.FkSize.width)
  return _internal_width();
}
inline void FkSize::_internal_set_width(int32_t value) {
  
  width_ = value;
}
inline void FkSize::set_width(int32_t value) {
  _internal_set_width(value);
  // @@protoc_insertion_point(field_set:com.alimin.fk.pb.FkSize.width)
}

// int32 height = 2;
inline void FkSize::clear_height() {
  height_ = 0;
}
inline int32_t FkSize::_internal_height() const {
  return height_;
}
inline int32_t FkSize::height() const {
  // @@protoc_insertion_point(field_get:com.alimin.fk.pb.FkSize.height)
  return _internal_height();
}
inline void FkSize::_internal_set_height(int32_t value) {
  
  height_ = value;
}
inline void FkSize::set_height(int32_t value) {
  _internal_set_height(value);
  // @@protoc_insertion_point(field_set:com.alimin.fk.pb.FkSize.height)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace fk
}  // namespace alimin
}  // namespace com

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_FkSize_2eproto
