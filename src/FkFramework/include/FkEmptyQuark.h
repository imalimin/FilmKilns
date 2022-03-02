/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 23:50:58
*/

#ifndef FK_FRAMEWORK_FKEMPTYQUARK_H
#define FK_FRAMEWORK_FKEMPTYQUARK_H

#include "FkQuark.h"

FK_CLASS FkEmptyQuark FK_EXTEND FkQuark {
public:
    FkEmptyQuark();

    virtual ~FkEmptyQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;
};

#endif //FK_FRAMEWORK_FKEMPTYQUARK_H