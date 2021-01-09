/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkQuark.h"
#include "FkOnCreateProt.h"
#include "FkOnDestroyProt.h"
#include "FkOnStartProt.h"
#include "FkOnStopProt.h"

FkQuark::FkQuark() : FkObject(), state(FkQuark::kState::IDL) {
    prot = std::make_shared<FkProtDesc>(this);
    FK_PROT_DESC_ADD(prot, FkOnCreateProt, FkQuark::_onCreate);
    FK_PROT_DESC_ADD(prot, FkOnDestroyProt, FkQuark::_onDestroy);
    FK_PROT_DESC_ADD(prot, FkOnStartProt, FkQuark::_onStart);
    FK_PROT_DESC_ADD(prot, FkOnStopProt, FkQuark::_onStop);
}

FkQuark::~FkQuark() {

}

FkResult FkQuark::onCreate() {
    return FK_OK;
}

FkResult FkQuark::onDestroy() {
    return FK_OK;
}

FkResult FkQuark::onStart() {
    return FK_OK;
}

FkResult FkQuark::onStop() {
    return FK_OK;
}

FkResult FkQuark::dispatch(std::shared_ptr<FkProtocol> p) {
    if (this->state != FkQuark::kState::STARTED
        && !FK_INSTANCE_OF(p, FkOnCreateProt)
        && !FK_INSTANCE_OF(p, FkOnDestroyProt)
        && !FK_INSTANCE_OF(p, FkOnStartProt)
        && !FK_INSTANCE_OF(p, FkOnStopProt)) {
        FkLogI("FkTest", "Result FK_INVALID_STATE");
        return FK_INVALID_STATE;
    }
    auto handler = prot->find(p.get());
    if (handler) {
        return (this->*handler)(p);
    }
    return FK_FAIL;
}

FkResult FkQuark::_onCreate(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState(FkQuark::kState::IDL, FkQuark::kState::CREATED);
    if (FK_OK != ret) {
        return ret;
    }
    describeProtocols(prot);
    return onCreate();
}

FkResult FkQuark::_onDestroy(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState(FkQuark::kState::STOPPED, FkQuark::kState::IDL);
    if (FK_OK != ret) {
        return ret;
    }
    return onDestroy();
}

FkResult FkQuark::_onStart(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState(FkQuark::kState::CREATED, FkQuark::kState::STARTED);
    if (FK_OK != ret) {
        return ret;
    }
    return onStart();
}

FkResult FkQuark::_onStop(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState(FkQuark::kState::STARTED, FkQuark::kState::STOPPED);
    if (FK_OK != ret) {
        return ret;
    }
    return onStop();
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
