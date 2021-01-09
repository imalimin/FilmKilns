/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkDecreaseQuark.h"
#include "FkCalculatePrt.h"

FkDecreaseQuark::FkDecreaseQuark() : FkQuark() {

}

FkDecreaseQuark::~FkDecreaseQuark() {

}

void FkDecreaseQuark::describeProtocols(std::shared_ptr<FkProtDesc> desc) {
    desc->add(std::static_pointer_cast<FkProtocol>(std::make_shared<FkCalculatePrt>()),
              reinterpret_cast<FkQuark::ProtHandler>(&FkDecreaseQuark::_onDecrease));

}

FkResult FkDecreaseQuark::_onDecrease(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculatePrt>(p);
    cp->number -= 1;
    return FK_OK;
}