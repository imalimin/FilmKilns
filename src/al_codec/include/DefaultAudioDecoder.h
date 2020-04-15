/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_DEFAULTAUDIODECODER_H
#define HARDWAREVIDEOCODEC_DEFAULTAUDIODECODER_H

#include "AbsAudioDecoder.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "HwAudioTranslator.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff/libavcodec/avcodec.h"
#include "ff/libavformat/avformat.h"
#include "ff/libavutil/avutil.h"
#include "ff/libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class DefaultAudioDecoder : public AbsAudioDecoder {
public:
    DefaultAudioDecoder();

    virtual ~DefaultAudioDecoder();

    virtual bool prepare(string path) override;

    void seek(int64_t us);

    /**
     * @return 1: video, 2: audio, 0: failed
     */
    virtual HwResult grab(HwAbsMediaFrame **frame) override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

private:
    bool openTrack(int track, AVCodecContext **context);

    AVSampleFormat getBestSampleFormat(AVSampleFormat in);

    HwAbsMediaFrame *resample(AVFrame *avFrame);

    void matchPts(AVFrame *frame, int track);

    void printCodecInfo();

    void handleAction();

private:
    bool enableDebug = false;
    string path;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext *aCodecContext = nullptr;
    HwAudioTranslator *translator = nullptr;
    int audioTrack = -1;
    AVPacket *avPacket = nullptr;
    AVFrame *audioFrame = nullptr;
    HwAbsMediaFrame *outHwFrame = nullptr;
    AVSampleFormat outSampleFormat = AV_SAMPLE_FMT_S16;
    AVRational outputTimeBase = AVRational{1, 1000000};
    int64_t audioDurationUs = -1;
    int64_t durationUs = -1;
    bool eof = false;
    /** action */
    int64_t actionSeekInUs = -1;
};


#endif //HARDWAREVIDEOCODEC_DEFAULTAUDIODECODER_H
