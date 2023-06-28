/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERREQUESTPRT_H
#define FK_GRAPHIC_FKRENDERREQUESTPRT_H

#include "FkWinSizeProto.h"
#include "FkRenderRequest.h"

FK_SUPER_CLASS(FkRenderRequestPrt, FkWinSizeProto) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderRequestPrt)

public:
    FkRenderRequestPrt();

    FkRenderRequestPrt(const FkRenderRequestPrt &o);

    virtual ~FkRenderRequestPrt();

public:
    std::shared_ptr<FkRenderRequest> req = nullptr;
};


#endif //FK_GRAPHIC_FKRENDERREQUESTPRT_H
