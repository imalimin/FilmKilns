/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAndroidFrameBuffer.h"
#include "Logcat.h"
//#include <media/NdkImageReader.h>

HwAndroidFrameBuffer::HwAndroidFrameBuffer(int w, int h) : HwAbsFrameBuffer(w, h) {
    createTexture();
}

HwAndroidFrameBuffer::~HwAndroidFrameBuffer() {
#if defined(ANDROID) && __ANDROID_API__ >= 26
    if (buf) {
        AHardwareBuffer_release(buf);
        buf = nullptr;
    }
    if (pImageKHR && dpy) {
        eglDestroyImageKHR(dpy, pImageKHR);
        pImageKHR = nullptr;
    }
    dpy = EGL_NO_DISPLAY;
#endif
}

bool HwAndroidFrameBuffer::read(uint8_t *pixels) {
#if defined(ANDROID) && __ANDROID_API__ >= 26
    if (!pixels) {
        return false;
    }
    ARect rect = {0, 0, width(), height()};
    int32_t fence = -1;
    uint8_t *pBuf = nullptr;
    AHardwareBuffer_lock(buf, AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN, fence, &rect,
                         reinterpret_cast<void **>(&pBuf));
    Logcat::i("HWVC", "HwAndroidFrameBuffer::read %p", pBuf);
    memcpy(pixels, pBuf, width() * height() * 4);
    AHardwareBuffer_unlock(buf, &fence);
    return true;
#else
    return HwAbsFrameBuffer::read(pixels);
#endif
}

void HwAndroidFrameBuffer::createTexture() {
#if defined(ANDROID) && __ANDROID_API__ >= 26
    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    createEGLImage(dpy, &pImageKHR);
    glGenTextures(1, &id);
    glGenFramebuffers(1, &fbo);
    glBindTexture(GL_TEXTURE_2D, id);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES) pImageKHR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, id, 0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
#else
    glGenTextures(1, &id);
    glGenFramebuffers(1, &fbo);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, size->width, size->height, 0,
                 fmt, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, id, 0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
#endif
}

#if defined(ANDROID) && __ANDROID_API__ >= 26

bool HwAndroidFrameBuffer::createEGLImage(EGLDisplay dpy, EGLImageKHR *pImageKHR) {
    AHardwareBuffer_Desc desc;
    desc.format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM;
    desc.width = static_cast<uint32_t>(width());
    desc.height = static_cast<uint32_t>(height());
    desc.layers = 1;
    desc.rfu0 = 0;
    desc.rfu1 = 0;
    desc.stride = 10;
    desc.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_NEVER |
                 AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT;
    AHardwareBuffer_allocate(&desc, &buf);

    AHardwareBuffer_Desc desc1;
    AHardwareBuffer_describe(buf, &desc1);
    EGLClientBuffer clientBuf = eglGetNativeClientBufferANDROID(buf);
    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    *pImageKHR = eglCreateImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_CONTEXT,
                                   EGL_NATIVE_BUFFER_ANDROID,
                                   clientBuf, eglImageAttributes);
    return true;
}

#endif