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
                                                    colorFormat(o.colorFormat),
                                                    rotation(o.rotation) {
};

AlBitmapInfo::~AlBitmapInfo() {

}

void AlBitmapInfo::dump() {
    Logcat::i("AlBitmapInfo", "%s(%d): %dx%d, depth=%d, colorFormat=%d, rotation=%f",
              __FUNCTION__,
              __LINE__,
              width,
              height,
              depth,
              colorFormat,
              rotation.toFloat());
}

void AlBitmapInfo::reset() {
    width = 0;
    height = 0;
    depth = 0;
    colorFormat = AlColorFormat::NONE;
}

bool AlBitmapInfo::isNull() {
    return width <= 0 || height <= 0 || depth <= 0 || AlColorFormat::NONE == colorFormat;
}

int AlBitmapInfo::bitsPerPixel(AlColorFormat colorFormat) {
    switch (colorFormat) {
        case AlColorFormat::GRAY:
            return 8;
        case AlColorFormat::YV12:
        case AlColorFormat::NV12:
            return 12;
        case AlColorFormat::RGB:
            return 24;
        case AlColorFormat::RGBA:
            return 32;
        default:
            return 0;
    }
}
