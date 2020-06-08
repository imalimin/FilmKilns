/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPRIMITIVETYPE_H
#define HWVC_ANDROID_ALPRIMITIVETYPE_H

#include "Object.h"
#include "StringUtils.h"

template<typename T>
AL_CLASS AlPrimitiveType AL_EXTEND Object {
public:
    AlPrimitiveType(T value) : Object(), _value(value) {

    }

    AlPrimitiveType(const AlPrimitiveType &o) : Object(), _value(o._value) {

    }

    T value() {
        return _value;
    }

    virtual ~AlPrimitiveType() {

    }

    virtual std::string toString() override {
        return StringUtils::valueOf(_value);
    }

    bool operator==(const AlPrimitiveType &o) const {
        return o._value == _value;
    }

private:
    T _value;
};

AL_CLASS AlChar AL_EXTEND AlPrimitiveType<char> {
public:
    AlChar(char value) : AlPrimitiveType(value) {

    }

    AlChar(const AlChar &o) : AlPrimitiveType(o) {

    }

    virtual ~AlChar() {

    }
};

AL_CLASS AlByte AL_EXTEND AlPrimitiveType<uint8_t> {
public:
    AlByte(uint8_t value) : AlPrimitiveType(value) {

    }

    AlByte(const AlByte &o) : AlPrimitiveType(o) {

    }

    virtual ~AlByte() {

    }
};

AL_CLASS AlInteger AL_EXTEND AlPrimitiveType<int32_t> {
public:
    AlInteger(int value) : AlPrimitiveType(value) {

    }

    AlInteger(const AlInteger &o) : AlPrimitiveType(o) {

    }

    virtual ~AlInteger() {

    }
};

AL_CLASS AlLong AL_EXTEND AlPrimitiveType<int64_t> {
public:
    static constexpr int64_t ZERO = 0L;
    AlLong(int64_t value) : AlPrimitiveType(value) {

    }

    AlLong(const AlLong &o) : AlPrimitiveType(o) {

    }

    virtual ~AlLong() {

    }
};

AL_CLASS AlFloat AL_EXTEND AlPrimitiveType<float> {
public:
    AlFloat(float value) : AlPrimitiveType(value) {

    }

    AlFloat(const AlFloat &o) : AlPrimitiveType(o) {

    }

    virtual ~AlFloat() {

    }
};

AL_CLASS AlDouble AL_EXTEND AlPrimitiveType<double> {
public:
    AlDouble(double value) : AlPrimitiveType(value) {

    }

    AlDouble(const AlDouble &o) : AlPrimitiveType(o) {

    }

    virtual ~AlDouble() {

    }
};

#endif //HWVC_ANDROID_ALPRIMITIVETYPE_H
