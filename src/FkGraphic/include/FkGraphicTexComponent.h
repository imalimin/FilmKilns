//
// Created by mingyi.li on 2021-01-31.
//

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
    std::shared_ptr<FkGraphicTexture> texture = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICTEXCOMPONENT_H
