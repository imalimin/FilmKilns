/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-20 00:29:03
*/

#ifndef FK_IMAGE_FKSHADOWLAYERCOMPO_H
#define FK_IMAGE_FKSHADOWLAYERCOMPO_H

#include "FkComponent.h"

FK_SUPER_CLASS(FkShadowLayerCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkShadowLayerCompo)

public:
    FkShadowLayerCompo(FkID _parentLayerId);

    FkShadowLayerCompo(const FkShadowLayerCompo &o);

    virtual ~FkShadowLayerCompo();

public:
    FkID parentLayerId = FK_ID_NONE;
};

#endif //FK_IMAGE_FKSHADOWLAYERCOMPO_H