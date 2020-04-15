/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_AUDIOPROCESSOR_H
#define HARDWAREVIDEOCODEC_AUDIOPROCESSOR_H

#include "HwAbsProcessor.h"

using namespace std;

class HwAudioProcessor : public HwAbsProcessor {
public:
    HwAudioProcessor();

    virtual ~HwAudioProcessor();

    virtual void onDestroy() override;

    void setSource(const string path);

    void prepare();

    void start();

    void pause();

    void stop();

    void seek(int64_t us);

    void setPlayProgressListener(function<void(int64_t, int64_t)> listener);

private:
    const string ALIAS_OF_AUDIO = "AUDIO";
    const string ALIAS_OF_SPEAKER = "SPEAKER";
    function<void(int64_t, int64_t)> playProgressListener = nullptr;
};


#endif //HARDWAREVIDEOCODEC_AUDIOPROCESSOR_H
