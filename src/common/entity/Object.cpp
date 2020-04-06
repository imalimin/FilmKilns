/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Object.h"
#include <typeinfo>

Object::Object() {
}

Object::~Object() {
}

std::string Object::toString() {
    return std::string(typeid(*this).name());
}

void Object::ref() {
    ++countOfRef;
}

void Object::unref() {
    --countOfRef;
}
