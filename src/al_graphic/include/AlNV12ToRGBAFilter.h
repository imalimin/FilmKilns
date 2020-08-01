/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALNV12TORGBAFILTER_H
#define HWVC_ANDROID_ALNV12TORGBAFILTER_H

#include "HwAbsFilter.h"

AL_CLASS AlNV12ToRGBAFilter AL_EXTEND HwAbsFilter {
public:
    AlNV12ToRGBAFilter();

    virtual ~AlNV12ToRGBAFilter();

    virtual AlAbsGLProgram *createProgram() override;

    virtual void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    virtual void draw(HwAbsTexture *y, HwAbsTexture *uv, HwAbsTexture *dest);

private:
    HwAbsTexture *uvTexture = nullptr;
    int32_t uvLocation = -1;
    int32_t uStrideOffset = -1;

};


#endif //HWVC_ANDROID_ALNV12TORGBAFILTER_H
