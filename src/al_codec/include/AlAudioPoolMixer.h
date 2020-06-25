/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALAUDIOPOOLMIXER_H
#define HWVC_ANDROID_ALAUDIOPOOLMIXER_H

#include "Object.h"
#include "HwResult.h"
#include "HwAudioFrame.h"
#include "HwSampleFormat.h"
#include <map>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavutil/audio_fifo.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>

#ifdef __cplusplus
}
#endif

AL_CLASS AlAudioPoolMixer AL_EXTEND Object {
public:
    AlAudioPoolMixer(const HwSampleFormat &format);

    virtual ~AlAudioPoolMixer();

    HwResult put(int32_t track, HwAudioFrame *f);

    HwResult pop(size_t nbSamples, HwAbsMediaFrame **f);

    HwResult remove(int32_t track);

    int32_t samplesOfTrack(int32_t track);

private:
    AlAudioPoolMixer(const AlAudioPoolMixer &o) : Object(), format(HwSampleFormat::NONE) {};

    HwResult _findFifoOByTrack(int32_t track, AVAudioFifo **fifo);

    HwResult _request(size_t nbSamples);

    void _checkFrame(HwAudioFrame **ff, int32_t nbSamples);

private:
    HwSampleFormat format;
    std::map<int32_t, AVAudioFifo *> map;
    HwAudioFrame *frame = nullptr;
    HwAudioFrame *cache = nullptr;
};


#endif //HWVC_ANDROID_ALAUDIOPOOLMIXER_H
