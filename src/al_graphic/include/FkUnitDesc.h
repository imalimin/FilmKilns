/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_FKUNITDESC_H
#define HWVC_ANDROID_FKUNITDESC_H

#include "AlParcelable.h"

AL_CLASS FkUnitDesc AL_EXTEND AlParcelable {
public:
    FkUnitDesc();

    ~FkUnitDesc();

    virtual void writeToParcel(std::shared_ptr<AlParcel> parcel) override;

public:
    std::string name;
    std::vector<int64_t> msgVec;
};


#endif //HWVC_ANDROID_FKUNITDESC_H
