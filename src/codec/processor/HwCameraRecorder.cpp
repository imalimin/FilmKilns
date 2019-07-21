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
#include "../include/HwVideoOutput.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "NativeWindow.h"
#include "../include/HwSequenceModel.h"

HwCameraRecorder::HwCameraRecorder() : HwAbsProcessor("HwCameraRecorder") {
    startPipeline();
    registerAnUnit(new HwMicrophone());
    registerAnUnit(new HwCameraInput());
    registerAnUnit(new HwRender());
    registerAnUnit(new HwScreen());
    registerAnUnit(new HwVideoOutput());
}

HwCameraRecorder::~HwCameraRecorder() {
    stopPipeline();
}

HwAbsPipelineModel *HwCameraRecorder::createModel() {
    return HwSequenceModel::build();
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
    int width = static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->width;
    int height = static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->height;
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = new ObjectBox(new Size(width, height));
    msg->msg = "RENDER";
    msg->arg1 = textureId;
    msg->arg2 = tsInNs;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

void HwCameraRecorder::setOutputFilePath(string filePath) {
    static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->path = filePath;
}

void HwCameraRecorder::setOutputSize(int width, int height) {
    static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->width = width;
    static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->height = height;
}