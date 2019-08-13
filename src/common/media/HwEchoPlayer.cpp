/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwEchoPlayer.h"

HwEchoPlayer::HwEchoPlayer(int channels, int sampleHz, int format, int samplesPerBuffer) {
    this->samplesPerBuffer = samplesPerBuffer;
    this->buffer = new uint8_t[minBufferSize];
    this->engine = new SLEngine();
    recorder = new HwAudioRecorder(engine,
                                   HwAudioDeviceMode::Normal,
                                   channels,
                                   sampleHz,
                                   format,
                                   samplesPerBuffer);
    player = new HwAudioPlayer(engine,
                               HwAudioDeviceMode::Normal,
                               channels,
                               sampleHz,
                               format,
                               samplesPerBuffer);
    this->minBufferSize = player->getBufferByteSize();
    this->pipeline = new EventPipeline("HwEchoPlayer");
}

HwEchoPlayer::~HwEchoPlayer() {
    stop();
}

void HwEchoPlayer::start() {
    running = true;
    if (player) {
        player->start();
    }
    if (recorder) {
        recorder->start();
    }
    loop();
}

void HwEchoPlayer::stop() {
    if (!running) {
        return;
    }
    simpleLock.lock();
    running = false;
    simpleLock.unlock();
    if (pipeline) {
        delete pipeline;
        pipeline = nullptr;
    }
    if (recorder) {
        recorder->stop();
        delete recorder;
        recorder = nullptr;
    }
    if (player) {
        player->stop();
        delete player;
        player = nullptr;
    }
    if (engine) {
        delete engine;
        engine = nullptr;
    }
    if (buffer) {
        delete[] buffer;
        buffer = nullptr;
    }
    minBufferSize = 0;
}

void HwEchoPlayer::loop() {
    if (!running) {
        return;
    }
    pipeline->queueEvent([this] {
        HwBuffer *buffer = recorder->read(minBufferSize);
        if (player && buffer) {
//            Logcat::i("HWVC", "Echo write %d", buffer->size());
            player->write(buffer->data(), buffer->size());
        }
        delete buffer;
        this->loop();
    });
}
