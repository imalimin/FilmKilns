/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWTEXTURE_H
#define HWVC_ANDROID_HWTEXTURE_H

#include "HwAbsTexture.h"
#include "Size.h"
#include "HwBuffer.h"

class HwTexture : public HwAbsTexture {
public:
#ifdef __ANDROID__

    static HwTexture *allocOES();

#endif

    static HwTexture *alloc();

    static HwTexture *alloc(uint32_t target);

    static HwTexture *wrap(uint32_t target, uint32_t tex, int w, int h);

public:
    virtual ~HwTexture();

    virtual void bind() override;

    virtual void unbind() override;

    virtual void update(HwBuffer *buf, int32_t w, int32_t h) override;

private:
    HwTexture(uint32_t target);

    HwTexture(uint32_t target, uint32_t tex, int w, int h);

private:
    bool applied = false;
    bool isRef;
};


#endif //HWVC_ANDROID_HWTEXTURE_H
