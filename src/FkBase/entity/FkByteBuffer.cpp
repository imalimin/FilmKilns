/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkByteBuffer.h"

FkByteBuffer::FkByteBuffer() : FkObject() {
    FK_MARK_SUPER
}

FkByteBuffer::FkByteBuffer(const FkByteBuffer &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkByteBuffer::~FkByteBuffer() {

}