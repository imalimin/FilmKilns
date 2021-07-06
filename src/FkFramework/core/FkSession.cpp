/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSession.h"

std::shared_ptr<FkSession> FkSession::with(std::shared_ptr<FkProtocol> p) {
    auto session = std::make_shared<FkSession>();
    session->classType = std::make_shared<FkClassType>(p->getClassType());
    session->protoType = p->getType();
    return session;
}

FkSession::FkSession() : FkObject() {

}

FkSession::~FkSession() {
    if (FkSession::kState::IDL != state) {
        FkLogE(FK_DEF_TAG, "Session(%s): Close before delete please.", classType->getName().c_str());
    }
}

FkResult FkSession::connectTo(const std::shared_ptr<FkQuark> quark) {
    std::lock_guard<std::mutex> guard(mtx);
    if (FkSession::kState::IDL != state) {
        FkLogE(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    if (nullptr == quark) {
        return FK_FAIL;
    }
    if (FK_OK == quark->accept(protoType)) {
        link.emplace_back(quark);
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkSession::disconnect(const std::shared_ptr<FkQuark> quark) {
    std::lock_guard<std::mutex> guard(mtx);
    if (FkSession::kState::IDL != state) {
        FkLogE(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    if (nullptr == quark) {
        return FK_FAIL;
    }
    auto itr = link.begin();
    while (link.end() != itr) {
        if ((*itr) == quark) {
            FkLogI(FK_DEF_TAG, "Disconnect quark %p", (void *) quark.get());
            link.erase(itr);
            return FK_OK;
        }
        ++itr;
    }
    return FK_FAIL;
}

FkResult FkSession::open() {
    std::lock_guard<std::mutex> guard(mtx);
    if (FkSession::kState::IDL != state) {
        FkLogE(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    if (link.empty()) {
        FkLogE(FK_DEF_TAG, "Session(%s): Connect quark first.", classType->getName().c_str());
        return FK_EMPTY_DATA;
    }
    state = FkSession::kState::OPENED;
    return FK_OK;
}

FkResult FkSession::close() {
    std::lock_guard<std::mutex> guard(mtx);
    if (FkSession::kState::OPENED != state) {
        FkLogE(FK_DEF_TAG, "Session(%s) close failed. Invalid state", classType->getName().c_str());
        return FK_INVALID_STATE;
    }
    link.clear();
    state = FkSession::kState::IDL;
    return FK_OK;
}

FkResult FkSession::send(std::shared_ptr<FkProtocol> protocol) {
    if (nullptr == protocol) {
        return FK_INVALID_DATA;
    }
    if (protocol->getClassType() != (*classType)) {
        return FK_PROTOCOL_NOT_ACCEPT;
    }
    {
        std::lock_guard<std::mutex> guard(mtx);
        if (FkSession::kState::OPENED != state) {
            FkLogE(FK_DEF_TAG, "Invalid state");
            return FK_INVALID_STATE;
        }
    }
    if (link.empty()) {
        return FK_FAIL;
    }
    FkResult ret = FK_FAIL;
    for (auto &it : link) {
        ret = it->dispatch(protocol);
        if (FK_SKIP == ret) {
            FkLogE(FK_DEF_TAG, "Session(%s Quark(%s)) skip.", protocol->getClassType().getName().c_str(), it->getClassType().getName().c_str());
            continue;
        }
        if (FK_OK != ret) {
            FkLogE(FK_DEF_TAG, "Session(%s Quark(%s)) send fail.", protocol->getClassType().getName().c_str(), it->getClassType().getName().c_str());
            return ret;
        }
    }
    return ret;
}

std::string FkSession::toString() {
    return classType->toString();
}
