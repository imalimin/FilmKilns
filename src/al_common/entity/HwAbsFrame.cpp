/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAbsFrame.h"

HwAbsFrame::HwAbsFrame(size_t size) : Object() {
    if (size > 0) {
        buf = HwBuffer::alloc(size);
    }
}

HwAbsFrame::~HwAbsFrame() {
    delete buf;
    buf = nullptr;
}

uint8_t *HwAbsFrame::data() {
    if (buf) {
        return buf->data();
    }
    return nullptr;
}

size_t HwAbsFrame::size() {
    if (buf) {
        return buf->size();
    }
    return 0;
}