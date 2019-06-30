/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwCameraRecorder.h"
#include "../include/HwCameraInput.h"
#include "ObjectBox.h"
#include "NativeWindow.h"

HwCameraRecorder::HwCameraRecorder() : HwAbsProcessor("HwCameraRecorder") {
    startPipeline();
    registerAnUnit(new HwCameraInput());
}

HwCameraRecorder::~HwCameraRecorder() {
    stopPipeline();
}

void HwCameraRecorder::prepare(HwWindow *win) {
    Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}