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

#include "FkGraphicComponent.h"

FK_CLASS FkMaterialCompo FK_EXTEND FkGraphicComponent {
public:
    FkMaterialCompo(FkID id);

    FkMaterialCompo(const FkMaterialCompo &o);

    virtual ~FkMaterialCompo();

    bool isUseless();

    FkID id() const;

private:
    FkID _id = FK_ID_NONE;
};

#endif //FK_GRAPHIC_FKMATERIALCOMPO_H