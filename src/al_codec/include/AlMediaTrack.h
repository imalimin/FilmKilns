/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIATRACK_H
#define HWVC_ANDROID_ALMEDIATRACK_H

#include "Object.h"
#include "AlMediaClip.h"
#include "AlIdentityCreator.h"
#include "AlVector.h"
#include <map>

AL_CLASS AlMediaTrack AL_EXTEND Object {
public:
    AL_ENUM kType : int {
        UNKNOWN = -1,
        VIDEO,
        AUDIO,
        VIDEO_REF,
        AUDIO_REF
    };
public:
    AlMediaTrack(AlID id, AlMediaTrack::kType type);

    AlMediaTrack(const AlMediaTrack &o);

    virtual ~AlMediaTrack();

    AlID id();

    AlMediaTrack::kType type();

    int64_t getSeqIn();

    int64_t getSeqOut();

    int64_t getDuration();

    int64_t getMinFrameDuration();

    AlID addClip(AlID id, const AlFileDescriptor &o);

    AlMediaClip *findClip(AlID id);

    /// Find clips by time in us.
    /// \param array clips copy.
    /// \param timeInUS
    /// \return
    size_t findClips(AlVector<std::shared_ptr<AlMediaClip>> &array, int64_t timeInUS);

    /// Find all clips.
    /// \param array clips copy.
    /// \return
    size_t findAllClips(AlVector<std::shared_ptr<AlMediaClip>> &array);

private:
    std::map<AlID, std::unique_ptr<AlMediaClip>> clips;
    AlID _id = AlIdentityCreator::NONE_ID;
    AlMediaTrack::kType _type;
};


#endif //HWVC_ANDROID_ALMEDIATRACK_H
