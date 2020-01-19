/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPAINTROUNDFILTER_H
#define HWVC_ANDROID_ALPAINTROUNDFILTER_H

#include "HwAbsFilter.h"

al_class_ex(AlPaintRoundFilter, HwAbsFilter) {
public:
    AlPaintRoundFilter();

    ~AlPaintRoundFilter();

    AlAbsGLProgram *createProgram() override;

    void draw(HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    AlPaintRoundFilter(const AlPaintRoundFilter &o) : HwAbsFilter() {}

    void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    int32_t uRadius;
    float radius = 0.0f;
};


#endif //HWVC_ANDROID_ALPAINTROUNDFILTER_H
