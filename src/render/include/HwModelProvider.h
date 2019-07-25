/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWMODELPROVIDER_H
#define HWVC_ANDROID_HWMODELPROVIDER_H

#include "Unit.h"
#include "HwBundle.h"

class HwModelProvider : public Unit {
public:
    HwModelProvider();

    virtual ~HwModelProvider();

    bool eventRelease(Message *msg) override;

    bool eventPutInt32(Message *msg);

    bool eventPutInt64(Message *msg);

    bool eventPutString(Message *msg);

    bool eventPutObject(Message *msg);

    int32_t getInt32(string key);

    int64_t getInt64(string key);

    string getString(string key);

    Object *getObject(string key);

public:
    static const int EVENT_PUT_INT32;
    static const int EVENT_PUT_INT64;
    static const int EVENT_PUT_STRING;
    static const int EVENT_PUT_OBJECT;

private:
    HwBundle bundle;
};


#endif //HWVC_ANDROID_HWMODELPROVIDER_H
