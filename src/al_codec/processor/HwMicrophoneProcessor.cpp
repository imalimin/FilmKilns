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
    postEvent(AlMessage::obtain(EVENT_COMMON_PREPARE));
}

void HwMicrophoneProcessor::start() {
    postEvent(AlMessage::obtain(EVENT_COMMON_START));
}

void HwMicrophoneProcessor::pause() {
    postEvent(AlMessage::obtain(EVENT_COMMON_PAUSE));
}