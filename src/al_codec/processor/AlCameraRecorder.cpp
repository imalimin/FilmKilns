/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <Size.h>
#include "AlCameraRecorder.h"
#include "HwMicrophone.h"
#include "AlVideoCompiler.h"
#include "AlCameraInput.h"
#include "AlGImage.h"
#include "AlUTexReader.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"
#include "HwTexture.h"
#include "AlRunnable.h"

#define TAG "AlCameraRecorder"

AlCameraRecorder::AlCameraRecorder() : AlAbsProcessor(TAG) {
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    /// 由于AlVideoCompiler可能会使用硬编进行纹理编码，此时会有EGL环境
    /// 故顺序尽可能考前，否则释放EGL时会把context设置为none，影响其它模块的纹理释放
    registerAnUnit(new AlVideoCompiler(ALIAS_OF_COMPILER));
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(new AlCameraInput(ALIAS_OF_CAMERA));
    registerAnUnit(new AlGImage(ALIAS_OF_RENDER));
    registerAnUnit(new AlUTexReader(ALIAS_OF_READER));
    al_reg_msg(MSG_CAMERA_OES_TEX_NOTIFY, AlCameraRecorder::_onOESTexNotify);
    al_reg_msg(MSG_VIDEO_COMPILER_TIME, AlCameraRecorder::_onRecordProgress);
}

AlCameraRecorder::~AlCameraRecorder() {
    this->onNativeReadyListener = nullptr;
    this->onRecordListener = nullptr;
}

void AlCameraRecorder::onCreate() {
    AlAbsProcessor::onCreate();
    aSharedContext = AlEgl::offScreen(TAG);
}

void AlCameraRecorder::onDestroy() {
    AlAbsProcessor::onDestroy();
    delete aSharedContext;
    aSharedContext = nullptr;
}

void AlCameraRecorder::updateWindow(HwWindow *win) {
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlCameraRecorder::start() {
    postEvent(AlMessage::obtain(EVENT_COMMON_START));
}

void AlCameraRecorder::pause() {
    postEvent(AlMessage::obtain(EVENT_COMMON_PAUSE));
}

void AlCameraRecorder::invalidate(AlMatrix *matrix, int64_t tsInNs) {
    AlMessage *msg = AlMessage::obtain(EVENT_CAMERA_INVALIDATE, new AlMatrix(*matrix),
                                       Message::QUEUE_MODE_UNIQUE);
    msg->arg2 = tsInNs;
    postEvent(msg);
}

void AlCameraRecorder::setOutputFilePath(string filePath) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PATH);
    msg->desc = filePath;
    postMessage(msg);
}

void AlCameraRecorder::setFormat(int width, int height, HwSampleFormat format) {
    postMessage(AlMessage::obtain(MSG_VIDEO_OUTPUT_SIZE, new AlSize(width, height)));
    postMessage(AlMessage::obtain(MSG_MICROPHONE_FORMAT, new HwSampleFormat(format)));
    auto *msg = AlMessage::obtain(EVENT_CANVAS_RESIZE, new AlSize(width, height));
    postEvent(msg);
}

void AlCameraRecorder::setFilter(HwAbsFilter *filter) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_SET_FILTER);
    msg->obj = ObjectBox::wrap(filter);
    postEvent(msg);
}

void AlCameraRecorder::runOnCameraContext(function<void()> func) {
    postEvent(AlMessage::obtain(MSG_CAMERA_RUN, new AlRunnable([this, func](Object *o) {
        this->aSharedContext->makeCurrent();
        func();
    })));
}

void AlCameraRecorder::setCameraSize(int32_t w, int32_t h) {
    AlMessage *msg = AlMessage::obtain(MSG_CAMERA_UPDATE_SIZE);
    msg->ptr = new AlSize(w, h);
    postEvent(msg);
}

void AlCameraRecorder::backward() {
    postEvent(AlMessage::obtain(MSG_VIDEO_COMPILER_BACKWARD));
}

void AlCameraRecorder::setRecordListener(function<void(int64_t)> listener) {
    this->onRecordListener = listener;
}

void AlCameraRecorder::setOnNativeReadyListener(OnNativeReadyListener l) {
    this->onNativeReadyListener = l;
}

bool AlCameraRecorder::_onOESTexNotify(AlMessage *msg) {
    auto *tex = msg->ptr.as<HwAbsTexture>();
    if (tex && onNativeReadyListener) {
        oesTex = tex->texId();
        AlLogI(TAG, "%d", oesTex);
        onNativeReadyListener(oesTex);
    }
    return true;
}

bool AlCameraRecorder::_onRecordProgress(AlMessage *msg) {
    if (onRecordListener) {
        onRecordListener(msg->arg2);
    }
    return true;
}

void AlCameraRecorder::setVideoBitLevel(int level) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_BITRATE_LEVEL);
    msg->arg1 = level;
    postEvent(msg);
}

void AlCameraRecorder::setProfile(std::string profile) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PROFILE);
    msg->desc = profile;
    postEvent(msg);
}

void AlCameraRecorder::setPreset(std::string preset) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_PRESET);
    msg->desc = preset;
    postEvent(msg);
}

void AlCameraRecorder::setEnableHardware(bool enable) {
    auto *msg = AlMessage::obtain(MSG_VIDEO_OUTPUT_ENABLE_HARD);
    msg->arg1 = enable;
    postEvent(msg);
}
