/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERENGINE_H
#define FK_GRAPHIC_FKRENDERENGINE_H

#include "FkObject.h"

FK_CLASS FkRenderEngine FK_EXTEND FkObject {
public:
    FkRenderEngine();

    FkRenderEngine(const FkRenderEngine &o);

    virtual ~FkRenderEngine();
};

#endif //FK_GRAPHIC_FKRENDERENGINE_H