/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTEXQUARK_H
#define FK_GRAPHIC_FKGRAPHICTEXQUARK_H

#include "FkQuark.h"

FK_CLASS FkGraphicTexQuark FK_EXTEND FkQuark {
public:
    FkGraphicTexQuark();

    FkGraphicTexQuark(const FkGraphicTexQuark &o) = delete;

    virtual ~FkGraphicTexQuark();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

};


#endif //FK_GRAPHIC_FKGRAPHICTEXQUARK_H
