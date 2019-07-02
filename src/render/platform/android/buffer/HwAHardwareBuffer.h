/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWAHARDWAREBUDDER_H
#define HWVC_ANDROID_HWAHARDWAREBUDDER_H

#include "HwAbsGraphicBuffer.h"
#include "HwAHardwareBufferLoader.h"
#include "HwEGLExtLoader.h"

class HwAHardwareBuffer : public HwAbsGraphicBuffer {
public:
    HwAHardwareBuffer(int w, int h);

    ~HwAHardwareBuffer();

    bool bind() override;

    bool read(uint8_t *pixels) override;

private:
    AHardwareBuffer *buf = nullptr;
    EGLDisplay dpy = EGL_NO_DISPLAY;
    EGLImageKHR pImageKHR = nullptr;
};


#endif //HWVC_ANDROID_HWAHARDWAREBUDDER_H
