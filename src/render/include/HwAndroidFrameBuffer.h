/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWANDROIDFRAMEBUFFER_H
#define HWVC_ANDROID_HWANDROIDFRAMEBUFFER_H

#include "HwAbsFrameBuffer.h"
#include "HwAHardwareBufferLoader.h"
#include "HwEGLExtLoader.h"
#include "HwFrameBuffer.h"
#include "HwAndroidUtils.h"
#include "GLES2/gl2ext.h"

/**
 * Also, you must add -DEGL_EGLEXT_PROTOTYPES and -DGL_GLEXT_PROTOTYPES to you cmake flags.
 */
class HwAndroidFrameBuffer : public HwAbsFrameBuffer {
public:
    HwAndroidFrameBuffer(int w, int h);

    virtual ~HwAndroidFrameBuffer();

    virtual GLuint getFrameTexture() override;

    virtual GLuint getFrameBuffer() override;

    virtual bool read(uint8_t *pixels) override;

protected:
    virtual void createTexture() override;

private:
    bool createEGLImage(EGLDisplay dpy, EGLImageKHR *pImageKHR);

private:
    HwFrameBuffer *frameBuffer = nullptr;
    AHardwareBuffer *buf = nullptr;
    EGLDisplay dpy = EGL_NO_DISPLAY;
    EGLImageKHR pImageKHR = nullptr;
};


#endif //HWVC_ANDROID_HWANDROIDFRAMEBUFFER_H
