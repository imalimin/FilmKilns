/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBitmapInfo.h"
#include "Logcat.h"

AlBitmapInfo::AlBitmapInfo() : Object() {};

AlBitmapInfo::AlBitmapInfo(const AlBitmapInfo &o) : Object(),
                                                    width(o.width),
                                                    height(o.height),
                                                    depth(o.depth),
                                                    colorSpace(o.colorSpace),
                                                    rotation(o.rotation) {
};

AlBitmapInfo::~AlBitmapInfo() {

}

void AlBitmapInfo::dump() {
    Logcat::i("AlBitmapInfo", "%s(%d): %dx%d, %d, %d",
              __FUNCTION__,
              __LINE__,
              width,
              height,
              depth,
              colorSpace);
}

void AlBitmapInfo::reset() {
    width = 0;
    height = 0;
    depth = 0;
    colorSpace = AlColorSpace::NONE;
}

bool AlBitmapInfo::isNull() {
    return width <= 0 || height <= 0 || depth <= 0 || AlColorSpace::NONE == colorSpace;
}