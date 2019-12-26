/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsOperateModel.h"

AlAbsOperateModel::AlAbsOperateModel() : Object() {

}

AlAbsOperateModel::AlAbsOperateModel(const AlAbsOperateModel &o) : Object() {

}

AlAbsOperateModel::~AlAbsOperateModel() {

}

void AlAbsOperateModel::setCanvasSize(AlSize size) {
    canvasSize.width = size.width;
    canvasSize.height = size.height;
}
