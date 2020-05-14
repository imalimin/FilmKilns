/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALEVENTPIPELINE_H
#define HWVC_ANDROID_ALEVENTPIPELINE_H

#include "Object.h"
#include "AlHandler.h"
#include "AlHandlerThread.h"

al_class AlEventPipeline al_extend Object {
public:
    static AlEventPipeline *create(std::string name);

    static AlEventPipeline *create(AlLooper *looper);

private:
    AlEventPipeline(std::string name);

    AlEventPipeline(AlLooper *looper);

    AlEventPipeline(AlEventPipeline &e) : Object() {}

public:
    ~AlEventPipeline();

    void queueEvent(function<void()> func);

    void quit();

private:
    AlHandler *mHandler;
    AlHandlerThread *mThread = nullptr;
    std::atomic_bool exited;
};


#endif //HWVC_ANDROID_ALEVENTPIPELINE_H
