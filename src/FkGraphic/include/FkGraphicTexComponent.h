/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTEXCOMPONENT_H
#define FK_GRAPHIC_FKGRAPHICTEXCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkGraphicTexture.h"

class FkGraphicTexComponent FK_EXTEND FkGraphicComponent {
public:
    FkGraphicTexComponent();

    FkGraphicTexComponent(const FkGraphicTexComponent &o);

    virtual ~FkGraphicTexComponent();

public:
    std::shared_ptr<FkGraphicTexture> tex = nullptr;
    int32_t index = 0;
};


#endif //FK_GRAPHIC_FKGRAPHICTEXCOMPONENT_H
