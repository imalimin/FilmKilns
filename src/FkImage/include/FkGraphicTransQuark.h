/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTRANSQUARK_H
#define FK_GRAPHIC_FKGRAPHICTRANSQUARK_H

#include "FkQuark.h"

FK_CLASS FkGraphicTransQuark FK_EXTEND FkQuark {
public:
    FkGraphicTransQuark();

    FkGraphicTransQuark(const FkGraphicTransQuark &o) = delete;

    virtual ~FkGraphicTransQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

};


#endif //FK_GRAPHIC_FKGRAPHICTRANSQUARK_H
