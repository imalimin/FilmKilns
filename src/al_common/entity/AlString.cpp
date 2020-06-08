/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlString.h"

std::string AlString::valueOf(int32_t value) {
    return _valueOf<int32_t>(value);
}

std::string AlString::valueOf(int64_t value) {
    return _valueOf<int64_t>(value);
}

std::string AlString::valueOf(float value) {
    return _valueOf<float>(value);
}

std::string AlString::valueOf(double value) {
    return _valueOf<double>(value);
}

AlString::AlString() : AlString("") {

}

AlString::AlString(const char *str)
        : Object(), _str(str) {

}

AlString::AlString(const std::string &str)
        : Object(), _str(str) {
}

AlString::AlString(const AlString &o)
        : Object(), _str(o._str) {

}

AlString::~AlString() {

}

std::string &AlString::str() {
    return _str;
}

const char *AlString::c_str() {
    return _str.c_str();
}

AlString &AlString::append(const std::string &str) {
    _str.append(str);
    return *this;
}

AlString &AlString::append(const char *str) {
    _str.append(str);
    return *this;
}

AlString &AlString::append(int32_t val) {
    return append(valueOf(val));
}

AlString &AlString::append(int64_t val) {
    return append(valueOf(val));
}

AlString &AlString::append(float val) {
    return append(valueOf(val));
}

AlString &AlString::append(double val) {
    return append(valueOf(val));
}

size_t AlString::length() {
    return _str.length();
}

void AlString::clear() {
    _str.clear();
}

AlString &AlString::replaceAll(const char *oldStr, const char *newStr) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += strlen(newStr)) {
        pos = _str.find(oldStr, pos);
        if (pos != std::string::npos)
            _str.replace(pos, strlen(oldStr), newStr);
        else
            break;
    }
    return *this;
}

std::string AlString::toString() {
    return str();
}
