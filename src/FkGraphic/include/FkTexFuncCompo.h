/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-04-02 22:56:32
*/

#ifndef FK_GRAPHIC_FKTEXFUNCCOMPO_H
#define FK_GRAPHIC_FKTEXFUNCCOMPO_H

#include "FkFuncCompo.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkTexFuncCompo, FkFuncCompo) {
FK_DEF_CLASS_TYPE_FUNC(FkTexFuncCompo)

public:
    FkTexFuncCompo(std::function<void(uint32_t, FkSize, int64_t)> func);

    FkTexFuncCompo(const FkTexFuncCompo &o);

    virtual ~FkTexFuncCompo();

public:
    std::function<void(uint32_t, FkSize, int64_t)> texFunc;
};

#endif //FK_GRAPHIC_FKTEXFUNCCOMPO_H