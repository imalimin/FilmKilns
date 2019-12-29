/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsOperateModel.h"

const std::string AlAbsOperateModel::TYPE_CROP = "crop";
const std::string AlAbsOperateModel::TYPE_ALIGN_CROP = "align_crop";

AlAbsOperateModel::AlAbsOperateModel(std::string type) : Object(), type(type) {

}

AlAbsOperateModel::AlAbsOperateModel(const AlAbsOperateModel &o)
        : Object(), type(o.type), canvasSize(o.canvasSize) {

}

AlAbsOperateModel::~AlAbsOperateModel() {

}

void AlAbsOperateModel::setCanvasSize(AlSize size) {
    canvasSize.width = size.width;
    canvasSize.height = size.height;
}

AlSize AlAbsOperateModel::getCanvasSize() {
    return canvasSize;
}

std::string AlAbsOperateModel::getType() {
    return type;
}
