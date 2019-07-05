/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAudioProcessor.h"
#include "../include/HwAudioInput.h"
#include "../include/HwSpeaker.h"
#include "ObjectBox.h"

HwAudioProcessor::HwAudioProcessor() : HwAbsProcessor("AudioProcessor") {
    startPipeline();
    HwAudioInput *inputUnit = new HwAudioInput();
    inputUnit->setPlayListener([this](int64_t us, int64_t duration) {
        this->playProgressListener(us, duration);
    });
    registerAnUnit(inputUnit);
    registerAnUnit(new HwSpeaker());
}

HwAudioProcessor::~HwAudioProcessor() {
    stopPipeline();
    playProgressListener = nullptr;
}

HwAbsSequence *HwAudioProcessor::createSequence() {
    return new HwAbsSequence();
}

void HwAudioProcessor::setSource(const string path) {
    Message *msg = new Message(EVENT_AUDIO_SET_SOURCE, nullptr);
    msg->obj = new ObjectBox(new string(path));
    postEvent(msg);
}

void HwAudioProcessor::prepare() {
    Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
    msg->obj = new ObjectBox(nullptr);
    postEvent(msg);
}

void HwAudioProcessor::start() {
    Message *msg = new Message(EVENT_AUDIO_START, nullptr);
    postEvent(msg);
}

void HwAudioProcessor::pause() {
    Message *msg = new Message(EVENT_AUDIO_PAUSE, nullptr);
    postEvent(msg);
}

void HwAudioProcessor::stop() {
    Message *msg = new Message(EVENT_AUDIO_STOP, nullptr);
    postEvent(msg);
}

void HwAudioProcessor::seek(int64_t us) {
    removeAllMessage(EVENT_AUDIO_SEEK);
    Message *msg = new Message(EVENT_AUDIO_SEEK, nullptr);
    msg->arg2 = us;
    postEvent(msg);
}

void HwAudioProcessor::setPlayProgressListener(function<void(int64_t, int64_t)> listener) {
    this->playProgressListener = listener;
}