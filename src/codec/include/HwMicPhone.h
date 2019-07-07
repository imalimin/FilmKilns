/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWMICPHONE_H
#define HWVC_ANDROID_HWMICPHONE_H

#include "Unit.h"

class HwMicPhone : public Unit {
public:
    HwMicPhone();

    virtual ~HwMicPhone();

    bool eventPrepare(Message *msg);

    bool eventRelease(Message *msg) override;
};


#endif //HWVC_ANDROID_HWMICPHONE_H
