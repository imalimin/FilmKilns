/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwCameraInput.h"

HwCameraInput::HwCameraInput() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwCameraInput::eventPrepare));
}

HwCameraInput::~HwCameraInput() {

}

bool HwCameraInput::eventPrepare(Message *msg) {
    return true;
}

bool HwCameraInput::eventRelease(Message *msg) {
    return true;
}