/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSession.h"

FK_IMPL_CLASS_TYPE(FkSession, FkObject)

std::shared_ptr<FkSession> FkSession::with(FkProtocol::Desc protoDesc) {
    auto session = std::make_shared<FkSession>();
    session->protoDesc = protoDesc;
    return session;
}

FkSession::FkSession() : FkObject() {

}

FkSession::~FkSession() {
    if (FkSession::kState::IDL != state) {
        FkLogE(FK_DEF_TAG, "Session(%s): Close before delete please.", protoDesc.name);
    }
}

FkResult FkSession::connectTo(const std::shared_ptr<FkQuark> quark) {
    std::lock_guard<std::mutex> guard(mtx);
    if (FkSession::kState::IDL != state) {
        FkLogE(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    if (nullptr == quark) {
        FkLogE(FK_DEF_TAG, "Quark is null.");
        return FK_FAIL;
    }
    if (FK_OK == quark->accept(protoDesc.type)) {
        link.emplace_back(quark);
        return FK_OK;
    }
    FkLogD(FK_DEF_TAG, "Connect to %s failed. Can not accept this protocol(%s).", quark->getClassType().getName(), protoDesc.name);
    return FK_PROTOCOL_NOT_ACCEPT;
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
        FkLogE(FK_DEF_TAG, "Session(%s): Connect quark first.", getClassType().getName());
        return FK_EMPTY_DATA;
    }
    state = FkSession::kState::OPENED;
    return FK_OK;
}

FkResult FkSession::close() {
    std::lock_guard<std::mutex> guard(mtx);
    if (FkSession::kState::OPENED != state) {
        FkLogE(FK_DEF_TAG, "Session(%s) close failed. Invalid state", protoDesc.name);
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
    if (protocol->getClassType().getId() != protoDesc.type) {
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
    for (auto &quark : link) {
        ret = quark->dispatch(protocol);
        if (FK_SKIP == ret || FK_DO_NOTHING== ret) {
            FkLogD(FK_DEF_TAG, "%s`s session on %s is skipped.", protocol->getClassType().getName(), quark->getClassType().getName());
            continue;
        }
        if (FK_DONE == ret) {
            FkLogD(FK_DEF_TAG, "%s`s session on %s is done.", protocol->getClassType().getName(), quark->getClassType().getName());
            return FK_OK;
        }
        if (FK_OK != ret) {
            FkLogE(FK_DEF_TAG, "%s`s session on %s process failed.", protocol->getClassType().getName(), quark->getClassType().getName());
            return ret;
        }
    }
    return ret;
}

std::string FkSession::toString() {
    return std::string(protoDesc.name);
}
