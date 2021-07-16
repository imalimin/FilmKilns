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

FK_CLASS FkWinModelQuark FK_EXTEND FkQuark {
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
    FkSize winSize;
};

#endif //FK_GRAPHIC_FKWINMODELQUARK_H