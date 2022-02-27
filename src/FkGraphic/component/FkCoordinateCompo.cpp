/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 14:40:57
*/

#include "FkCoordinateCompo.h"

FkCoordinateCompo::FkCoordinateCompo() : FkVertexCompo() {
    FK_MARK_SUPER
}

FkCoordinateCompo::FkCoordinateCompo(const FkCoordinateCompo &o) : FkVertexCompo(o) {
    FK_MARK_SUPER
}

FkCoordinateCompo::~FkCoordinateCompo() {

}