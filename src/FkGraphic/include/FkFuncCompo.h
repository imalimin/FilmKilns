/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 22:05:26
*/

#ifndef FK_GRAPHIC_FKFUNCCOMPO_H
#define FK_GRAPHIC_FKFUNCCOMPO_H

#include "FkComponent.h"

FK_SUPER_CLASS(FkFuncCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkFuncCompo)

public:
    FkFuncCompo(std::function<void()> func);

    FkFuncCompo(const FkFuncCompo &o);

    virtual ~FkFuncCompo();

public:
    std::function<void()> func;
};

#endif //FK_GRAPHIC_FKFUNCCOMPO_H