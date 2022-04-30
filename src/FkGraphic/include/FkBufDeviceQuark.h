/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 12:42:33
*/

#ifndef FK_GRAPHIC_FKBUFDEVICEQUARK_H
#define FK_GRAPHIC_FKBUFDEVICEQUARK_H

#include "FkQuark.h"

FK_SUPER_CLASS(FkBufDeviceQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkBufDeviceQuark)

public:
    FkBufDeviceQuark();

    virtual ~FkBufDeviceQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> p);
};

#endif //FK_GRAPHIC_FKBUFDEVICEQUARK_H