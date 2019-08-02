/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWFFCODEC_H
#define HWVC_ANDROID_HWFFCODEC_H

#include "HwAbsCodec.h"
#include "HwAudioTranslator.h"

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

    virtual HwResult configure(HwBundle *format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket
     */
    virtual HwResult encode(HwAbsMediaFrame *frame, void **packet) override;

    virtual int32_t type() override;

    virtual int32_t getExtraBuffer(string key, uint8_t **buf) override;

private:
    void release();

    bool configureVideo(AVCodecID id, AVCodec *codec);

    bool configureAudio(AVCodecID id, AVCodec *codec);

private:
    AVCodecContext *ctx = nullptr;
    AVFrame *avFrame = nullptr;
    AVPacket *avPacket = nullptr;
    /**
     * Just for audio codec.
     */
    HwAudioTranslator *translator = nullptr;
};

#ifdef __cplusplus
}
#endif

#endif //HWVC_ANDROID_HWFFCODEC_H
