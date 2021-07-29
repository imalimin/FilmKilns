/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKVERTEXCOMPONENT_H
#define FK_GRAPHIC_FKVERTEXCOMPONENT_H

#include "FkGraphicComponent.h"

FK_CLASS FkVertexComponent FK_EXTEND FkGraphicComponent {
public:
    FkVertexComponent();

    FkVertexComponent(const FkVertexComponent &o);

    virtual ~FkVertexComponent();

public:
    size_t countVertex = 0;
    size_t countPerVertex = 0;
    size_t offset = 0;
    void *data = nullptr;
    size_t byteOfData = 0;
};

#endif //FK_GRAPHIC_FKVERTEXCOMPONENT_H