/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBuffer.h"

AlBuffer *AlBuffer::alloc(size_t size) {
    return new AlBuffer(size);
}

AlBuffer *AlBuffer::wrap(uint8_t *buf, size_t size) {
    return new AlBuffer(buf, size);
}

AlBuffer::AlBuffer(size_t size) : Object() {
    this->_size = size;
    this->buf = new uint8_t[size];
    this->isRef = false;
}

AlBuffer::AlBuffer(uint8_t *refBuf, size_t size) : Object() {
    this->_size = size;
    this->buf = refBuf;
    this->isRef = true;
}

AlBuffer::~AlBuffer() {
    _size = 0;
    if (!isRef && buf) {
        delete[] buf;
        buf = nullptr;
    }
}

size_t AlBuffer::size() { return this->_size; }

uint8_t *AlBuffer::data() { return this->buf; }

size_t AlBuffer::put(AlBuffer *buf) {
    if (!buf) return 0;
    size_t s = min(buf->size(), size());
    memcpy(data(), buf->data(), s);
    return s;
}

size_t AlBuffer::put(uint8_t *data, size_t size) {
    if (!data || size <= 0) return 0;
    size_t s = min(size, this->size());
    memcpy(this->data(), data, s);
    return s;
}

size_t AlBuffer::get(uint8_t *data, int32_t offset, size_t size) {
    if (!data || size <= 0 || offset > this->size()) return 0;
    size_t s = min(size, this->size() - offset);
    memcpy(data, this->data(), s);
    return s;
}