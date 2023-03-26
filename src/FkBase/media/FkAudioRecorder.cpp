/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAudioRecorder.h"

#define TAG "FkAudioRecorder"

FK_IMPL_CLASS_TYPE(FkAudioRecorder, FkSLAudioDevice)

void bufferDequeueCallback(SLAndroidSimpleBufferQueueItf slBufferQueueItf, void *context) {
    FkAudioRecorder *recorder = static_cast<FkAudioRecorder *>(context);
    recorder->bufferDequeue(slBufferQueueItf);
}

void FkAudioRecorder::bufferDequeue(SLAndroidSimpleBufferQueueItf slBufferQueueItf) {
    if (this->buffer) {
        (*slBufferQueueItf)->Enqueue(slBufferQueueItf, buffer->data(), buffer->size());
        if (fifo) {
            fifo->push(buffer->data(), buffer->size());
        }
    }
}

FkAudioRecorder::FkAudioRecorder(uint16_t channels,
                                 uint32_t sampleRate,
                                 uint16_t format,
                                 uint32_t samplesPerBuffer)
        : FkSLAudioDevice(FkAudioDevice::kMode::Normal,
                        channels,
                        sampleRate,
                        format,
                        samplesPerBuffer) {
    initialize(nullptr);
}

FkAudioRecorder::FkAudioRecorder(FkAudioDevice::kMode mode,
                                 uint16_t channels,
                                 uint32_t sampleRate,
                                 uint16_t format,
                                 uint32_t samplesPerBuffer) : FkSLAudioDevice(mode,
                                                                            channels,
                                                                            sampleRate,
                                                                            format,
                                                                            samplesPerBuffer) {
    initialize(nullptr);
}

FkAudioRecorder::FkAudioRecorder(FkSLEngine *engine,
                                 FkAudioDevice::kMode mode,
                                 uint16_t channels,
                                 uint32_t sampleRate,
                                 uint16_t format,
                                 uint32_t samplesPerBuffer) : FkSLAudioDevice(mode,
                                                                            channels,
                                                                            sampleRate,
                                                                            format,
                                                                            samplesPerBuffer) {
    initialize(engine);
}

void FkAudioRecorder::initialize(FkSLEngine *_engine) {
    this->engine = _engine;
    FkLogI(TAG, "Create FkAudioRecorder, channels=%d, sampleHz=%d",
         this->channels,
         this->sampleRate);
    uint32_t bufSize = getBufferByteSize() * 16;
    switch (mode) {
        case FkAudioDevice::kMode::LowLatency:
            bufSize = getBufferByteSize() * 3;
            break;
        case FkAudioDevice::kMode::Normal:
            bufSize = getBufferByteSize() * 16;
            break;
        case FkAudioDevice::kMode::HighLatency:
            bufSize = getBufferByteSize() * 32;
    }
    this->fifo = new FkFIFOBuffer(bufSize);
    this->buffer = FkBuffer::alloc(getBufferByteSize());
    auto ret = this->createEngine();
    if (FK_OK != ret) {
        FkLogE(TAG, "FkAudioRecorder start failed");
    }

}

FkAudioRecorder::~FkAudioRecorder() {
    FkLogI(TAG, "FkAudioRecorder");
    stop();
}

FkResult FkAudioRecorder::start() {
    auto ret = recorder->stop();
    if (FK_OK != ret) {
        return ret;
    }
    ret = recorder->clear();
    if (FK_OK != ret) {
        return ret;
    }
    bufferDequeue(recorder->getQueue());
    ret = recorder->start();
    if (FK_OK != ret) {
        FkLogE(TAG, "Recorder SetRecordState start failed!");
        return ret;
    }
    return FK_OK;
}

void FkAudioRecorder::stop() {
    if (recorder) {
        auto ret = recorder->stop();
        if (FK_OK != ret) {
            FkLogE(TAG, "Recorder SetRecordState stop failed!");
        }
        delete recorder;
        recorder = nullptr;
    }
    destroyEngine();
    if (buffer) {
        buffer = nullptr;
    }
    if (fifo) {
        delete fifo;
        fifo = nullptr;
    }
}

std::shared_ptr<FkBuffer> FkAudioRecorder::read(size_t size) {
    return read(size, 0);
}

std::shared_ptr<FkBuffer> FkAudioRecorder::read(size_t size, int timeOut) {
    if (!fifo) {
        FkLogE(TAG, "FIFO invalid");
        return 0;
    }
    return fifo->take(size, timeOut);
}

void FkAudioRecorder::flush() {
    if (fifo) {
        fifo->flush();
    }
}

FkResult FkAudioRecorder::createEngine() {
    if (!engine) {
        ownEngine = true;
        engine = new FkSLEngine();
        if (!engine || !engine->valid()) {
            FkLogE(TAG, "AudioPlayer create failed");
            stop();
            return FK_FAIL;
        }
    }
    return createBufferQueueObject();
}

FkResult FkAudioRecorder::createBufferQueueObject() {
    recorder = new FkSLRecorder(engine);
    auto ret = recorder->initialize(this);
    if (FK_OK != ret) {
        return ret;
    }
    ret = recorder->registerCallback(bufferDequeueCallback, this);
    if (FK_OK != ret) {
        return ret;
    }
    return FK_OK;
}

void FkAudioRecorder::destroyEngine() {
    if (ownEngine && engine) {
        delete engine;
        engine = nullptr;
        ownEngine = false;
    }
}