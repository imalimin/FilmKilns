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

AlCropOperateModel::AlCropOperateModel() : AlAbsOperateModel(TYPE_CROP) {
    invalidate = true;
}

AlCropOperateModel::AlCropOperateModel(const AlCropOperateModel &o)
        : AlAbsOperateModel(o),
          rectF(o.rectF),
          scale(o.scale),
          rotation(o.rotation),
          position(o.position) {

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
    if (invalidate && canvasSize.width > 0 && canvasSize.height > 0) {
        invalidate = false;
        this->scale = layer.getScale();
        this->rotation = layer.getRotation();
        this->position = layer.getPosition();
        AlSize layerSize = layer.getSize();
        AlRectF cropRectF = rectF;
        AlPointF layerPos = this->position;
        layerPos.x = -layerPos.x;
        layerPos.y = -layerPos.y;
        AlPositionTranslator::translate(canvasSize, layerSize, cropRectF.left, cropRectF.top);
        AlPositionTranslator::translate(canvasSize, layerSize, cropRectF.right, cropRectF.bottom);
        AlPositionTranslator::translate(canvasSize, layerSize, layerPos.x, layerPos.y);

        double alpha = -AlMath::PI * rotation.num / rotation.den;
        cropSize = AlSize(layerSize.width * cropRectF.getWidth() / 2.0f,
                          layerSize.height * cropRectF.getHeight() / 2.0f);
        aMeasurer.updateOrthogonal(cropSize, layerSize);

        float dx = (cropRectF.left + cropRectF.right) / 2.0f + layerPos.x;
        float dy = (cropRectF.top + cropRectF.bottom) / 2.0f + layerPos.y;
        dx /= scale.x;
        dy /= scale.y;

        if (cropSize.width / (float) cropSize.height > layerSize.width / (float) layerSize.height) {
            aMeasurer.setScale(cropRectF.getWidth() / 2.0f / scale.x,
                               cropRectF.getWidth() / 2.0f / scale.y);
        } else {
            aMeasurer.setScale(cropRectF.getHeight() / 2.0f / scale.x,
                               cropRectF.getHeight() / 2.0f / scale.y);
        }
        aMeasurer.setRotation(alpha);
        aMeasurer.setTranslate(dx, dy, alpha, 1, -1);

        AlVec2 lt;
        AlVec2 lb;
        AlVec2 rb;
        AlVec2 rt;
        aMeasurer.measureTransLORectF(lt, lb, rb, rt);
        quad.setLeftTop((lt + 1.0f) / 2.0f);
        quad.setLeftBottom((lb + 1.0f) / 2.0f);
        quad.setRightBottom((rb + 1.0f) / 2.0f);
        quad.setRightTop((rt + 1.0f) / 2.0f);
    }
#ifndef ENABLE_CROP_DEBUG
    AlRational nr = AlRational();
    nr.den = 100000;
    nr.num = static_cast<int32_t>((layer.getRotation().toFloat() - this->rotation.toFloat()) *
                                  nr.den);
    layer.setScale(layer.getScale().x / scale.x, layer.getScale().y / scale.y);
    layer.setRotation(nr);
    layer.setPosition(layer.getPosition().x - position.x, layer.getPosition().y - position.y);
    layer.setSize(cropSize);
#endif
    description->cropQuad.setLeftTop(quad.leftTop());
    description->cropQuad.setLeftBottom(quad.leftBottom());
    description->cropQuad.setRightBottom(quad.rightBottom());
    description->cropQuad.setRightTop(quad.rightTop());
    return Hw::SUCCESS;
}

AlRectF AlCropOperateModel::getRect() {
    return rectF;
}

AlVec2 AlCropOperateModel::getScale() {
    return scale;
}

AlRational AlCropOperateModel::getRotation() {
    return rotation;
}

AlVec2 AlCropOperateModel::getPosition() {
    return position;
}
