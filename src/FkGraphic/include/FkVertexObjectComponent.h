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
    FK_ENUM kValueLoc : int {
        VERTEX = 0,
        COORDINATE,
    };
public:
    FkVertexObjectComponent();

    FkVertexObjectComponent(const FkVertexObjectComponent &o);

    virtual ~FkVertexObjectComponent();

    void bind();

    void unbind();

    FkResult setup(std::shared_ptr<FkVertexObject> _vbo,
                   float *position, FkVertexDesc &posDesc,
                   float *coord, FkVertexDesc &coordDesc);

    FkResult getValueLoc(kValueLoc _loc, int32_t &start, FkVertexDesc &desc);

private:
    std::shared_ptr<FkVertexObject> vbo = nullptr;
    std::map<kValueLoc, std::pair<int32_t, FkVertexDesc>> loc;
};

#endif //FK_GRAPHIC_FKVERTEXOBJECTCOMPONENT_H