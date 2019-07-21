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
    function<void(Message *msg)> runnable = nullptr;
    int what = 0;
    int arg1 = 0;
    int64_t arg2 = 0;
    string msg;
    Object *obj = nullptr;
    int16_t queueMode = QUEUE_MODE_NORMAL;

    Message(int what, function<void(Message *msg)> runnable);

    Message(int what, Object *obj, function<void(Message *msg)> runnable);

    Message(int what, Object *obj, int16_t queueMode, function<void(Message *msg)> runnable);

    virtual ~Message();

    void *tyrUnBox();

public:
    static constexpr int16_t QUEUE_MODE_NORMAL = 0x00;
    static constexpr int16_t QUEUE_MODE_UNIQUE = 0x01;
    static constexpr int16_t QUEUE_MODE_FIRST_ALWAYS = 0x02;
    static constexpr int16_t QUEUE_MODE_CLEAR = 0x04;
};

#endif //HARDWAREVIDEOCODEC_MESSAGE_H
