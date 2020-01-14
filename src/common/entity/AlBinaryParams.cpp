/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlBinaryParams.h"

AlBinaryParams::AlBinaryParams(int32_t flags) : Object(), flags(flags) {

}

AlBinaryParams::AlBinaryParams(const AlBinaryParams &o) : Object(), flags(o.flags) {

}

AlBinaryParams::~AlBinaryParams() {
    flags = 0;
}

int32_t AlBinaryParams::toInt() {
    return flags;
}

void AlBinaryParams::set(int32_t index, bool value) {
    if (value) {
        flags |= (0x1 << index);
    } else {
        flags &= (~(0x1 << index));
    }
}

bool AlBinaryParams::get(int32_t index) {
    return 0 != (flags & (0x1 << index));
}
