/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 23:17:54
*/

#ifndef FK_GRAPHIC_FKRENDERINFOQUARK_H
#define FK_GRAPHIC_FKRENDERINFOQUARK_H

#include "FkQuark.h"
#include "FkSize.h"
#include <unordered_map>

FK_CLASS FkRenderInfoQuark FK_EXTEND FkQuark {
public:
    FkRenderInfoQuark();

    virtual ~FkRenderInfoQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onSaveMaterialSize(std::shared_ptr<FkProtocol> p);

    FkResult _onRender(std::shared_ptr<FkProtocol> &p);

private:
    std::unordered_map<FkID, FkSize> sMap;
};

#endif //FK_GRAPHIC_FKRENDERINFOQUARK_H