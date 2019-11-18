/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageCanvasModel.h"

AlImageCanvasModel::AlImageCanvasModel() : Object() {

}

AlImageCanvasModel::AlImageCanvasModel(const AlImageCanvasModel &e) : Object() {

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
