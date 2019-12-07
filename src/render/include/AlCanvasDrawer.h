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
#include "AlRectF.h"
#include "AlSize.h"
#include "AlQuad.h"

class AlCanvasDrawer : public HwAbsFilter {
public:
    AlCanvasDrawer();

    virtual ~AlCanvasDrawer();

    void setAlpha(float alpha);

    void setMatrix(AlMatrix &matrix);

    void setVertexRectF(AlRectF &rectF);

    void setPositionQuad(AlQuad &quad);

private:
    HwProgram *createProgram() override;

    void drawStart(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void drawEnd(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void _resetUV();

private:
    AlRectF sRectF;
    AlRectF dRectF;
    float vertex[8];
    float uv[8];
    AlMatrix matrix;
    float alpha = 0.0;
};


#endif //HWVC_ANDROID_ALCANVASDRAWER_H
