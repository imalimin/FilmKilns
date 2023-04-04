/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWPACKET_H
#define HWVC_ANDROID_HWPACKET_H

#include "FkObject.h"
#include "FkBuffer.h"

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
FK_SUPER_CLASS(FkPacket, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkPacket)

public:
    static FkPacket *wrap(AVPacket *pkt);

    static FkPacket *wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags = 0);

    static FkPacket *create(size_t size, int64_t pts, int64_t dts);

    static std::shared_ptr<FkPacket> clone(FkPacket *pkt);

public:
    virtual ~FkPacket();

    uint8_t *data();

    size_t size();

    int64_t getPts();

    int64_t getDts();

    int64_t getDuration();

    void setDuration(int64_t _duration);

    bool ref(AVPacket **pkt);

    int32_t getFlags();

    std::string getFlagsStr();

private:
    FkPacket();

private:
    std::shared_ptr<FkBuffer> buf = nullptr;
    int64_t pts = INT64_MIN, dts = INT64_MIN;
    int64_t duration = 1;
    int32_t flags = 0;
};


#endif //HWVC_ANDROID_HWPACKET_H
