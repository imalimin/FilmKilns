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
#include "AlQuad.h"

/// 用于存放图层绘制信息
/// {@link AlCanvasDrawer}根据此信息可以准确在画布上绘制图层
al_class(AlImageLayerDrawModel) {
public:
    AlImageLayerDrawModel();

    AlImageLayerDrawModel(const AlImageLayerDrawModel &o);

    virtual ~AlImageLayerDrawModel();

    void setLayerSize(int32_t width, int32_t height);

    void setLayerSize(AlSize size);

    AlSize getLayerSize();

private:
    AlSize layerSize;
public:
    HwAbsTexture *tex = nullptr;
    float alpha = 1.0f;
    AlMatrix mat;
    AlRectF vertexRectF;
    AlQuad cropQuad;
};


#endif //HWVC_ANDROID_ALIMAGELAYERDRAWMODEL_H
