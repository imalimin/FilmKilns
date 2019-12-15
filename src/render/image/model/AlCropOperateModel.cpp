/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCropOperateModel.h"
#include "AlVec4.h"

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
    AlMatrix rMat, t1Mat, t2Mat;
    t1Mat.setTranslate(-0.5f, -0.5f);
    rMat.setRotation(rotation.toFloat());
    t2Mat.setTranslate(0.5f, 0.5f);
    AlMatrix mat = t1Mat * rMat * t2Mat;
    AlVec4 lt(rectF.left, rectF.top);
    AlVec4 lb(rectF.left, rectF.bottom);
    AlVec4 rb(rectF.right, rectF.bottom);
    AlVec4 rt(rectF.right, rectF.top);
    description->cropQuad.setLeftTop(lt.multiply(mat).xy());
    description->cropQuad.setLeftBottom(lb.multiply(mat).xy());
    description->cropQuad.setRightBottom(rb.multiply(mat).xy());
    description->cropQuad.setRightTop(rt.multiply(mat).xy());
    AlSize layerSize = description->getLayerSize();
    description->setLayerSize(static_cast<int32_t>(layerSize.width * rectF.getWidth()),
                              static_cast<int32_t>(layerSize.width * rectF.getHeight()));
    return Hw::SUCCESS;
}
