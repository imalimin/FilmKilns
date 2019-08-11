/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDDECODER_H
#define HWVC_ANDROID_HWANDROIDDECODER_H

#include "../../../include/AbsAudioDecoder.h"
#include "../../../include/AbsVideoDecoder.h"
#include "../../../include/HwAbsMediaFrame.h"
#include "../../../include/HwFrameAllocator.h"
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

class HwAndroidDecoder : public AbsVideoDecoder {
public:
    HwAndroidDecoder();

    virtual ~HwAndroidDecoder();

    virtual bool prepare(string path) override;

    virtual int width() override;

    virtual int height() override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual void seek(int64_t us) override;

    /**
     * @return 1: video, 2: audio, 0: failed
     */
    virtual HwResult grab(HwAbsMediaFrame **frame);

    virtual int64_t getVideoDuration() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

private:
    bool openTrack(int track, AVCodecContext **context);

    bool configure();

    bool configureBSF();

    HwResult push(AVPacket *pkt);

    HwResult pop(int32_t waitInUS);

private:
    const int COLOR_FormatYUV420Flexible = 0x7F420888;
    const int COLOR_FormatYUV420Planar = 19;
    const int COLOR_FormatYUV420SemiPlanar = 21;
    const int BUFFER_FLAG_KEY_FRAME = 1;
    const int BUFFER_FLAG_CODEC_CONFIG = 2;
    string path;
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext *vCodecContext = nullptr;
    AVCodecContext *aCodecContext = nullptr;
    AVSampleFormat outSampleFormat = AV_SAMPLE_FMT_NONE;
    AVBSFContext *bsf = nullptr;
    AVPacket *avPacket = nullptr;
    AVPacket *bsfPacket = nullptr;
    int audioTrack = -1, videoTrack = -1;
    AMediaCodec *codec = nullptr;
    int32_t stride = 0;
    HwBuffer *buffers[3] = {nullptr, nullptr, nullptr};
    HwAbsMediaFrame *outFrame = nullptr;
    bool eof = false;
    int64_t videoDurationUs = -1;
    int64_t audioDurationUs = -1;
    int64_t durationUs = -1;
};


#endif //HWVC_ANDROID_HWANDROIDDECODER_H
