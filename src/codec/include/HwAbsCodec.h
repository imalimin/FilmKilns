/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSCODEC_H
#define HWVC_ANDROID_HWABSCODEC_H

#include "Object.h"
#include "HwResult.h"

class HwAbsCodec : public Object {
public:
    HwAbsCodec(int32_t codecId);

    virtual  ~HwAbsCodec();

    virtual HwResult start()=0;

protected:
    int32_t codecId = INT32_MIN;
};


#endif //HWVC_ANDROID_HWABSCODEC_H
