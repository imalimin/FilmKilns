/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:14:50
*/

#ifndef FK_GRAPHIC_FKNEWTEXPROTO_H
#define FK_GRAPHIC_FKNEWTEXPROTO_H

#include "FkProtocol.h"
#include "FkTexEntity.h"

FK_CLASS FkNewTexProto FK_EXTEND FkProtocol {
public:
    FkNewTexProto();

    FkNewTexProto(const FkNewTexProto &o);

    virtual ~FkNewTexProto();

public:
    std::shared_ptr<FkTexEntity> texEntity;
};

#endif //FK_GRAPHIC_FKNEWTEXPROTO_H