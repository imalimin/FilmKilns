/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERREQUESTPRT_H
#define FK_GRAPHIC_FKRENDERREQUESTPRT_H

#include "FkProtocol.h"
#include "FkRenderRequest.h"

FK_CLASS FkRenderRequestPrt FK_EXTEND FkProtocol {
public:
    FkRenderRequestPrt();

    FkRenderRequestPrt(const FkRenderRequestPrt &o);

    virtual ~FkRenderRequestPrt();

public:
    std::shared_ptr<FkRenderRequest> req = nullptr;
};


#endif //FK_GRAPHIC_FKRENDERREQUESTPRT_H
