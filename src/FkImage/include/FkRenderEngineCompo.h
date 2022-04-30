/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 20:42:01
*/

#ifndef FK_IMAGE_FKRENDERENGINECOMPO_H
#define FK_IMAGE_FKRENDERENGINECOMPO_H

#include "FkComponent.h"
#include "FkEngine.h"

FK_SUPER_CLASS(FkRenderEngineCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderEngineCompo)

public:
    FkRenderEngineCompo(std::shared_ptr<FkEngine> &_engine);

    FkRenderEngineCompo(const FkRenderEngineCompo &o);

    virtual ~FkRenderEngineCompo();

public:
    std::shared_ptr<FkEngine> engine = nullptr;
};

#endif //FK_IMAGE_FKRENDERENGINECOMPO_H