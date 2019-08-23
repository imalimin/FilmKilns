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
#include "HwBuffer.h"

class HwTexture : public Object {
public:
#ifdef __ANDROID__

    static HwTexture *allocOES();

#endif

    static HwTexture *alloc();

    static HwTexture *alloc(uint32_t target);

    static HwTexture *wrap(uint32_t target, uint32_t tex, int w, int h);

public:
    virtual ~HwTexture();

    uint32_t texId();

    int getWidth();

    int getHeight();

    void bind();

    void unbind();

    uint32_t target();

    void update(HwBuffer *buf, int32_t w, int32_t h);

private:
    HwTexture(uint32_t target);

    HwTexture(uint32_t target, uint32_t tex, int w, int h);

private:
    uint32_t tar;
    uint32_t tex;
    uint32_t fmt;
    Size size;
    bool applied = false;
    bool isRef;
};


#endif //HWVC_ANDROID_HWTEXTURE_H
