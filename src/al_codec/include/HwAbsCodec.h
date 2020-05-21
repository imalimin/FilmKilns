/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSCODEC_H
#define HWVC_ANDROID_HWABSCODEC_H

#include "AlCodec.h"

class HwAbsCodec : public AlCodec {
public:
    HwAbsCodec(AlCodec::kID id);

    virtual  ~HwAbsCodec();
};


#endif //HWVC_ANDROID_HWABSCODEC_H
