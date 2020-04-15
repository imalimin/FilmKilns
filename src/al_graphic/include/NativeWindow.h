//
// Created by mingyi.li on 2018/12/25.
//

#ifndef HARDWAREVIDEOCODEC_ANATIVEWINDOW_H
#define HARDWAREVIDEOCODEC_ANATIVEWINDOW_H

#include "Object.h"
#include "Egl.h"
#include "HwWindow.h"

class NativeWindow : public Object {
public:
    HwWindow *win;

    EGLContext context = EGL_NO_CONTEXT;

    NativeWindow(HwWindow *win, EGLContext context);

    bool hasContext();

    virtual ~NativeWindow();

};


#endif //HARDWAREVIDEOCODEC_ANATIVEWINDOW_H
