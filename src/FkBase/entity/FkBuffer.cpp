/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkBuffer.h"

FkBuffer::FkBuffer(size_t size) : FkObject(), _capacity(size) {
    FK_MARK_SUPER
    _data = static_cast<uint8_t *>(malloc(_capacity));
}

FkBuffer::FkBuffer(const FkBuffer &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkBuffer::~FkBuffer() {

}

size_t FkBuffer::capacity() {
    return _capacity;
}

uint8_t *FkBuffer::data() const {
    return _data;
}
