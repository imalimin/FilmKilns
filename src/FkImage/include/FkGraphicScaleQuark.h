/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICSCALEQUARK_H
#define FK_GRAPHIC_FKGRAPHICSCALEQUARK_H

#include "FkQuark.h"

FK_CLASS FkGraphicScaleQuark FK_EXTEND FkQuark {
public:
    FkGraphicScaleQuark();

    FkGraphicScaleQuark(const FkGraphicScaleQuark &o) = delete;

    virtual ~FkGraphicScaleQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

};


#endif //FK_GRAPHIC_FKGRAPHICSCALEQUARK_H
