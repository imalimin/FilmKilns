/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkIncreaseQuark.h"
#include "FkCalculatePrt.h"

FK_IMPL_CLASS_TYPE(FkIncreaseQuark, FkQuark)

FkIncreaseQuark::FkIncreaseQuark() : FkQuark() {

}

FkIncreaseQuark::~FkIncreaseQuark() {

}

void FkIncreaseQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkCalculatePrt, FkIncreaseQuark::_onIncrease);
}

FkResult FkIncreaseQuark::_onIncrease(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculatePrt>(p);
    cp->number += 1;
    cp->number += 1;
    cp->number += 1;
    return FK_OK;
}