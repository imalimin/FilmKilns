/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALQUADDRAWER_H
#define HWVC_ANDROID_ALQUADDRAWER_H

#include "HwNormalFilter.h"
#include "AlTexAllocator.h"
#include "AlQuad.h"

al_class AlQuadDrawer al_extend HwNormalFilter {
public:
    AlQuadDrawer();

    virtual ~AlQuadDrawer();

    bool prepare(AlTexAllocator *texAllocator);

    void setQuad(AlQuad &quad);

    void draw(HwAbsTexture *dest);

private:
    AlQuadDrawer(const AlQuadDrawer &o) : HwNormalFilter() {};

    void drawStart(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void drawEnd(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    HwAbsTexture *cover = nullptr;
    float vertex[8];
};


#endif //HWVC_ANDROID_ALQUADDRAWER_H
