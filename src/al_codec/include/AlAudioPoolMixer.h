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

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libavcodec/jni.h"

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

private:
    AlAudioPoolMixer(const AlAudioPoolMixer &o) : Object() {};
private:
};


#endif //HWVC_ANDROID_ALAUDIOPOOLMIXER_H
