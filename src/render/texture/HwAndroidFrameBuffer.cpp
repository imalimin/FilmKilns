/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAndroidFrameBuffer.h"
#include "HwUILoader.h"
#include "Logcat.h"
//#include <media/NdkImageReader.h>

HwAndroidFrameBuffer::HwAndroidFrameBuffer(int w, int h) : HwAbsFrameBuffer(w, h) {
    sdk = HwAndroidUtils::getAndroidApi();
    /**
     * Support GraphicBuffer.
     * sdk >= 26: AHardwareBuffer.
     * sdk == 24 || sdk == 25: Normal.
     * sdk <= 23: GraphicBuffer.
     */
    if (sdk >= 26 || sdk <= 23) {
        createTexture();
    } else {
        frameBuffer = new HwFrameBuffer(w, h);
    }
}

HwAndroidFrameBuffer::~HwAndroidFrameBuffer() {
    if (buf) {
        HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_release(buf);
        buf = nullptr;
    }
    if (pImageKHR && dpy) {
        eglDestroyImageKHR(dpy, pImageKHR);
        pImageKHR = nullptr;
    }
    dpy = EGL_NO_DISPLAY;
    if (frameBuffer) {
        delete frameBuffer;
        frameBuffer = nullptr;
    }
}

GLuint HwAndroidFrameBuffer::getFrameBuffer() {
    if (frameBuffer) {
        return frameBuffer->getFrameBuffer();
    } else {
        return HwAbsFrameBuffer::getFrameBuffer();
    }
}

GLuint HwAndroidFrameBuffer::getFrameTexture() {
    if (frameBuffer) {
        return frameBuffer->getFrameTexture();
    } else {
        return HwAbsFrameBuffer::getFrameTexture();
    }
}

bool HwAndroidFrameBuffer::read(uint8_t *pixels) {
    if (frameBuffer) {
        return frameBuffer->read(pixels);
    }
    if (!pixels) {
        return false;
    }
    ARect rect = {0, 0, width(), height()};
    int32_t fence = -1;
    uint8_t *pBuf = nullptr;
    HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_lock(buf,
                                                                  AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN,
                                                                  fence, &rect,
                                                                  reinterpret_cast<void **>(&pBuf));
    Logcat::i("HWVC", "HwAndroidFrameBuffer::read egl image %p", pBuf);
    memcpy(pixels, pBuf, width() * height() * 4);
    HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_unlock(buf, &fence);
    return true;
}

void HwAndroidFrameBuffer::createTexture() {
    glGenTextures(1, &id);
    glGenFramebuffers(1, &fbo);
    glBindTexture(GL_TEXTURE_2D, id);
    if (sdk >= 26) {
        dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        createEGLImage(dpy, &pImageKHR);
        glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES) pImageKHR);
    } else if (sdk <= 23) {
        //TODO
    } else {
        return;
    }
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
}

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
//    AHardwareBuffer_allocate(&desc, &buf);
    HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_allocate(&desc, &buf);

    AHardwareBuffer_Desc desc1;
//    AHardwareBuffer_describe(buf, &desc1);
    HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_describe(buf, &desc1);
    EGLClientBuffer clientBuf = HwEGLExtLoader::getInstance()->FeglGetNativeClientBufferANDROID(
            buf);
    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    *pImageKHR = eglCreateImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_CONTEXT,
                                   EGL_NATIVE_BUFFER_ANDROID,
                                   clientBuf, eglImageAttributes);
    return true;
}