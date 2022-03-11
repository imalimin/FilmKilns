/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKDEFINITION_H
#define FK_BASE_FKDEFINITION_H

#include "AlLogcat.h"
#include "FkClassType.h"

// +-------- Define --------+
#define FK_CLASS class
#define FK_ABS_CLASS class
#define FK_INTERFACE class
#define FK_ENUM enum class
#define FK_EXTEND : public
#define FK_EXT_AND , public
#define FK_NULLABLE
#define Fk_POINTER_CAST(CLASS, ptr) std::static_pointer_cast<CLASS>(ptr)

#ifndef __FK_DEBUG__
#define __FK_OPTIMIZE_CLASS_TYPE__
#endif

#ifdef __FK_OPTIMIZE_CLASS_TYPE__
#define FK_MARK_SUPER clsType.super(typeid(*this).hash_code());
#else
#define FK_MARK_SUPER clsType.super(typeid(*this).name());
#endif

#define FK_CLASS_TYPE_EQUALS(src, target) ((src).getClassType() == (target).getClassType())
#define FK_CLASS_TYPE_EQUALS2(src, target) ((src)->getClassType() == (target)->getClassType())
#define FK_INSTANCE_OF(src, CLASS) ((src)->getClassType() == FkClassType::type<CLASS>())

// +-------- Log --------+
#define FK_DEF_TAG "FilmKilns"
#define FkLogV(tag, fmt, args...) AlLogV(tag, fmt, ##args)
#define FkLogD(tag, fmt, args...) AlLogD(tag, fmt, ##args)
#define FkLogI(tag, fmt, args...) AlLogI(tag, fmt, ##args)
#define FkLogE(tag, fmt, args...) AlLogE(tag, fmt, ##args)
#define FkLogW(tag, fmt, args...) AlLogW(tag, fmt, ##args)

// +-------- Assert --------+
#ifdef __FK_DEBUG__
#include <assert.h>
#define FkAssert(condition, value) assert(condition)
#else
#define FkAssert(condition, value)            \
    if(!(condition)) {                        \
        FkLogW("FkAssert", "assert(%s) failed.", #condition); \
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
    CENTER_MATRIX = 0,
    CENTER_INSIDE,
    CENTER_CROP,
};

#endif //FK_BASE_FKDEFINITION_H
