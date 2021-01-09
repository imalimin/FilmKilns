/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKCALCULATEADDQUARK_H
#define FK_FRAMEWORK_FKCALCULATEADDQUARK_H

#include "FkQuark.h"
#include "FkProtocol.h"

FK_CLASS FkIncreaseQuark FK_EXTEND FkQuark {
public:
    FkIncreaseQuark();

    virtual ~FkIncreaseQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkProtDesc> desc) override;

private:
    FkIncreaseQuark(const FkIncreaseQuark &o) : FkQuark() {};

private:
    FkResult _onIncrease(std::shared_ptr<FkProtocol> p);

};


#endif //FK_FRAMEWORK_FKCALCULATEADDQUARK_H
