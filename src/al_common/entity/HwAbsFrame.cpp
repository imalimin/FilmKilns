/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAbsFrame.h"

HwAbsFrame::HwAbsFrame(size_t size) : Object() {
    buf = HwBuffer::alloc(size);
}

HwAbsFrame::~HwAbsFrame() {
    if (buf) {
        delete buf;
        buf = nullptr;
    }
}

uint8_t *HwAbsFrame::data() { return buf->data(); }

size_t HwAbsFrame::size() { return buf->size(); }