/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAudioPlayer.h"
#include "AlLogcat.h"
#include "HwResult.h"

#define TAG "AlAudioPlayer"

void bufferQueueCallbackV2(SLAndroidSimpleBufferQueueItf slBufferQueueItf, void *context) {
    AlAudioPlayer *player = static_cast<AlAudioPlayer *>(context);
    player->bufferEnqueue(slBufferQueueItf);
}

AlAudioPlayer::AlAudioPlayer(uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : SLAudioDevice(AudioDevice::kMode::Normal,
                                                                        channels,
                                                                        sampleRate,
                                                                        format,
                                                                        samplesPerBuffer) {
    initialize(nullptr);
}

AlAudioPlayer::AlAudioPlayer(AudioDevice::kMode mode,
                             uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : SLAudioDevice(mode,
                                                                        channels,
                                                                        sampleRate,
                                                                        format,
                                                                        samplesPerBuffer) {
    initialize(nullptr);
}

AlAudioPlayer::AlAudioPlayer(SLEngine *engine,
                             AudioDevice::kMode mode,
                             uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : SLAudioDevice(mode,
                                                                        channels,
                                                                        sampleRate,
                                                                        format,
                                                                        samplesPerBuffer) {
    initialize(engine);
}

void AlAudioPlayer::initialize(SLEngine *engine) {
    this->silenceData = new uint8_t[getBufferByteSize()];
    memset(silenceData, 0, getBufferByteSize());
    this->engine = engine;
    uint32_t size = 16;
    switch (mode) {
        case HwAudioDeviceMode::LowLatency:
            size = 3;
            break;
        case HwAudioDeviceMode::Normal:
            size = 16;
            break;
        case HwAudioDeviceMode::HighLatency:
            size = 32;
    }
    configCache(size);
    AlLogI(TAG, "Create AlAudioPlayer, channels=%d, sampleHz=%d, minBufferSize=%d, format=%d",
           this->channels,
           this->sampleRate,
           this->samplesPerBuffer,
           this->format);
    mixObject = nullptr;
    playObject = nullptr;
    playItf = nullptr;
    HwResult ret = this->createEngine();
    if (Hw::SUCCESS != ret) {
        AlLogI(TAG, "AlAudioPlayer create failed");
    }

}

void AlAudioPlayer::configCache(int size) {
    for (int i = 0; i < size; ++i) {
        auto it = std::shared_ptr<AlBuffer>(AlBuffer::alloc(getBufferByteSize()));
        cache.push(it);
    }
}

AlAudioPlayer::~AlAudioPlayer() {
    AlLogI(TAG, "AlAudioPlayerer");
    stop();
    if (silenceData) {
        delete[]silenceData;
        silenceData = nullptr;
    }
}

HwResult AlAudioPlayer::createEngine() {
    if (!engine) {
        ownEngine = true;
        engine = new SLEngine();
        if (!engine || !engine->valid()) {
            AlLogI(TAG, "AlAudioPlayer create failed");
            stop();
            return Hw::FAILED;
        }
    }

    SLresult result = (*engine->getEngine())->CreateOutputMix(engine->getEngine(), &mixObject, 0,
                                                              nullptr, nullptr);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "CreateOutputMix failed!");
        return Hw::FAILED;
    }
    result = (*mixObject)->Realize(mixObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "OutputMix Realize failed!");
        return Hw::FAILED;
    }
    return createBufferQueueAudioPlayer();
}

