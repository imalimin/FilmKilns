/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:29:28
*/

#ifndef FK_GRAPHIC_FKENVENTITY_H
#define FK_GRAPHIC_FKENVENTITY_H

#include "FkEntity.h"
#include "FkContextCompo.h"

FK_CLASS FkEnvEntity FK_EXTEND FkEntity {
public:
    FkEnvEntity();

    FkEnvEntity(const FkEnvEntity &o);

    virtual ~FkEnvEntity();

    std::shared_ptr<FkContextCompo> getContext();
};

#endif //FK_GRAPHIC_FKENVENTITY_H