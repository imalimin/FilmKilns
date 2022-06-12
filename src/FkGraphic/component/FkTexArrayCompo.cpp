/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 21:33:52
*/

#include "FkTexArrayCompo.h"

FK_IMPL_CLASS_TYPE(FkTexArrayCompo, FkComponent)

FkTexArrayCompo::FkTexArrayCompo(FkSize size, int32_t x, int32_t y)
        : FkComponent(), blocks(x, y), size(size) {

}

FkTexArrayCompo::FkTexArrayCompo(const FkTexArrayCompo &o)
        : FkComponent(o), blocks(o.blocks),size(o.size) {
    for (auto &it: o.textures) {
        textures.emplace_back(it);
    }
}

FkTexArrayCompo::~FkTexArrayCompo() {
    blocks = {0, 0};
    textures.clear();
}

bool FkTexArrayCompo::empty() {
    return textures.empty();
}

size_t FkTexArrayCompo::countOfTexture() {
    return textures.size();
}

std::shared_ptr<FkGraphicTexture> &FkTexArrayCompo::operator[](int32_t i) {
    return textures[i];
}

const std::shared_ptr<FkGraphicTexture> &FkTexArrayCompo::operator[](int32_t i) const {
    return textures[i];
}
