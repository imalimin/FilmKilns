/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwVideoProcessor.h"
#include "HwRender.h"
#include "../include/HwVideoInput.h"
#include "HwScreen.h"
#include "NativeWindow.h"
#include "ObjectBox.h"
#include <string>

HwVideoProcessor::HwVideoProcessor() : Object() {
//    unitHandler = new HandlerThread("VideoUnits");
//    screenHandler = new HandlerThread("ScreenUnit");
    pipeline = new UnitPipeline("VideoProcessor");
    pipeline->registerAnUnit(new HwVideoInput());
    pipeline->registerAnUnit(new HwRender());
    pipeline->registerAnUnit(new HwScreen());
}

HwVideoProcessor::~HwVideoProcessor() {
    if (pipeline) {
        pipeline->release();
        delete pipeline;
        pipeline = nullptr;
    }
    if (unitHandler) {
        delete unitHandler;
        unitHandler = nullptr;
    }
    if (screenHandler) {
        delete screenHandler;
        screenHandler = nullptr;
    }
}

void HwVideoProcessor::setSource(char *path) {
    if (pipeline) {
        Message *msg = new Message(EVENT_VIDEO_SET_SOURCE, nullptr);
        msg->obj = new ObjectBox(path);
        pipeline->postEvent(msg);
    }
}

void HwVideoProcessor::prepare(HwWindow *win) {
    if (pipeline) {
        Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
        msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
        pipeline->postEvent(msg);
    }
}

void HwVideoProcessor::start() {
    if (pipeline) {
        Message *msg = new Message(EVENT_VIDEO_START, nullptr);
        pipeline->postEvent(msg);
    }
}

void HwVideoProcessor::pause() {
    if (pipeline) {
        Message *msg = new Message(EVENT_VIDEO_PAUSE, nullptr);
        pipeline->postEventAtFront(msg);
    }
}

void HwVideoProcessor::stop() {
    if (pipeline) {
        Message *msg = new Message(EVENT_VIDEO_STOP, nullptr);
        pipeline->postEventAtFront(msg);
    }
}

void HwVideoProcessor::seek(int64_t us) {
    if (pipeline) {
        pipeline->removeAllMessage(EVENT_VIDEO_SEEK);
        Message *msg = new Message(EVENT_VIDEO_SEEK, nullptr);
        msg->arg2 = us;
        pipeline->postEvent(msg);
    }
}

void HwVideoProcessor::setFilter(Filter *filter) {
    Message *msg = new Message(EVENT_RENDER_SET_FILTER, nullptr);
    msg->obj = new ObjectBox(filter);
    pipeline->postEvent(msg);
}