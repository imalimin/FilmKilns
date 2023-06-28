/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef FK_BASE_FKAUDIORECORDER_H
#define FK_BASE_FKAUDIORECORDER_H

#include "FkObject.h"
#include "FkAudioDevice.h"
#include "FkHandlerThread.h"
#include "FkHandler.h"
#include "FkSimpleLock.h"
#include "FkSLEngine.h"
#include "FkSLRecorder.h"
#include "FkFIFOBuffer.h"
#include "FkBuffer.h"

FK_SUPER_CLASS(FkAudioRecorder, FkSLAudioDevice) {
FK_DEF_CLASS_TYPE_FUNC(FkAudioRecorder)

public:
    FkAudioRecorder(uint16_t channels,
                    uint32_t sampleRate,
                    uint16_t format);

    FkAudioRecorder(uint16_t channels,
                    uint32_t sampleRate,
                    uint16_t format,
                    uint32_t samplesPerBuffer);

    FkAudioRecorder(FkAudioDevice::kMode mode,
                    uint16_t channels,
                    uint32_t sampleRate,
                    uint16_t format,
                    uint32_t samplesPerBuffer);

    FkAudioRecorder(FkSLEngine *engine,
                    FkAudioDevice::kMode mode,
                    uint16_t channels,
                    uint32_t sampleRate,
                    uint16_t format,
                    uint32_t samplesPerBuffer);

    virtual ~FkAudioRecorder();

    virtual FkResult start();

    virtual void stop();

    virtual std::shared_ptr<FkBuffer> read(size_t size);

    virtual std::shared_ptr<FkBuffer> read(size_t size, int timeOut);

    virtual void flush();

    void bufferDequeue(SLAndroidSimpleBufferQueueItf slBufferQueueItf);

private:
    FkSLEngine *engine = nullptr;
    bool ownEngine = false;
    FkSLRecorder *recorder = nullptr;
    FkFIFOBuffer *fifo = nullptr;
   std::shared_ptr<FkBuffer> buffer = nullptr;

    FkResult createEngine();

    void destroyEngine();

    FkResult createBufferQueueObject();

    void initialize(FkSLEngine *_engine);

};


#endif //FK_BASE_FKAUDIORECORDER_H
