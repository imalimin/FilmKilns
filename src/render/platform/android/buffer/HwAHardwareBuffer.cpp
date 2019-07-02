/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAHardwareBuffer.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Logcat.h"

HwAHardwareBuffer::HwAHardwareBuffer(int w, int h) : HwAbsGraphicBuffer(w, h) {
    AHardwareBuffer_Desc desc;
    desc.format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM;
    desc.width = static_cast<uint32_t>(width);
    desc.height = static_cast<uint32_t>(height);
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
    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    pImageKHR = eglCreateImageKHR(dpy, EGL_NO_CONTEXT,
                                  EGL_NATIVE_BUFFER_ANDROID,
                                  clientBuf, eglImageAttributes);
}

HwAHardwareBuffer::~HwAHardwareBuffer() {
    if (buf) {
        HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_release(buf);
        buf = nullptr;
    }
    if (pImageKHR && dpy) {
        eglDestroyImageKHR(dpy, pImageKHR);
        pImageKHR = nullptr;
    }
    dpy = EGL_NO_DISPLAY;
}

bool HwAHardwareBuffer::bind() {
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES) pImageKHR);
    return true;
}

bool HwAHardwareBuffer::read(uint8_t *pixels) {
    if (!pixels) {
        return false;
    }
    ARect rect = {0, 0, width, height};
    int32_t fence = -1;
    uint8_t *pBuf = nullptr;
    HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_lock(buf,
                                                                  AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN,
                                                                  fence, &rect,
                                                                  reinterpret_cast<void **>(&pBuf));
    Logcat::i("HWVC", "HwAHardwareBuffer::read egl image %p", pBuf);
    memcpy(pixels, pBuf, width * height * 4);
    HwAHardwareBufferLoader::getInstance()->FAHardwareBuffer_unlock(buf, &fence);
    return true;
}
