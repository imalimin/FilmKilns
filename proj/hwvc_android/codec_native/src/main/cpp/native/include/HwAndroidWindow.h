/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_HWANDROIDWINDOW_H
#define HARDWAREVIDEOCODEC_HWANDROIDWINDOW_H

#include <jni.h>
#include <android/native_window_jni.h>
#include "HwWindow.h"

class HwAndroidWindow : public HwWindow {
public:
    HwAndroidWindow();

    HwAndroidWindow(JNIEnv *env, jobject surface);

    virtual ~HwAndroidWindow();
};


#endif //HARDWAREVIDEOCODEC_HWANDROIDWINDOW_H
