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
#include "AlRunnable.h"

#define TAG "HwCameraRecorder"

HwCameraRecorder::HwCameraRecorder() : AlAbsProcessor("HwCameraRecorder") {
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(new HwCameraInput(ALIAS_OF_CAMERA));
//    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
//    registerAnUnit(new HwVideoCompiler(ALIAS_OF_COMPILER));
//    c->setRecordListener([this](int64_t timeInUs) {
//        this->recordListener(timeInUs);
//    });
    registerEvent(MSG_CAMERA_OES_TEX_NOTIFY,
                  reinterpret_cast<EventFunc>(&HwCameraRecorder::_onOESTexNotify));
}

HwCameraRecorder::~HwCameraRecorder() {
    delete audioFormat;
    audioFormat = nullptr;
    this->onNativeReadyListener = nullptr;
}

void HwCameraRecorder::onCreate() {
    AlAbsProcessor::onCreate();
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
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

void HwCameraRecorder::runOnCameraContext(function<void()> func) {
    AlMessage *msg = AlMessage::obtain(MSG_CAMERA_RUN, new AlRunnable([func](Object *o) {
        func();
    }));
    postEvent(msg);
}

void HwCameraRecorder::setCameraSize(int32_t w, int32_t h) {
    AlMessage *msg = AlMessage::obtain(MSG_CAMERA_UPDATE_SIZE);
    msg->ptr = new AlSize(w, h);
    postEvent(msg);
}

void HwCameraRecorder::backward() {
    postEvent(AlMessage::obtain(EVENT_VIDEO_COMPILER_BACKWARD));
}

void HwCameraRecorder::setRecordListener(function<void(int64_t)> listener) {
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
