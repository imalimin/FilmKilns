/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAudioPoolMixer.h"

AlAudioPoolMixer::AlAudioPoolMixer(const HwSampleFormat &format)
        : Object(),
          format(format) {
}

AlAudioPoolMixer::~AlAudioPoolMixer() {
    delete frame;
    frame = nullptr;
    delete cache;
    cache = nullptr;
    for (auto itr = map.begin(); map.end() != itr; ++itr) {
        av_audio_fifo_free(itr->second);
    }
    map.clear();
}

HwResult AlAudioPoolMixer::put(int32_t track, HwAudioFrame *f) {
    if (nullptr == f || track <= 0) {
        return Hw::FAILED;
    }
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK != _findFifoOByTrack(track, &fifo)) {
        fifo = av_audio_fifo_alloc(
                HwAbsMediaFrame::convertAudioFrameFormat(f->getFormat()),
                f->getChannels(),
                f->getSampleCount() * 3);
        map.insert(std::make_pair(track, fifo));
    }
    uint8_t *data = f->data();
    av_audio_fifo_write(fifo, reinterpret_cast<void **>(&data), f->getSampleCount());
    return Hw::OK;
}

HwResult AlAudioPoolMixer::pop(size_t nbSamples, HwAbsMediaFrame **f) {
    if (Hw::OK != _request(nbSamples)) {
        return Hw::FAILED;
    }
    _checkFrame(&frame, nbSamples);
    _checkFrame(&cache, nbSamples);
    int32_t len = this->frame->size();
    memset(this->frame->data(), 0, len);
    auto *data = this->cache->data();
    for (auto itr = map.begin(); map.end() != itr; ++itr) {
        av_audio_fifo_read(itr->second, reinterpret_cast<void **>(&data), nbSamples);
        for (int i = 0; i < len; ++i) {
            this->frame->data()[i] += data[i];
        }
    }
    *f = this->frame;
    return Hw::OK;
}

HwResult AlAudioPoolMixer::remove(int32_t track) {
    auto itr = map.find(track);
    if (map.end() == itr) {
        return Hw::FAILED;
    }
    auto *fifo = itr->second;
    if (nullptr == fifo) {
        return Hw::FAILED;
    }
    av_audio_fifo_free(fifo);
    map.erase(itr);
    return Hw::OK;
}

int32_t AlAudioPoolMixer::samplesOfTrack(int32_t track) {
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK == _findFifoOByTrack(track, &fifo)) {
        return av_audio_fifo_size(fifo);
    }
    return -1;
}

HwResult AlAudioPoolMixer::_request(size_t nbSamples) {
    if (map.empty()) {
        return Hw::FAILED;
    }
    for (auto itr = map.begin(); map.end() != itr; ++itr) {
        if (av_audio_fifo_size(itr->second) < nbSamples) {
            return Hw::FAILED;
        }
    }
    return Hw::OK;
}

HwResult AlAudioPoolMixer::_findFifoOByTrack(int32_t track, AVAudioFifo **fifo) {
    auto itr = map.find(track);
    if (map.end() == itr) {
        return Hw::FAILED;
    }
    auto *tmp = itr->second;
    if (nullptr == fifo) {
        return Hw::FAILED;
    }
    *fifo = tmp;
    return Hw::OK;
}

void AlAudioPoolMixer::_checkFrame(HwAudioFrame **ff, int32_t nbSamples) {
    if (nullptr == ff) {
        return;
    }
    size_t len = format.getBytesPerSample() * nbSamples;
    if (nullptr == (*ff) || len != (*ff)->size()) {
        delete (*ff);
        *ff = new HwAudioFrame(format.getFormat(),
                               format.getChannels(),
                               format.getSampleRate(),
                               nbSamples);
    }
}
