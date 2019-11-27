/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCANVASDRAWER_H
#define HWVC_ANDROID_ALCANVASDRAWER_H

#include "HwAbsFilter.h"
#include "HwMatrix.h"

class AlCanvasDrawer : public HwAbsFilter {
public:
    AlCanvasDrawer();

    virtual ~AlCanvasDrawer();

    void setScale(float scaleX, float scaleY);

    void setRotation(float rotation);

private:
    HwProgram *createProgram() override;

    void drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    HwMatrix matrix;
};


#endif //HWVC_ANDROID_ALCANVASDRAWER_H
