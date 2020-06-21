/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIACLIP_H
#define HWVC_ANDROID_ALMEDIACLIP_H

#include "Object.h"
#include "AlFileDescriptor.h"

AL_CLASS AlMediaClip AL_EXTEND Object {
public:
    AlMediaClip(int32_t id, const AlFileDescriptor &descriptor);

    AlMediaClip(const AlMediaClip &o);

    virtual ~AlMediaClip();

    int32_t id();

    void setSeqIn(int64_t timeInUS);

    int64_t getSeqIn();

    int64_t getSeqOut();

    void setTrimIn(int64_t timeInUS);

    int64_t getTrimIn();

    int64_t getTrimOut();

    void setDuration(int64_t timeInUS);

    int64_t getDuration();

    void setFrameDuration(int64_t timeInUS);

    int64_t getFrameDuration();

    std::shared_ptr<AlFileDescriptor> getInputDescriptor();

private:
    int32_t _id = INT32_MIN;
    std::shared_ptr<AlFileDescriptor> iDescriptor;
    int64_t seqIn = 0;
    int64_t trimIn = 0;
    int64_t duration = 0;
    int64_t frameDuration = 0;
};


#endif //HWVC_ANDROID_ALMEDIACLIP_H
