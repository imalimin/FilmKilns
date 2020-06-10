/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWFFCODEC_H
#define HWVC_ANDROID_HWFFCODEC_H

#include "AlCodec.h"
#include "HwAudioTranslator.h"
#include "HwBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwFFCodec : public AlCodec {
public:
    HwFFCodec(AlCodec::kID id);

    virtual ~HwFFCodec();

    virtual HwResult configure(AlBundle &format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) override;

    virtual void flush() override;

private:
    void release();

    bool configureVideo(AVCodecID id, AVCodec *codec);

    bool configureAudio(AVCodecID id, AVCodec *codec);

    void _configureBitrate(int32_t bitrate);

private:
    AVCodecContext *ctx = nullptr;
    AVFrame *avFrame = nullptr;
    AVPacket *avPacket = nullptr;
    HwPacket *hwPacket = nullptr;
    /**
     * Just for audio codec.
     */
    HwAudioTranslator *translator = nullptr;
    AlBuffer *mExtraData = nullptr;
    int64_t vLastPts = 0;
    bool reqExtraData = true;
};

#endif //HWVC_ANDROID_HWFFCODEC_H
