/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFIFOBuffer.h"

#define TAG "FkFIFOBuffer"

FK_IMPL_CLASS_TYPE(FkFIFOBuffer, FkObject)

/**
 * 规则
 * 1. writer不能等于reader
 * 2. writer不能越过reader，同时reader也不能越过writer
 */
FkFIFOBuffer::FkFIFOBuffer(size_t capacity) : FkObject() {
    this->capacity = capacity;
    this->buf = new uint8_t[capacity];
    this->_size = 0;
    this->reader = first();
    this->writer = first();
    this->endFlag = end();
}

FkFIFOBuffer::~FkFIFOBuffer() {
    flush();
    if (buf) {
        delete[]buf;
        buf = nullptr;
        writer = nullptr;
        reader = nullptr;
        endFlag = nullptr;
    }
    capacity = 0;
}

bool FkFIFOBuffer::willWrite(size_t size) {
    /*-----------------*/
    /* ... w ... r ... */
    /*-----------------*/
    if (this->writer < this->reader && this->writer + size >= this->reader) {
        return false;
    }
    /*-----------------*/
    /* ... r ... w ... */
    /*-----------------*/
    if (this->writer > this->reader) {
        size_t left = end() + 1 - this->writer;
        if (left <= size) {//末尾剩余空间不足，需要移动到first
            /*-----------------*/
            /* r ... w ... */
            /*-----------------*/
            if (this->reader == first()) {
                return false;
            }
            if (left < size) {
                this->writer = first();
                /*-------------*/
                /* w ... r ... */
                /*-------------*/
                if (this->writer + size >= this->reader) {
                    return false;
                }
                this->endFlag = this->writer - 1;
            }
        }
    }
    return true;
}

uint8_t *FkFIFOBuffer::move(uint8_t *pointer, size_t size) {
    if (pointer + size > end()) {
        return first();
    }
    return pointer + size;
}

size_t FkFIFOBuffer::push(uint8_t *data, size_t size) {
    return push(data, size, 0);
}

size_t FkFIFOBuffer::push(uint8_t *data, size_t size, int32_t timeOut) {
    if (!buf) {
        return 0;
    }
    if (!willWrite(size)) {
#ifdef LOG_FkFIFOBuffer
        FkLogE(TAG, "FkFIFOBuffer::push Capacity is insufficient(left=%d). Wait",
                  leftCapacity());
#endif
        if (timeOut <= 0) {
            return 0;
        }
        pushLock.wait(timeOut);
        if (requestFlush || !willWrite(size)) {//if time out or flush, return this.
            requestFlush = false;
            return 0;
        }
    }
    memcpy(this->writer, data, size);
    this->writer = move(this->writer, size);
    this->_size += size;
    takeLock.notify();
#ifdef LOG_FkFIFOBuffer
    FkLogI(TAG, "FkFIFOBuffer::push %d, %d",
              size,
              this->size());
#endif
    printBufferState();
    return size;
}

bool FkFIFOBuffer::willRead(size_t size) {
    /*-----------------*/
    /* ... r ... w ... */
    /*-----------------*/
    if (this->reader < this->writer && this->reader + size >= this->writer) {
#ifdef LOG_FkFIFOBuffer
        FkLogE(TAG, "FkFIFOBuffer::take failed(cross a)");
#endif
        return false;
    }
    /*-----------------*/
    /* ... w ... r ... */
    /*-----------------*/
    if (this->reader > this->writer) {
        size_t left = this->endFlag + 1 - this->reader;
        if (left <= size) {
            /*-------------*/
            /* w ... r ... */
            /*-------------*/
            if (this->writer == first()) {
#ifdef LOG_FkFIFOBuffer
                FkLogI(TAG, "FkFIFOBuffer::take failed(First occupied)");
#endif
                return false;
            }
            if (left <= 0) {
#ifdef LOG_FkFIFOBuffer
                FkLogI(TAG, "FkFIFOBuffer::take failed(error) %d", left);
#endif
                return false;
            }
//            if (left < size && left != 1) {
//                size_t right = size - left;
//                if (first() + right >= this->writer) {
//                    FkLogE(TAG, "FkFIFOBuffer::take failed(cross b)");
//                    return nullptr;
//                }
//                HwBuffer *buf = HwBuffer::alloc(size);
//                memcpy(buf->getData(), this->reader, left);
//                memcpy(buf->getData() + left, first(), right);
//                this->endFlag = end();//this->endFlag失效，一个循环只允许使用一次
//                this->reader = first() + right;
//                this->_size -= size;
//                notifyLock.notify();
//                FkLogI(TAG, "FkFIFOBuffer::take c %d, %d",
//                          buf->size(),
//                          this->size());
//                printBufferState();
//                return buf;
//            }
        }
    }
    return true;
}

