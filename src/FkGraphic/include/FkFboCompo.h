/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 20:35:11
*/

#ifndef FK_GRAPHIC_FKFBOCOMPO_H
#define FK_GRAPHIC_FKFBOCOMPO_H

#include "FkComponent.h"
#include "FkGraphicFrameObject.h"

FK_SUPER_CLASS(FkFboCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkFboCompo)

public:
    FkFboCompo(std::shared_ptr<FkGraphicFrameObject> &fbo);

    FkFboCompo(const FkFboCompo &o);

    virtual ~FkFboCompo();

public:
    std::shared_ptr<FkGraphicFrameObject> fbo = nullptr;
};

#endif //FK_GRAPHIC_FKFBOCOMPO_H