// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FkImageLayer.proto

#include "FkImageLayer.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace fk_pb {
constexpr FkImageLayer::FkImageLayer(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : file_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , size_(nullptr)
  , rotation_(nullptr)
  , scale_(nullptr)
  , trans_(nullptr){}
struct FkImageLayerDefaultTypeInternal {
  constexpr FkImageLayerDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~FkImageLayerDefaultTypeInternal() {}
  union {
    FkImageLayer _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT FkImageLayerDefaultTypeInternal _FkImageLayer_default_instance_;
}  // namespace fk_pb
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_FkImageLayer_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_FkImageLayer_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_FkImageLayer_2eproto = nullptr;

const uint32_t TableStruct_FkImageLayer_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::fk_pb::FkImageLayer, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::fk_pb::FkImageLayer, size_),
  PROTOBUF_FIELD_OFFSET(::fk_pb::FkImageLayer, rotation_),
  PROTOBUF_FIELD_OFFSET(::fk_pb::FkImageLayer, scale_),
  PROTOBUF_FIELD_OFFSET(::fk_pb::FkImageLayer, trans_),
  PROTOBUF_FIELD_OFFSET(::fk_pb::FkImageLayer, file_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::fk_pb::FkImageLayer)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::fk_pb::_FkImageLayer_default_instance_),
};

const char descriptor_table_protodef_FkImageLayer_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\022FkImageLayer.proto\022\005fk_pb\032\014FkSize.prot"
  "o\032\020FkRational.proto\032\017FkIntVec3.proto\032\021Fk"
  "FloatVec3.proto\"\242\001\n\014FkImageLayer\022\033\n\004size"
  "\030\001 \001(\0132\r.fk_pb.FkSize\022#\n\010rotation\030\002 \001(\0132"
  "\021.fk_pb.FkRational\022!\n\005scale\030\003 \001(\0132\022.fk_p"
  "b.FkFloatVec3\022\037\n\005trans\030\004 \001(\0132\020.fk_pb.FkI"
  "ntVec3\022\014\n\004file\030\005 \001(\tb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_FkImageLayer_2eproto_deps[4] = {
  &::descriptor_table_FkFloatVec3_2eproto,
  &::descriptor_table_FkIntVec3_2eproto,
  &::descriptor_table_FkRational_2eproto,
  &::descriptor_table_FkSize_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_FkImageLayer_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_FkImageLayer_2eproto = {
  false, false, 268, descriptor_table_protodef_FkImageLayer_2eproto, "FkImageLayer.proto", 
  &descriptor_table_FkImageLayer_2eproto_once, descriptor_table_FkImageLayer_2eproto_deps, 4, 1,
  schemas, file_default_instances, TableStruct_FkImageLayer_2eproto::offsets,
  file_level_metadata_FkImageLayer_2eproto, file_level_enum_descriptors_FkImageLayer_2eproto, file_level_service_descriptors_FkImageLayer_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_FkImageLayer_2eproto_getter() {
  return &descriptor_table_FkImageLayer_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_FkImageLayer_2eproto(&descriptor_table_FkImageLayer_2eproto);
namespace fk_pb {

// ===================================================================

class FkImageLayer::_Internal {
 public:
  static const ::fk_pb::FkSize& size(const FkImageLayer* msg);
  static const ::fk_pb::FkRational& rotation(const FkImageLayer* msg);
  static const ::fk_pb::FkFloatVec3& scale(const FkImageLayer* msg);
  static const ::fk_pb::FkIntVec3& trans(const FkImageLayer* msg);
};

const ::fk_pb::FkSize&
FkImageLayer::_Internal::size(const FkImageLayer* msg) {
  return *msg->size_;
}
const ::fk_pb::FkRational&
FkImageLayer::_Internal::rotation(const FkImageLayer* msg) {
  return *msg->rotation_;
}
const ::fk_pb::FkFloatVec3&
FkImageLayer::_Internal::scale(const FkImageLayer* msg) {
  return *msg->scale_;
}
const ::fk_pb::FkIntVec3&
FkImageLayer::_Internal::trans(const FkImageLayer* msg) {
  return *msg->trans_;
}
void FkImageLayer::clear_size() {
  if (GetArenaForAllocation() == nullptr && size_ != nullptr) {
    delete size_;
  }
  size_ = nullptr;
}
void FkImageLayer::clear_rotation() {
  if (GetArenaForAllocation() == nullptr && rotation_ != nullptr) {
    delete rotation_;
  }
  rotation_ = nullptr;
}
void FkImageLayer::clear_scale() {
  if (GetArenaForAllocation() == nullptr && scale_ != nullptr) {
    delete scale_;
  }
  scale_ = nullptr;
}
void FkImageLayer::clear_trans() {
  if (GetArenaForAllocation() == nullptr && trans_ != nullptr) {
    delete trans_;
  }
  trans_ = nullptr;
}
FkImageLayer::FkImageLayer(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:fk_pb.FkImageLayer)
}
FkImageLayer::FkImageLayer(const FkImageLayer& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  file_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    file_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_file().empty()) {
    file_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_file(), 
      GetArenaForAllocation());
  }
  if (from._internal_has_size()) {
    size_ = new ::fk_pb::FkSize(*from.size_);
  } else {
    size_ = nullptr;
  }
  if (from._internal_has_rotation()) {
    rotation_ = new ::fk_pb::FkRational(*from.rotation_);
  } else {
    rotation_ = nullptr;
  }
  if (from._internal_has_scale()) {
    scale_ = new ::fk_pb::FkFloatVec3(*from.scale_);
  } else {
    scale_ = nullptr;
  }
  if (from._internal_has_trans()) {
    trans_ = new ::fk_pb::FkIntVec3(*from.trans_);
  } else {
    trans_ = nullptr;
  }
  // @@protoc_insertion_point(copy_constructor:fk_pb.FkImageLayer)
}

