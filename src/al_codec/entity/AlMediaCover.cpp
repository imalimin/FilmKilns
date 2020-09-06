/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCover.h"

AlMediaCover::AlMediaCover(int32_t width, int32_t height)
        : AlParcelable(), width(width), height(height) {
    _buf = AlBuffer::alloc(width * height * 4);
}

AlMediaCover::~AlMediaCover() {
    delete _buf;
}

void AlMediaCover::writeToParcel(std::shared_ptr<AlParcel> parcel) {
    parcel->writeLong(timestamp);
    parcel->writeInt(width);
    parcel->writeInt(height);
    parcel->writeByteArray(_buf->data(), _buf->size());
}

void AlMediaCover::setTimestamp(int64_t timeInUS) {
    this->timestamp = timeInUS;
}

AlBuffer *AlMediaCover::buf() {
    return _buf;
}