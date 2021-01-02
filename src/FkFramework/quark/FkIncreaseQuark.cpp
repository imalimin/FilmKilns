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
    desc->add(std::static_pointer_cast<FkProtocol>(std::make_shared<FkCalculateProt>()),
              reinterpret_cast<FkQuark::ProtHandler>(&FkIncreaseQuark::_onIncrease));

}

FkResult FkIncreaseQuark::_onIncrease(std::shared_ptr<FkProtocol> p) {
    return FK_FAIL;
}