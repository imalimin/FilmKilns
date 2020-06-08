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

AlBuffer *AlBuffer::wrap(AlBuffer *buf) {
    return new AlBuffer(*buf);
}

AlBuffer::AlBuffer(size_t size)
        : Object(),
          _size(size),
          _limit(size),
          _capacity(size),
          _position(0),
          buf(new uint8_t[size]),
          isRef(false) {
}

AlBuffer::AlBuffer(uint8_t *refBuf, size_t size)
        : Object(),
          _size(size),
          _limit(size),
          _capacity(size),
          _position(0),
          buf(refBuf),
          isRef(true) {
}

AlBuffer::AlBuffer(const AlBuffer &o)
        : Object(),
          _size(o._size),
          _limit(o._limit),
          _capacity(o._capacity),
          _position(o._position),
          buf(o.buf),
          isRef(true) {

}

AlBuffer::~AlBuffer() {
    this->_size = 0;
    this->_limit = 0;
    this->_capacity = 0;
    this->_position = 0;
    if (!isRef && buf) {
        delete[] buf;
        buf = nullptr;
    }
}

size_t AlBuffer::size() { return this->_size; }

void AlBuffer::rewind() {
    _position = 0;
//    mark = -1;
}

size_t AlBuffer::remaining() {
    return _limit - _position;
}

size_t AlBuffer::capacity() {
    return _capacity;
}

void AlBuffer::limit(size_t newLimit) {
    assert(_limit <= _capacity && _limit > 0);
    _limit = newLimit;
}

uint8_t *AlBuffer::data() { return this->buf + this->_position; }

size_t AlBuffer::put(AlBuffer *buf) {
    if (!buf) return 0;
    return put(buf->data(), buf->remaining());
}

size_t AlBuffer::put(uint8_t *data, size_t size) {
    if (nullptr == data || size <= 0) return 0;
    if (size > remaining()) {
        return 0;
    }
    memcpy(this->data(), data, size);
    _movePosition(size);
    return size;
}

size_t AlBuffer::get(uint8_t *dst, size_t size) {
    if (nullptr == dst || size <= 0 || size > remaining()) return 0;
    memcpy(dst, this->data(), size);
    return size;
}

void AlBuffer::_movePosition(size_t offset) {
    _position += offset;
}
