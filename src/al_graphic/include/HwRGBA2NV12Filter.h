/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWRGBA2NV12FILTER_H
#define HWVC_ANDROID_HWRGBA2NV12FILTER_H

#include "HwAbsFilter.h"


/// YUV Format
/// +------------------------------------------+
/// |    YUV420P(3Plane)   |  YUV420SP2Plane)  |
/// +------------------------------------------+
/// | YU12/I420   |  YV12  |   NV12   |  NV21  |
/// +------------------------------------------+
/// |    Y+U+V    | Y+V+U  |   Y+UV   |  Y+VU  |
/// +------------------------------------------+
class HwRGBA2NV12Filter : public HwAbsFilter {
public:
    HwRGBA2NV12Filter();

    virtual ~HwRGBA2NV12Filter();

    virtual AlAbsGLProgram *createProgram() override;

    virtual void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    int32_t widthLocation = 0;
};


#endif //HWVC_ANDROID_HWRGBA2NV12FILTER_H
