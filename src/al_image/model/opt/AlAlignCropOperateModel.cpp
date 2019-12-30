/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAlignCropOperateModel.h"
#include "AlMath.h"
#include "StringUtils.h"

#define VAL_RATIONAL_NUM                    "num"
#define VAL_RATIONAL_DEN                    "den"

AlAlignCropOperateModel::AlAlignCropOperateModel() : AlAbsOperateModel(TYPE_ALIGN_CROP) {

}

AlAlignCropOperateModel::AlAlignCropOperateModel(const AlAlignCropOperateModel &o)
        : AlAbsOperateModel(o), rotation(o.rotation) {

}

AlAlignCropOperateModel::~AlAlignCropOperateModel() {

}

HwResult AlAlignCropOperateModel::measure(AlImgLayerDescription &layer,
                                          AlImageLayerDrawModel *description) {
    AlSize size = layer.getSize();
    float cRatio = canvasSize.width / (float) canvasSize.height;
    float lRatio = size.width / (float) size.height;
    float fitCanvasScale;
    if (cRatio > lRatio) {
        fitCanvasScale = (canvasSize.width * size.height)
                         / (float) (canvasSize.height * size.width);
    } else {
        fitCanvasScale = (size.width * canvasSize.height)
                         / (float) (size.height * canvasSize.width);
    }
    ///scale = (h / w / tan(PI / 2 - alpha) + 1) * cos(alpha)
    double alpha = rotation.toDouble();
    double scale = (size.height / (double) size.width
                    / abs(tan(AlMath::PI / 2.0f - alpha * AlMath::PI))
                    + 1.0f) * abs(cos(alpha * AlMath::PI));
    if (size.width > size.height) {
        scale = (size.width / (double) size.height
                 / abs(tan(AlMath::PI / 2.0f - alpha * AlMath::PI))
                 + 1.0f) * abs(cos(alpha * AlMath::PI));
    }
    scale *= fitCanvasScale;
    layer.setScale(static_cast<float>(scale), static_cast<float>(scale));
    layer.setRotation(rotation);
    layer.setPosition(0.0f, 0.0f);
    return Hw::SUCCESS;
}

void AlAlignCropOperateModel::setRotation(AlRational &r) {
    rotation.num = r.num;
    rotation.den = r.den;
}

AlRational AlAlignCropOperateModel::getRotation() {
    return rotation;
}

HwResult AlAlignCropOperateModel::fromElement(AlElement *element) {
    return Hw::SUCCESS;
}

HwResult AlAlignCropOperateModel::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_OPT);
    root->addAttr(VAL_TYPE, type);
    *element = root;

    AlElement *rotation = new AlElement(TAG_OPT);
    rotation->addAttr(VAL_RATIONAL_NUM, StringUtils::valueOf(this->rotation.num));
    rotation->addAttr(VAL_RATIONAL_DEN, StringUtils::valueOf(this->rotation.den));

    root->addChild(rotation);
    return Hw::SUCCESS;
}
