/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 11:52:32
*/

#ifndef FK_GRAPHIC_FKBUFCOMPO_H
#define FK_GRAPHIC_FKBUFCOMPO_H

#include "FkComponent.h"
#include "FkBuffer.h"

FK_SUPER_CLASS(FkBufCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkBufCompo)

public:
    FkBufCompo(std::shared_ptr<FkBuffer> &buf);

    FkBufCompo(const FkBufCompo &o);

    virtual ~FkBufCompo();

public:
    std::shared_ptr<FkBuffer> buf = nullptr;
};

#endif //FK_GRAPHIC_FKBUFCOMPO_H