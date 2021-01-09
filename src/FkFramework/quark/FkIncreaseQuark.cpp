/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkIncreaseQuark.h"
#include "FkCalculateProt.h"

FkIncreaseQuark::FkIncreaseQuark() : FkQuark() {

}

FkIncreaseQuark::~FkIncreaseQuark() {

}

void FkIncreaseQuark::describeProtocols(std::shared_ptr<FkProtDesc> desc) {
    FK_PROT_DESC_ADD(desc, FkCalculateProt, FkIncreaseQuark::_onIncrease);
}

FkResult FkIncreaseQuark::_onIncrease(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculateProt>(p);
    cp->number += 1;
    cp->number += 1;
    cp->number += 1;
    return FK_OK;
}