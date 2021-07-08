/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKPRERENDERQUARK_H
#define FK_GRAPHIC_FKPRERENDERQUARK_H

#include "FkQuark.h"

FK_CLASS FkPreRenderQuark FK_EXTEND FkQuark {
public:
    FkPreRenderQuark();

    FkPreRenderQuark(const FkPreRenderQuark &o) = delete;

    virtual ~FkPreRenderQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

private:
    uint32_t vbo = 0;
};

#endif //FK_GRAPHIC_FKPRERENDERQUARK_H