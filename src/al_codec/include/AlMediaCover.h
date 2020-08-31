/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIACOVER_H
#define HWVC_ANDROID_ALMEDIACOVER_H

#include "AlParcelable.h"

AL_CLASS AlMediaCover AL_EXTEND AlParcelable {
public:
    AlMediaCover(int32_t width, int32_t height);

    virtual ~AlMediaCover();

    virtual void writeToParcel(std::shared_ptr<AlParcel> parcel) override ;

    void setTimestamp(int64_t timeInUS);

private:
    int64_t timestamp = 0;
    int32_t width = 0;
    int32_t height = 0;
    AlBuffer *buf = nullptr;
};


#endif //HWVC_ANDROID_ALMEDIACOVER_H
