/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKDECREASEQUARK_H
#define FK_FRAMEWORK_FKDECREASEQUARK_H

#include "FkQuark.h"
#include "FkProtocol.h"

FK_CLASS FkDecreaseQuark FK_EXTEND FkQuark {
public:
    FkDecreaseQuark();

    virtual ~FkDecreaseQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkProtDesc> desc) override;

private:
    FkDecreaseQuark(const FkDecreaseQuark &o) : FkQuark() {};

private:
    FkResult _onDecrease(std::shared_ptr<FkProtocol> p);

};


#endif //FK_FRAMEWORK_FKDECREASEQUARK_H
