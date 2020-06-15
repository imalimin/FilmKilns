/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsInputDescriptor.h"

AlAbsInputDescriptor::AlAbsInputDescriptor(kType type)
        : Object(), _type(type) {

}

AlAbsInputDescriptor::AlAbsInputDescriptor(const AlAbsInputDescriptor &o)
        : Object(), _type(o._type) {

}

AlAbsInputDescriptor::~AlAbsInputDescriptor() {

}

AlAbsInputDescriptor::kType AlAbsInputDescriptor::type() {
    return _type;
}

std::string AlAbsInputDescriptor::path() {
    return std::string("");
}
