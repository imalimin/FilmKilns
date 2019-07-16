/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWFFMPEGENCODER_H
#define HWVC_ANDROID_HWFFMPEGENCODER_H

#include "HwAbsEncoder.h"
#include "HwResult.h"
#include "HwAbsMediaFrame.h"
#include "HwAudioTranslator.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff/libavcodec/avcodec.h"
#include "ff/libavformat/avformat.h"
#include "ff/libavutil/avutil.h"
#include "ff/libswresample/swresample.h"

class HwFFmpegEncoder : public HwAbsEncoder {
public:
    HwFFmpegEncoder();

    virtual ~HwFFmpegEncoder();

    virtual bool prepare(string path, int width, int height) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    bool initialize();

    void configure(AVCodecContext *ctx);

    bool openVideoTrack();

    bool openAudioTrack();

private:
    string path;
    int width = 0;
    int height = 0;
    AVFormatContext *pFormatCtx = nullptr;
    AVStream *pVideoStream = nullptr;
    AVStream *pAudioStream = nullptr;
    AVRational outTimeBase = AVRational{1, 1000000};
    AVFrame *avFrame = nullptr;
    AVPacket *avPacket = nullptr;
    AVFrame *avAudioFrame = nullptr;
    HwAudioTranslator *translator = nullptr;
};

#ifdef __cplusplus
}
#endif


#endif //HWVC_ANDROID_HWFFMPEGENCODER_H
