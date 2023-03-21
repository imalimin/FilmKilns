/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKPRIMITIVETYPE_H
#define FK_BASE_FKPRIMITIVETYPE_H

#include "FkObject.h"
#include "FkString.h"

FK_SUPER_TEMPLATE_CLASS(FkPrimitiveType, FkObject, class T) {
FK_DEF_CLASS_TYPE_FUNC(FkPrimitiveType)

public:
    FkPrimitiveType(T value) : FkObject(), _value(value) {

    }

    FkPrimitiveType(const FkPrimitiveType &o) : FkObject(), _value(o._value) {

    }

    T value() {
        return _value;
    }

    virtual ~FkPrimitiveType() {

    }

    virtual std::string toString() override {
        return FkString::valueOf(_value);
    }

    bool operator==(const FkPrimitiveType &o) const {
        return o._value == _value;
    }

private:
    T _value;
};

#define FK_DEFINE_PRIMITIVE(NAME, TYPE)  \
FK_SUPER_TEMPLATE_CLASS_IMPL(NAME, FkPrimitiveType)<TYPE> { \
FK_DEF_CLASS_TYPE_FUNC(NAME) \
public: \
    NAME(TYPE value) : FkPrimitiveType(value) {} \
    NAME(const NAME &o) : FkPrimitiveType(o) {} \
    virtual ~NAME() {}\
};                                       \

FK_DEFINE_PRIMITIVE(FkChar, char)
FK_DEFINE_PRIMITIVE(FkByte, uint8_t)
FK_DEFINE_PRIMITIVE(FkInteger, int32_t)
FK_DEFINE_PRIMITIVE(FkLong, int64_t)
FK_DEFINE_PRIMITIVE(FkFloat, float)
FK_DEFINE_PRIMITIVE(FkDouble, double)

#endif //FK_BASE_FKPRIMITIVETYPE_H
