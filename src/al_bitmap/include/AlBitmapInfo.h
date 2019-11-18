/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWBITMAPINFO_H
#define HWVC_ANDROID_HWBITMAPINFO_H

#include "Object.h"

al_class(AlBitmapInfo) {
public:
    enum class ColorSpace : int {
        NONE = -1,
        RGB,
        RGBA,
        YV12,      /** YUV420P. Called I420 on android. */
        NV12,      /** YUV420SP */
        GRAY,
    };
public:
    AlBitmapInfo();

    AlBitmapInfo(const AlBitmapInfo &e);

    virtual ~AlBitmapInfo();

    void dump();

public:
    int32_t width = 0;
    int32_t height = 0;
    uint32_t depth = 0;
    ColorSpace colorSpace = ColorSpace::NONE;
};


#endif //HWVC_ANDROID_HWBITMAPINFO_H
