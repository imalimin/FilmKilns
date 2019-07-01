/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWABSFRAMEBUFFER_H
#define HWVC_ANDROID_HWABSFRAMEBUFFER_H

#include "Object.h"
#include "Size.h"
#include <GLES2/gl2.h>

class HwAbsFrameBuffer : public Object {
public:
    HwAbsFrameBuffer(int w, int h);

    virtual ~HwAbsFrameBuffer();

    int width();

    int height();

    GLuint getFrameTexture();

    GLuint getFrameBuffer();

    virtual bool read(uint8_t *pixels);

protected:
    virtual void createTexture() = 0;

protected:
    GLuint id = GL_NONE;
    GLuint fbo = GL_NONE;
    Size *size = nullptr;
    GLenum fmt = GL_RGBA;
};


#endif //HWVC_ANDROID_HWABSFRAMEBUFFER_H
