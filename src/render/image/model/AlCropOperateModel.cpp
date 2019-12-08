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
                                     AlImageLayerDrawModel &drawModel) {
    AlMatrix mat;
    mat.setRotation(rotation.toFloat());
    AlVec4 lt(rectF.left, rectF.top);
    AlVec4 lb(rectF.left, rectF.bottom);
    AlVec4 rb(rectF.right, rectF.bottom);
    AlVec4 rt(rectF.right, rectF.top);
    drawModel.cropQuad.setLeftTop(lt.multiply(mat).xy());
    drawModel.cropQuad.setLeftBottom(lb.multiply(mat).xy());
    drawModel.cropQuad.setRightBottom(rb.multiply(mat).xy());
    drawModel.cropQuad.setRightTop(rt.multiply(mat).xy());
    AlSize layerSize = drawModel.getLayerSize();
    drawModel.setLayerSize(static_cast<int32_t>(layerSize.width * rectF.getWidth()),
                           static_cast<int32_t>(layerSize.width * rectF.getHeight()));
    return Hw::SUCCESS;
}
