/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAndroidCodecCompat.h"

#define TAG "AlAndroidCodecCompat"

AlAndroidCodecCompat::AlAndroidCodecCompat(int32_t codecId, bool makeNalSelf)
        : HwAbsCodec(codecId) {
    bridge = new AlMediaCodecBridge(codecId, makeNalSelf);
}

AlAndroidCodecCompat::~AlAndroidCodecCompat() {
    delete packet;
    packet = nullptr;
    delete bridge;
    bridge = nullptr;
}

HwResult AlAndroidCodecCompat::configure(HwBundle &format) {
    HwAbsCodec::configure(format);
    int32_t width = format.getInt32(KEY_WIDTH);
    int32_t height = format.getInt32(KEY_HEIGHT);
    int32_t bitrate = (int32_t) format.getInt32(KEY_BIT_RATE);
    if (AlCodec::H264 == codecId) {
        fps = format.getInt32(KEY_FPS);
        return bridge->configure(width, height, bitrate,
                                 COLOR_FormatYUV420Flexible, 3, fps);
    }
    return Hw::FAILED;
}

HwResult AlAndroidCodecCompat::start() {
    return bridge->start() ? Hw::SUCCESS : Hw::FAILED;
}

HwResult AlAndroidCodecCompat::process(HwAbsMediaFrame **frame, HwPacket **pkt) {
    AlBuffer *src = AlBuffer::wrap((*frame)->data(), (*frame)->size());
    if (Hw::SUCCESS != bridge->process(src, (*frame)->getPts())) {
        AlLogE(TAG, "failed");
        return Hw::FAILED;
    }
    HwBuffer *dst = bridge->getBuffer();
    if (dst) {
        size_t size = 0;
        int64_t pts = 0;
        bool keyFrame = false;
        bridge->getBufferInfo(size, pts, keyFrame);
        delete packet;
        packet = HwPacket::wrap(dst->data(), size, pts, pts, keyFrame ? AV_PKT_FLAG_KEY : 0);
        packet->setDuration(static_cast<int64_t>(AV_TIME_BASE / (float) fps));
        *pkt = packet;
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

int32_t AlAndroidCodecCompat::type() {
    return bridge->type();
}

HwBuffer *AlAndroidCodecCompat::getExtraBuffer(string key) {
    return bridge->getExtraBuffer(key);
}

void AlAndroidCodecCompat::flush() {
    bridge->flush();
}
