/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-23 14:15:54
*/

#include "FkAndroidWindow.h"
#include "android/native_window.h"
#include "android/native_window_jni.h"

FK_IMPL_CLASS_TYPE(FkAndroidWindow, FkGraphicWindow)

FkAndroidWindow::FkAndroidWindow(jobject surface)
        : FkGraphicWindow(0, 0) {

    JNIEnv *env = nullptr;
    FkAssert(FkJavaRuntime::getInstance().findEnv(&env) == true,)
    auto win = ANativeWindow_fromSurface(env, surface);
    _size.set(ANativeWindow_getWidth(win), ANativeWindow_getHeight(win));
    winHandle = win;
    FkLogI(FK_DEF_TAG, "FkAndroidWindow");
}

FkAndroidWindow::~FkAndroidWindow() {
    FkLogI(FK_DEF_TAG, "~FkAndroidWindow");
    if (winHandle) {
        auto *win = static_cast<ANativeWindow *>(winHandle);
        ANativeWindow_release(win);
        winHandle = nullptr;
    }
}

void *FkAndroidWindow::getWindow() {
    return winHandle;
}
