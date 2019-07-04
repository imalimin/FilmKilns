/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWCAMERARECORDER_H
#define HWVC_ANDROID_HWCAMERARECORDER_H

#include "HwAbsProcessor.h"
#include "HwWindow.h"

class HwCameraRecorder : public HwAbsProcessor {
public:
    HwCameraRecorder();

    virtual ~HwCameraRecorder();

    void prepare(HwWindow *win);

    void updateWindow(HwWindow *win);

    void start();

    void pause();

    void invalidate(int textureId, int w, int h);

};


#endif //HWVC_ANDROID_HWCAMERARECORDER_H