inline void FkImageLayer::SharedCtor() {
file_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  file_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&size_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&trans_) -
    reinterpret_cast<char*>(&size_)) + sizeof(trans_));
}

FkImageLayer::~FkImageLayer() {
  // @@protoc_insertion_point(destructor:fk_pb.FkImageLayer)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void FkImageLayer::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  file_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete size_;
  if (this != internal_default_instance()) delete rotation_;
  if (this != internal_default_instance()) delete scale_;
  if (this != internal_default_instance()) delete trans_;
}

void FkImageLayer::ArenaDtor(void* object) {
  FkImageLayer* _this = reinterpret_cast< FkImageLayer* >(object);
  (void)_this;
}
void FkImageLayer::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void FkImageLayer::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void FkImageLayer::Clear() {
// @@protoc_insertion_point(message_clear_start:fk_pb.FkImageLayer)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  file_.ClearToEmpty();
  if (GetArenaForAllocation() == nullptr && size_ != nullptr) {
    delete size_;
  }
  size_ = nullptr;
  if (GetArenaForAllocation() == nullptr && rotation_ != nullptr) {
    delete rotation_;
  }
  rotation_ = nullptr;
  if (GetArenaForAllocation() == nullptr && scale_ != nullptr) {
    delete scale_;
  }
  scale_ = nullptr;
  if (GetArenaForAllocation() == nullptr && trans_ != nullptr) {
    delete trans_;
  }
  trans_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FkImageLayer::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .fk_pb.FkSize size = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_size(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .fk_pb.FkRational rotation = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_rotation(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .fk_pb.FkFloatVec3 scale = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_scale(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .fk_pb.FkIntVec3 trans = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          ptr = ctx->ParseMessage(_internal_mutable_trans(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string file = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          auto str = _internal_mutable_file();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "fk_pb.FkImageLayer.file"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* FkImageLayer::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:fk_pb.FkImageLayer)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .fk_pb.FkSize size = 1;
  if (this->_internal_has_size()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        1, _Internal::size(this), target, stream);
  }

  // .fk_pb.FkRational rotation = 2;
  if (this->_internal_has_rotation()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        2, _Internal::rotation(this), target, stream);
  }

  // .fk_pb.FkFloatVec3 scale = 3;
  if (this->_internal_has_scale()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        3, _Internal::scale(this), target, stream);
  }

  // .fk_pb.FkIntVec3 trans = 4;
  if (this->_internal_has_trans()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        4, _Internal::trans(this), target, stream);
  }

  // string file = 5;
  if (!this->_internal_file().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_file().data(), static_cast<int>(this->_internal_file().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "fk_pb.FkImageLayer.file");
    target = stream->WriteStringMaybeAliased(
        5, this->_internal_file(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:fk_pb.FkImageLayer)
  return target;
}

size_t FkImageLayer::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:fk_pb.FkImageLayer)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string file = 5;
  if (!this->_internal_file().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_file());
  }

  // .fk_pb.FkSize size = 1;
  if (this->_internal_has_size()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *size_);
  }

  // .fk_pb.FkRational rotation = 2;
  if (this->_internal_has_rotation()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *rotation_);
  }

  // .fk_pb.FkFloatVec3 scale = 3;
  if (this->_internal_has_scale()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *scale_);
  }

  // .fk_pb.FkIntVec3 trans = 4;
  if (this->_internal_has_trans()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *trans_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData FkImageLayer::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    FkImageLayer::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*FkImageLayer::GetClassData() const { return &_class_data_; }

void FkImageLayer::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<FkImageLayer *>(to)->MergeFrom(
      static_cast<const FkImageLayer &>(from));
}


void FkImageLayer::MergeFrom(const FkImageLayer& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:fk_pb.FkImageLayer)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_file().empty()) {
    _internal_set_file(from._internal_file());
  }
  if (from._internal_has_size()) {
    _internal_mutable_size()->::fk_pb::FkSize::MergeFrom(from._internal_size());
  }
  if (from._internal_has_rotation()) {
    _internal_mutable_rotation()->::fk_pb::FkRational::MergeFrom(from._internal_rotation());
  }
  if (from._internal_has_scale()) {
    _internal_mutable_scale()->::fk_pb::FkFloatVec3::MergeFrom(from._internal_scale());
  }
  if (from._internal_has_trans()) {
    _internal_mutable_trans()->::fk_pb::FkIntVec3::MergeFrom(from._internal_trans());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void FkImageLayer::CopyFrom(const FkImageLayer& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:fk_pb.FkImageLayer)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FkImageLayer::IsInitialized() const {
  return true;
}

void FkImageLayer::InternalSwap(FkImageLayer* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &file_, lhs_arena,
      &other->file_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(FkImageLayer, trans_)
      + sizeof(FkImageLayer::trans_)
      - PROTOBUF_FIELD_OFFSET(FkImageLayer, size_)>(
          reinterpret_cast<char*>(&size_),
          reinterpret_cast<char*>(&other->size_));
}

::PROTOBUF_NAMESPACE_ID::Metadata FkImageLayer::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_FkImageLayer_2eproto_getter, &descriptor_table_FkImageLayer_2eproto_once,
      file_level_metadata_FkImageLayer_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace fk_pb
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::fk_pb::FkImageLayer* Arena::CreateMaybeMessage< ::fk_pb::FkImageLayer >(Arena* arena) {
  return Arena::CreateMessageInternal< ::fk_pb::FkImageLayer >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
