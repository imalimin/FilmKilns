/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALVIDEOV2PROCESSOR_H
#define HWVC_ANDROID_ALVIDEOV2PROCESSOR_H

#include "AlAbsProcessor.h"
#include "AlMediaTrack.h"
#include "AlEgl.h"

AL_CLASS AlVideoV2Processor AL_EXTEND AlAbsProcessor {
public:
    typedef function<void(int64_t, int64_t)> OnPlayProgressListener;
    typedef function<void(std::shared_ptr<AlMediaTrack>)> OnTrackUpdateListener;

public:
    AlVideoV2Processor();

    virtual ~AlVideoV2Processor();

    virtual void onCreate() override;

    virtual void onDestroy() override;

    int32_t addTrack(AlMediaTrack::kType type, std::string path,
                     int64_t seqInInUS, int64_t seqOutInUS,
                     int64_t trimInInUS, int64_t trimOutInUS);

    void removeTrack(int32_t trackID);

    void start();

    void pause();

    void seek(int64_t timeInUS);

    void setPlayProgressListener(OnPlayProgressListener l);

    void updateWindow(HwWindow *win);

    void setOnTrackUpdateListener(OnTrackUpdateListener l);

private:
    bool _onTimelineInUS(AlMessage *msg);

    bool _onAddTrackDone(AlMessage *msg);

    bool _onRemoveTrackDone(AlMessage *msg);

    bool _onTrackUpdate(AlMessage *msg);

private:
    const std::string ALIAS_TIMELINE = "TIMELINE_OF_VIDEO2";
    const std::string ALIAS_SEQUENCE = "SEQUENCE_OF_VIDEO2";
    const std::string ALIAS_AUDIOS = "SEQUENCE_OF_AUDIOS";
    const std::string ALIAS_SPEAKER = "SEQUENCE_OF_SPEAKER";
    const std::string ALIAS_VIDEOS = "SEQUENCE_OF_VIDEOS";
    const std::string ALIAS_IMAGE = "IMAGE_OF_VIDEO2";
    const std::string ALIAS_SCREEN = "SCREEN_OF_VIDEO2";

    AlEgl *aBaseCtx = nullptr;
    OnPlayProgressListener playProgressListener = nullptr;
    OnTrackUpdateListener trackUpdateListener = nullptr;
    int32_t mCurTrackID = -1;
};


#endif //HWVC_ANDROID_ALVIDEOV2PROCESSOR_H
