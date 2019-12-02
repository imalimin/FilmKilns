/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYER_H
#define HWVC_ANDROID_ALIMAGELAYER_H

#include "Object.h"
#include "AlImageCanvas.h"
#include "HwAbsTexture.h"
#include "AlCanvasDrawer.h"
#include "AlImageLayerModel.h"

class AlImageLayerManager;

al_class(AlImageLayer) {
public:
public:
    static AlImageLayer *create(AlImageLayerModel *model, HwAbsTexture *tex);

public:
    ~AlImageLayer();

    void draw(AlImageCanvas *canvas);

    int32_t getWidth();

    int32_t getHeight();

private:
    AlImageLayer(AlImageLayerModel *model, HwAbsTexture *tex);

    AlImageLayer(const AlImageLayer &e) : Object() {};

    void _draw(AlImageCanvas *canvas);

    void _applyParams(AlSize &dest);

    void _setScale(float scaleX, float scaleY);

    void _setRotation(float rotation);

    void _setTranslate(float x, float y, AlRectF rectF);

    /// 计算正交矩阵
    /// \param src 图层大小
    /// \param dest 窗口大小
    void _calculateRect(AlSize &src, AlSize &dest, AlRectF &srcRectF, AlRectF &destRectF);

private:
    AlImageLayerModel *model = nullptr;
    //Manager by TextureAllocator. DON`T delete here.
    friend AlImageLayerManager;
    HwAbsTexture *tex = nullptr;
    AlCanvasDrawer *mCanvasDrawer = nullptr;
    ///用于变换操作的矩阵
    ///Orthogonal matrix
    AlOrthMatrix oMat;
    ///Translate matrix
    AlMatrix tMat;
};


#endif //HWVC_ANDROID_ALIMAGELAYER_H
