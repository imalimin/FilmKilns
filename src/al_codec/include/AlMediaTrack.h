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
#include "AlAbsInputDescriptor.h"
#include <vector>

AL_CLASS AlMediaTrack AL_EXTEND Object {
public:
    AlMediaTrack();

    AlMediaTrack(const AlMediaTrack &o);

    virtual ~AlMediaTrack();

private:
    std::vector<std::unique_ptr<AlMediaClip>> clips;
    int32_t _id = INT32_MIN;
    std::shared_ptr<AlAbsInputDescriptor> iDescriptor;
};


#endif //HWVC_ANDROID_ALMEDIATRACK_H
