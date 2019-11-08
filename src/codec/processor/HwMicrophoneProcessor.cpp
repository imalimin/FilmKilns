/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwMicrophoneProcessor.h"
#include "../include/HwMicrophone.h"
#include "../include/HwAudioCompiler.h"
#include "ObjectBox.h"

HwMicrophoneProcessor::HwMicrophoneProcessor() : HwAbsProcessor("HwMicrophoneProcessor") {
    registerAnUnit(new HwMicrophone(ALIAS_OF_MIC));
    registerAnUnit(new HwAudioCompiler(ALIAS_OF_COMPILER));
}

HwMicrophoneProcessor::~HwMicrophoneProcessor() {
}

void HwMicrophoneProcessor::prepare(string filePath, HwSampleFormat format) {
    this->format = format;
    putString("path", filePath).to({ALIAS_OF_COMPILER});
    putObject("format", &this->format).to({ALIAS_OF_COMPILER});
    postEvent(new Message(EVENT_COMMON_PREPARE, nullptr));
}

void HwMicrophoneProcessor::start() {
    postEvent(new Message(EVENT_COMMON_START, nullptr));
}

void HwMicrophoneProcessor::pause() {
    postEvent(new Message(EVENT_COMMON_PAUSE, nullptr));
}