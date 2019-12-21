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

}

AlCropOperateModel::AlCropOperateModel(const AlCropOperateModel &o)
        : AlAbsOperateModel(o), rectF(o.rectF) {

}

AlCropOperateModel::~AlCropOperateModel() {

}

void AlCropOperateModel::setRect(float left, float top, float right, float bottom) {
    this->rectF.set(left, top, right, bottom);
}

void AlCropOperateModel::setScale(float scaleX, float scaleY) {
    this->scale.x = scaleX;
    this->scale.y = scaleY;
}

void AlCropOperateModel::setRotation(AlRational &r) {
    this->rotation.num = r.num;
    this->rotation.den = r.den;
}

void AlCropOperateModel::setPosition(AlVec2 &position) {
    this->position.x = position.x;
    this->position.y = position.y;
}

HwResult AlCropOperateModel::measure(AlImgLayerDescription &layer,
                                     AlImageLayerDrawModel *description) {
    AlSize layerSize = description->getLayerSize();
    AlRectF cropRectF = rectF;
    AlPointF layerPos = position;
    AlPositionTranslator::translate(canvasSize, layerSize, cropRectF.left, cropRectF.top);
    AlPositionTranslator::translate(canvasSize, layerSize, cropRectF.right, cropRectF.bottom);
    AlPositionTranslator::translate(canvasSize, layerSize, layerPos.x, layerPos.y);

    double alpha = AlMath::PI * rotation.num / rotation.den;
    AlSize cropSize(layerSize.width * cropRectF.getWidth() / 2.0f,
                    layerSize.height * cropRectF.getHeight() / 2.0f);
    aMeasure.updateOrthogonal(cropSize, layerSize);

    float dx = (cropRectF.left + cropRectF.right) / 2.0f + layerPos.x;
    float dy = (cropRectF.top + cropRectF.bottom) / 2.0f + layerPos.y;
    dx /= scale.x;
    dy /= scale.y;
    Logcat::i(TAG, "dx=%f, dy=%f", dx, dy);

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
    description->cropQuad.setLeftTop((lt + 1.0f) / 2.0f);
    description->cropQuad.setLeftBottom((lb + 1.0f) / 2.0f);
    description->cropQuad.setRightBottom((rb + 1.0f) / 2.0f);
    description->cropQuad.setRightTop((rt + 1.0f) / 2.0f);
//    description->cropQuad.mirrorVertical();
#ifndef ENABLE_CROP_DEBUG
    layer.setSize(cropSize);
#endif
    Logcat::i(TAG, "[%f,%f], [%f,%f]",
              description->cropQuad.leftTop().x, description->cropQuad.leftTop().y,
              description->cropQuad.rightTop().x, description->cropQuad.rightTop().y);
    Logcat::i(TAG, "[%f,%f], [%f,%f]",
              description->cropQuad.leftBottom().x, description->cropQuad.leftBottom().y,
              description->cropQuad.rightBottom().x, description->cropQuad.rightBottom().y);
    return Hw::SUCCESS;
}