/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMVPQUARK_H
#define FK_GRAPHIC_FKGRAPHICMVPQUARK_H

#include "FkQuark.h"

FK_CLASS FkGraphicMVPQuark FK_EXTEND FkQuark {
public:
    FkGraphicMVPQuark();

    FkGraphicMVPQuark(const FkGraphicMVPQuark &o) = delete;

    virtual ~FkGraphicMVPQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;
};

#endif //FK_GRAPHIC_FKGRAPHICMVPQUARK_H