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
#include "AlCodec.h"
#include "AlIdentityCreator.h"
#include "AlVector.h"
#include <map>

AL_CLASS AlMediaTrack AL_EXTEND Object {
public:
    AlMediaTrack(AlID id, AlCodec::kMediaType type);

    AlMediaTrack(const AlMediaTrack &o);

    virtual ~AlMediaTrack();

    AlID id();

    int64_t getSeqIn();

    int64_t getSeqOut();

    int64_t getDuration();

    AlID addClip(AlID id, const AlAbsInputDescriptor &o);

    AlMediaClip *findClip(AlID id);

    size_t findClips(AlVector<std::shared_ptr<AlMediaClip>> &array, int64_t timeInUS);

private:
    std::map<AlID, std::unique_ptr<AlMediaClip>> clips;
    AlID _id = AlIdentityCreator::NONE_ID;
    AlCodec::kMediaType _type;
};


#endif //HWVC_ANDROID_ALMEDIATRACK_H
