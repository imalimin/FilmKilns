/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <Size.h>
#include "../include/HwCameraRecorder.h"
#include "../include/HwCameraInput.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"

HwCameraRecorder::HwCameraRecorder() : HwAbsProcessor("HwCameraRecorder") {
    startPipeline();
    registerAnUnit(new HwCameraInput());
    registerAnUnit(new HwRender());
    registerAnUnit(new HwScreen());
}

HwCameraRecorder::~HwCameraRecorder() {
    stopPipeline();
}

void HwCameraRecorder::prepare(HwWindow *win) {
    Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwCameraRecorder::updateWindow(HwWindow *win) {
    Message *msg = new Message(EVENT_SCREEN_UPDATE_WINDOW, nullptr);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwCameraRecorder::invalidate(int textureId, int w, int h) {
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = new ObjectBox(new Size(w, h));
    msg->msg = "RENDER";
    msg->arg1 = textureId;
    postEvent(msg);
}