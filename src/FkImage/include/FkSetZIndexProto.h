/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-02 21:32:22
*/

#ifndef FK_IMAGE_FKSETZINDEXPROTO_H
#define FK_IMAGE_FKSETZINDEXPROTO_H

#include "FkProtocol.h"

FK_SUPER_CLASS(FkSetZIndexProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkSetZIndexProto)

public:
    FkSetZIndexProto(FkID layerId, int32_t zIndex);

    FkSetZIndexProto(const FkSetZIndexProto &o);

    virtual ~FkSetZIndexProto();

public:
    FkID layerId;
    int32_t zIndex;
};

#endif //FK_IMAGE_FKSETZINDEXPROTO_H