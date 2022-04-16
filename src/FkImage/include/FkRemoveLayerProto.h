/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-16 21:06:37
*/

#ifndef FK_IMAGE_FKREMOVELAYERPROTO_H
#define FK_IMAGE_FKREMOVELAYERPROTO_H

#include "FkLayerIdProto.h"

FK_CLASS FkRemoveLayerProto FK_EXTEND FkLayerIdProto {
public:
    FkRemoveLayerProto();

    FkRemoveLayerProto(FkID layerId);

    FkRemoveLayerProto(const FkRemoveLayerProto &o);

    virtual ~FkRemoveLayerProto();
};

#endif //FK_IMAGE_FKREMOVELAYERPROTO_H