/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIACOVERSEQENCE_H
#define HWVC_ANDROID_ALMEDIACOVERSEQENCE_H

#include "AlParcelable.h"
#include "AlMediaCover.h"
#include <vector>

AL_CLASS AlMediaCoverSequence AL_EXTEND AlParcelable {
public:
    AlMediaCoverSequence();

    virtual ~AlMediaCoverSequence();

    virtual void writeToParcel(std::shared_ptr<AlParcel> parcel) override ;

private:
    std::vector<std::shared_ptr<AlMediaCover>> covers;
};


#endif //HWVC_ANDROID_ALMEDIACOVERSEQENCE_H
