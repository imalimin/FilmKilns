/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVertexValue.h"

FkVertexValue::FkVertexValue() : FkProgramValue() {
    FK_MARK_SUPER
}

FkVertexValue::FkVertexValue(const FkVertexValue &o) : FkProgramValue(o) {
    FK_MARK_SUPER
}

FkVertexValue::~FkVertexValue() {

}