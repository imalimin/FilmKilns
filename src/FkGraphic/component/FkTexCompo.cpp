/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:30:26
*/

#include "FkTexCompo.h"

FkTexCompo::FkTexCompo(std::shared_ptr<FkGraphicTexture> &tex) : FkComponent(), tex(tex) {
    FK_MARK_SUPER
}

FkTexCompo::FkTexCompo(const FkTexCompo &o) : FkComponent(o) {
    FK_MARK_SUPER
}

FkTexCompo::~FkTexCompo() {

}