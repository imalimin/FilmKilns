/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWYV122RGBAFILTER_H
#define HWVC_ANDROID_HWYV122RGBAFILTER_H

#include "HwAbsFilter.h"

class HwYV122RGBAFilter : public HwAbsFilter {
public:
    HwYV122RGBAFilter();

    virtual ~HwYV122RGBAFilter();

    virtual HwProgram *createProgram() override;

    virtual void drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    virtual void draw(HwAbsTexture *y, HwAbsTexture *u, HwAbsTexture *v, HwAbsTexture *dest);

private:
    HwAbsTexture *uTexture = nullptr;
    HwAbsTexture *vTexture = nullptr;
    int32_t uLocation = -1;
    int32_t vLocation = -1;
};


#endif //HWVC_ANDROID_HWYV122RGBAFILTER_H
