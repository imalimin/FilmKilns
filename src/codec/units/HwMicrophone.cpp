/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwMicrophone.h"
#include "TimeUtils.h"
#include "Thread.h"

#define TAG "HwMicrophone"

HwMicrophone::HwMicrophone(string alias) : Unit(alias),
                                           format(HwFrameFormat::HW_SAMPLE_S32, 2, 44100) {
    registerEvent(MSG_MICROPHONE_FORMAT, reinterpret_cast<EventFunc>(&HwMicrophone::_onFormat));
    registerEvent(EVENT_COMMON_START, reinterpret_cast<EventFunc>(&HwMicrophone::_onStart));
    registerEvent(EVENT_COMMON_PAUSE, reinterpret_cast<EventFunc>(&HwMicrophone::_onPause));
    registerEvent(EVENT_MICROPHONE_LOOP, reinterpret_cast<EventFunc>(&HwMicrophone::_onLoop));
}

HwMicrophone::~HwMicrophone() {

}

bool HwMicrophone::onCreate(AlMessage *msg) {
    return true;
}

bool HwMicrophone::onDestroy(AlMessage *msg) {
    if (recorder) {
        recorder->stop();
        delete recorder;
        recorder = nullptr;
    }
    if (frame) {
        delete frame;
        frame = nullptr;
    }
    return true;
}

bool HwMicrophone::_onFormat(AlMessage *msg) {
    auto *f = msg->getObj<HwSampleFormat *>();
    if (f) {
        AlLogI(TAG, "");
        format = *f;
        int sampleCount = 1024;
        frame = new HwAudioFrame(nullptr, format.getFormat(), format.getChannels(),
                                 format.getSampleRate(), sampleCount);
        uint16_t f = 0x0020;
        switch (this->format.getFormat()) {
            case HwFrameFormat::HW_SAMPLE_S32: {
                f = 0x0020;
                break;
            }
            case HwFrameFormat::HW_SAMPLE_S16: {
                f = 0x0010;
                break;
            }
            default: {
                AlLogW(TAG, "UnSupport format(%d)", (int) this->format.getFormat());
                f = 0x0020;
            }
        }
        recorder = new HwAudioRecorder(format.getChannels(), format.getSampleRate(),
                                       f, sampleCount);
        recorder->start();
        loop();
    }
    return true;
}

bool HwMicrophone::_onStart(AlMessage *msg) {
    looping = true;
    loop();
    return true;
}

bool HwMicrophone::_onPause(AlMessage *msg) {
    looping = false;
    return true;
}

bool HwMicrophone::_onLoop(AlMessage *msg) {
    if (recorder && looping) {
        HwBuffer *buf = recorder->read(frame->size());
        if (buf) {
            send(buf);
        }
        loop();
    }
    return true;
}

void HwMicrophone::loop() {
    postEvent(AlMessage::obtain(EVENT_MICROPHONE_LOOP, nullptr, Message::QUEUE_MODE_UNIQUE));
}

void HwMicrophone::send(HwBuffer *buf) {
    if (buf && frame) {
        memcpy(frame->data(), buf->data(), buf->size());
        AlMessage *msg = AlMessage::obtain(EVENT_MICROPHONE_OUT_SAMPLES);
        msg->arg1 = 1;
        msg->arg2 = TimeUtils::getCurrentTimeUS() * 1000;
        msg->obj = HwBuffer::wrap(frame->data(), frame->size());
        postEvent(msg);
    }
}
