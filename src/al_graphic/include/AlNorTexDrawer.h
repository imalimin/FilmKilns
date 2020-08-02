/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALNORTEXDRAWER_H
#define HWVC_ANDROID_ALNORTEXDRAWER_H

#include "Object.h"
#include "HwAbsTexture.h"
#include "AlColorFormat.h"
#include "HwYV122RGBAFilter.h"
#include "AlNV12ToRGBAFilter.h"

AL_CLASS AlNorTexDrawer AL_EXTEND Object {
public:
    AlNorTexDrawer();

    virtual ~AlNorTexDrawer();

    void draw(AlBuffer *buf, AlSize *size, AlColorFormat fmt, HwAbsTexture *dst);

private:
    AlNorTexDrawer(const AlNorTexDrawer &o) : Object() {};

    void _drawYUV420P(AlBuffer *buf, AlSize *size, HwAbsTexture *dst);

    void _drawYUV420SP(AlBuffer *buf, AlSize *size, HwAbsTexture *dst);

    void _drawRGBA(AlBuffer *buf, AlSize *size, HwAbsTexture *dst);

private:
    HwAbsFBObject *fbo = nullptr;

    HwAbsTexture *y = nullptr, *u = nullptr, *v = nullptr;
    HwYV122RGBAFilter *yv12Filter = nullptr;
    AlNV12ToRGBAFilter *nv12Filter = nullptr;
};


#endif //HWVC_ANDROID_ALNORTEXDRAWER_H
