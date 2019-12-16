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

void AlCropOperateModel::setRotation(AlRational &r) {
    this->rotation.num = r.num;
    this->rotation.den = r.den;
}

HwResult AlCropOperateModel::measure(AlAbsOperateModel::AlLayerDesc desc,
                                     AlImageLayerDrawModel *description) {
    AlSize layerSize = description->getLayerSize();
    AlSize cropSize(layerSize.width * rectF.getWidth() / 2.0f,
                    layerSize.height * rectF.getHeight() / 2.0f);
    aMeasure.updateOrthogonal(cropSize, layerSize);

    AlRectF targetRectF = aMeasure.getTargetPosRectF();
    float tx = (rectF.left + rectF.right) / 2.0f;
    float ty = (rectF.top + rectF.bottom) / 2.0f;
    Logcat::i(TAG, "tx=%f, ty=%f", tx, ty);

    if (cropSize.width / (float) cropSize.height > layerSize.width / (float) layerSize.height) {
        aMeasure.setScale(rectF.getWidth() / 2.0f, rectF.getWidth() / 2.0f);
    } else {
        aMeasure.setScale(rectF.getHeight() / 2.0f, rectF.getHeight() / 2.0f);
    }
    aMeasure.setRotation(AlMath::PI * rotation.toFloat());
    aMeasure.setTranslate(tx, ty);

    AlVec2 lt;
    AlVec2 lb;
    AlVec2 rb;
    AlVec2 rt;
    aMeasure.measureTransLORectF(lt, lb, rb, rt);
    description->cropQuad.setLeftTop((lt + 1.0f) / 2.0f);
    description->cropQuad.setLeftBottom((lb + 1.0f) / 2.0f);
    description->cropQuad.setRightBottom((rb + 1.0f) / 2.0f);
    description->cropQuad.setRightTop((rt + 1.0f) / 2.0f);
    description->cropQuad.mirrorVertical();
    description->setLayerSize(cropSize.width, cropSize.height);
    return Hw::SUCCESS;
}