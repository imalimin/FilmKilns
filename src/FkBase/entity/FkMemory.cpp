/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMemory.h"

FK_IMPL_CLASS_TYPE(FkMemory, FkObject)

FkMemory::FkMemory() : FkObject() {

}

FkMemory::FkMemory(const FkMemory &o) : FkObject(o) {

}

FkMemory::~FkMemory() {

}