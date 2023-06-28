/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSimpleIncrease2Atom.h"
#include "FkCalculatePrt.h"
#include "FkIncreaseQuark.h"
#include "FkDivideQuark.h"

FK_IMPL_CLASS_TYPE(FkSimpleIncrease2Atom, FkSimpleAtom)

FkSimpleIncrease2Atom::FkSimpleIncrease2Atom() : FkSimpleAtom() {

}

FkSimpleIncrease2Atom::~FkSimpleIncrease2Atom() {

}

void FkSimpleIncrease2Atom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkCalculatePrt, FkSimpleIncrease2Atom::_onIncrease2);
}

void FkSimpleIncrease2Atom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkIncreaseQuark>();
    chain->next<FkDivideQuark>();
}

FkResult FkSimpleIncrease2Atom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleIncrease2Atom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleIncrease2Atom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleIncrease2Atom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleIncrease2Atom::_onIncrease2(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculatePrt>(p);
    return dispatchNext(p);
}