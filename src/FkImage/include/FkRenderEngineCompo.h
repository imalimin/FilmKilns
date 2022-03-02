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
#include "FkRenderEngine.h"

FK_CLASS FkRenderEngineCompo FK_EXTEND FkComponent {
public:
    FkRenderEngineCompo(std::shared_ptr<FkRenderEngine> &_engine);

    FkRenderEngineCompo(const FkRenderEngineCompo &o);

    virtual ~FkRenderEngineCompo();

public:
    std::shared_ptr<FkRenderEngine> engine = nullptr;
};

#endif //FK_IMAGE_FKRENDERENGINECOMPO_H