/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwModelProvider.h"
#include "HwPair.h"

const int HwModelProvider::EVENT_PUT_INT32 = KID('M', 'O', 'P', 0x01);
const int HwModelProvider::EVENT_PUT_INT64 = KID('M', 'O', 'P', 0x02);
const int HwModelProvider::EVENT_PUT_STRING = KID('M', 'O', 'P', 0x03);

HwModelProvider::HwModelProvider() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_PUT_INT32, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutInt32));
    registerEvent(EVENT_PUT_INT64, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutInt64));
    registerEvent(EVENT_PUT_STRING, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutString));
}

HwModelProvider::~HwModelProvider() {

}

bool HwModelProvider::eventRelease(Message *msg) {
    return true;
}

bool HwModelProvider::eventPutInt32(Message *msg) {
    HwPair<string, int32_t> *p = static_cast<HwPair<string, int32_t> *>(msg->obj);
    if (p) {
        bundle.putInt32(p->key(), p->value());
    }
    return true;
}

bool HwModelProvider::eventPutInt64(Message *msg) {
    HwPair<string, int64_t> *p = static_cast<HwPair<string, int64_t> *>(msg->obj);
    if (p) {
        bundle.putInt64(p->key(), p->value());
    }
    return true;
}

bool HwModelProvider::eventPutString(Message *msg) {
    HwPair<string, string> *p = static_cast<HwPair<string, string> *>(msg->obj);
    if (p) {
        bundle.putString(p->key(), p->value());
    }
    return true;
}

int32_t HwModelProvider::getInt32(string key) {
    return bundle.getInt32(key);
}

int64_t HwModelProvider::getInt64(string key) {
    return bundle.getInt64(key);
}

string HwModelProvider::getString(string key) {
    return bundle.getString(key);
}