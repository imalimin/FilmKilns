/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDDECODER_H
#define HWVC_ANDROID_HWANDROIDDECODER_H

#include "AbsAudioDecoder.h"
#include "AbsVideoDecoder.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "AlCodec.h"
#include "media/NdkMediaCodec.h"
#include "media/NdkMediaExtractor.h"

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

    virtual void seek(int64_t us, AbsDecoder::kSeekMode mode = AbsDecoder::kSeekMode::BACKWARD) override;


    virtual void setOutSampleFormat(HwSampleFormat format) override {};
    /**
     * @return 1: video, 2: audio, 0: failed
     */
    virtual HwResult grab(HwAbsMediaFrame **frame) override;

    virtual int64_t getVideoDuration() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual int64_t getAudioStartTime() override { return 0; };

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

private:
    bool configure();

    void handleAction();

private:
    string path;
    AMediaFormat *vFmt = nullptr;
    AMediaFormat *aFmt = nullptr;
    AMediaExtractor *extractor = nullptr;
    AlCodec *codec = nullptr;
    HwBuffer *vSampleBuf = nullptr;
    bool eof = false;
    int64_t videoDurationUs = -1;
    int64_t audioDurationUs = -1;
    int64_t durationUs = -1;
    int32_t mWidth = 0, mHeight = 0;
    /** action */
    int64_t actionSeekInUs = -1;
};


#endif //HWVC_ANDROID_HWANDROIDDECODER_H
