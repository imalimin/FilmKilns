/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_FKMSGSTATE_H
#define HWVC_ANDROID_FKMSGSTATE_H

#include "AlParcelable.h"

AL_CLASS FkMsgState AL_EXTEND AlParcelable {
public:
    FkMsgState();

    ~FkMsgState();

    virtual void writeToParcel(std::shared_ptr<AlParcel> parcel) override;

    void copyTo(std::shared_ptr<FkMsgState> o);

public:
    int64_t what = 0;
    int64_t postTimeInUS = 0;
    int64_t handleTimeInUS = 0;
    int64_t handleDoneInUS = 0;
    int64_t costTimeInUS = 0;
};


#endif //HWVC_ANDROID_FKMSGSTATE_H
