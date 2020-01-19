/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPAINTFILTER_H
#define HWVC_ANDROID_ALPAINTFILTER_H

#include "HwAbsFilter.h"
#include "AlPointF.h"
#include "AlColor.h"
#include "AlPaintRoundFilter.h"
#include <vector>

al_class_ex(AlPaintFilter, HwAbsFilter) {
public:
    AlPaintFilter();

    ~AlPaintFilter();

    bool prepare() override;

    void setPath(std::vector<float> *vec, bool clear = false);

    void setPaintSize(float size);

    void setColor(AlColor color);

    AlAbsGLProgram *createProgram() override;

    void draw(HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    AlPaintFilter(const AlPaintFilter &o) : HwAbsFilter() {}

    void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void drawEnd(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    HwAbsTexture *roundTex = nullptr;
    int32_t uSize = -1;
    int32_t uColor = -1;
    float paintSize = 0.0f;
    AlColor color = AlColor(0xffffff);
    std::vector<float> path;
};


#endif //HWVC_ANDROID_ALPAINTFILTER_H
