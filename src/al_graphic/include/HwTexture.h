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
#include <GLES2/gl2.h>

class HwTexture : public HwAbsTexture {
public:
#ifdef __ANDROID__

    static HwTexture *allocOES();

#endif

    static HwTexture *alloc(AlTexDescription &desc);

    static HwAbsTexture *wrap(HwAbsTexture *tex);

public:
    virtual ~HwTexture();

    virtual void bind() override;

    virtual void unbind() override;

    virtual void update(AlBuffer *buf, int32_t w, int32_t h, uint32_t fmt = GL_RGBA) override;

public:
    virtual bool read(uint8_t *pixels) override;

private:
    HwTexture(AlTexDescription &desc);

    HwTexture(const HwAbsTexture &o);

private:
    bool applied = false;
    bool isRef;
};


#endif //HWVC_ANDROID_HWTEXTURE_H
