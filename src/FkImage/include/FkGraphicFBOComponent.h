/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICFBOCOMPONENT_H
#define FK_GRAPHIC_FKGRAPHICFBOCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkGraphicFrameObject.h"

class FkGraphicFBOComponent FK_EXTEND FkGraphicComponent {
public:
    FkGraphicFBOComponent();

    FkGraphicFBOComponent(const FkGraphicFBOComponent &o);

    virtual ~FkGraphicFBOComponent();

public:
    std::shared_ptr<FkGraphicFrameObject> fbo = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICFBOCOMPONENT_H
