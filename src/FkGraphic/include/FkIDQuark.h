/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-21 23:50:21
*/

#ifndef FK_GRAPHIC_FKIDQUARK_H
#define FK_GRAPHIC_FKIDQUARK_H

#include "FkQuark.h"

FK_CLASS FkIDQuark FK_EXTEND FkQuark {
public:
    FkIDQuark();

    FkIDQuark(const FkIDQuark &o) = delete;

    virtual ~FkIDQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onGenID(std::shared_ptr<FkProtocol> p);

private:
    std::mutex mtx;
    FkID next = FK_ID_NONE;
    std::list<FkID> ids;
};

#endif //FK_GRAPHIC_FKIDQUARK_H