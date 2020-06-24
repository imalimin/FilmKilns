/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAudioPoolMixer.h"

AlAudioPoolMixer::AlAudioPoolMixer()
        : Object() {

}

AlAudioPoolMixer::~AlAudioPoolMixer() {
    for (auto itr = map.begin(); map.end() != itr; ++itr) {
        av_audio_fifo_free(itr->second);
    }
    map.clear();
}


HwResult AlAudioPoolMixer::put(int32_t track, HwAudioFrame *frame) {
    if (nullptr == frame || track <= 0) {
        return Hw::FAILED;
    }
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK != _findFIFOByTrack(track, &fifo)) {
        fifo = av_audio_fifo_alloc(
                HwAbsMediaFrame::convertAudioFrameFormat(frame->getFormat()),
                frame->getChannels(), frame->getSampleCount() * 3);
        map.insert(std::make_pair(track, fifo));
    }
    uint8_t *data = frame->data();
    av_audio_fifo_write(fifo, reinterpret_cast<void **>(&data), frame->getSampleCount());
    return Hw::OK;
}

HwResult AlAudioPoolMixer::request(size_t nbSamples) {
    for (auto itr = map.begin(); map.end() != itr; ++itr) {
        if (av_audio_fifo_size(itr->second) < nbSamples) {
            return Hw::FAILED;
        }
    }
    return Hw::OK;
}

HwResult AlAudioPoolMixer::pop(HwAbsMediaFrame **frame) {
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
    map.emplace(itr);
    return Hw::OK;
}

size_t AlAudioPoolMixer::samplesOfTrack(int32_t track) {
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK == _findFIFOByTrack(track, &fifo)) {
        return av_audio_fifo_size(fifo);
    }
    return -1;
}

HwResult AlAudioPoolMixer::_findFIFOByTrack(int32_t track, AVAudioFifo **fifo) {
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
