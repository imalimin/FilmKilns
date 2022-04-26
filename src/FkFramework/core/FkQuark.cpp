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

#define TAG "FkQuark"

FkQuark::FkQuark() : FkObject(), state(kState::IDL) {
    FK_MARK_SUPER
    desc = std::make_shared<FkPortDesc>();
    FK_PORT_DESC_QUICK_ADD(desc, FkOnCreatePrt, FkQuark::_onCreate);
    FK_PORT_DESC_QUICK_ADD(desc, FkOnDestroyPrt, FkQuark::_onDestroy);
    FK_PORT_DESC_QUICK_ADD(desc, FkOnStartPrt, FkQuark::_onStart);
    FK_PORT_DESC_QUICK_ADD(desc, FkOnStopPrt, FkQuark::_onStop);
}

FkQuark::~FkQuark() {

}

std::shared_ptr<FkQuarkContext> FkQuark::getContext() {
    return context;
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
    if (this->state != kState::RUNNING
        && !FK_INSTANCE_OF(p, FkOnCreatePrt)
        && !FK_INSTANCE_OF(p, FkOnDestroyPrt)
        && !FK_INSTANCE_OF(p, FkOnStartPrt)
        && !FK_INSTANCE_OF(p, FkOnStopPrt)) {
        FkLogE(FK_DEF_TAG, "Quark(%s) dispatch protocol(%s) on invalid state(%d)", typeid(*this).name(), typeid(*p).name(), this->state);
        return FK_INVALID_STATE;
    }
    auto port = desc->find(p->getType());
    if (nullptr != port) {
        return port->chat(this, p);
    }
    FkLogE(FK_DEF_TAG, "Quark(%s) protocol(%s) not found", getClassType().getName().c_str(), p->getClassType().getName().c_str());
    return FK_PORT_NOT_FOUND;
}

FkResult FkQuark::_onCreate(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState((uint32_t) kState::IDL, kState::CREATED);
    if (FK_OK != ret) {
        FkLogE(FK_DEF_TAG, "Quark(%s) create failed with %d", getClassType().getName().c_str(), ret);
        return ret;
    }
    auto proto = std::dynamic_pointer_cast<FkOnCreatePrt>(p);
    if (proto) {
        context = proto->context;
    }
    describeProtocols(desc);
    return onCreate();
}

FkResult FkQuark::_onDestroy(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState((uint32_t) kState::STOPPED, kState::IDL);
    if (FK_OK != ret) {
        return ret;
    }
    return onDestroy();
}

FkResult FkQuark::_onStart(std::shared_ptr<FkProtocol> p) {
    auto from = (uint32_t) kState::CREATED | (uint32_t) kState::STOPPED;
    auto ret = _changeState(from, kState::RUNNING);
    if (FK_OK != ret) {
        return ret;
    }
    return onStart();
}

FkResult FkQuark::_onStop(std::shared_ptr<FkProtocol> p) {
    auto ret = _changeState((uint32_t) kState::RUNNING, kState::STOPPED);
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

FkResult FkQuark::accept(const size_t protoType) {
    if (nullptr != desc->find(protoType)) {
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkQuark::_changeState(uint32_t src, kState dst) {
    std::lock_guard<std::mutex> guard(mtx);
    if (src & ((uint32_t) this->state)) {
        this->state = dst;
        return FK_OK;
    }
    FkLogW(FK_DEF_TAG, "Invalid state");
    return FK_INVALID_STATE;
}

FkResult FkQuark::queryProtocols(std::list<std::shared_ptr<FkProtocol>> &protocols) {
    return desc->query(protocols);
}
