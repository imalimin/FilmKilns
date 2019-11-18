/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBitmapInfo.h"
#include "Logcat.h"

AlBitmapInfo::AlBitmapInfo() : Object() {};

AlBitmapInfo::AlBitmapInfo(const AlBitmapInfo &e) : Object(),
                                                    width(e.width),
                                                    height(e.height),
                                                    depth(e.depth),
                                                    colorSpace(e.colorSpace) {
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
};