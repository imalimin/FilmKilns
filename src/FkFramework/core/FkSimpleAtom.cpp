/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSimpleAtom.h"

FkConnectChain::FkConnectChain() : FkObject() {
    FK_MARK_SUPER

}

FkConnectChain::~FkConnectChain() {

}

void FkConnectChain::_next(std::shared_ptr<FkQuark> quark) {
    chain.emplace_back(quark);
    rChain.emplace_front(quark);
}

std::shared_ptr<FkSession> FkConnectChain::connectSession(std::shared_ptr<FkProtocol> p, bool reverse) {
    auto session = FkSession::with(p);
    for (auto &it : reverse ? rChain : chain) {
        FkResult ret = session->connectTo(it);
        if (FK_OK != ret) {
            FkLogW(FK_DEF_TAG, "Skip session(%s) connect(%s), ret=%d",
                   p->getClassType().getName().c_str(), it->getClassType().getName().c_str(), ret);
        }
    }
    return session;
}

bool FkConnectChain::empty() {
    return chain.empty();
}

void FkConnectChain::clear() {
    rChain.clear();
    chain.clear();
}

FkSimpleAtom::FkSimpleAtom() : FkAtom() {
    FK_MARK_SUPER
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
    std::list<std::shared_ptr<FkProtocol>> protocols;
    protocols.emplace_back(std::make_shared<FkOnCreatePrt>());
    protocols.emplace_back(std::make_shared<FkOnStartPrt>());
    protocols.emplace_back(std::make_shared<FkOnStopPrt>());
    protocols.emplace_back(std::make_shared<FkOnDestroyPrt>());
    for (auto &it : protocols) {
        if (mSessionMap.end() != mSessionMap.find(it->getType())) {
            continue;
        }
        bool reverse = FK_INSTANCE_OF(it, FkOnStopPrt) || FK_INSTANCE_OF(it, FkOnDestroyPrt);
        auto session = chain->connectSession(it, reverse);
        FkResult ret = session->open();
        if (FK_OK == ret) {
            mSessionMap.emplace(std::make_pair(it->getType(), session));
        } else {
            FkLogW(FK_DEF_TAG, "Session(%s, Atom(%s)) open failed, ret=%d",
                   it->getClassType().getName().c_str(), getClassType().getName().c_str(), ret);
        }
    }
}

void FkSimpleAtom::_connectSession() {
    std::list<std::shared_ptr<FkProtocol>> protocols;
    if (FK_OK == queryProtocols(protocols)) {
        for (auto &it : protocols) {
            if (mSessionMap.end() != mSessionMap.find(it->getType())) {
                continue;
            }
            auto session = chain->connectSession(it);
            FkResult ret = session->open();
            if (FK_OK == ret) {
                mSessionMap.emplace(std::make_pair(it->getType(), session));
            } else {
                FkLogW(FK_DEF_TAG, "Session(%s, Atom(%s)) open failed, ret=%d",
                       it->getClassType().getName().c_str(), getClassType().getName().c_str(), ret);
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
    FkLogW(FK_DEF_TAG, "Atom(%s) can not find session or session map is empty.", getClassType().getName().c_str());
    return FK_SKIP;
}