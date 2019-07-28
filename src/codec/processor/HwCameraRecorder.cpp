/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <Size.h>
#include "../include/HwCameraRecorder.h"
#include "../include/HwMicrophone.h"
#include "../include/HwCameraInput.h"
#include "../include/HwVideoCompiler.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"
#include "HwTexture.h"

HwCameraRecorder::HwCameraRecorder() : HwAbsProcessor("HwCameraRecorder") {
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(new HwCameraInput(ALIAS_OF_CAMERA));
    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    registerAnUnit(new HwVideoCompiler(ALIAS_OF_COMPILER));
}

HwCameraRecorder::~HwCameraRecorder() {
}

void HwCameraRecorder::onDestroy() {
    HwAbsProcessor::onDestroy();
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

void HwCameraRecorder::start() {
    postEvent(new Message(EVENT_COMMON_START, nullptr));
}

void HwCameraRecorder::pause() {
    postEvent(new Message(EVENT_COMMON_PAUSE, nullptr));
}

void HwCameraRecorder::invalidate(int textureId, int64_t tsInNs, int w, int h) {
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = new HwTexture(textureId, w, h);
    msg->desc = "RENDER";
    msg->arg2 = tsInNs;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

void HwCameraRecorder::setOutputFilePath(string filePath) {
    putString("path", filePath).to({ALIAS_OF_COMPILER});
}

void HwCameraRecorder::setOutputSize(int width, int height) {
    putInt32("width", width).to({ALIAS_OF_COMPILER});
    putInt32("height", height).to({ALIAS_OF_COMPILER});
}

void HwCameraRecorder::setFilter(Filter *filter) {
    Message *msg = new Message(EVENT_RENDER_SET_FILTER, nullptr);
    msg->obj = new ObjectBox(filter);
    postEvent(msg);
}