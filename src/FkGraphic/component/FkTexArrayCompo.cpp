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
#include "FkMVPMatrix.h"

FK_IMPL_CLASS_TYPE(FkTexArrayCompo, FkComponent)

FkTexArrayCompo::FkTexArrayCompo(FkSize size, int32_t x, int32_t y,
                                 int32_t blockWidth, int32_t blockHeight)
        : FkComponent(), blocks(x, y), size(size), blockSize(blockWidth, blockHeight) {

}

FkTexArrayCompo::FkTexArrayCompo(const FkTexArrayCompo &o)
        : FkComponent(o), blocks(o.blocks), size(o.size), blockSize(o.blockSize) {
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

std::shared_ptr<FkMatrix> FkTexArrayCompo::calcViewportMatrix(int32_t index, FkIntVec2 &pos) {
    auto tex = textures[index];
    auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    FkFloatVec3 scale(size.getWidth() * 1.0f / tex->desc.size.getWidth(),
                      size.getHeight() * 1.0f / tex->desc.size.getHeight(), 1.0f);
    FkFloatVec3 trans(0.0f, 0.0f, 0.0f);
    trans.x = (size.getWidth() - tex->desc.size.getWidth() - pos.x * 2.0f) / size.getWidth();
    trans.y = (size.getHeight() - tex->desc.size.getHeight() - pos.y * 2.0f) / size.getHeight();
    matrix->setScale(scale);
    matrix->setTranslate(trans);
    matrix->calc();
    return matrix;
}
