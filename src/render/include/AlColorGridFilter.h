/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCOLORGRIDFILTER_H
#define HWVC_ANDROID_ALCOLORGRIDFILTER_H

#include "HwAbsFilter.h"
#include "AlSize.h"

class AlColorGridFilter : public HwAbsFilter {
public:
    static AlColorGridFilter *create();

public:
    ~AlColorGridFilter();

    bool prepare(HwAbsTexture *tex);

    void draw(HwAbsTexture *dest);

private:
    AlColorGridFilter();

    AlColorGridFilter(const AlColorGridFilter &o) : HwAbsFilter() {};

    bool prepare() override;

    AlAbsGLProgram *createProgram() override;

    void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void draw(HwAbsTexture *src, HwAbsTexture *dest) override;

    void _update(AlSize &canvasSize);

private:
    HwAbsTexture *srcTex = nullptr;
    static constexpr int GRID_SIZE = 24;
    AlSize canvasSize = AlSize();
    int32_t size = 0;
};


#endif //HWVC_ANDROID_ALCOLORGRIDFILTER_H
