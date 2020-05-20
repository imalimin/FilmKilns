/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALEGL_H
#define HWVC_ANDROID_ALEGL_H

#include "Object.h"
#include "HwWindow.h"
#include <EGL/egl.h>

al_class AlEgl al_extend Object {
public:
    static EGLContext currentContext();

    static AlEgl *offScreen(EGLContext context = nullptr);

    static AlEgl *window(HwWindow *win, EGLContext context = nullptr);

#ifdef __ANDROID__

    static al_nullable AlEgl *androidCodec(HwWindow *win, EGLContext context = nullptr);

#endif

public:
    virtual ~AlEgl();

    void makeCurrent();

    void swapBuffers();

    int width();

    int height();

    bool updateWindow(HwWindow *win);

    EGLContext getContext();

    bool isAttachWindow();

#ifdef __ANDROID__

    bool setTimestamp(int64_t nsecs);

#endif

private:
    AlEgl();

    AlEgl(const AlEgl &o) : Object() {};

    void init(EGLContext context, HwWindow *win, const int *config);

    EGLDisplay createDisplay(EGLNativeDisplayType display_id);

    EGLConfig createConfig(const int *configSpec);

    EGLContext createContext(EGLContext context);

    EGLSurface createPbufferSurface();

    EGLSurface createWindowSurface(HwWindow *win);

    EGLint getParams(EGLint attribute);

    bool checkError();

private:
    typedef EGLBoolean (EGLAPIENTRYP EGL_PRESENTATION_TIME_ANDROID)(EGLDisplay display,
                                                                    EGLSurface surface,
                                                                    khronos_stime_nanoseconds_t time);

    HwWindow *win = nullptr;
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLConfig eglConfig = nullptr;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLSurface eglSurface = EGL_NO_SURFACE;
    EGL_PRESENTATION_TIME_ANDROID eglPresentationTimeANDROID = nullptr;
};


#endif //HWVC_ANDROID_ALEGL_H
