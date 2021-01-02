/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkHelloQuark.h"
#include "FkHelloProt.h"

FkHelloQuark::FkHelloQuark() : FkQuark() {

}

FkHelloQuark::~FkHelloQuark() {

}

void FkHelloQuark::describeProtocols(std::shared_ptr<FkProtDesc> desc) {
    desc->add(std::static_pointer_cast<FkProtocol>(std::make_shared<FkHelloProt>()),
              reinterpret_cast<FkQuark::ProtHandler>(&FkHelloQuark::_onHello));
}

int FkHelloQuark::_onHello(std::shared_ptr<FkProtocol> p) {
    return 0;
}

int FkHelloQuark::_onSay(std::shared_ptr<FkProtocol> p) {
    return 0;
}
