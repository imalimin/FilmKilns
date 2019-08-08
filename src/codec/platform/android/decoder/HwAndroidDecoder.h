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
    HwFrameAllocator *hwFrameAllocator = nullptr;
    string path;
    AVFormatContext *pFormatCtx = nullptr;

};


#endif //HWVC_ANDROID_HWANDROIDDECODER_H
