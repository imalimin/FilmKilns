/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwVideoProcessor.h"
#include "../include/HwVideoInput.h"
#include "../include/HwSpeaker.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "NativeWindow.h"
#include "ObjectBox.h"
#include <string>

HwVideoProcessor::HwVideoProcessor() : HwAbsProcessor("VideoProcessor") {
    startPipeline();
    HwVideoInput *inputUnit = new HwVideoInput();
    inputUnit->setPlayListener([this](int64_t us, int64_t duration) {
        this->playProgressListener(us, duration);
    });
    registerAnUnit(inputUnit);
    registerAnUnit(new HwRender());
    registerAnUnit(new HwScreen());
    registerAnUnit(new HwSpeaker(HwAudioDeviceMode::LowLatency));
}

HwVideoProcessor::~HwVideoProcessor() {
    stopPipeline();
    if (unitHandler) {
        delete unitHandler;
        unitHandler = nullptr;
    }
    if (screenHandler) {
        delete screenHandler;
        screenHandler = nullptr;
    }
    playProgressListener = nullptr;
}

HwAbsSequence *HwVideoProcessor::createSequence() {
    return new HwAbsSequence();
}

void HwVideoProcessor::setSource(const string path) {
    Message *msg = new Message(EVENT_VIDEO_SET_SOURCE, nullptr);
    msg->obj = new ObjectBox(new string(path));
    postEvent(msg);
}

void HwVideoProcessor::prepare(HwWindow *win) {
    Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwVideoProcessor::start() {
    Message *msg = new Message(EVENT_VIDEO_START, nullptr);
    postEvent(msg);
}

void HwVideoProcessor::pause() {
    Message *msg = new Message(EVENT_VIDEO_PAUSE, nullptr);
    postEventAtFront(msg);
}

void HwVideoProcessor::stop() {
    Message *msg = new Message(EVENT_VIDEO_STOP, nullptr);
    postEventAtFront(msg);
}

void HwVideoProcessor::seek(int64_t us) {
    removeAllMessage(EVENT_VIDEO_SEEK);
    Message *msg = new Message(EVENT_VIDEO_SEEK, nullptr);
    msg->arg2 = us;
    postEvent(msg);
}

void HwVideoProcessor::setFilter(Filter *filter) {
    Message *msg = new Message(EVENT_RENDER_SET_FILTER, nullptr);
    msg->obj = new ObjectBox(filter);
    postEvent(msg);
}

void HwVideoProcessor::updateWindow(HwWindow *win) {
    Message *msg = new Message(EVENT_SCREEN_UPDATE_WINDOW, nullptr);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwVideoProcessor::setPlayProgressListener(function<void(int64_t, int64_t)> listener) {
    this->playProgressListener = listener;
}