/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwMicPhone.h"

HwMicPhone::HwMicPhone() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwMicPhone::eventPrepare));
    registerEvent(EVENT_MICROPHONE_LOOP, reinterpret_cast<EventFunc>(&HwMicPhone::eventLoop));

}

HwMicPhone::~HwMicPhone() {

}

bool HwMicPhone::eventPrepare(Message *msg) {
    frame = new HwAudioFrame(nullptr, HwFrameFormat::HW_SAMPLE_S32, 2, 44100, 1024);
    recorder = new HwAudioRecorder(2, 44100, 0x0010, 1024);
    recorder->start();
    loop();
    return true;
}

bool HwMicPhone::eventRelease(Message *msg) {
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

bool HwMicPhone::eventLoop(Message *msg) {
    if (recorder) {
        HwBuffer *buf = recorder->read(1024);
        send(buf);
        loop();
    }
    return true;
}

void HwMicPhone::loop() {
    postEvent(new Message(EVENT_MICROPHONE_LOOP, nullptr));
}

void HwMicPhone::send(HwBuffer *buf) {
    if (buf && frame) {
        memcpy(frame->getBuffer()->getData(), buf->getData(), buf->size());
    }
}