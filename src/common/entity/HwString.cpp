/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwString.h"

HwString::HwString(string str) : Object(), value(str) {

}

HwString::HwString(const HwString &str) {
    this->value = str.value;
}

HwString::~HwString() {

}

string HwString::str() {
    return value;
}