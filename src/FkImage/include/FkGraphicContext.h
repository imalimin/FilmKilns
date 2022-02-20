/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCONTEXT_H
#define FK_GRAPHIC_FKGRAPHICCONTEXT_H

#include "FkContextCompo.h"

FK_CLASS FkGraphicContext FK_EXTEND FkContextCompo {
public:
    FkGraphicContext(const std::string alias);

    FkGraphicContext(const FkGraphicContext &o) = delete;

    virtual ~FkGraphicContext();
};


#endif //FK_GRAPHIC_FKGRAPHICCONTEXT_H
