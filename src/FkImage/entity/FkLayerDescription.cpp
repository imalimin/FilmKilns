/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-12 00:11:02
*/

#include "FkLayerDescription.h"

FK_IMPL_CLASS_TYPE(FkLayerDescription, FkObject)

FkLayerDescription::FkLayerDescription() : FkObject() {

}

FkLayerDescription::FkLayerDescription(const FkLayerDescription &o)
        : FkObject(o), deviceImage(o.deviceImage) {

}

FkLayerDescription::~FkLayerDescription() {

}