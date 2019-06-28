/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDMEDIADECODER_H
#define HWVC_ANDROID_HWANDROIDMEDIADECODER_H

#include "AbsAudioDecoder.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "media/NdkMediaCodec.h"

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

class HwAndroidAudioDecoder : public AbsAudioDecoder {
public:
    HwAndroidAudioDecoder();

    virtual ~HwAndroidAudioDecoder();

    bool prepare(string path) override;

    virtual HwResult grab(HwAbsMediaFrame **frame);

    void seek(int64_t us) override;

    int getChannels() override;

    int getSampleHz() override;

    int getSampleFormat() override;

    int getSamplesPerBuffer() override;

    int64_t getAudioDuration() override;

private:
    bool prepareCodec();

    int getProfile();

private:
    string path;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    AMediaCodec *codec = nullptr;
    AVFormatContext *pFormatCtx = nullptr;
    AVSampleFormat outSampleFormat = AV_SAMPLE_FMT_S16;
    AVRational outTimeBase = AVRational{1, 1000000};
    int64_t aDurationInUs = -1;
    int track = -1;
    AVPacket *avPacket = nullptr;
    HwAbsMediaFrame *outHwFrame = nullptr;
    bool eof = false;
    SimpleLock readPkgLock;

private:
    static const int AACObjectMain = 1;
    static const int AACObjectLC = 2;
    static const int AACObjectSSR = 3;
    static const int AACObjectLTP = 4;
    static const int AACObjectHE = 5;
    static const int AACObjectScalable = 6;
    static const int AACObjectERLC = 17;
    static const int AACObjectERScalable = 20;
    static const int AACObjectLD = 23;
    static const int AACObjectHE_PS = 29;
    static const int AACObjectELD = 39;
};


#endif //HWVC_ANDROID_HWANDROIDMEDIADECODER_H
