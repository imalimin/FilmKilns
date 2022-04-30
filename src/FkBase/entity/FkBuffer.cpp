/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkBuffer.h"

FK_IMPL_CLASS_TYPE(FkBuffer, FkObject)

std::shared_ptr<FkBuffer> FkBuffer::wrap(uint8_t *data, size_t size) {
    auto buf = new FkBuffer(data, size);
    return std::shared_ptr<FkBuffer>(buf);
}

std::shared_ptr<FkBuffer> FkBuffer::alloc(size_t size) {
    auto buf = new FkBuffer(size);
    return std::shared_ptr<FkBuffer>(buf);
}

FkBuffer::FkBuffer(uint8_t *data, size_t size)
        : FkObject(), _capacity(size), _data(data), isRef(true) {

}

FkBuffer::FkBuffer(size_t size) : FkObject(), _capacity(size), isRef(false) {

    _data = static_cast<uint8_t *>(malloc(_capacity));
}

FkBuffer::~FkBuffer() {
    if (!isRef) {
        free(_data);
        _data = nullptr;
    }
}

size_t FkBuffer::capacity() {
    return _capacity;
}

uint8_t *FkBuffer::data() const {
    return _data;
}
