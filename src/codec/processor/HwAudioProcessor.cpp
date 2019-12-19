/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAudioProcessor.h"
#include "../include/HwAudioInput.h"
#include "../include/HwSpeaker.h"
#include "ObjectBox.h"

HwAudioProcessor::HwAudioProcessor() : HwAbsProcessor("AudioProcessor") {
    HwAudioInput *inputUnit = new HwAudioInput(ALIAS_OF_AUDIO);
    inputUnit->setPlayListener([this](int64_t us, int64_t duration) {
        this->playProgressListener(us, duration);
    });
    registerAnUnit(inputUnit);
    registerAnUnit(new HwSpeaker(ALIAS_OF_SPEAKER));
}

HwAudioProcessor::~HwAudioProcessor() {
}

void HwAudioProcessor::onDestroy() {
    playProgressListener = nullptr;
}

void HwAudioProcessor::setSource(const string path) {
    putString("path", path).to({ALIAS_OF_AUDIO});
}

void HwAudioProcessor::prepare() {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_PREPARE);
    msg->obj = new ObjectBox(nullptr);
    postEvent(msg);
}

void HwAudioProcessor::start() {
    AlMessage *msg = AlMessage::obtain(EVENT_AUDIO_START);
    postEvent(msg);
}

void HwAudioProcessor::pause() {
    AlMessage *msg = AlMessage::obtain(EVENT_AUDIO_PAUSE);
    postEvent(msg);
}

void HwAudioProcessor::stop() {
    AlMessage *msg = AlMessage::obtain(EVENT_AUDIO_STOP);
    postEvent(msg);
}

void HwAudioProcessor::seek(int64_t us) {
    AlMessage *msg = AlMessage::obtain(EVENT_AUDIO_SEEK);
    msg->arg2 = us;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

void HwAudioProcessor::setPlayProgressListener(function<void(int64_t, int64_t)> listener) {
    this->playProgressListener = listener;
}