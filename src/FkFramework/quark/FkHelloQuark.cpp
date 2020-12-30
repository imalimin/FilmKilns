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

void FkHelloQuark::describeProtocols(std::shared_ptr<FkProtDesc> prot) {
    FkHelloProt p0;
    FkHelloProt p1;
    if(FK_CLASS_TYPE_EQUALS(p0, p1)) {

    }
//    prot->add(std::make_shared<FkHelloProt>(), reinterpret_cast<ProtHandler>(FkHelloQuark::_onHello));
}

int FkHelloQuark::_onHello(FkProtocol *p) {
    return 0;
}
