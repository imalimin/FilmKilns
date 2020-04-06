/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwVideoProcessor.h"
#include "../include/HwVideoInput.h"
#include "../include/HwSpeaker.h"
#include "AlGImage.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "NativeWindow.h"
#include "ObjectBox.h"
#include <string>

HwVideoProcessor::HwVideoProcessor() : AlAbsProcessor("VideoProcessor") {
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    registerAnUnit(new HwVideoInput(ALIAS_OF_VIDEO));
    registerAnUnit(new AlGImage(ALIAS_OF_RENDER));
    registerAnUnit(new HwSpeaker(ALIAS_OF_SPEAKER, HwAudioDeviceMode::LowLatency));

    registerEvent(MSG_VIDEO_PROGRESS,
                  reinterpret_cast<EventFunc>(&HwVideoProcessor::_onPlayProgress));
}

HwVideoProcessor::~HwVideoProcessor() {
}

void HwVideoProcessor::onCreate() {
    AlAbsProcessor::onCreate();
    this->aBaseCtx = AlEgl::offScreen();
}

void HwVideoProcessor::onDestroy() {
    AlAbsProcessor::onDestroy();
    delete this->aBaseCtx;
    this->aBaseCtx = nullptr;
    playProgressListener = nullptr;
}

void HwVideoProcessor::setSource(const string path) {
    AlMessage *msg = AlMessage::obtain(EVENT_VIDEO_SET_SOURCE);
    msg->desc = path;
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

bool HwVideoProcessor::_onPlayProgress(AlMessage *msg) {
    if (playProgressListener) {
        playProgressListener(msg->arg1, msg->arg2);
    }
    return true;
}
