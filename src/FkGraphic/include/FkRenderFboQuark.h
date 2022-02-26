/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 20:39:39
*/

#ifndef FK_GRAPHIC_FKRENDERFBOQUARK_H
#define FK_GRAPHIC_FKRENDERFBOQUARK_H

#include "FkQuark.h"
#include "FkGraphicFrameObject.h"

FK_CLASS FkRenderFboQuark FK_EXTEND FkQuark {
public:
    FkRenderFboQuark();

    virtual ~FkRenderFboQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> &p);

    FkResult _onAllocTex(std::shared_ptr<FkProtocol> &p);

private:
    std::shared_ptr<FkGraphicFBOAllocator> fboAllocator = nullptr;
};

#endif //FK_GRAPHIC_FKRENDERFBOQUARK_H