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
#include "AlOrthMatrix.h"

class AlCanvasDrawer : public HwAbsFilter {
public:
    AlCanvasDrawer();

    virtual ~AlCanvasDrawer();

    void setScale(float scaleX, float scaleY);

    void setRotation(float rotation);

    void setTranslate(float x, float y);

    void setAlpha(float alpha);

private:
    HwProgram *createProgram() override;

    void drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    AlOrthMatrix matrix;
    float alpha = 0.0;
};


#endif //HWVC_ANDROID_ALCANVASDRAWER_H
