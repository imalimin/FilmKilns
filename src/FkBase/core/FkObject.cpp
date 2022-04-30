/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkObject.h"
#include <typeinfo>

const char *FkObject_Class::name = "FkObject";
const FkClassType FkObject_Class::type(name, typeid(FkObject_Class::Type).hash_code());

FkObject::FkObject() {
}

FkObject::~FkObject() {

}

std::string FkObject::toString() {
    return std::string(getClassType().getName());
}
