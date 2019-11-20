/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCANVASFILTER_H
#define HWVC_ANDROID_ALCANVASFILTER_H

#include "HwAbsFilter.h"

class AlCanvasFilter : public HwAbsFilter {
public:
    AlCanvasFilter();

    virtual ~AlCanvasFilter();

private:
    HwProgram *createProgram() override;

    void drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;
};


#endif //HWVC_ANDROID_ALCANVASFILTER_H
