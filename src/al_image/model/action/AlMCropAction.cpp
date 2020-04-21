/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMCropAction.h"
#include "AlMath.h"
#include "AlVec4.h"
#include "AlOrthMatrix.h"
#include "Logcat.h"
#include "AlCoordsTranslator.h"
#include "StringUtils.h"
#include "Al2DCoordinate.h"

#define TAG "AlMCropAction"
#define VAL_WIDTH                           "width"
#define VAL_HEIGHT                          "height"
#define TAG_SCALE                           "scale"
#define TAG_ROTATION                        "rotation"
#define TAG_POSITION                        "position"
#define VAL_RATIONAL_NUM                    "num"
#define VAL_RATIONAL_DEN                    "den"
#define VAL_VEC2_X                          "x"
#define VAL_VEC2_Y                          "y"
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

AlMCropAction::AlMCropAction() : AlAbsMAction(TYPE_CROP) {
    invalidate = true;
}

AlMCropAction::AlMCropAction(const AlMCropAction &o)
        : AlAbsMAction(o),
          rectF(o.rectF),
          scale(o.scale),
          rotation(o.rotation),
          position(o.position) {

}

AlMCropAction::~AlMCropAction() {
    invalidate = false;
}

void AlMCropAction::setRect(float left, float top, float right, float bottom) {
    this->rectF.set(left, top, right, bottom);
    invalidate = true;
}

