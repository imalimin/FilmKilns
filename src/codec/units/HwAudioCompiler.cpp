/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAudioCompiler.h"
#include "HwBuffer.h"

HwAudioCompiler::HwAudioCompiler() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE,
                  reinterpret_cast<EventFunc>(&HwAudioCompiler::eventPrepare));
    registerEvent(EVENT_MICROPHONE_OUT_SAMPLES,
                  reinterpret_cast<EventFunc>(&HwAudioCompiler::eventReceiveData));
}

HwAudioCompiler::~HwAudioCompiler() {

}

bool HwAudioCompiler::eventRelease(Message *msg) {
    if (file) {
        fclose(file);
        file = nullptr;
    }
    return true;
}

bool HwAudioCompiler::eventPrepare(Message *msg) {
    file = fopen("/sdcard/test.pcm", "wb+");
    return true;
}

bool HwAudioCompiler::eventReceiveData(Message *msg) {
    HwBuffer *buf = dynamic_cast<HwBuffer *>(msg->obj);
    if (buf) {
        fwrite(buf->getData(), 1, buf->size(), file);
    }
    return true;
}