/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-15 00:03:45
*/

#include "FkDeviceImage.h"

FK_IMPL_CLASS_TYPE(FkDeviceImage, FkObject)

FkDeviceImage::FkDeviceImage() : FkObject() {

}

FkDeviceImage::FkDeviceImage(const FkDeviceImage &o) : FkObject(o) {

}

FkDeviceImage::~FkDeviceImage() {

}

FkDeviceImage::kType FkDeviceImage::type() {
    return kType::Default;
}