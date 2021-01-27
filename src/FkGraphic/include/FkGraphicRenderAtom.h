/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICRENDERATOM_H
#define FK_GRAPHIC_FKGRAPHICRENDERATOM_H

#include "FkAtom.h"

FK_CLASS FkGraphicRenderAtom FK_EXTEND FkAtom {
public:
    FkGraphicRenderAtom();

    FkGraphicRenderAtom(const FkGraphicRenderAtom &o) = delete;

    virtual ~FkGraphicRenderAtom();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);
};

#endif //FK_GRAPHIC_FKGRAPHICRENDERATOM_H
