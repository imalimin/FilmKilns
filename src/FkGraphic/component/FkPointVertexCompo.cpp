/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-19 21:05:06
*/

#include "FkPointVertexCompo.h"

FkPointVertexCompo::FkPointVertexCompo() : FkVertexCompo() {
    FK_MARK_SUPER
}

FkPointVertexCompo::FkPointVertexCompo(const FkPointVertexCompo &o)
        : FkVertexCompo(o) {
    FK_MARK_SUPER
}

FkPointVertexCompo::~FkPointVertexCompo() {

}