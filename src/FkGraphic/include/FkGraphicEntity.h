/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICENTITY_H
#define FK_GRAPHIC_FKGRAPHICENTITY_H

#include "FkObject.h"
#include "FkGraphicComponent.h"

FK_CLASS FkGraphicEntity FK_EXTEND FkObject {
public:
    FkGraphicEntity();

    FkGraphicEntity(const FkGraphicEntity &o);

    virtual ~FkGraphicEntity();

    FkResult addComponent(std::shared_ptr<FkGraphicComponent> comp);

private:
    std::list<std::shared_ptr<FkGraphicComponent>> components;
};


#endif //FK_GRAPHIC_FKGRAPHICENTITY_H
