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

class HwFFCodec : public AlCodec {
public:
    HwFFCodec(AlCodec::kID id);

    virtual ~HwFFCodec();

    virtual HwResult configure(HwBundle &format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) override;

    virtual HwBuffer *getExtraBuffer(string key) override;

    virtual void flush() override;

    virtual AlCodec::kMediaType getMediaType() override;

private:
    void release();

    bool configureVideo(AVCodecID id, AVCodec *codec);

    bool configureAudio(AVCodecID id, AVCodec *codec);

    bool parseExtraData();

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
    HwBuffer *buffers[4] = {nullptr, nullptr, nullptr, nullptr};
    int64_t vLastPts = 0;
};

#ifdef __cplusplus
}
#endif

#endif //HWVC_ANDROID_HWFFCODEC_H
