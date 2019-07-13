/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSENCODER_H
#define HWVC_ANDROID_HWABSENCODER_H

#include "Object.h"
#include <string>
#include "HwResult.h"
#include "HwAbsMediaFrame.h"

using namespace std;

class HwAbsEncoder : public Object {
public:
    HwAbsEncoder();

    virtual ~HwAbsEncoder();

    virtual bool prepare(string path, int width, int height) = 0;

    virtual HwResult write(HwAbsMediaFrame *frame) = 0;

    virtual bool stop() = 0;

    virtual void release() = 0;

};


#endif //HWVC_ANDROID_HWABSENCODER_H
