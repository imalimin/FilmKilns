/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDENCODER_H
#define HWVC_ANDROID_HWANDROIDENCODER_H

#include "../../../include/HwAbsVideoEncoder.h"
#include "media/NdkMediaCodec.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "../../../include/ff/libavcodec/avcodec.h"
#include "../../../include/ff/libavformat/avformat.h"
#include "../../../include/ff/libavutil/avutil.h"
#include "../../../include/ff/libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwAndroidEncoder : public HwAbsVideoEncoder {
public:
    HwAndroidEncoder();

    virtual ~HwAndroidEncoder();

    virtual bool prepare(string path, int width, int height, HwSampleFormat audioFormat) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    bool configure();

    bool configureMuxer();

    void configureCodec(AVCodecContext *ctx);

    HwResult push(HwAbsMediaFrame *frame);

    HwResult pop();

    HwResult write();

    void flush();

private:
    const int COLOR_FormatYUV420Flexible = 0x7F420888;
    const int BUFFER_FLAG_KEY_FRAME = 1;
    const int BUFFER_FLAG_CODEC_CONFIG = 2;
    string path;
    int width = 0, height = 0;
    HwSampleFormat audioFormat = HwSampleFormat::NONE;
    AMediaCodec *codec = nullptr;
    HwBuffer *configBuf = nullptr;
    HwBuffer *keyFrameBuf = nullptr;
    AVRational outTimeBase = AVRational{1, 1000000};
    AVFormatContext *pFormatCtx = nullptr;
    AVStream *pVideoStream = nullptr;
    AVPacket *avPacket = nullptr;
};


#endif //HWVC_ANDROID_HWANDROIDENCODER_H
