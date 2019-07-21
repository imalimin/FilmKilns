/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwMicrophone.h"
#include "TimeUtils.h"
#include "Thread.h"

HwMicrophone::HwMicrophone() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwMicrophone::eventPrepare));
    registerEvent(EVENT_COMMON_START, reinterpret_cast<EventFunc>(&HwMicrophone::eventStart));
    registerEvent(EVENT_COMMON_PAUSE, reinterpret_cast<EventFunc>(&HwMicrophone::eventPause));
    registerEvent(EVENT_MICROPHONE_LOOP, reinterpret_cast<EventFunc>(&HwMicrophone::eventLoop));

}

HwMicrophone::~HwMicrophone() {

}

bool HwMicrophone::eventPrepare(Message *msg) {
    frame = new HwAudioFrame(nullptr, HwFrameFormat::HW_SAMPLE_S32, 2, 44100, 1024);
    recorder = new HwAudioRecorder(2, 44100, 0x0020, 1024);
    recorder->start();
    loop();
    return true;
}

bool HwMicrophone::eventRelease(Message *msg) {
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

bool HwMicrophone::eventStart(Message *msg) {
    looping = true;
    loop();
    return true;
}

bool HwMicrophone::eventPause(Message *msg) {
    looping = false;
    return true;
}

bool HwMicrophone::eventLoop(Message *msg) {
    if (recorder && looping) {
        HwBuffer *buf = recorder->read(8192);
        if (buf) {
            send(buf);
        }
        loop();
    }
    return true;
}

void HwMicrophone::loop() {
    postEvent(new Message(EVENT_MICROPHONE_LOOP, nullptr, Message::QUEUE_MODE_UNIQUE, nullptr));
}

void HwMicrophone::send(HwBuffer *buf) {
    if (buf && frame) {
        memcpy(frame->getBuffer()->getData(), buf->getData(), buf->size());
        Message *msg = new Message(EVENT_MICROPHONE_OUT_SAMPLES, nullptr);
        msg->arg2 = TimeUtils::getCurrentTimeUS();
        msg->obj = HwBuffer::wrap(frame->getBuffer()->getData(), frame->getBuffer()->size());
        postEvent(msg);
    }
}