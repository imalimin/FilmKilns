/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/AlDisplayRecorder.h"
#include "../include/HwMicrophone.h"
#include "include/AlVideoCompiler.h"
#include "../include/AlUOESTexInput.h"
#include "AlGImage.h"
#include "AlUTexReader.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"
#include "HwTexture.h"
#include "AlRunnable.h"
#include "AlSize.h"
#include "AlOperateCrop.h"
#include "AlIdentityCreator.h"
#include "AlMath.h"

#define TAG "HwCameraRecorder"

AlDisplayRecorder::AlDisplayRecorder() : AlAbsProcessor("AlDisplayRecorder") {
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(new AlUOESTexInput(ALIAS_OF_CAMERA));
    registerAnUnit(new AlGImage(ALIAS_OF_RENDER));
    registerAnUnit(new AlUTexReader(ALIAS_OF_READER));
    registerAnUnit(new AlVideoCompiler(ALIAS_OF_COMPILER));
    registerEvent(MSG_CAMERA_OES_TEX_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlDisplayRecorder::_onOESTexNotify));
    registerEvent(MSG_VIDEO_COMPILER_TIME,
                  reinterpret_cast<EventFunc>(&AlDisplayRecorder::_onRecordProgress));
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlDisplayRecorder::_onCanvasUpdate));
}

AlDisplayRecorder::~AlDisplayRecorder() {
    this->onNativeReadyListener = nullptr;
    this->onRecordListener = nullptr;
}

void AlDisplayRecorder::onCreate() {
    AlAbsProcessor::onCreate();
    aSharedContext = AlEgl::offScreen();
}

void AlDisplayRecorder::onDestroy() {
    AlAbsProcessor::onDestroy();
    delete aSharedContext;
    aSharedContext = nullptr;
}

void AlDisplayRecorder::updateWindow(HwWindow *win) {
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlDisplayRecorder::start() {
    postEvent(AlMessage::obtain(EVENT_COMMON_START));
}

void AlDisplayRecorder::pause() {
    postEvent(AlMessage::obtain(EVENT_COMMON_PAUSE));
}

void AlDisplayRecorder::invalidate(AlMatrix *matrix, int64_t tsInNs) {
    AlMessage *msg = AlMessage::obtain(EVENT_CAMERA_INVALIDATE, new AlMatrix(*matrix),
                                       Message::QUEUE_MODE_UNIQUE);
    msg->arg2 = tsInNs;
    postEvent(msg);
}

void AlDisplayRecorder::setOutputFilePath(string filePath) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PATH);
    msg->desc = filePath;
    postMessage(msg);
}

void AlDisplayRecorder::setBitrate(int32_t bitrate) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_BITRATE_LEVEL);
    msg->arg1 = bitrate;
    postEvent(msg);
}

void AlDisplayRecorder::setProfile(std::string profile) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PROFILE);
    msg->desc = profile;
    postEvent(msg);
}

void AlDisplayRecorder::setPreset(std::string preset) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PRESET);
    msg->desc = preset;
    postEvent(msg);
}

void AlDisplayRecorder::setFormat(int width, int height, HwSampleFormat format) {
    postMessage(AlMessage::obtain(MSG_VIDEO_OUTPUT_SIZE, new AlSize(width, height)));
    postMessage(AlMessage::obtain(MSG_MICROPHONE_FORMAT, new HwSampleFormat(format)));
    postEvent(AlMessage::obtain(EVENT_CANVAS_RESIZE, new AlSize(width, height)));
}

void AlDisplayRecorder::setMaxSize(int width, int height) {
    postMessage(AlMessage::obtain(MSG_VIDEO_OUTPUT_MAX_SIZE, new AlSize(width, height)));
}

void AlDisplayRecorder::cropOutputSize(float left, float top, float right, float bottom) {
    auto *opt = new AlOperateCrop(AlIdentityCreator::NONE_ID, left, top, right, bottom);
    opt->coordIdx = AlOperateDesc::CoordIdx::CANVAS;
    auto *msg = AlMessage::obtain(EVENT_CANVAS_CROP, opt, AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
}

void AlDisplayRecorder::setFilter(HwAbsFilter *filter) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_SET_FILTER);
    msg->obj = ObjectBox::wrap(filter);
    postEvent(msg);
}

void AlDisplayRecorder::runOnCameraContext(function<void()> func) {
    postEvent(AlMessage::obtain(MSG_CAMERA_RUN, new AlRunnable([func](Object *o) {
        func();
    })));
}

void AlDisplayRecorder::setCameraSize(int32_t w, int32_t h) {
    AlMessage *msg = AlMessage::obtain(MSG_CAMERA_UPDATE_SIZE);
    msg->ptr = new AlSize(w, h);
    postEvent(msg);
}

void AlDisplayRecorder::backward() {
    postEvent(AlMessage::obtain(MSG_VIDEO_COMPILER_BACKWARD));
}

void AlDisplayRecorder::setRecordListener(function<void(int64_t)> listener) {
    this->onRecordListener = listener;
}

void AlDisplayRecorder::setOnNativeReadyListener(OnNativeReadyListener l) {
    this->onNativeReadyListener = l;
}

bool AlDisplayRecorder::_onOESTexNotify(AlMessage *msg) {
    auto *tex = msg->ptr.as<HwAbsTexture>();
    if (tex && onNativeReadyListener) {
        oesTex = tex->texId();
        AlLogI(TAG, "%d", oesTex);
        onNativeReadyListener(oesTex);
    }
    return true;
}

bool AlDisplayRecorder::_onRecordProgress(AlMessage *msg) {
    if (onRecordListener) {
        onRecordListener(msg->arg2);
    }
    return true;
}

bool AlDisplayRecorder::_onCanvasUpdate(AlMessage *msg) {
    int32_t width = msg->arg1;
    int32_t height = static_cast<int>(msg->arg2);
    postMessage(AlMessage::obtain(MSG_VIDEO_OUTPUT_SIZE, new AlSize(width, height),
                                  AlMessage::QUEUE_MODE_FIRST_ALWAYS));
    return true;
}
