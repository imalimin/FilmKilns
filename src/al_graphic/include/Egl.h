/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_EGL_H
#define HARDWAREVIDEOCODEC_EGL_H

#include <EGL/egl.h>
#include "Object.h"
#include "HwWindow.h"
#include <atomic>

al_class Egl al_extend Object {
public:
    static EGLContext currentContext();

    static Egl *create(EGLContext context, HwWindow *win, bool focusTypeWin = false);

private:
    Egl(EGLContext context, HwWindow *win, bool focusTypeWin);

    Egl(const Egl &o) : Object() {};

public:
    Egl();

    Egl(EGLContext context);

    Egl(HwWindow *win);

    Egl(EGLContext context, HwWindow *win);

    virtual ~Egl();

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
    void init(EGLContext context, HwWindow *win);

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

    std::atomic_bool focusTypeWin;
    HwWindow *win = nullptr;
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLConfig eglConfig = nullptr;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLSurface eglSurface = EGL_NO_SURFACE;
    EGL_PRESENTATION_TIME_ANDROID eglPresentationTimeANDROID = nullptr;
};


#endif //HARDWAREVIDEOCODEC_EGL_H
