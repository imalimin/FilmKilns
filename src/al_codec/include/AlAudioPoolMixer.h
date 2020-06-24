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
    AlAudioPoolMixer();

    virtual ~AlAudioPoolMixer();

    HwResult put(int32_t track, HwAudioFrame *frame);

    HwResult request(size_t nbSamples);

    HwResult pop(HwAbsMediaFrame **frame);

    HwResult remove(int32_t track);

    size_t samplesOfTrack(int32_t track);

private:
    AlAudioPoolMixer(const AlAudioPoolMixer &o) : Object() {};

    HwResult _findFIFOByTrack(int32_t track, AVAudioFifo **fifo);

private:
    std::map<int32_t, AVAudioFifo *> map;
};


#endif //HWVC_ANDROID_ALAUDIOPOOLMIXER_H
