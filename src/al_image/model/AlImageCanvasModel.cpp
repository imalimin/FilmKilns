/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageCanvasModel.h"
#include "StringUtils.h"

#define TAG_CANVAS                          "canvas"
#define VAL_WIDTH                           "width"
#define VAL_HEIGHT                          "height"
#define VAL_COLOR                           "color"

AlImageCanvasModel::AlImageCanvasModel() : AlAbsElemented() {

}

AlImageCanvasModel::AlImageCanvasModel(const AlImageCanvasModel &e) : AlAbsElemented() {

}

AlImageCanvasModel::~AlImageCanvasModel() {

}

void AlImageCanvasModel::setSize(int32_t width, int32_t height) {
    this->width = width;
    this->height = height;
}

void AlImageCanvasModel::set(int32_t width, int32_t height, int32_t color) {
    this->color = color;
    setSize(width, height);
}

int32_t AlImageCanvasModel::getWidth() {
    return width;
}

int32_t AlImageCanvasModel::getHeight() {
    return height;
}

int32_t AlImageCanvasModel::getColor() {
    return color;
}

HwResult AlImageCanvasModel::fromElement(AlElement *element) {
    if (nullptr == element) {
        return Hw::FAILED;
    }
    std::string name = element->name();
    if (!element->nameIs(TAG_CANVAS)) {
        return Hw::FAILED;
    }
    width = element->attrInt(VAL_WIDTH);
    height = element->attrInt(VAL_HEIGHT);
    color = element->attrInt(VAL_COLOR);
    if (width <= 0 || height <= 0) {
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlImageCanvasModel::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_CANVAS);
    root->addAttr(VAL_WIDTH, StringUtils::valueOf(width));
    root->addAttr(VAL_HEIGHT, StringUtils::valueOf(height));
    root->addAttr(VAL_COLOR, StringUtils::valueOf(color));
    *element = root;
    return Hw::SUCCESS;

}