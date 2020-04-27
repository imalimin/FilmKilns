/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAndroidCodecCompat.h"

AlAndroidCodecCompat::AlAndroidCodecCompat(int32_t codecId, bool makeNalSelf)
        : HwAbsCodec(codecId) {
    bridge = new AlMediaCodecBridge(codecId, makeNalSelf);
}

AlAndroidCodecCompat::~AlAndroidCodecCompat() {
    delete bridge;
    bridge = nullptr;
}

HwResult AlAndroidCodecCompat::configure(HwBundle &format) {
    return HwAbsCodec::configure(format);
}

HwResult AlAndroidCodecCompat::start() {
    return Hw::SUCCESS;
}

HwResult AlAndroidCodecCompat::process(HwAbsMediaFrame **frame, HwPacket **pkt) {
    return Hw::SUCCESS;
}

int32_t AlAndroidCodecCompat::type() {
    switch (codecId) {
        case AlCodec::H264: {
            return 0;
        }
        default: {
            return 0;
        }
    }
}

HwBuffer *AlAndroidCodecCompat::getExtraBuffer(string key) {
    return nullptr;
}

void AlAndroidCodecCompat::flush() {

}
