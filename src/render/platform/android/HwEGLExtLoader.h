/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWEGLEXTLOADER_H
#define HWVC_ANDROID_HWEGLEXTLOADER_H

#include "../HwAbsLibLoader.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/hardware_buffer.h>

using namespace std;

class HwEGLExtLoader : public HwAbsLibLoader {
public:
    static HwEGLExtLoader *getInstance();

    typedef EGLClientBuffer (*signature_B_PB)(const struct AHardwareBuffer *);

    signature_B_PB FeglGetNativeClientBufferANDROID;

protected:
    bool open() override;

    void load() override;

    bool close() override;

    template<typename T>
    static T load_func(void *handler, const char *funcName);

private:
    HwEGLExtLoader();

    ~HwEGLExtLoader();

private:
    const string LIB_NAME = "libEGL.so";
    void *pLibHandler = nullptr;

};


#endif //HWVC_ANDROID_HWEGLEXTLOADER_H
