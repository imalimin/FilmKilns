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
#include "StringUtils.h"

#define TAG "AlCropOperateModel"
#define TAG_SCALE                           "scale"
#define TAG_ROTATION                        "rotation"
#define TAG_POSITION                        "position"
#define VAL_RATIONAL_NUM                    "num"
#define VAL_RATIONAL_DEN                    "den"
#define VAL_VEC2_X                          "x"
#define VAL_VEC2_Y                          "y"
#define TAG_OPT                             "opt"
#define VAL_TYPE                            "type"
#define TAG_RECTF                           "rectf"
#define VAL_LEFT                            "left"
#define VAL_TOP                             "top"
#define VAL_RIGHT                           "right"
#define VAL_BOTTOM                          "bottom"
#define TAG_SIZE                            "size"
#define TAG_QUAD                            "quad"
#define VAL_LT_X                            "lt_x"
#define VAL_LT_Y                            "lt_y"
#define VAL_RT_X                            "rt_x"
#define VAL_RT_Y                            "rt_y"
#define VAL_LB_X                            "lb_x"
#define VAL_LB_Y                            "lb_y"
#define VAL_RB_X                            "rb_x"
#define VAL_RB_Y                            "rb_y"
#define TAG_BOOL                            "bool"
#define VAL_INVALIDATE                      "invalidate"

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

HwResult AlCropOperateModel::fromElement(AlElement *element) {
    return Hw::FAILED;
}

HwResult AlCropOperateModel::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_OPT);
    root->addAttr(VAL_TYPE, type);
    *element = root;
    AlElement *rect = new AlElement(TAG_RECTF);
    rect->addAttr(VAL_LEFT, StringUtils::valueOf(rectF.left));
    rect->addAttr(VAL_TOP, StringUtils::valueOf(rectF.top));
    rect->addAttr(VAL_RIGHT, StringUtils::valueOf(rectF.right));
    rect->addAttr(VAL_BOTTOM, StringUtils::valueOf(rectF.bottom));

    AlElement *scale = new AlElement(TAG_SCALE);
    scale->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->scale.x));
    scale->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->scale.y));

    AlElement *rotation = new AlElement(TAG_ROTATION);
    rotation->addAttr(VAL_RATIONAL_NUM, StringUtils::valueOf(this->rotation.num));
    rotation->addAttr(VAL_RATIONAL_DEN, StringUtils::valueOf(this->rotation.den));

    AlElement *pos = new AlElement(TAG_POSITION);
    pos->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->position.x));
    pos->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->position.y));

    root->addChild(rect);
    root->addChild(scale);
    root->addChild(rotation);
    root->addChild(pos);
    return Hw::FAILED;
}
