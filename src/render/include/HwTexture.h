/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWTEXTURE_H
#define HWVC_ANDROID_HWTEXTURE_H

#include "Object.h"
#include "Size.h"

class HwTexture : public Object {
public:
    HwTexture(uint32_t tex, int w, int h);

    virtual ~HwTexture();

    uint32_t texId();

    int getWidth();

    int getHeight();

private:
    uint32_t tex;
    Size size;
};


#endif //HWVC_ANDROID_HWTEXTURE_H
