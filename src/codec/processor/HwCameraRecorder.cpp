/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <Size.h>
#include "../include/HwCameraRecorder.h"
#include "../include/HwMicrophone.h"
#include "../include/HwVideoCompiler.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"
#include "HwTexture.h"

HwCameraRecorder::HwCameraRecorder() : HwAbsProcessor("HwCameraRecorder") {
    camera = new HwCameraInput(ALIAS_OF_CAMERA);
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(camera);
    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    HwVideoCompiler *c = new HwVideoCompiler(ALIAS_OF_COMPILER);
    c->setRecordListener([this](int64_t timeInUs) {
        this->recordListener(timeInUs);
    });
    registerAnUnit(c);
}

HwCameraRecorder::~HwCameraRecorder() {
}

void HwCameraRecorder::onDestroy() {
    HwAbsProcessor::onDestroy();
    if (audioFormat) {
        delete audioFormat;
        audioFormat = nullptr;
    }
    camera = nullptr;
    this->recordListener = nullptr;
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

void HwCameraRecorder::invalidate(HwMatrix *matrix, int w, int h) {
    Message *msg = new Message(EVENT_CAMERA_INVALIDATE, nullptr);
    msg->obj = new HwMatrix(*matrix);
    msg->arg1 = w;
    msg->arg2 = h;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

void HwCameraRecorder::setOutputFilePath(string filePath) {
    putString("path", filePath).to({ALIAS_OF_COMPILER});
}

void HwCameraRecorder::setFormat(int width, int height, HwSampleFormat format) {
    if (audioFormat) {
        delete audioFormat;
    }
    audioFormat = new HwSampleFormat(format);
    putInt32("width", width).to({ALIAS_OF_COMPILER});
    putInt32("height", height).to({ALIAS_OF_COMPILER});
    putObject("audioFormat", audioFormat).to({ALIAS_OF_MIC, ALIAS_OF_COMPILER});
}

void HwCameraRecorder::setFilter(Filter *filter) {
    Message *msg = new Message(EVENT_RENDER_SET_FILTER, nullptr);
    msg->obj = new ObjectBox(filter);
    postEvent(msg);
}

uint32_t HwCameraRecorder::getTex() {
    return camera->getTex();
}

void HwCameraRecorder::mackCameraCurrent() {
    if (camera) {
        camera->mackCurrent();
    }
}

void HwCameraRecorder::setCameraSize(int32_t w, int32_t h) {
    putInt32("camera_width", w).to({ALIAS_OF_CAMERA});
    putInt32("camera_height", h).to({ALIAS_OF_CAMERA});
}

void HwCameraRecorder::backward() {
    postEvent(new Message(EVENT_VIDEO_COMPILER_BACKWARD, nullptr));
}

void HwCameraRecorder::setRecordListener(function<void(int64_t)> listener) {
    this->recordListener = listener;
}