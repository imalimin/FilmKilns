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

}

HwMicPhone::~HwMicPhone() {

}

bool HwMicPhone::eventPrepare(Message *msg) {
    return true;
}

bool HwMicPhone::eventRelease(Message *msg) {
    return true;
}