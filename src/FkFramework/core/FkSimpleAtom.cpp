/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSimpleAtom.h"

FK_IMPL_CLASS_TYPE(FkConnectChain, FkObject)
FK_IMPL_CLASS_TYPE(FkSimpleAtom, FkAtom)

FkConnectChain::FkConnectChain() : FkObject() {


}

FkConnectChain::~FkConnectChain() {

}

void FkConnectChain::_next(std::shared_ptr<FkQuark> quark) {
    chain.emplace_back(quark);
    rChain.emplace_front(quark);
}

std::shared_ptr<FkSession> FkConnectChain::connectSession(FkProtocol::Desc protoDesc, bool reverse) {
    auto session = FkSession::with(protoDesc);
    int count = 0;
    for (auto &it : reverse ? rChain : chain) {
        FkResult ret = session->connectTo(it);
        if (FK_OK == ret) {
            ++count;
        }
    }
    return count > 0 ? session : nullptr;
}

bool FkConnectChain::findAllProtocols(std::list<FkProtocol::Desc> &protocols) {
    for (auto &quark : chain) {
        quark->queryProtocols(protocols);
    }
    return true;
}

bool FkConnectChain::empty() {
    return chain.empty();
}

void FkConnectChain::clear() {
    rChain.clear();
    chain.clear();
}

FkSimpleAtom::FkSimpleAtom() : FkAtom() {

    chain = std::make_shared<FkConnectChain>();
}

FkSimpleAtom::~FkSimpleAtom() {

}

FkResult FkSimpleAtom::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    client = std::make_shared<FkLocalClient>();
    onConnect(chain);
    if (chain->empty()) {
        FkLogE(FK_DEF_TAG, "Chain is empty. Connect quark on onConnect first.");
        return ret;
    }
    /// Connect create/destroy/start/stop.
    _connectBaseSession();
    auto proto = std::make_shared<FkOnCreatePrt>();
    proto->context = getContext();
    ret = dispatchNext(proto);
    _prepareDeliveryProtocols();
    /// Connect left protocols.
    _connectSession();
    return ret;
}

FkResult FkSimpleAtom::onDestroy() {
    auto ret = FkAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    ret = dispatchNext(std::make_shared<FkOnDestroyPrt>());
    _disconnectSession();
    chain->clear();
    return ret;
}

FkResult FkSimpleAtom::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    ret = dispatchNext(std::make_shared<FkOnStartPrt>());
    return ret;
}

FkResult FkSimpleAtom::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    ret = dispatchNext(std::make_shared<FkOnStopPrt>());
    return ret;
}

void FkSimpleAtom::_connectBaseSession() {
    std::list<FkProtocol::Desc> protocols;
    FkProtocol::Desc desc = {FkOnCreatePrt_Class::type.getId(),
                             FkOnCreatePrt_Class::type.getName()};
    protocols.emplace_back(desc);
    desc = {FkOnStartPrt_Class::type.getId(), FkOnStartPrt_Class::type.getName()};
    protocols.emplace_back(desc);
    desc = {FkOnStopPrt_Class::type.getId(), FkOnStopPrt_Class::type.getName()};
    protocols.emplace_back(desc);
    desc = {FkOnDestroyPrt_Class::type.getId(), FkOnDestroyPrt_Class::type.getName()};
    protocols.emplace_back(desc);
    for (auto &it : protocols) {
        if (mSessionMap.end() != mSessionMap.find(it.type)) {
            continue;
        }
        bool reverse = it.type == FkOnStopPrt_Class::type.getId()
                       || it.type == FkOnDestroyPrt_Class::type.getId();
        auto session = chain->connectSession(it, reverse);
        FkResult ret = session->open();
        if (FK_OK == ret) {
            mSessionMap.emplace(std::make_pair(it.type, session));
        } else {
            FkLogW(FK_DEF_TAG, "Session(%s, Atom(%s)) open failed, ret=%d",
                   it.name, getClassType().getName(), ret);
        }
    }
}

void FkSimpleAtom::_connectSession() {
    std::list<FkProtocol::Desc> protocols;
    if (FK_OK == queryProtocols(protocols)) {
        for (auto &protoDesc : protocols) {
            if (mSessionMap.end() != mSessionMap.find(protoDesc.type)) {
                continue;
            }
            auto session = chain->connectSession(protoDesc);
            if (session == nullptr) {
                FkLogD(FK_DEF_TAG, "Skip session on %s. No quark can accept this protocol",
                       getClassType().getName());
                continue;
            }
            FkResult ret = session->open();
            if (FK_OK == ret) {
                mSessionMap.emplace(std::make_pair(protoDesc.type, session));
            } else {
                FkLogW(FK_DEF_TAG, "%s session on %s open failed, ret=%d", getClassType().getName(), ret);
            }
        }
    }
}

void FkSimpleAtom::_disconnectSession() {
    for (auto &it : mSessionMap) {
        FkResult ret = it.second->close();
        if (FK_OK != ret) {
            FkLogW(FK_DEF_TAG, "Session close failed, ret=%d", it.second->toString().c_str(), ret);
        }
    }
}

FkResult FkSimpleAtom::dispatchNext(std::shared_ptr<FkProtocol> p) {
    auto itr = mSessionMap.find(p->getType());
    if (mSessionMap.end() != itr) {
        return client->send(itr->second, p);
    }
    FkLogW(FK_DEF_TAG, "Atom(%s) can not find session or session map is empty.", getClassType().getName());
    return FK_SKIP;
}

void FkSimpleAtom::_prepareDeliveryProtocols() {
    std::list<FkProtocol::Desc> protocols;
    if (chain->findAllProtocols(protocols)) {
        for (auto &protoDesc : protocols) {
            if (FK_OK != accept(protoDesc.type)) {
                addPort(0, protoDesc, reinterpret_cast<FkPort::PortFunc>(&FkSimpleAtom::dispatchNext));
            }
        }
    }
}
