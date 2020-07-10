/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAudioPoolMixer.h"
#include "AlLogcat.h"

#define TAG "AlAudioPoolMixer"

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

HwResult AlAudioPoolMixer::put(int32_t clip, HwAudioFrame *f) {
    if (nullptr == f || clip <= 0) {
        return Hw::FAILED;
    }
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK != _findFifoOByClip(clip, &fifo)) {
        fifo = av_audio_fifo_alloc(
                HwAbsMediaFrame::convertAudioFrameFormat(f->getFormat()),
                f->getChannels(),
                f->getSampleCount() * 3);
        map.insert(std::make_pair(clip, fifo));
    }
    if (av_audio_fifo_space(fifo) < f->getSampleCount() * 3) {
        if (0 != av_audio_fifo_realloc(fifo, f->getSampleCount() * 3)) {
            AlLogW(TAG, "failed.");
        }
    }
    uint8_t *data = f->data();
    av_audio_fifo_write(fifo, reinterpret_cast<void **>(&data), f->getSampleCount());
    return Hw::OK;
}

HwResult AlAudioPoolMixer::put(int32_t clip, HwSampleFormat &format,
                               uint8_t *data, int nbSamples) {
    if (clip <= 0 || nullptr == data || nbSamples <= 0) {
        return Hw::FAILED;
    }
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK != _findFifoOByClip(clip, &fifo)) {
        fifo = av_audio_fifo_alloc(
                HwAbsMediaFrame::convertAudioFrameFormat(format.getFormat()),
                format.getChannels(),
                nbSamples * 3);
        map.insert(std::make_pair(clip, fifo));
    }
    if (av_audio_fifo_space(fifo) < nbSamples * 3) {
        if (0 != av_audio_fifo_realloc(fifo, nbSamples * 3)) {
            AlLogW(TAG, "failed.");
        }
    }
    av_audio_fifo_write(fifo, reinterpret_cast<void **>(&data), nbSamples);
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

HwResult AlAudioPoolMixer::remove(int32_t clip) {
    auto itr = map.find(clip);
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

int32_t AlAudioPoolMixer::samplesOfTrack(int32_t clip) {
    AVAudioFifo *fifo = nullptr;
    if (Hw::OK == _findFifoOByClip(clip, &fifo)) {
        return av_audio_fifo_size(fifo);
    }
    return -1;
}

HwResult AlAudioPoolMixer::_request(size_t nbSamples) {
    if (map.empty()) {
        return Hw::FAILED;
    }
    HwResult ret = Hw::FAILED;
    for (auto itr = map.begin(); map.end() != itr; ++itr) {
        if (sClips.end() == std::find(sClips.begin(), sClips.end(), itr->first)) {
            continue;
        }
        if (av_audio_fifo_size(itr->second) < nbSamples) {
            return Hw::FAILED;
        }
        ret = Hw::OK;
    }
    return ret;
}

HwResult AlAudioPoolMixer::_findFifoOByClip(int32_t clip, AVAudioFifo **fifo) {
    auto itr = map.find(clip);
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

void AlAudioPoolMixer::clearSelect() {
    sClips.clear();
}

void AlAudioPoolMixer::select(int32_t clip) {
    auto itr = std::find(sClips.begin(), sClips.end(), clip);
    if (sClips.end() == itr) {
        sClips.emplace_back(clip);
    }
}
