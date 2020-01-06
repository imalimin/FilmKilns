/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsMAction.h"

const std::string AlAbsMAction::TYPE_CROP = "crop";
const std::string AlAbsMAction::TYPE_ALIGN_CROP = "align_crop";

AlAbsMAction::AlAbsMAction(std::string type) : AlAbsElemented(), type(type) {

}

AlAbsMAction::AlAbsMAction(const AlAbsMAction &o)
        : AlAbsElemented(), type(o.type), canvasSize(o.canvasSize) {

}

AlAbsMAction::~AlAbsMAction() {

}

void AlAbsMAction::setCanvasSize(AlSize size) {
    canvasSize.width = size.width;
    canvasSize.height = size.height;
}

AlSize AlAbsMAction::getCanvasSize() {
    return canvasSize;
}

std::string AlAbsMAction::getType() {
    return type;
}
