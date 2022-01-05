/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCTXCOMPONENT_H
#define FK_GRAPHIC_FKGRAPHICCTXCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkGraphicContext.h"

class FkGraphicCtxComponent FK_EXTEND FkGraphicComponent {
public:
    FkGraphicCtxComponent();

    FkGraphicCtxComponent(const FkGraphicCtxComponent &o);

    virtual ~FkGraphicCtxComponent();

public:
    std::shared_ptr<FkGraphicContext> context = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICCTXCOMPONENT_H
