/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALVIDEOV2PROCESSOR_H
#define HWVC_ANDROID_ALVIDEOV2PROCESSOR_H

#include "AlAbsProcessor.h"

AL_CLASS AlVideoV2Processor AL_EXTEND AlAbsProcessor {
public:
    AlVideoV2Processor();

    virtual ~AlVideoV2Processor();

    virtual void onCreate() override;

    virtual void onDestroy() override;
};


#endif //HWVC_ANDROID_ALVIDEOV2PROCESSOR_H
