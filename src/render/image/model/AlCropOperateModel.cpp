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
    AlRectF cropRectF, layerRectF;
    _calculateRect(cropSize, layerSize, cropRectF, layerRectF);

    AlOrthMatrix oMat;
    oMat.update(layerRectF.left, layerRectF.right, layerRectF.bottom, layerRectF.top, -1.0f, 1.0f);

    AlMatrix tMat;
    tMat.setScale(rectF.getHeight() / 2.0f, rectF.getHeight() / 2.0f);
    tMat.setRotation(AlMath::PI * rotation.toFloat());
    float tx = (rectF.left + rectF.right) * layerRectF.getWidth() / 4.0f;
    float ty = (rectF.top + rectF.bottom) * layerRectF.getHeight() / 4.0f;
    Logcat::i(TAG, "tx=%f, ty=%f", tx, -ty);
    tMat.setTranslate(tx, -ty);
    AlMatrix mat = oMat * tMat;

    AlVec4 lt(cropRectF.left, cropRectF.top);
    AlVec4 lb(cropRectF.left, cropRectF.bottom);
    AlVec4 rb(cropRectF.right, cropRectF.bottom);
    AlVec4 rt(cropRectF.right, cropRectF.top);
    description->cropQuad.setLeftTop((lt.multiply(mat).xy() + 1.0f) / 2.0f);
    description->cropQuad.setLeftBottom((lb.multiply(mat).xy() + 1.0f) / 2.0f);
    description->cropQuad.setRightBottom((rb.multiply(mat).xy() + 1.0f) / 2.0f);
    description->cropQuad.setRightTop((rt.multiply(mat).xy() + 1.0f) / 2.0f);
    description->setLayerSize(cropSize.width, cropSize.height);
    return Hw::SUCCESS;
}

void AlCropOperateModel::_calculateRect(AlSize &src, AlSize &dest,
                                        AlRectF &srcRectF, AlRectF &destRectF) {
    float aspectRatio = dest.width > dest.height ?
                        (float) dest.width / (float) dest.height :
                        (float) dest.height / (float) dest.width;

    /// 计算正交矩阵，小边为1，大边>1
    if (dest.width > dest.height) {
        destRectF.left = -aspectRatio;
        destRectF.right = -destRectF.left;
        destRectF.bottom = -1.0f;
        destRectF.top = -destRectF.bottom;
    } else {
        destRectF.left = -1.0f;
        destRectF.right = -destRectF.left;
        destRectF.bottom = -aspectRatio;
        destRectF.top = -destRectF.bottom;
    }
    if (src.width / (float) src.height > dest.width / (float) dest.height) {
        srcRectF.left = destRectF.left;
        srcRectF.right = -srcRectF.left;
        srcRectF.bottom = srcRectF.left * src.height / (float) src.width;
        srcRectF.top = -srcRectF.bottom;
    } else {
        srcRectF.bottom = destRectF.bottom;
        srcRectF.top = -srcRectF.bottom;
        srcRectF.left = srcRectF.bottom * src.width / (float) src.height;
        srcRectF.right = -srcRectF.left;
    }
}