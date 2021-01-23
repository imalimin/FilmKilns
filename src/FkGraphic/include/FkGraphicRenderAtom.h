/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICRENDERATOM_H
#define FK_GRAPHIC_FKGRAPHICRENDERATOM_H

#include "FkAtom.h"

FK_CLASS FkGraphicRenderAtom FK_EXTEND FkAtom {
public:
    FkGraphicRenderAtom();

    FkGraphicRenderAtom(const FkGraphicRenderAtom &o) = delete;

    virtual ~FkGraphicRenderAtom();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;
};

#endif //FK_GRAPHIC_FKGRAPHICRENDERATOM_H
