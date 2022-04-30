/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 15:29:27
*/

#ifndef FK_GRAPHIC_FKVBOCOMPO_H
#define FK_GRAPHIC_FKVBOCOMPO_H

#include "FkComponent.h"
#include "FkVertexObject.h"

FK_SUPER_CLASS(FkVboCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkVboCompo)

public:
    FK_ENUM kValueLoc : int {
        VERTEX = 0,
        COORDINATE,
    };
public:
    FkVboCompo();

    FkVboCompo(const FkVboCompo &o);

    virtual ~FkVboCompo();

    void bind();

    void unbind();

    FkResult setup(std::shared_ptr<FkVertexObject> _vbo,
                   float *position, FkVertexDesc &posDesc);

    FkResult setup(std::shared_ptr<FkVertexObject> _vbo,
                   float *position, FkVertexDesc &posDesc,
                   float *coord, FkVertexDesc &coordDesc);

    FkResult getValueLoc(kValueLoc _loc, int32_t &start, FkVertexDesc &desc);

private:
    std::shared_ptr<FkVertexObject> vbo = nullptr;
    std::map<kValueLoc, std::pair<int32_t, FkVertexDesc>> loc;
};

#endif //FK_GRAPHIC_FKVBOCOMPO_H