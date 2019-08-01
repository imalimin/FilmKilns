/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWFFCODEC_H
#define HWVC_ANDROID_HWFFCODEC_H

#include "HwAbsCodec.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff/libavcodec/avcodec.h"
#include "ff/libavformat/avformat.h"
#include "ff/libavutil/avutil.h"
#include "ff/libswresample/swresample.h"

class HwFFCodec : public HwAbsCodec {
public:
    HwFFCodec(int32_t codecId);

    virtual ~HwFFCodec();

    virtual HwResult start() override;

private:
    void release();

private:
    AVCodecContext *ctx = nullptr;
};

#ifdef __cplusplus
}
#endif

#endif //HWVC_ANDROID_HWFFCODEC_H
