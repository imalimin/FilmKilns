/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-02 10:02:37
*/

#include "FkCharMapCompo.h"

FK_IMPL_CLASS_TYPE(FkCharMapCompo, FkComponent)

FkCharMapCompo::FkCharMapCompo(const std::shared_ptr<FkTextureCharMap> &charMap)
        : FkComponent(), charMap(charMap) {

}

FkCharMapCompo::FkCharMapCompo(const FkCharMapCompo &o)
        : FkComponent(o), charMap(o.charMap) {

}

FkCharMapCompo::~FkCharMapCompo() {

}