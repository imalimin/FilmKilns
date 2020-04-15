/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWCONFIG_H
#define HWVC_ANDROID_HWCONFIG_H

#include "Object.h"

class HwConfig : public Object {
public:
    HwConfig();

    virtual ~HwConfig();

private:
    int width = 0;
    int height = 0;
};


#endif //HWVC_ANDROID_HWCONFIG_H
