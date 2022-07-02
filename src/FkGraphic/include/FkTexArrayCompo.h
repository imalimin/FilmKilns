/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 21:33:52
*/

#ifndef FK_GRAPHIC_FKTEXARRAYCOMPO_H
#define FK_GRAPHIC_FKTEXARRAYCOMPO_H

#include "FkComponent.h"
#include "FkIntVec2.h"
#include "FkGraphicTexture.h"
#include "FkMatrix.h"

FK_SUPER_CLASS(FkTexArrayCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkTexArrayCompo)

public:
    FkTexArrayCompo(FkSize size, int32_t x, int32_t y, int32_t blockWidth, int32_t blockHeight);

    FkTexArrayCompo(const FkTexArrayCompo &o);

    virtual ~FkTexArrayCompo();

    bool empty();

    size_t countOfTexture();

    std::shared_ptr<FkGraphicTexture> &operator[](int32_t i);

    const std::shared_ptr<FkGraphicTexture> &operator[](int32_t i) const;

    std::shared_ptr<FkMatrix> calcViewportMatrix(int32_t index, FkIntVec2 &pos);

public:
    FkIntVec2 blocks;
    FkSize blockSize;
    FkSize size;
    std::vector<std::shared_ptr<FkGraphicTexture>> textures;
};

#endif //FK_GRAPHIC_FKTEXARRAYCOMPO_H