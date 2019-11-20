/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayerModel.h"


AlImageLayerModel *AlImageLayerModel::create(const std::string path) {
    return new AlImageLayerModel(path);
}

AlImageLayerModel::AlImageLayerModel(const std::string &path) : Object(), path(path) {

}

AlImageLayerModel::AlImageLayerModel(const AlImageLayerModel &e) : Object() {

}

AlImageLayerModel::~AlImageLayerModel() {

}

std::string AlImageLayerModel::getPath() {
    return path;
}
