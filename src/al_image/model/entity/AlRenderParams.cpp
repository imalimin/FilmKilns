/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRenderParams.h"

AlRenderParams::AlRenderParams(int32_t flags) : AlBinaryParams(flags) {

}

AlRenderParams::AlRenderParams(const AlRenderParams &o) : AlBinaryParams() {

}

AlRenderParams::~AlRenderParams() {
}

void AlRenderParams::setRenderScreen(bool value) {
    set(0, !value);
}

bool AlRenderParams::isRenderScreen() {
    return !get(0);
}

void AlRenderParams::setTransparent(bool value) {
    set(1, value);
}

bool AlRenderParams::isTransparent() {
    return get(1);
}

void AlRenderParams::setReqClear(bool value) {
    set(2, value);
}

bool AlRenderParams::isReqClear() {
    return get(2);
}
