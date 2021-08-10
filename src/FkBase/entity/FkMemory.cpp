/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMemory.h"

FkMemory::FkMemory() : FkObject() {
    FK_MARK_SUPER
}

FkMemory::FkMemory(const FkMemory &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkMemory::~FkMemory() {

}