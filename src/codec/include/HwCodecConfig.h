/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWCODECCONFIG_H
#define HWVC_ANDROID_HWCODECCONFIG_H

#include "HwConfig.h"
#include <string>

using namespace std;

class HwCodecConfig : public HwConfig {
public:
    static HwCodecConfig *build();

public:
    virtual ~HwCodecConfig();

private:
    HwCodecConfig();

public:
    string path;
    int width = 0, height = 0;
};


#endif //HWVC_ANDROID_HWCODECCONFIG_H
