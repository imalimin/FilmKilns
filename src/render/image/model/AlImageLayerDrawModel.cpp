/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayerDrawModel.h"

AlImageLayerDrawModel::AlImageLayerDrawModel(AlSize canvasSize)
        : Object(), canvasSize(canvasSize) {

}

AlImageLayerDrawModel::AlImageLayerDrawModel(const AlImageLayerDrawModel &o)
        : Object(), canvasSize(o.canvasSize),
          tex(o.tex),
          alpha(o.alpha),
          mat(o.mat),
          vertexRectF(o.vertexRectF) {

}

AlImageLayerDrawModel::~AlImageLayerDrawModel() {

}

AlSize AlImageLayerDrawModel::getCanvasSize() {
    return canvasSize;
}
