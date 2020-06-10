/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWPACKET_H
#define HWVC_ANDROID_HWPACKET_H

#include "Object.h"
#include "HwBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"

#ifdef __cplusplus
}
#endif

/**
 *
 * For frame data after encoded. Like AVPacket
 */
class HwPacket : public Object {
public:
    /// Same with AV_PKT_FLAG_KEY/AV_PKT_FLAG_CORRUPT...
    static constexpr int32_t FLAG_KEY = 0x0001;
    static constexpr int32_t FLAG_CORRUPT = 0x0002;
    static constexpr int32_t FLAG_DISCARD = 0x0004;
    static constexpr int32_t FLAG_TRUSTED = 0x0008;
    static constexpr int32_t FLAG_DISPOSABLE = 0x0010;
    static constexpr int32_t FLAG_CONFIG = 0x0020;

public:
    static HwPacket *wrap(AVPacket *pkt);

    static HwPacket *wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags = 0);

    static HwPacket *create(size_t size, int64_t pts, int64_t dts);

public:
    virtual ~HwPacket();

    uint8_t *data();

    size_t size();

    int64_t getPts();

    int64_t getDts();

    int64_t getDuration();

    void setDuration(int64_t duration);

    bool ref(AVPacket **pkt);

    int32_t getFlags();

private:
    HwPacket();

private:
    HwBuffer *buf = nullptr;
    int64_t pts = INT64_MIN, dts = INT64_MIN;
    int64_t duration = 1;
    int32_t flags = 0;
};


#endif //HWVC_ANDROID_HWPACKET_H
