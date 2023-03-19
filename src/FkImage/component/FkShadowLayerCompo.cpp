/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-20 00:29:03
*/

#include "FkShadowLayerCompo.h"

FK_IMPL_CLASS_TYPE(FkShadowLayerCompo, FkComponent)

FkShadowLayerCompo::FkShadowLayerCompo(FkID _parentLayerId)
        : FkComponent(), parentLayerId(_parentLayerId) {

}

FkShadowLayerCompo::FkShadowLayerCompo(const FkShadowLayerCompo &o)
        : FkComponent(o), parentLayerId(o.parentLayerId) {

}

FkShadowLayerCompo::~FkShadowLayerCompo() {

}