/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMAlignCropAction.h"
#include "AlMath.h"
#include "StringUtils.h"

#define TAG_ROTATION                        "rotation"
#define VAL_RATIONAL_NUM                    "num"
#define VAL_RATIONAL_DEN                    "den"

AlMAlignCropAction::AlMAlignCropAction() : AlAbsMAction(TYPE_ALIGN_CROP) {

}

AlMAlignCropAction::AlMAlignCropAction(const AlMAlignCropAction &o)
        : AlAbsMAction(o), rotation(o.rotation) {

}

AlMAlignCropAction::~AlMAlignCropAction() {

}

HwResult AlMAlignCropAction::measure(AlImgLayerDescription &layer,
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

void AlMAlignCropAction::setRotation(AlRational &r) {
    rotation.num = r.num;
    rotation.den = r.den;
}

AlRational AlMAlignCropAction::getRotation() {
    return rotation;
}

HwResult AlMAlignCropAction::fromElement(AlElement *element) {
    if (nullptr == element) {
        return Hw::FAILED;
    }
    std::string name = element->name();
    if (!element->nameIs(TAG_ACTION)) {
        return Hw::FAILED;
    }
    type = element->attr(VAL_TYPE);
    this->rotation.num = INT32_MIN;
    this->rotation.den = INT32_MIN;
    size_t size = element->size();
    for (int i = 0; i < size; ++i) {
        AlElement *child = element->childAt(i);
        if (child->nameIs(TAG_ROTATION)) {
            this->rotation.num = child->attrInt(VAL_RATIONAL_NUM);
            this->rotation.den = child->attrInt(VAL_RATIONAL_DEN);
        }
    }
    if (INT32_MIN == this->rotation.num || INT32_MIN == this->rotation.den) {
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlMAlignCropAction::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_ACTION);
    root->addAttr(VAL_TYPE, type);
    *element = root;

    AlElement *rotation = new AlElement(TAG_ROTATION);
    rotation->addAttr(VAL_RATIONAL_NUM, StringUtils::valueOf(this->rotation.num));
    rotation->addAttr(VAL_RATIONAL_DEN, StringUtils::valueOf(this->rotation.den));

    root->addChild(rotation);
    return Hw::SUCCESS;
}
