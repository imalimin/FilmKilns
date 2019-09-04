/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWABSTEXTURE_H
#define HWVC_ANDROID_HWABSTEXTURE_H

#include "Object.h"
#include "Size.h"
#include "HwBuffer.h"
#include "HwAbsFBObject.h"
#include "GLES2/gl2.h"

class HwAbsTexture : public Object {
private:
    friend class HwAbsFBObject;

public:
    HwAbsTexture(uint32_t target);

    virtual ~HwAbsTexture();

    uint32_t texId();

    int getWidth();

    int getHeight();

    virtual void bind()=0;

    virtual void unbind()=0;

    uint32_t target();

    uint32_t fmt();

    virtual void update(HwBuffer *buf, int32_t w, int32_t h, uint32_t fmt = GL_RGBA);

public:
    /**
     * Call by HwAbsFBObject
     */
    virtual bool read(uint8_t *pixels)=0;

protected:
    uint32_t tar;
    uint32_t tex;
    uint32_t _fmt;
    Size size;
};


#endif //HWVC_ANDROID_HWABSTEXTURE_H
