/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWABSTEXTURE_H
#define HWVC_ANDROID_HWABSTEXTURE_H

#include "Object.h"
#include "AlSize.h"
#include "AlBuffer.h"
#include "HwAbsFBObject.h"
#include "GLES2/gl2.h"

al_class(AlTexDescription) {
public:
    enum WrapMode : int {
        REPEAT = 0,
        EDGE,
        BORDER,
    };
public:
    AlTexDescription();

    AlTexDescription(uint32_t target, uint32_t fmt);

    ~AlTexDescription();

    AlTexDescription(const AlTexDescription &o);

public:
    /// GL_TEXTURE_2D,GL_TEXTURE_EXTERNAL_OES
    uint32_t target;
    /// GL_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER_EXT
    WrapMode wrapMode;
    /// GL_CLAMP_TO_BORDER_EXT mode color
    float borderColor[4];
    AlSize size;
    /// GL_RGBA
    uint32_t fmt;
};

/**
 * Use with HwAbsFBObject
 */
class HwAbsTexture : public Object {
private:
    friend class HwAbsFBObject;

public:
    HwAbsTexture(AlTexDescription desc);

    virtual ~HwAbsTexture();

    uint32_t texId();

    int getWidth();

    int getHeight();

    virtual void bind() = 0;

    virtual void unbind() = 0;

    uint32_t target();

    uint32_t fmt();

    virtual void update(AlBuffer *buf, int32_t w, int32_t h, uint32_t fmt = GL_RGBA);

public:
    /**
     * Call by HwAbsFBObject
     */
    virtual bool read(uint8_t *pixels) = 0;

protected:
    AlTexDescription desc;
    uint32_t tex;
};


#endif //HWVC_ANDROID_HWABSTEXTURE_H
