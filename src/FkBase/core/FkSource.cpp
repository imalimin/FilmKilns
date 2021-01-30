/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSource.h"

FkSource::FkSource() : FkObject() {
    FK_MARK_SUPER
}

FkSource::FkSource(const FkSource &o) : FkObject(o), id(o.id) {
    FK_MARK_SUPER
}

FkSource::~FkSource() {

}