HwResult AlAudioPlayer::start() {
    (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
    uint32_t bufSize = getBufferByteSize();
    uint8_t buffer[bufSize];
    memset(buffer, 0, bufSize);
    write(buffer, bufSize);
    bufferEnqueue(bufferQueueItf);
    SLresult result = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "Recorder SetRecordState start failed!");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlAudioPlayer::createBufferQueueAudioPlayer() {
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

    SLAndroidDataFormat_PCM_EX format_pcm = {SL_DATAFORMAT_PCM,
                                             channels,
                                             sampleRate * 1000,
                                             format,
                                             format,
                                             getChannelMask(),
                                             SL_BYTEORDER_LITTLEENDIAN};
    //在 Android 5.0（API 级别 21）及更高版本上运行的应用可以使用单精度浮点格式向音频播放器提供数据。
//    SLAndroidDataFormat_PCM_EX format_pcm = {SL_ANDROID_DATAFORMAT_PCM_EX,
//                                             channels,
//                                             sampleRate * 1000,
//                                             SL_PCMSAMPLEFORMAT_FIXED_32,
//                                             SL_PCMSAMPLEFORMAT_FIXED_32,
//                                             getChannelMask(),
//                                             SL_BYTEORDER_LITTLEENDIAN,
//                                             SL_ANDROID_PCM_REPRESENTATION_FLOAT};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX,
                                          mixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
    /*
     * create fast path audio player: SL_IID_BUFFERQUEUE and SL_IID_VOLUME
     * and other non-signal processing interfaces are ok.
     */
    SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult result = (*engine->getEngine())->CreateAudioPlayer(engine->getEngine(),
                                                                &playObject,
                                                                &audioSrc,
                                                                &audioSnk,
                                                                sizeof(ids) / sizeof(ids[0]),
                                                                ids,
                                                                req);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "CreateAudioPlayer failed! ret=%d", result);
        return Hw::FAILED;
    }
    result = (*playObject)->Realize(playObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "Player Realize failed!");
        return Hw::FAILED;
    }
    result = (*playObject)->GetInterface(playObject, SL_IID_PLAY, &playItf);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "Player GetInterface failed!");
        return Hw::FAILED;
    }
    result = (*playObject)->GetInterface(playObject, SL_IID_BUFFERQUEUE, &bufferQueueItf);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "Player GetInterface buffer queue failed!");
        return Hw::FAILED;
    }
    result = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "Player SetPlayState stop failed!");
        return Hw::FAILED;
    }
    result = (*bufferQueueItf)->RegisterCallback(bufferQueueItf,
                                                 bufferQueueCallbackV2,
                                                 this);
    if (SL_RESULT_SUCCESS != result) {
        AlLogI(TAG, "Player RegisterCallback failed!");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

static int64_t ttime = 0;

void AlAudioPlayer::bufferEnqueue(SLAndroidSimpleBufferQueueItf slBufferQueueItf) {
    auto buf = pop();
    if (nullptr != buf) {
        (*slBufferQueueItf)->Enqueue(bufferQueueItf, buf->data(), buf->size());
        recycle(buf);
        return;
    }
    AlLogW(TAG, "Play silence data.");
    (*slBufferQueueItf)->Enqueue(bufferQueueItf, silenceData, getBufferByteSize());
}

HwResult AlAudioPlayer::write(uint8_t *buffer, size_t size) {
    return write(buffer, size, 0);
}

HwResult AlAudioPlayer::write(uint8_t *buffer, size_t size, int timeOut) {
    auto ret = push(buffer, size);
    if (Hw::OK == ret) {
#if defined(__AL_DEBUG__)
        start();
        AlLogE(TAG, "failed, try flush & restart.");
#endif
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlAudioPlayer::push(uint8_t *buffer, size_t size) {
    std::lock_guard<std::mutex> guard(mtx);
    if (cache.empty()) {
        return Hw::FAILED;
    }
    auto it = cache.front();
    cache.pop();
    it->rewind();
    it->put(buffer, size);
    input.push(it);
    return Hw::SUCCESS;
}

std::shared_ptr<AlBuffer> AlAudioPlayer::pop() {
    std::lock_guard<std::mutex> guard(mtx);
    if (input.empty()) {
        return std::shared_ptr<AlBuffer>(nullptr);
    }
    auto it = input.front();
    input.pop();
    return it;
}

void AlAudioPlayer::recycle(std::shared_ptr<AlBuffer> buf) {
    std::lock_guard<std::mutex> guard(mtx);
    cache.push(buf);
}

void AlAudioPlayer::flush() {
    std::lock_guard<std::mutex> guard(mtx);
    while (!input.empty()) {
        auto it = input.front();
        input.pop();
        cache.push(it);
    }
}

void AlAudioPlayer::stop() {
    AlLogI(TAG, "");
    if (playObject) {
        SLresult result = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
        if (SL_RESULT_SUCCESS != result) {
            AlLogI(TAG, "Player SetPlayState stop failed!");
        }
        (*playObject)->Destroy(playObject);
        playObject = nullptr;
        bufferQueueItf = nullptr;
        playItf = nullptr;
    }
    if (nullptr != mixObject) {
        (*mixObject)->Destroy(mixObject);
        mixObject = nullptr;
    }
    destroyEngine();
    std::lock_guard<std::mutex> guard(mtx);
    while (!input.empty()) {
        input.pop();
    }
    while (!cache.empty()) {
        cache.pop();
    }
}

void AlAudioPlayer::destroyEngine() {
    if (ownEngine && engine) {
        delete engine;
        engine = nullptr;
        ownEngine = false;
    }
}