/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKVERTEXOBJECTCOMPONENT_H
#define FK_GRAPHIC_FKVERTEXOBJECTCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkVertexObject.h"

FK_CLASS FkVertexObjectComponent FK_EXTEND FkGraphicComponent {
public:
    FkVertexObjectComponent();

    FkVertexObjectComponent(const FkVertexObjectComponent &o);

    virtual ~FkVertexObjectComponent();

public:
    std::shared_ptr<FkVertexObject> vbo = nullptr;
};

#endif //FK_GRAPHIC_FKVERTEXOBJECTCOMPONENT_H