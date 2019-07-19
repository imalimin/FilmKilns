/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWMICROPHONEPROCESSOR_H
#define HWVC_ANDROID_HWMICROPHONEPROCESSOR_H

#include "HwAbsProcessor.h"
#include "HwSampleFormat.h"

class HwMicrophoneProcessor : public HwAbsProcessor {
public:
    HwMicrophoneProcessor();

    ~HwMicrophoneProcessor();

    void prepare(string filePath, HwSampleFormat format);

    void start();

    void pause();

protected:
    HwAbsPipelineModel *createModel() override;

private:
    HwSampleFormat format = HwSampleFormat::NONE;
};


#endif //HWVC_ANDROID_HWMICROPHONEPROCESSOR_H
