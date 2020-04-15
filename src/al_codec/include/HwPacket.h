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

#include "ff/libavcodec/avcodec.h"

#ifdef __cplusplus
}
#endif

/**
 *
 * For frame data after encoded. Like AVPacket
 */
class HwPacket : public Object {
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

private:
    HwPacket();

private:
    HwBuffer *buf = nullptr;
    int64_t pts = INT64_MIN, dts = INT64_MIN;
    int64_t duration = 1;
    int32_t flags = 0;
};


#endif //HWVC_ANDROID_HWPACKET_H
