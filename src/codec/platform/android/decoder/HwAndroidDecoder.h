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
    bool configure();

    HwResult push();

    HwResult pop(int32_t waitInUS);

private:
    const int COLOR_FormatYUV420Flexible = 0x7F420888;
    const int COLOR_FormatYUV420Planar = 19;
    const int COLOR_FormatYUV420SemiPlanar = 21;
    const int BUFFER_FLAG_KEY_FRAME = 1;
    const int BUFFER_FLAG_CODEC_CONFIG = 2;
    string path;
    AMediaFormat *vFmt = nullptr;
    AMediaFormat *aFmt = nullptr;
    AMediaExtractor *extractor = nullptr;
    AMediaCodec *codec = nullptr;
    int32_t stride = 0;
    HwBuffer *buffers[3] = {nullptr, nullptr, nullptr};
    HwAbsMediaFrame *outFrame = nullptr;
    bool eof = false;
    int64_t videoDurationUs = -1;
    int64_t audioDurationUs = -1;
    int64_t durationUs = -1;

    int32_t mWidth = 0, mHeight = 0;
};


#endif //HWVC_ANDROID_HWANDROIDDECODER_H
