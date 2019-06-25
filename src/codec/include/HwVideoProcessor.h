/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H
#define HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H

#include "Object.h"
#include "UnitPipeline.h"
#include "Filter.h"
#include "HwWindow.h"

class HwVideoProcessor : public Object {
public:
    HwVideoProcessor();

    virtual ~HwVideoProcessor();

    void setSource(char *path);

    void prepare(HwWindow *win);

    void start();

    void pause();

    void stop();

    void seek(int64_t us);

    void setFilter(Filter *filter);

    void updateWindow(HwWindow *win);

private:
    UnitPipeline *pipeline = nullptr;
    HandlerThread *unitHandler = nullptr;
    HandlerThread *screenHandler = nullptr;
};


#endif //HARDWAREVIDEOCODEC_VIDEOPROCESSOR_H
