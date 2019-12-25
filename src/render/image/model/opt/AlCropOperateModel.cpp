/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCropOperateModel.h"
#include "AlMath.h"
#include "AlVec4.h"
#include "AlOrthMatrix.h"
#include "Logcat.h"
#include "AlPositionTranslator.h"

#define TAG "AlCropOperateModel"

AlCropOperateModel::AlCropOperateModel() : AlAbsOperateModel() {
    invalidate = true;
}

AlCropOperateModel::AlCropOperateModel(const AlCropOperateModel &o)
        : AlAbsOperateModel(o), rectF(o.rectF) {

}

AlCropOperateModel::~AlCropOperateModel() {
    invalidate = false;
}

void AlCropOperateModel::setRect(float left, float top, float right, float bottom) {
    this->rectF.set(left, top, right, bottom);
    invalidate = true;
}

HwResult AlCropOperateModel::measure(AlImgLayerDescription &layer,
                                     AlImageLayerDrawModel *description) {
    if (invalidate) {
        invalidate = false;
        AlSize layerSize = layer.getSize();
        AlRectF cropRectF = rectF;
        AlVec2 scale = layer.getScale();
        AlRational rotation = layer.getRotation();
        AlPointF layerPos = layer.getPosition();
        rotation.num = rotation.num;
        layerPos.x = -layerPos.x;
        layerPos.y = -layerPos.y;
        AlPositionTranslator::translate(canvasSize, layerSize, cropRectF.left, cropRectF.top);
        AlPositionTranslator::translate(canvasSize, layerSize, cropRectF.right, cropRectF.bottom);
        AlPositionTranslator::translate(canvasSize, layerSize, layerPos.x, layerPos.y);

        double alpha = -AlMath::PI * rotation.num / rotation.den;
        cropSize = AlSize(layerSize.width * cropRectF.getWidth() / 2.0f,
                          layerSize.height * cropRectF.getHeight() / 2.0f);
        aMeasure.updateOrthogonal(cropSize, layerSize);

        float dx = (cropRectF.left + cropRectF.right) / 2.0f + layerPos.x;
        float dy = (cropRectF.top + cropRectF.bottom) / 2.0f + layerPos.y;
        dx /= scale.x;
        dy /= scale.y;

        if (cropSize.width / (float) cropSize.height > layerSize.width / (float) layerSize.height) {
            aMeasure.setScale(cropRectF.getWidth() / 2.0f / scale.x,
                              cropRectF.getWidth() / 2.0f / scale.y);
        } else {
            aMeasure.setScale(cropRectF.getHeight() / 2.0f / scale.x,
                              cropRectF.getHeight() / 2.0f / scale.y);
        }
        aMeasure.setRotation(alpha);
        aMeasure.setTranslate(dx, dy, alpha, 1, -1);

        AlVec2 lt;
        AlVec2 lb;
        AlVec2 rb;
        AlVec2 rt;
        aMeasure.measureTransLORectF(lt, lb, rb, rt);
        quad.setLeftTop((lt + 1.0f) / 2.0f);
        quad.setLeftBottom((lb + 1.0f) / 2.0f);
        quad.setRightBottom((rb + 1.0f) / 2.0f);
        quad.setRightTop((rt + 1.0f) / 2.0f);
#ifndef ENABLE_CROP_DEBUG
        AlRational nr = AlRational();
        layer.setScale(1, 1);
        layer.setRotation(nr);
        layer.setPosition(0, 0);
#endif
    }
#ifndef ENABLE_CROP_DEBUG
    layer.setSize(cropSize);
#endif
    description->cropQuad.setLeftTop(quad.leftTop());
    description->cropQuad.setLeftBottom(quad.leftBottom());
    description->cropQuad.setRightBottom(quad.rightBottom());
    description->cropQuad.setRightTop(quad.rightTop());
    return Hw::SUCCESS;
}