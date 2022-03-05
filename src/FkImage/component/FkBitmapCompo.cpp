/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-5 0:43:52
*/

#include "FkBitmapCompo.h"

FkBitmapCompo::FkBitmapCompo(std::shared_ptr<FkBitmap> bmp) : FkComponent(), bmp(bmp) {
    FK_MARK_SUPER
}

FkBitmapCompo::FkBitmapCompo(const FkBitmapCompo &o) : FkComponent(o), bmp(o.bmp) {
    FK_MARK_SUPER
}

FkBitmapCompo::~FkBitmapCompo() {

}