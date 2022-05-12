/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 14:07:16
*/

#include "FkPathCompo.h"

FK_IMPL_CLASS_TYPE(FkPathCompo, FkComponent)

FkPathCompo::FkPathCompo(std::shared_ptr<FkPath> _path) : FkComponent(), path(_path) {
}

FkPathCompo::FkPathCompo(const FkPathCompo &o) : FkComponent(o) {

}

FkPathCompo::~FkPathCompo() {

}

void FkPathCompo::addPoint(int32_t x, int32_t y) {
    std::lock_guard<std::mutex> guard(mtx);
    FkDoubleVec2 point(x, y);
    path->addPoint(point);
}

void FkPathCompo::finish() {
    std::lock_guard<std::mutex> guard(mtx);
    path->finish();
}

size_t FkPathCompo::readPoints(std::vector<FkDoubleVec2> &_points) {
    std::lock_guard<std::mutex> guard(mtx);
    return path->readPoints(_points);
}

size_t FkPathCompo::size() {
    std::lock_guard<std::mutex> guard(mtx);
    return path->size();
}
