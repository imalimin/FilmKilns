/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKWINMODELQUARK_H
#define FK_GRAPHIC_FKWINMODELQUARK_H

#include "FkQuark.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkWinModelQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkWinModelQuark)

public:
    FkWinModelQuark();

    FkWinModelQuark(const FkWinModelQuark &o) = delete;

    virtual ~FkWinModelQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onSetViewSize(std::shared_ptr<FkProtocol> p);

    FkResult _onWithWinSize(std::shared_ptr<FkProtocol> p);

private:
    FkSize winSize;
};

#endif //FK_GRAPHIC_FKWINMODELQUARK_H