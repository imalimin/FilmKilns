/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCANVASDRAWER_H
#define HWVC_ANDROID_ALCANVASDRAWER_H

#include "HwAbsFilter.h"
#include "AlOrthMatrix.h"
#include "AlRectF.h"
#include "AlSize.h"
#include "AlQuad.h"

class AlCanvasDrawer : public HwAbsFilter {
public:
    AlCanvasDrawer();

    virtual ~AlCanvasDrawer();

    void setAlpha(float alpha);

    void setVertexQuad(AlQuad &quad);

    void setPositionQuad(AlQuad &quad);

private:
    AlAbsGLProgram *createProgram() override;

    void drawStart(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void drawEnd(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void _resetUV();

private:
    AlMatrix matrix;
    float vertex[8];
    float uv[8];
    float alpha = 0.0;
};


#endif //HWVC_ANDROID_ALCANVASDRAWER_H
