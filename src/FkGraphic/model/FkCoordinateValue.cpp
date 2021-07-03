/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkCoordinateValue.h"

FkCoordinateValue::FkCoordinateValue() : FkVertexValue() {
    FK_MARK_SUPER
}

FkCoordinateValue::FkCoordinateValue(const FkCoordinateValue &o) : FkVertexValue(o) {
    FK_MARK_SUPER
}

FkCoordinateValue::~FkCoordinateValue() {

}