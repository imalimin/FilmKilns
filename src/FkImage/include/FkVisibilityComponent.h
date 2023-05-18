/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2023-5-19 0:10:49
*/

#ifndef FK_IMAGE_FKVISIBILITYCOMPONENT_H
#define FK_IMAGE_FKVISIBILITYCOMPONENT_H

#include "FkComponent.h"

FK_SUPER_CLASS(FkVisibilityComponent, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkVisibilityComponent)

public:
    FkVisibilityComponent();

    FkVisibilityComponent(const FkVisibilityComponent &o);

    virtual ~FkVisibilityComponent();

public:
    int visibility = FK_VISIBLE;
};


#endif //FK_IMAGE_FKVISIBILITYCOMPONENT_H
