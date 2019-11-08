/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
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
    HwVideoInput *inputUnit = new HwVideoInput(ALIAS_OF_VIDEO);
    inputUnit->setPlayListener([this](int64_t us, int64_t duration) {
        this->playProgressListener(us, duration);
    });
    registerAnUnit(inputUnit);
    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    registerAnUnit(new HwSpeaker(ALIAS_OF_SPEAKER, HwAudioDeviceMode::LowLatency));
}

HwVideoProcessor::~HwVideoProcessor() {
}

void HwVideoProcessor::onDestroy() {
    HwAbsProcessor::onDestroy();
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
    msg->queueMode = Message::QUEUE_MODE_FIRST_ALWAYS;
    postEvent(msg);
}

void HwVideoProcessor::stop() {
    Message *msg = new Message(EVENT_VIDEO_STOP, nullptr);
    msg->queueMode = Message::QUEUE_MODE_FIRST_ALWAYS;
    postEvent(msg);
}

void HwVideoProcessor::seek(int64_t us) {
    Message *msg = new Message(EVENT_VIDEO_SEEK, nullptr);
    msg->arg2 = us;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

void HwVideoProcessor::setFilter(HwAbsFilter *filter) {
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