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
#define FK_NULLABLE
#define Fk_POINTER_CAST(CLASS, ptr) std::static_pointer_cast<CLASS>(ptr)

#define FK_MARK_SUPER clsType.super(typeid(*this).name());
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
        FkLogW("FkAssert", "Assert failed."); \
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

// +------ ID ------+
typedef int32_t FkID;
#define FK_ID_NONE -1

// +------ KID ------+
#define FK_KID(a, b, c, d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

FK_ENUM kState : int {
    IDL = -1,
    CREATED,
    STARTED,
    STOPPED,
};

#endif //FK_BASE_FKDEFINITION_H
