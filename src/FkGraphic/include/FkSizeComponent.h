/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSIZECOMPONENT_H
#define FK_GRAPHIC_FKSIZECOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkSize.h"

FK_CLASS FkSizeComponent FK_EXTEND FkGraphicComponent {
public:
    FkSizeComponent();

    FkSizeComponent(const FkSizeComponent &o);

    virtual ~FkSizeComponent();

public:
    FkSize size;
};


#endif //FK_GRAPHIC_FKSIZECOMPONENT_H
