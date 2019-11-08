/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <functional>
#include <string>
#include "Object.h"

using namespace std;
#ifndef HARDWAREVIDEOCODEC_MESSAGE_H
#define HARDWAREVIDEOCODEC_MESSAGE_H

class Message : public Object {
public:
    typedef function<void(Message *msg)> Runnable;
public:
    Runnable runnable = nullptr;
    int32_t what = 0;
    int32_t arg1 = 0;
    int64_t arg2 = 0;
    string desc;
    Object *obj = nullptr;
    int16_t queueMode = QUEUE_MODE_NORMAL;

    Message(int32_t what, Runnable runnable);

    Message(int32_t what, Object *obj, Runnable runnable);

    Message(int32_t what, Object *obj, int16_t queueMode, Runnable runnable);

    virtual ~Message();

    void *tyrUnBox();

public:
    static constexpr int16_t QUEUE_MODE_NORMAL = 0x00;
    static constexpr int16_t QUEUE_MODE_UNIQUE = 0x01;
    static constexpr int16_t QUEUE_MODE_FIRST_ALWAYS = 0x02;
    static constexpr int16_t QUEUE_MODE_CLEAR = 0x04;
};

#endif //HARDWAREVIDEOCODEC_MESSAGE_H
