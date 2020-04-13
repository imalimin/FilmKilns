/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSVIDEOENCODER_H
#define HWVC_ANDROID_HWABSVIDEOENCODER_H

#include "HwAbsEncoder.h"
#include "HwSampleFormat.h"

class HwAbsVideoEncoder : public HwAbsEncoder {
public:
    HwAbsVideoEncoder();

    virtual ~HwAbsVideoEncoder();

    virtual bool prepare(string path, int width, int height, HwSampleFormat audioFormat) = 0;

    virtual void setBitrate(int32_t rate) = 0;

    virtual void setProfile(std::string profile) = 0;
};


#endif //HWVC_ANDROID_HWABSVIDEOENCODER_H
