/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKDEFINITION_H
#define FK_BASE_FKDEFINITION_H

#include "FkLogcat.h"
#include "FkClassType.h"
#include <functional>

// +-------- Define --------+
#define FK_DEF_CLASS_TYPE(NAME, SUPER)                                   \
namespace NAME ## _Class {                                               \
class Type {};                                                           \
extern const char *name;                                                 \
extern const FkClassType type;                                           \
}                                                                        \

#define FK_IMPL_CLASS_TYPE(NAME, SUPER)                                  \
const char *NAME ## _Class::name = #NAME;                                \
const FkClassType NAME ## _Class::type(name, typeid(NAME ## _Class::Type).hash_code(), SUPER ## _Class::type); \

#define FK_SUPER_TEMPLATE_CLASS(NAME, SUPER, ...)                        \
FK_DEF_CLASS_TYPE(NAME, SUPER)                                           \
template<__VA_ARGS__>                                                    \
class NAME : public SUPER                                                \

#define FK_SUPER_TEMPLATE_CLASS_IMPL(NAME, SUPER)                        \
FK_DEF_CLASS_TYPE(NAME, SUPER)                                           \
class NAME : public SUPER                                                \


#define FK_SUPER_CLASS(NAME, SUPER)                                      \
FK_DEF_CLASS_TYPE(NAME, SUPER)                                           \
class NAME : public SUPER                                                \

#define FK_DEF_CLASS_TYPE_FUNC(NAME)        \
public:                                     \
virtual const FkClassType &getClassType() override { \
    return NAME ## _Class::type;            \
}                                           \

#define FK_INTERFACE class
#define FK_ENUM enum class
#define FK_EXTEND : public
#define FK_EXT_AND , public
#define FK_NULLABLE
#define Fk_POINTER_CAST(CLASS, ptr) std::static_pointer_cast<CLASS>(ptr)

#define FK_INSTANCE_OF(obj, CLASS) (obj->getClassType().is(CLASS ## _Class::type))

// +-------- Log --------+
#define FK_DEF_TAG "FilmKilns"
#define FkLogV(tag, fmt, args...) FkLogcat::v(tag, fmt, ##args)
#define FkLogD(tag, fmt, args...) FkLogcat::d(tag, fmt, ##args)
#define FkLogI(tag, fmt, args...) FkLogcat::i(tag, fmt, ##args)
#define FkLogE(tag, fmt, args...) FkLogcat::e(tag, fmt, ##args)
#define FkLogW(tag, fmt, args...) FkLogcat::w(tag, fmt, ##args)

// +-------- Assert --------+
#ifdef __FK_DEBUG__
#include <assert.h>
#define FkAssert(condition, value) assert(condition)
#else
#define FkAssert(condition, value)            \
    if(!(condition)) {                        \
        FkLogE("FkAssert", "assert(%s) failed.", #condition); \
        return value;                         \
    }
#endif

// +------ State Code ------+
typedef int32_t FkResult;
#define FK_OK 0
#define FK_FAIL -1
#define FK_INVALID_STATE -2
#define FK_PROTOCOL_NOT_ACCEPT -3
#define FK_INVALID_DATA -4
#define FK_EMPTY_DATA -5
#define FK_SKIP -6
#define FK_NPE -7
#define FK_PORT_NOT_FOUND -9
#define FK_SOURCE_NOT_FOUND -10
#define FK_NOT_SUPPORT -11
//Image
#define FK_EMPTY_CANVAS -100
//File
#define FK_FILE_NOT_FOUND -200
#define FK_FILE_OPEN_FAIL -201
//IO
#define FK_IO_FAIL -301

// +------ ID ------+
typedef int32_t FkID;
#define FK_ID_NONE -1
#define Fk_CANVAS_ID 0

// +------ KID ------+
#define FK_KID(a, b, c, d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

FK_ENUM kState : uint32_t {
    IDL = 1,
    CREATING = 1 << 1,
    CREATED = 1 << 2,
    STARTING = 1 << 3,
    RUNNING = 1 << 4,
    STOPPING = 1 << 5,
    STOPPED = 1 << 6,
    DESTROYING = 1 << 7,
};

FK_ENUM kScaleType : int {
    DEFAULT = 0,
    CENTER_MATRIX,
    CENTER_INSIDE,
    CENTER_CROP,
};

#endif //FK_BASE_FKDEFINITION_H
