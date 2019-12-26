/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImgLayerDescription.h"

AlImgLayerDescription::AlImgLayerDescription(const AlImgLayerDescription &o)
        : AlImageLayerModel(o), size(o.size) {

}

AlImgLayerDescription::AlImgLayerDescription(const AlImageLayerModel &o) : AlImageLayerModel(o) {

}

AlImgLayerDescription::~AlImgLayerDescription() {

}

void AlImgLayerDescription::setSize(AlSize size) {
    this->size.width = size.width;
    this->size.height = size.height;
}

AlSize AlImgLayerDescription::getSize() {
    return size;
}
