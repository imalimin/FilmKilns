/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwResult.h"

HwResult::HwResult(int code) : Object(),code(code) {
}

HwResult::HwResult(const HwResult &value) : Object(), code(value.code) {
}

HwResult::~HwResult() {

}

bool HwResult::operator==(const HwResult &ret) const {
    return ret.code == code;
}

bool HwResult::operator!=(const HwResult &ret) const {
    return ret.code != code;
}

HwResult &HwResult::operator=(const HwResult &value) {
    code = value.code;
    return *this;
}

bool HwResult::operator==(int ret) const {
    return ret == code;
}

bool HwResult::operator!=(int ret) const {
    return ret != code;
}