std::shared_ptr<FkBuffer> FkFIFOBuffer::take(size_t size) {
    return take(size, 0);
}

std::shared_ptr<FkBuffer> FkFIFOBuffer::take(size_t size, int32_t timeOut) {
    if (!buf || nullptr == this->reader) {
        FkLogE(TAG, "FkFIFOBuffer::take failed(unready)");
        return nullptr;
    }
    if (!willRead(size)) {
#ifdef LOG_FkFIFOBuffer
        FkLogI(TAG, "FkFIFOBuffer::take Capacity is insufficient(left=%d). Wait",
                  leftCapacity());
#endif
        if (timeOut <= 0) {
            return 0;
        }
        takeLock.wait(timeOut);
        if (requestFlush || !willRead(size)) {//if time out or flush, return this.
            requestFlush = false;
            return nullptr;
        }
    }
    /*-----------------*/
    /* ... w ... r ... */
    /*-----------------*/
    if (this->reader > this->writer) {
        size_t left = this->endFlag + 1 - this->reader;
        if (left <= size) {
            auto _buf = FkBuffer::wrap(this->reader, left);
            this->endFlag = end();//this->endFlag失效，一个循环只允许使用一次
            /*-------------*/
            /* r ... w ... */
            /*-------------*/
            this->reader = first();
            this->_size -= left;
            pushLock.notify();
#ifdef LOG_FkFIFOBuffer
            FkLogI(TAG, "FkFIFOBuffer::take a %d, %d",
                      buf->size(),
                      this->size());
#endif
            printBufferState();
            return _buf;
        }
    }
    auto _buf = FkBuffer::wrap(this->reader, size);
    this->reader += size;
    this->_size -= size;
    pushLock.notify();
#ifdef LOG_FkFIFOBuffer
    FkLogI(TAG, "FkFIFOBuffer::take b %d, %d",
              buf->size(),
              this->size());
#endif
    printBufferState();
    return _buf;
}

uint8_t *FkFIFOBuffer::first() {
    return this->buf;
}

uint8_t *FkFIFOBuffer::end() {
    return this->buf + capacity - 1;
}

size_t FkFIFOBuffer::leftCapacity() {
    if (this->writer > this->reader) {
        return capacity - (this->writer - this->reader);
    } else if (this->writer < this->reader) {
        return this->reader - this->writer;
    }
    return capacity - size();
}

size_t FkFIFOBuffer::size() {
    return _size;
}

void FkFIFOBuffer::flush() {
    this->reader = first();
    this->writer = first();
    this->_size = 0;
    pushLock.notify();
    takeLock.notify();
    requestFlush = true;
#ifdef LOG_FkFIFOBuffer
    FkLogI(TAG, "FkFIFOBuffer::flush");
#endif
}

void FkFIFOBuffer::printBufferState() {
#ifdef LOG_FkFIFOBuffer
    std::lock_guard<std::mutex> lock_guard(mutex);
    const uint8_t *reader = this->reader;
    const uint8_t *writer = this->writer;
    size_t delta = reader - writer;
    FkLogI(TAG,
              "/*--------------------------------------------------------------------------------------------------*/");
    if (delta > 0) {
        FkLogI(TAG, "/* %p .. %10d .. W%p .. %10d .. R%p .. %10d .. %p */",
                  first(),
                  writer - first(),
                  writer,
                  delta,
                  reader,
                  end() - reader,
                  end());
    } else if (delta < 0) {
        FkLogI(TAG, "/* %p .. %10d .. R%p .. %10d .. W%p .. %10d .. %p */",
                  first(),
                  reader - first(),
                  reader,
                  delta,
                  writer,
                  end() - writer,
                  end());
    } else {
        FkLogI(TAG, "/* %p .. %10d .. RW%p .. %10d .. %p */",
                  first(),
                  reader - first(),
                  reader,
                  end() - reader,
                  end());
    }
    FkLogI(TAG,
              "/*--------------------------------------------------------------------------------------------------*/");
#endif
}