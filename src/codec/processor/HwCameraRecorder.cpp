/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
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
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_PREPARE, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwCameraRecorder::updateWindow(HwWindow *win) {
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwCameraRecorder::start() {
    postEvent(AlMessage::obtain(EVENT_COMMON_START));
}

void HwCameraRecorder::pause() {
    postEvent(AlMessage::obtain(EVENT_COMMON_PAUSE));
}

void HwCameraRecorder::invalidate(AlMatrix *matrix, int64_t tsInNs) {
    AlMessage *msg = AlMessage::obtain(EVENT_CAMERA_INVALIDATE);
    msg->obj = new AlMatrix(*matrix);
    msg->arg2 = tsInNs;
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
    putInt32("width", width).to({ALIAS_OF_CAMERA, ALIAS_OF_COMPILER});
    putInt32("height", height).to({ALIAS_OF_CAMERA, ALIAS_OF_COMPILER});
    putObject("audioFormat", audioFormat).to({ALIAS_OF_MIC, ALIAS_OF_COMPILER});
}

void HwCameraRecorder::setFilter(HwAbsFilter *filter) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_SET_FILTER);
    msg->obj = ObjectBox::wrap(filter);
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
    postEvent(AlMessage::obtain(EVENT_VIDEO_COMPILER_BACKWARD));
}

void HwCameraRecorder::setRecordListener(function<void(int64_t)> listener) {
    this->recordListener = listener;
}