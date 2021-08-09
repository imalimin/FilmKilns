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

    FkResult setup(size_t _countVertex, size_t _countPerVertex, size_t _bytes, void *_data);

    void *getData();

    size_t getSize();

public:
    size_t countVertex = 0;
    size_t countPerVertex = 0;

private:
    void *data = nullptr;
    size_t byteOfData = 0;
};

#endif //FK_GRAPHIC_FKVERTEXCOMPONENT_H