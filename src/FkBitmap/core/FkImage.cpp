/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-21 23:01:50
*/

#include "FkImage.h"

FkImage::FkImage() : FkObject() {
    FK_MARK_SUPER
}

FkImage::FkImage(const FkImage &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkImage::~FkImage() {

}