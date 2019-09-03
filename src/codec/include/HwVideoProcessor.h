/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H
#define HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H

#include "Object.h"
#include "HwAbsProcessor.h"
#include "HwAbsFilter.h"
#include "HwWindow.h"

class HwVideoProcessor : public HwAbsProcessor {
public:
    HwVideoProcessor();

    virtual ~HwVideoProcessor();

    virtual void onDestroy() override;

    void setSource(const string path);

    void prepare(HwWindow *win);

    void start();

    void pause();

    void stop();

    void seek(int64_t us);

    void setFilter(HwAbsFilter *filter);

    void updateWindow(HwWindow *win);

    void setPlayProgressListener(function<void(int64_t, int64_t)> listener);

private:
    const string ALIAS_OF_VIDEO = "VIDEO";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    const string ALIAS_OF_SPEAKER = "SPEAKER";
    HandlerThread *unitHandler = nullptr;
    HandlerThread *screenHandler = nullptr;

    function<void(int64_t, int64_t)> playProgressListener = nullptr;
};


#endif //HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H
