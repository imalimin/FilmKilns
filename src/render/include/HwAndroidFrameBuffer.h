/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWANDROIDFRAMEBUFFER_H
#define HWVC_ANDROID_HWANDROIDFRAMEBUFFER_H

#include "HwAbsFrameBuffer.h"

/**
 * Also, you must add -DEGL_EGLEXT_PROTOTYPES and -DGL_GLEXT_PROTOTYPES to you cmake flags.
 */
#if defined(ANDROID) && __ANDROID_API__ >= 26

#include <android/hardware_buffer.h>
#include "GLES2/gl2ext.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>

#endif

class HwAndroidFrameBuffer : public HwAbsFrameBuffer {
public:
    HwAndroidFrameBuffer(int w, int h);

    virtual ~HwAndroidFrameBuffer();

protected:
    virtual void createTexture() override;

#if defined(ANDROID) && __ANDROID_API__ >= 26
private:

    bool createEGLImage(EGLDisplay dpy, EGLImageKHR *pImageKHR);

private:
    AHardwareBuffer *buf = nullptr;
    EGLDisplay dpy = EGL_NO_DISPLAY;
    EGLImageKHR pImageKHR = nullptr;

#endif
};


#endif //HWVC_ANDROID_HWANDROIDFRAMEBUFFER_H
