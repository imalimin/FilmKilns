/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRenderParams.h"

AlRenderParams::AlRenderParams(int32_t flags) : Object(), flags(flags) {

}

AlRenderParams::AlRenderParams(const AlRenderParams &o) : Object(), flags(o.flags) {

}

AlRenderParams::~AlRenderParams() {
    flags = 0;
}

int32_t AlRenderParams::toInt() {
    return flags;
}

void AlRenderParams::setRenderScreen(bool value) {
    if (value) {
        flags = flags & 0xfffffffe;
    } else {
        flags = flags | 0x1;
    }
}

bool AlRenderParams::isRenderScreen() {
    return 0 == (flags & 0x1);
}

void AlRenderParams::setTransparent(bool value) {
    if (value) {
        flags = flags | 0x2;
    } else {
        flags = flags & 0xfffffffd;
    }
}

bool AlRenderParams::isTransparent() {
    return 0 != (flags & 0x2);
}

void AlRenderParams::setReqSave(bool value) {
    if (value) {
        flags = flags | 0x4;
    } else {
        flags = flags & 0xfffffffb;
    }
}

bool AlRenderParams::isReqSave() {
    return 0 != (flags & 0x4);
}
