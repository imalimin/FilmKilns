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
    playProgressListener = nullptr;
}

void HwVideoProcessor::setSource(const string path) {
    AlMessage *msg = AlMessage::obtain(EVENT_VIDEO_SET_SOURCE);
    msg->obj = new ObjectBox(new string(path));
    postEvent(msg);
}

void HwVideoProcessor::prepare(HwWindow *win) {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_PREPARE);
    msg->obj = new NativeWindow(win, nullptr);
    postEvent(msg);
}

void HwVideoProcessor::start() {
    AlMessage *msg = AlMessage::obtain(EVENT_VIDEO_START);
    postEvent(msg);
}

void HwVideoProcessor::pause() {
    AlMessage *msg = AlMessage::obtain(EVENT_VIDEO_PAUSE);
    msg->queueMode = Message::QUEUE_MODE_FIRST_ALWAYS;
    postEvent(msg);
}

void HwVideoProcessor::stop() {
    AlMessage *msg = AlMessage::obtain(EVENT_VIDEO_STOP);
    msg->queueMode = Message::QUEUE_MODE_FIRST_ALWAYS;
    postEvent(msg);
}

void HwVideoProcessor::seek(int64_t us) {
    AlMessage *msg = AlMessage::obtain(EVENT_VIDEO_SEEK);
    msg->arg2 = us;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

void HwVideoProcessor::setFilter(HwAbsFilter *filter) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_SET_FILTER);
    msg->obj = ObjectBox::wrap(filter);
    postEvent(msg);
}

void HwVideoProcessor::updateWindow(HwWindow *win) {
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void HwVideoProcessor::setPlayProgressListener(function<void(int64_t, int64_t)> listener) {
    this->playProgressListener = listener;
}