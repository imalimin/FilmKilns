/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTEXQUARK_H
#define FK_GRAPHIC_FKGRAPHICTEXQUARK_H

#include "FkQuark.h"
#include "FkGraphicTexture.h"
#include <unordered_map>

FK_CLASS FkGraphicTexQuark FK_EXTEND FkQuark {
public:
    FkGraphicTexQuark();

    FkGraphicTexQuark(const FkGraphicTexQuark &o) = delete;

    virtual ~FkGraphicTexQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

    FkResult _onAllocTex(std::shared_ptr<FkProtocol> p);

    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkGraphicAllocator> allocator = nullptr;
    std::unordered_map<FkID, std::shared_ptr<FkGraphicTexture>> sMap;
};


#endif //FK_GRAPHIC_FKGRAPHICTEXQUARK_H
