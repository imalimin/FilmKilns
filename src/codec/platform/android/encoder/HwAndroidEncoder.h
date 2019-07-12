/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDENCODER_H
#define HWVC_ANDROID_HWANDROIDENCODER_H

#include "../../../include/HwAbsEncoder.h"
#include "../../../include/HwAbsMediaFrame.h"
#include "HwResult.h"
#include "media/NdkMediaCodec.h"

class HwAndroidEncoder : public HwAbsEncoder {
public:
    HwAndroidEncoder();

    virtual ~HwAndroidEncoder();

    virtual bool prepare(string path, int width, int height);

    virtual HwResult write(HwAbsMediaFrame *frame);

    virtual bool stop() override;

    virtual void release() override;

private:
    bool configure();

private:
    AMediaCodec *codec = nullptr;
    int width = 0, height = 0;
};


#endif //HWVC_ANDROID_HWANDROIDENCODER_H
