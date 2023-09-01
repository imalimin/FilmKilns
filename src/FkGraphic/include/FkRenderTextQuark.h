/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 13:32:27
*/

#ifndef FK_GRAPHIC_FKRENDERTEXTQUARK_H
#define FK_GRAPHIC_FKRENDERTEXTQUARK_H

#include "FkQuark.h"
#include "FkTextureCharMap.h"

FK_SUPER_CLASS(FkRenderTextQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderTextQuark)

public:
    FkRenderTextQuark();

    FkRenderTextQuark(const FkRenderTextQuark &o) = delete;

    virtual ~FkRenderTextQuark();

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

#endif //FK_GRAPHIC_FKRENDERTEXTQUARK_H