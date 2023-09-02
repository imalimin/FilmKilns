/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-02 00:00:04
*/

#ifndef FK_GRAPHIC_FKRENDERSOURCETEXTQUARK_H
#define FK_GRAPHIC_FKRENDERSOURCETEXTQUARK_H

#include "FkQuark.h"
#include "FkTextureCharMap.h"

FK_SUPER_CLASS(FkRenderSourceTextQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderSourceTextQuark)

public:
    FkRenderSourceTextQuark();

    FkRenderSourceTextQuark(const FkRenderSourceTextQuark &o) = delete;

    virtual ~FkRenderSourceTextQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRender(const std::shared_ptr<FkProtocol> &p);

private:
    std::shared_ptr<FkTextureCharMap> mTexCharMap = nullptr;
};

#endif //FK_GRAPHIC_FKRENDERSOURCETEXTQUARK_H