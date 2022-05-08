/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 15:54:05
*/

#ifndef FK_GRAPHIC_FKRENDERPATHQUARK_H
#define FK_GRAPHIC_FKRENDERPATHQUARK_H

#include "FkQuark.h"

FK_SUPER_CLASS(FkRenderPathQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderPathQuark)

public:
    FkRenderPathQuark();

    FkRenderPathQuark(const FkRenderPathQuark &o) = delete;

    virtual ~FkRenderPathQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> &p);
};

#endif //FK_GRAPHIC_FKRENDERPATHQUARK_H