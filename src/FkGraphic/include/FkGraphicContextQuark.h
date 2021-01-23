//
// Created by mingyi.li on 2021-01-23.
//

#ifndef FK_GRAPHIC_FKGRAPHICCONTEXTQUARK_H
#define FK_GRAPHIC_FKGRAPHICCONTEXTQUARK_H

#include "FkQuark.h"

FK_CLASS FkGraphicContextQuark FK_EXTEND FkQuark {
public:
    FkGraphicContextQuark();

    FkGraphicContextQuark(const FkGraphicContextQuark &o) = delete;

    virtual ~FkGraphicContextQuark();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

};


#endif //FK_GRAPHIC_FKGRAPHICCONTEXTQUARK_H
