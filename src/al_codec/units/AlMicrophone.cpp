/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlMicrophone.h"
#include "TimeUtils.h"
#include "Thread.h"
#include "AlBuffer.h"

#define TAG "AlMicrophone"

AlMicrophone::AlMicrophone(string alias) : Unit(alias),
                                           format(HwFrameFormat::HW_SAMPLE_S32, 2, 44100) {
    al_reg_msg(MSG_MICROPHONE_FORMAT, AlMicrophone::_onFormat);
    al_reg_msg(EVENT_COMMON_START, AlMicrophone::_onStart);
    al_reg_msg(EVENT_COMMON_PAUSE, AlMicrophone::_onPause);
    al_reg_msg(EVENT_MICROPHONE_LOOP, AlMicrophone::_onLoop);
}

AlMicrophone::~AlMicrophone() {

}

bool AlMicrophone::onCreate(AlMessage *msg) {
    return true;
}

bool AlMicrophone::onDestroy(AlMessage *msg) {
    if (recorder) {
        recorder->stop();
    }
    delete recorder;
    recorder = nullptr;
    delete frame;
    frame = nullptr;
    return true;
}

bool AlMicrophone::_onFormat(AlMessage *msg) {
    auto *fmt = msg->getObj<HwSampleFormat *>();
    if (nullptr == fmt || !fmt->valid()) {
        AlLogE(TAG, "Invalid audio format");
        return true;
    }
    format = *fmt;
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
    return true;
}

bool AlMicrophone::_onStart(AlMessage *msg) {
    if (!format.valid()) {
        AlLogE(TAG, "Invalid audio format");
        return true;
    }
    looping = true;
    loop();
    return true;
}

bool AlMicrophone::_onPause(AlMessage *msg) {
    if (!format.valid()) {
        AlLogE(TAG, "Invalid audio format");
        return true;
    }
    looping = false;
    return true;
}

bool AlMicrophone::_onLoop(AlMessage *msg) {
    if (recorder && looping) {
        HwBuffer *buf = recorder->read(frame->size());
        if (buf) {
            send(buf);
        }
        loop();
    }
    return true;
}

void AlMicrophone::loop() {
    postEvent(AlMessage::obtain(EVENT_MICROPHONE_LOOP, nullptr, Message::QUEUE_MODE_UNIQUE));
}

void AlMicrophone::send(HwBuffer *buf) {
    if (buf && frame) {
        memcpy(frame->data(), buf->data(), buf->size());
        AlMessage *msg = AlMessage::obtain(EVENT_MICROPHONE_OUT_SAMPLES);
        msg->arg1 = 1;
        msg->arg2 = TimeUtils::getCurrentTimeUS() * 1000;
        msg->obj = AlBuffer::wrap(frame->data(), frame->size());
        postEvent(msg);
    }
}
