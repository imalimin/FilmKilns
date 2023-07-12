/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkString.h"

FK_IMPL_CLASS_TYPE(FkStringBuilder, FkObject)
FK_IMPL_CLASS_TYPE(FkString, FkObject)

FkStringBuilder::FkStringBuilder(const FkString &str) : FkObject() {

}

std::string FkString::valueOf(const std::string &value) {
    return value;
}

std::string FkString::valueOf(const FkString &value) {
    return value._str;
}

std::vector<std::string> FkString::split(const std::string &str, const std::string &pattern) {
    char *c_str = new char[strlen(str.c_str()) + 1];
    strcpy(c_str, str.c_str());
    std::vector<std::string> result;
    char *tmpStr = strtok(c_str, pattern.c_str());
    while (tmpStr != NULL) {
        result.push_back(std::string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }
    delete[] c_str;
    return result;
}

FkString::FkString() : FkString("") {

}

FkString::FkString(const char *str)
        : FkObject(), _str(str) {

}

FkString::FkString(const std::string &str)
        : FkObject(), _str(str) {

}

FkString::FkString(const FkString &o)
        : FkObject(), _str(o._str) {

}

FkString::~FkString() {

}

std::string &FkString::str() {
    return _str;
}

const char *FkString::c_str() {
    return _str.c_str();
}

const char *FkString::c_str() const {
    return _str.c_str();
}

FkString &FkString::begin() {
    appendBegin = true;
    return *this;
}

FkString &FkString::end() {
    appendBegin = false;
    return *this;
}

size_t FkString::length() {
    return _str.length();
}

void FkString::clear() {
    _str.clear();
}

FkString &FkString::replaceAll(const char *oldStr, const char *newStr) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += strlen(newStr)) {
        pos = _str.find(oldStr, pos);
        if (pos != std::string::npos)
            _str.replace(pos, strlen(oldStr), newStr);
        else
            break;
    }
    return *this;
}

std::string FkString::toString() {
    return str();
}

bool FkString::startWith(std::string str) {
    if (_str.size() < str.size()) {
        return false;
    }
    for (int i = 0; i < str.size(); ++i) {
        if (_str[i] != str[i]) {
            return false;
        }
    }
    return true;
}