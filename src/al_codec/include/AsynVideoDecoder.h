/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_ASYNVIDEODECODER_H
#define HARDWAREVIDEOCODEC_ASYNVIDEODECODER_H

#include "AbsVideoDecoder.h"
#include "DefaultVideoDecoder.h"
#include "RecyclerBlockQueue.h"
#include "AlEventPipeline.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "SimpleLock.h"
#include "PlayState.h"
#include "HwVideoFrame.h"
#include <queue>
#include <atomic>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

class AsynVideoDecoder : public AbsVideoDecoder {
public:
    AsynVideoDecoder();

    virtual ~AsynVideoDecoder();

    virtual bool prepare(string path) override;

    virtual int width() override;

    virtual int height() override;

    virtual void setOutSampleFormat(HwSampleFormat format) override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual void seek(int64_t us) override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

    HwResult grab(HwAbsMediaFrame **frame) override;

    virtual int64_t getVideoDuration() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual int64_t getAudioStartTime() override;

private:
    void loop();

    bool grab();

    void clear();

private:
    const int MAX_FRAME_CACHE = 36;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    AbsVideoDecoder *decoder = nullptr;
    AlEventPipeline *pipeline = nullptr;
    queue<HwAbsMediaFrame *> cache;
    HwAbsMediaFrame *outputFrame = nullptr;//用于缓存一帧，以便在下次grab的时候进行回收
    atomic_bool playing;
    SimpleLock grabLock;
    SimpleLock releaseLock;
};

#ifdef __cplusplus
}
#endif


#endif //HARDWAREVIDEOCODEC_ASYNVIDEODECODER_H
