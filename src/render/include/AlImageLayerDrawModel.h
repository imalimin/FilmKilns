/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYERDRAWMODEL_H
#define HWVC_ANDROID_ALIMAGELAYERDRAWMODEL_H

#include "Object.h"
#include "AlSize.h"
#include "AlMatrix.h"
#include "AlRectF.h"
#include "HwAbsTexture.h"

al_class(AlImageLayerDrawModel) {
public:
    AlImageLayerDrawModel(AlSize canvasSize);

    AlImageLayerDrawModel(const AlImageLayerDrawModel &o);

    virtual ~AlImageLayerDrawModel();

    AlSize getCanvasSize();

private:
    AlSize canvasSize;
public:
    HwAbsTexture *tex = nullptr;
    float alpha = 1.0f;
    AlMatrix mat;
    AlRectF vertexRectF;
};


#endif //HWVC_ANDROID_ALIMAGELAYERDRAWMODEL_H
