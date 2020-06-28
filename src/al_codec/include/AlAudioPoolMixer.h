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
#include <vector>

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

    HwResult put(int32_t clip, HwAudioFrame *f);

    HwResult put(int32_t clip, HwSampleFormat &format, uint8_t *data, int nbSamples);

    HwResult pop(size_t nbSamples, HwAbsMediaFrame **f);

    HwResult remove(int32_t clip);

    int32_t samplesOfTrack(int32_t clip);

    /// Clear select mark.
    void clearSelect();

    /// Mark pop this clip
    /// \param clip id
    void select(int32_t clip);

private:
    AlAudioPoolMixer(const AlAudioPoolMixer &o) : Object(), format(HwSampleFormat::NONE) {};

    HwResult _findFifoOByClip(int32_t clip, AVAudioFifo **fifo);

    HwResult _request(size_t nbSamples);

    void _checkFrame(HwAudioFrame **ff, int32_t nbSamples);

private:
    HwSampleFormat format;
    /// Selected clips.
    std::vector<int32_t> sClips;
    std::map<int32_t, AVAudioFifo *> map;
    HwAudioFrame *frame = nullptr;
    HwAudioFrame *cache = nullptr;
};


#endif //HWVC_ANDROID_ALAUDIOPOOLMIXER_H
