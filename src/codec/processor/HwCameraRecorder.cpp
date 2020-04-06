/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <Size.h>
#include "../include/HwCameraRecorder.h"
#include "../include/HwMicrophone.h"
#include "include/AlVideoCompiler.h"
#include "AlGImage.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"
#include "HwTexture.h"
#include "AlRunnable.h"

#define TAG "HwCameraRecorder"

HwCameraRecorder::HwCameraRecorder() : AlAbsProcessor("HwCameraRecorder") {
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(new HwCameraInput(ALIAS_OF_CAMERA));
    registerAnUnit(new AlGImage(ALIAS_OF_RENDER));
    registerAnUnit(new AlVideoCompiler(ALIAS_OF_COMPILER));
    registerEvent(MSG_CAMERA_OES_TEX_NOTIFY,
                  reinterpret_cast<EventFunc>(&HwCameraRecorder::_onOESTexNotify));
    registerEvent(MSG_VIDEO_COMPILER_TIME,
                  reinterpret_cast<EventFunc>(&HwCameraRecorder::_onRecordProgress));
}

HwCameraRecorder::~HwCameraRecorder() {
    this->onNativeReadyListener = nullptr;
    this->onRecordListener = nullptr;
}

void HwCameraRecorder::onCreate() {
    AlAbsProcessor::onCreate();
    aSharedContext = AlEgl::offScreen();
}

void HwCameraRecorder::onDestroy() {
    AlAbsProcessor::onDestroy();
    delete aSharedContext;
    aSharedContext = nullptr;
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
    AlMessage *msg = AlMessage::obtain(EVENT_CAMERA_INVALIDATE, new AlMatrix(*matrix),
                                       Message::QUEUE_MODE_UNIQUE);
    msg->arg2 = tsInNs;
    postEvent(msg);
}

void HwCameraRecorder::setOutputFilePath(string filePath) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PATH);
    msg->desc = filePath;
    postMessage(msg);
}

void HwCameraRecorder::setFormat(int width, int height, HwSampleFormat format) {
    postMessage(AlMessage::obtain(MSG_VIDEO_OUTPUT_SIZE, new AlSize(width, height)));
    postMessage(AlMessage::obtain(MSG_MICROPHONE_FORMAT, new HwSampleFormat(format)));
    auto *msg = AlMessage::obtain(EVENT_CANVAS_RESIZE, new AlSize(width, height));
    postEvent(msg);
}

void HwCameraRecorder::setFilter(HwAbsFilter *filter) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_SET_FILTER);
    msg->obj = ObjectBox::wrap(filter);
    postEvent(msg);
}

void HwCameraRecorder::runOnCameraContext(function<void()> func) {
    postEvent(AlMessage::obtain(MSG_CAMERA_RUN, new AlRunnable([func](Object *o) {
        func();
    })));
}

void HwCameraRecorder::setCameraSize(int32_t w, int32_t h) {
    AlMessage *msg = AlMessage::obtain(MSG_CAMERA_UPDATE_SIZE);
    msg->ptr = new AlSize(w, h);
    postEvent(msg);
}

void HwCameraRecorder::backward() {
    postEvent(AlMessage::obtain(MSG_VIDEO_COMPILER_BACKWARD));
}

void HwCameraRecorder::setRecordListener(function<void(int64_t)> listener) {
    this->onRecordListener = listener;
}

void HwCameraRecorder::setOnNativeReadyListener(OnNativeReadyListener l) {
    this->onNativeReadyListener = l;
}

bool HwCameraRecorder::_onOESTexNotify(AlMessage *msg) {
    auto *tex = msg->ptr.as<HwAbsTexture>();
    if (tex && onNativeReadyListener) {
        oesTex = tex->texId();
        AlLogI(TAG, "%d", oesTex);
        onNativeReadyListener(oesTex);
    }
    return true;
}

bool HwCameraRecorder::_onRecordProgress(AlMessage *msg) {
    if (onRecordListener) {
        onRecordListener(msg->arg2);
    }
    return true;
}
