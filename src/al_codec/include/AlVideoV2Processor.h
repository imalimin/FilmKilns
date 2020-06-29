/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALVIDEOV2PROCESSOR_H
#define HWVC_ANDROID_ALVIDEOV2PROCESSOR_H

#include "AlAbsProcessor.h"
#include "AlCodec.h"

AL_CLASS AlVideoV2Processor AL_EXTEND AlAbsProcessor {
public:
    AlVideoV2Processor();

    virtual ~AlVideoV2Processor();

    virtual void onCreate() override;

    virtual void onDestroy() override;

    int32_t addTrack(AlCodec::kMediaType type, std::string path,
                     int64_t seqInInUS, int64_t seqOutInUS,
                     int64_t trimInInUS, int64_t trimOutInUS);

    void start();

    void pause();

    void seek(int64_t timeInUS);

    void setPlayProgressListener(function<void(int64_t, int64_t)> listener);

private:
    bool _onTimelineInUS(AlMessage *msg);

private:
    const std::string ALIAS_TIMELINE = "TIMELINE_OF_VIDEO2";
    const std::string ALIAS_SEQUENCE = "SEQUENCE_OF_VIDEO2";
    const std::string ALIAS_AUDIOS = "SEQUENCE_OF_AUDIOS";
    const std::string ALIAS_SPEAKER = "SEQUENCE_OF_SPEAKER";

    function<void(int64_t, int64_t)> playProgressListener = nullptr;
};


#endif //HWVC_ANDROID_ALVIDEOV2PROCESSOR_H
