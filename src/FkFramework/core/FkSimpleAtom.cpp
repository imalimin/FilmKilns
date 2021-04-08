/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSimpleAtom.h"

FkConnectChain::FkConnectChain() : FkObject() {

}

FkConnectChain::~FkConnectChain() {

}

void FkConnectChain::next(std::shared_ptr<FkQuark> quark) {
    chain.push_back(quark);
}

FkSimpleAtom::FkSimpleAtom() : FkAtom() {
    chain = std::make_shared<FkConnectChain>();
}

FkSimpleAtom::~FkSimpleAtom() {

}

FkResult FkSimpleAtom::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    onConnect(chain);
    return ret;
}

FkResult FkSimpleAtom::onDestroy() {
    auto ret = FkQuark::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleAtom::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleAtom::onStop() {
    auto ret = FkQuark::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}