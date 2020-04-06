/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H
#define HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H

#include "Object.h"
#include "AlAbsProcessor.h"
#include "HwAbsFilter.h"
#include "HwWindow.h"
#include "AlEgl.h"

class HwVideoProcessor : public AlAbsProcessor {
public:
    HwVideoProcessor();

    virtual ~HwVideoProcessor();

    virtual void onCreate() override;

    virtual void onDestroy() override;

    void setSource(const string path);

    void start();

    void pause();

    void stop();

    void seek(int64_t us);

    void setFilter(HwAbsFilter *filter);

    void updateWindow(HwWindow *win);

    void setPlayProgressListener(function<void(int64_t, int64_t)> listener);

private:
    bool _onPlayProgress(AlMessage *msg);

private:
    const string ALIAS_OF_VIDEO = "VIDEO";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    const string ALIAS_OF_SPEAKER = "SPEAKER";

    AlEgl *aBaseCtx = nullptr;
    function<void(int64_t, int64_t)> playProgressListener = nullptr;
};


#endif //HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H
