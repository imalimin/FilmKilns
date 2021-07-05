/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkObject.h"
#include <typeinfo>

FkObject::FkObject() : Object() {
    FK_MARK_SUPER
}

FkObject::~FkObject() {

}

const FkClassType &FkObject::getClassType() const {
    return clsType;
}