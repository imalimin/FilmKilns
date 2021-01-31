/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexPtl.h"

FkGraphicTexPtl::FkGraphicTexPtl() : FkProtocol() {
    FK_MARK_SUPER
}

FkGraphicTexPtl::FkGraphicTexPtl(const FkGraphicTexPtl &o) : FkProtocol(o), texId(o.texId) {
    FK_MARK_SUPER

}

FkGraphicTexPtl::~FkGraphicTexPtl() {
    texId = FK_ID_NONE;
}