HwResult AlMCropAction::measure(AlImgLayerDescription &layer,
                                AlImageLayerDrawModel *description) {
    if (invalidate && canvasSize.width > 0 && canvasSize.height > 0) {
        invalidate = false;
        this->scale = layer.getScale();
        this->rotation = layer.getRotation();
        this->position = layer.getPosition();
        AlSize layerSize = layer.getSize();
        Al2DCoordinate cc(canvasSize.width, canvasSize.height);
        Al2DCoordinate lc(layerSize.width, layerSize.height);
        lc.setScale(scale.x, scale.y);
        lc.setRotation(rotation);
        lc.setPosition(position.x, position.y);

        AlVec2 lt(rectF.left, rectF.top);
        AlVec2 lb(rectF.left, rectF.bottom);
        AlVec2 rb(rectF.right, rectF.bottom);
        AlVec2 rt(rectF.right, rectF.top);
        cc.translate(&lt, &lc);
        cc.translate(&lb, &lc);
        cc.translate(&rb, &lc);
        cc.translate(&rt, &lc);
        quad = AlQuad(lt, rt, rb, lb);
        quad.mirrorVertical();
        quad.setLeftTop((quad.leftTop() + 1.0f) / 2.0f);
        quad.setLeftBottom((quad.leftBottom() + 1.0f) / 2.0f);
        quad.setRightBottom((quad.rightBottom() + 1.0f) / 2.0f);
        quad.setRightTop((quad.rightTop() + 1.0f) / 2.0f);
        cropSize.height = std::sqrt(std::pow((lt.x - lb.x) * layerSize.width, 2.0) +
                                    std::pow((lt.y - lb.y) * layerSize.height, 2.0));
        cropSize.width = std::sqrt(std::pow((rb.x - lb.x) * layerSize.width, 2.0) +
                                   std::pow((rb.y - lb.y) * layerSize.height, 2.0));
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
    description->cropQuad = quad;
    return Hw::SUCCESS;
}

HwResult AlMCropAction::fromElement(AlElement *element) {
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
    this->rectF.set(MAXFLOAT, MAXFLOAT, MAXFLOAT, MAXFLOAT);
    size_t size = element->size();
    for (int i = 0; i < size; ++i) {
        AlElement *child = element->childAt(i);
        if (child->nameIs(TAG_RECTF)) {
            this->rectF.left = child->attrFloat(VAL_LEFT);
            this->rectF.top = child->attrFloat(VAL_TOP);
            this->rectF.right = child->attrFloat(VAL_RIGHT);
            this->rectF.bottom = child->attrFloat(VAL_BOTTOM);
        } else if (child->nameIs(TAG_SCALE)) {
            this->scale.x = child->attrFloat(VAL_VEC2_X);
            this->scale.y = child->attrFloat(VAL_VEC2_Y);
        } else if (child->nameIs(TAG_ROTATION)) {
            this->rotation.num = child->attrInt(VAL_RATIONAL_NUM);
            this->rotation.den = child->attrInt(VAL_RATIONAL_DEN);
        } else if (child->nameIs(TAG_POSITION)) {
            this->position.x = child->attrFloat(VAL_VEC2_X);
            this->position.y = child->attrFloat(VAL_VEC2_Y);
        } else if (child->nameIs(TAG_SIZE)) {
            this->cropSize.width = child->attrInt(VAL_WIDTH);
            this->cropSize.height = child->attrInt(VAL_HEIGHT);
        } else if (child->nameIs(TAG_QUAD)) {
            this->quad.setLeftTop(child->attrFloat(VAL_LT_X), child->attrFloat(VAL_LT_Y));
            this->quad.setRightTop(child->attrFloat(VAL_RT_X), child->attrFloat(VAL_RT_Y));
            this->quad.setRightBottom(child->attrFloat(VAL_RB_X), child->attrFloat(VAL_RB_Y));
            this->quad.setLeftBottom(child->attrFloat(VAL_LB_X), child->attrFloat(VAL_LB_Y));
        } else if (child->nameIs(TAG_BOOL)) {
            this->invalidate = static_cast<bool>(child->attrInt(VAL_INVALIDATE));
        }
    }
    if (MAXFLOAT == rectF.left || MAXFLOAT == rectF.top
        || MAXFLOAT == rectF.right || MAXFLOAT == rectF.bottom
        || MAXFLOAT == scale.x || MAXFLOAT == scale.y
        || INT32_MIN == rotation.num || INT32_MIN == rotation.den
        || MAXFLOAT == position.x || MAXFLOAT == position.y) {
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlMCropAction::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_ACTION);
    root->addAttr(VAL_TYPE, type);
    *element = root;
    AlElement *rect = new AlElement(TAG_RECTF);
    rect->addAttr(VAL_LEFT, StringUtils::valueOf(rectF.left));
    rect->addAttr(VAL_TOP, StringUtils::valueOf(rectF.top));
    rect->addAttr(VAL_RIGHT, StringUtils::valueOf(rectF.right));
    rect->addAttr(VAL_BOTTOM, StringUtils::valueOf(rectF.bottom));

    AlElement *scale = new AlElement(TAG_SCALE);
    scale->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->scale.x));
    scale->addAttr(VAL_VEC2_Y, StringUtils::valueOf(this->scale.y));

    AlElement *rotation = new AlElement(TAG_ROTATION);
    rotation->addAttr(VAL_RATIONAL_NUM, StringUtils::valueOf(this->rotation.num));
    rotation->addAttr(VAL_RATIONAL_DEN, StringUtils::valueOf(this->rotation.den));

    AlElement *pos = new AlElement(TAG_POSITION);
    pos->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->position.x));
    pos->addAttr(VAL_VEC2_Y, StringUtils::valueOf(this->position.y));

    AlElement *cropSize = new AlElement(TAG_SIZE);
    cropSize->addAttr(VAL_WIDTH, StringUtils::valueOf(this->cropSize.width));
    cropSize->addAttr(VAL_HEIGHT, StringUtils::valueOf(this->cropSize.height));

    AlElement *quad = new AlElement(TAG_QUAD);
    quad->addAttr(VAL_LT_X, StringUtils::valueOf(this->quad.leftTop().x));
    quad->addAttr(VAL_LT_Y, StringUtils::valueOf(this->quad.leftTop().y));
    quad->addAttr(VAL_RT_X, StringUtils::valueOf(this->quad.rightTop().x));
    quad->addAttr(VAL_RT_Y, StringUtils::valueOf(this->quad.rightTop().y));
    quad->addAttr(VAL_LB_X, StringUtils::valueOf(this->quad.leftBottom().x));
    quad->addAttr(VAL_LB_Y, StringUtils::valueOf(this->quad.leftBottom().y));
    quad->addAttr(VAL_RB_X, StringUtils::valueOf(this->quad.rightBottom().x));
    quad->addAttr(VAL_RB_Y, StringUtils::valueOf(this->quad.rightBottom().y));

    AlElement *invalidate = new AlElement(TAG_BOOL);
    invalidate->addAttr(VAL_INVALIDATE, StringUtils::valueOf((int) this->invalidate));

    root->addChild(rect);
    root->addChild(scale);
    root->addChild(rotation);
    root->addChild(pos);
    root->addChild(cropSize);
    root->addChild(quad);
    root->addChild(invalidate);
    return Hw::FAILED;
}