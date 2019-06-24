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

HwAudioProcessor::HwAudioProcessor() : Object() {
    pipeline = new UnitPipeline("AudioProcessor");
    pipeline->registerAnUnit(new HwAudioInput());
    pipeline->registerAnUnit(new HwSpeaker());
}

HwAudioProcessor::~HwAudioProcessor() {
    if (pipeline) {
        pipeline->release();
        delete pipeline;
        pipeline = nullptr;
    }
}

void HwAudioProcessor::setSource(const string *path) {
    if (pipeline) {
        Message *msg = new Message(EVENT_AUDIO_SET_SOURCE, nullptr);
        msg->obj = new ObjectBox(new string(path->c_str()));
        pipeline->postEvent(msg);
    }
}

void HwAudioProcessor::prepare() {
    if (pipeline) {
        Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
        msg->obj = new ObjectBox(nullptr);
        pipeline->postEvent(msg);
    }
}

void HwAudioProcessor::start() {
    if (pipeline) {
        Message *msg = new Message(EVENT_AUDIO_START, nullptr);
        pipeline->postEvent(msg);
    }
}

void HwAudioProcessor::pause() {
    if (pipeline) {
        Message *msg = new Message(EVENT_AUDIO_PAUSE, nullptr);
        pipeline->postEvent(msg);
    }
}

void HwAudioProcessor::stop() {
    if (pipeline) {
        Message *msg = new Message(EVENT_AUDIO_STOP, nullptr);
        pipeline->postEvent(msg);
    }
}

void HwAudioProcessor::seek(int64_t us) {
    if (pipeline) {
        pipeline->removeAllMessage(EVENT_AUDIO_SEEK);
        Message *msg = new Message(EVENT_AUDIO_SEEK, nullptr);
        msg->arg2 = us;
        pipeline->postEvent(msg);
    }
}