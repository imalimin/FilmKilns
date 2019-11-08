/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALHANDLER_H
#define HWVC_ANDROID_ALHANDLER_H

#include "Object.h"
#include "AlLooper.h"

al_class(AlHandler) {
public:
    typedef function<void(AlMessage *msg)> Callback;
public:
    AlHandler(Callback callback);

    AlHandler(AlLooper *looper, Callback callback);

    ~AlHandler();

    void sendMessage(AlMessage *msg);

    void dispatchMessage(AlMessage *msg);

private:
    void _enqueueMessage(AlMessage *msg);

private:
    AlLooper *looper = nullptr;
    Callback callback = nullptr;
};


#endif //HWVC_ANDROID_ALHANDLER_H
