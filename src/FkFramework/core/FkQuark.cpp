/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkQuark.h"
#include "FkOnCreatePrt.h"
#include "FkOnDestroyPrt.h"
#include "FkOnStartPrt.h"
#include "FkOnStopPrt.h"

FkQuark::FkQuark() : FkObject(), state(FkQuark::kState::IDL) {
    desc = std::make_shared<FkPortDesc>();
    FK_PORT_DESC_QUICK_ADD(desc, FkOnCreatePrt, FkQuark::_onCreate);
    FK_PORT_DESC_QUICK_ADD(desc, FkOnDestroyPrt, FkQuark::_onDestroy);
    FK_PORT_DESC_QUICK_ADD(desc, FkOnStartPrt, FkQuark::_onStart);
    FK_PORT_DESC_QUICK_ADD(desc, FkOnStopPrt, FkQuark::_onStop);
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
        && !FK_INSTANCE_OF(p, FkOnCreatePrt)
        && !FK_INSTANCE_OF(p, FkOnDestroyPrt)
        && !FK_INSTANCE_OF(p, FkOnStartPrt)
        && !FK_INSTANCE_OF(p, FkOnStopPrt)) {
        FkLogI("FkTest", "Result FK_INVALID_STATE");
        return FK_INVALID_STATE;
    }
    auto port = desc->find(p->getType());
    if (nullptr != port) {
        return port->communicate(this, p);
    }
    return FK_FAIL;
}

FkResult FkQuark::_onCreate(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState(FkQuark::kState::IDL, FkQuark::kState::CREATED);
    if (FK_OK != ret) {
        return ret;
    }
    describeProtocols(desc);
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
    if (nullptr != desc->find(p->getType())) {
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
