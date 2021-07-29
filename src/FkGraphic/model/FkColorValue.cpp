/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColorValue.h"

FkColorValue::FkColorValue() : FkObject(), color(FkColor::white()) {
    FK_MARK_SUPER
}

FkColorValue::FkColorValue(const FkColorValue &o) : FkObject(o), color(o.color) {
    FK_MARK_SUPER
}

FkColorValue::~FkColorValue() {

}