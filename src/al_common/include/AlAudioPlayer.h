/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALAUDIOPLAYER_H
#define HWVC_ANDROID_ALAUDIOPLAYER_H

#include "AudioDevice.h"
#include "SLEngine.h"
#include "AlBuffer.h"
#include "HwResult.h"
#include <mutex>
#include <condition_variable>
#include <queue>

AL_CLASS AlAudioPlayer AL_EXTEND SLAudioDevice {
public:
    AlAudioPlayer(uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    AlAudioPlayer(HwAudioDeviceMode mode,
                  uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    AlAudioPlayer(SLEngine *engine,
                  HwAudioDeviceMode mode,
                  uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    virtual ~AlAudioPlayer();

    virtual HwResult start();

    virtual void stop();

    virtual HwResult write(uint8_t *buffer, size_t size);

    virtual HwResult write(uint8_t *buffer, size_t size, int timeOut);

    virtual void flush();

    void bufferEnqueue(SLAndroidSimpleBufferQueueItf slBufferQueueItf);

private:
    HwResult createEngine();

    void destroyEngine();

    HwResult createBufferQueueAudioPlayer();

    void initialize(SLEngine *engine);

    void configCache(int size);

    HwResult push(uint8_t *buffer, size_t size, int us);

    std::shared_ptr<AlBuffer> pop();

    void recycle(std::shared_ptr<AlBuffer> buf);

private:
    SLEngine *engine = nullptr;
    SLObjectItf mixObject = nullptr;
    SLObjectItf playObject = nullptr;
    SLPlayItf playItf = nullptr;
    SLAndroidSimpleBufferQueueItf bufferQueueItf = nullptr;

    bool ownEngine = false;
    uint8_t *silenceData = nullptr;
    std::queue<std::shared_ptr<AlBuffer>> input;
    std::queue<std::shared_ptr<AlBuffer>> cache;
    std::shared_ptr<AlBuffer> tmpData = nullptr;
    std::mutex mtx;
    std::condition_variable cond;
};


#endif //HWVC_ANDROID_ALAUDIOPLAYER_H
