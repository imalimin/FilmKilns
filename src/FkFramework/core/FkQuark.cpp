/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkQuark.h"

FkQuark::FkQuark() : FkObject(), state(FkQuark::kState::IDL) {
    prot = std::make_shared<FkProtDesc>(this);
}

FkQuark::~FkQuark() {

}

FkResult FkQuark::onCreate() {
    auto ret = _changeState(FkQuark::kState::IDL, FkQuark::kState::CREATED);
    if (FK_OK != ret) {
        return ret;
    }
    describeProtocols(prot);
    return FK_OK;
}

FkResult FkQuark::onDestroy() {
    auto ret = _changeState(FkQuark::kState::STOPPED, FkQuark::kState::IDL);
    if (FK_OK != ret) {
        return ret;
    }
    return FK_OK;
}

FkResult FkQuark::onStart() {
    auto ret = _changeState(FkQuark::kState::CREATED, FkQuark::kState::STARTED);
    if (FK_OK != ret) {
        return ret;
    }
    return FK_OK;
}

FkResult FkQuark::onStop() {
    auto ret = _changeState(FkQuark::kState::STARTED, FkQuark::kState::STOPPED);
    if (FK_OK != ret) {
        return ret;
    }
    return FK_OK;
}

FkResult FkQuark::dispatch(std::shared_ptr<FkProtocol> p) {
    auto handler = prot->find(p.get());
    if (handler) {
        return (this->*handler)(p);
    }
    return FK_FAIL;
}

FkResult FkQuark::accept(const std::shared_ptr<FkProtocol> p) {
    if (prot->find(p.get())) {
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkQuark::_changeState(kState src, kState dst) {
    std::lock_guard<std::mutex> guard(mtx);
    if (src != state) {
        FkLogW(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    this->state = dst;
    return FK_OK;
}
