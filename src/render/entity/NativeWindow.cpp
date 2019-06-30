//
// Created by mingyi.li on 2018/12/25.
//

#include "../include/NativeWindow.h"

NativeWindow::NativeWindow(HwWindow *win, EGLContext context) {
    this->win = win;
    this->context = context;
}

NativeWindow::~NativeWindow() {
    this->win = nullptr;
    this->context = EGL_NO_CONTEXT;
}

bool NativeWindow::hasContext() {
    return EGL_NO_CONTEXT != context;
}
