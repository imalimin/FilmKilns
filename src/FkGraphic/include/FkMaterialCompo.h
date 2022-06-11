/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-24 23:50:22
*/

#ifndef FK_GRAPHIC_FKMATERIALCOMPO_H
#define FK_GRAPHIC_FKMATERIALCOMPO_H

#include "FkComponent.h"

FK_SUPER_CLASS(FkMaterialCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkMaterialCompo)

public:
    FkMaterialCompo(FkID id);

    FkMaterialCompo(const FkMaterialCompo &o);

    virtual ~FkMaterialCompo();

    bool isInvalid();

    FkID id() const;

private:
    FkID _id = FK_ID_NONE;
};

#endif //FK_GRAPHIC_FKMATERIALCOMPO_H