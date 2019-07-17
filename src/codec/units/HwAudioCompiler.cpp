/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAudioCompiler.h"

HwAudioCompiler::HwAudioCompiler() : Unit() {

}

HwAudioCompiler::~HwAudioCompiler() {

}

bool HwAudioCompiler::eventRelease(Message *msg) {
    return true;
}

bool HwAudioCompiler::eventPrepare(Message *msg) {
    return true;
}