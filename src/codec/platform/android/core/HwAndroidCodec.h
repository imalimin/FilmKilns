/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDCODEC_H
#define HWVC_ANDROID_HWANDROIDCODEC_H

#include "../../include/HwAbsCodec.h"
#include "HwBuffer.h"
#include "media/NdkMediaCodec.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../../include/ff/libavcodec/avcodec.h"
#include "../../include/ff/libavformat/avformat.h"
#include "../../include/ff/libavutil/avutil.h"
#include "../../include/ff/libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwAndroidCodec : public HwAbsCodec {
public:
    static const int32_t HW_ANDROID_AVC;
public:
    HwAndroidCodec(int32_t codecId);

    virtual ~HwAndroidCodec();

    virtual HwResult configure(HwBundle *format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket.
     */
    virtual HwResult encode(HwAbsMediaFrame *frame, void **packet) override;

    virtual int32_t type() override;

    virtual HwBuffer *getExtraBuffer(string key) override;

private:
    void flush();

    void release();

    HwResult push(HwAbsMediaFrame *frame);

    HwResult pop(int32_t waitInUS);

private:
    const int COLOR_FormatYUV420Flexible = 0x7F420888;
    const int BUFFER_FLAG_KEY_FRAME = 1;
    const int BUFFER_FLAG_CODEC_CONFIG = 2;
    HwBuffer *buffers[4] = {nullptr, nullptr, nullptr, nullptr};
    AMediaCodec *codec = nullptr;
    HwBuffer *configBuf = nullptr;
    HwBuffer *keyFrameBuf = nullptr;
    AVPacket *avPacket = nullptr;
};


#endif //HWVC_ANDROID_HWANDROIDCODEC_H
