/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 16:26:01
*/

#ifndef FK_GRAPHIC_FKMATCOMPO_H
#define FK_GRAPHIC_FKMATCOMPO_H

#include "FkComponent.h"
#include "FkMatrix.h"

FK_SUPER_CLASS(FkMatCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkMatCompo)

public:
    FkMatCompo();

    FkMatCompo(const FkMatCompo &o);

    virtual ~FkMatCompo();

public:
    std::shared_ptr<FkMatrix> value = nullptr;
};

#endif //FK_GRAPHIC_FKMATCOMPO_H