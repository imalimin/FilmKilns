/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAbsGraphicBuffer.h"

HwAbsGraphicBuffer::HwAbsGraphicBuffer(int w, int h) : Object(), width(w), height(h) {

}

HwAbsGraphicBuffer::~HwAbsGraphicBuffer() {
    width = 0;
    height = 0;
}
