/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkShape.h"

FkShape::FkShape() : FkObject() {
    FK_MARK_SUPER
}

FkShape::FkShape(const FkShape &o) : FkObject(o), type(o.type) {
    FK_MARK_SUPER
}

FkShape::~FkShape() {

}

FkShape::kType FkShape::getType() {
    return type;
}