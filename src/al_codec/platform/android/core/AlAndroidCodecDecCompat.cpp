/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAndroidCodecDecCompat.h"

#define TAG "AlAndroidCodecDecCompat"

AlAndroidCodecDecCompat::AlAndroidCodecDecCompat(AlCodec::kID id) : AlAndroidCodecCompat(id) {

}

AlAndroidCodecDecCompat::~AlAndroidCodecDecCompat() {

}

HwResult AlAndroidCodecDecCompat::configure(AlBundle &format) {
    AlCodec::configure(format);
    int64_t def = 0;
    int32_t width = format.get(KEY_WIDTH, INT32_MIN);
    int32_t height = format.get(KEY_HEIGHT, INT32_MIN);
    int64_t duration = format.get(KEY_DURATION, def);

    AlBuffer *csd0Buf = reinterpret_cast<AlBuffer *>(format.get(KEY_CSD_0, def));
    AlBuffer *csd1Buf = reinterpret_cast<AlBuffer *>(format.get(KEY_CSD_1, def));
    auto ret = bridge->configure(width, height, duration, csd0Buf, csd1Buf, CONFIGURE_FLAG_DECODE);
    if (Hw::OK != ret) {
        AlLogE(TAG, "failed.");
    }
    return ret;
}
