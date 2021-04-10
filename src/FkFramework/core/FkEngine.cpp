/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkEngine.h"
#include "AlRunnable.h"

FkMessageHandlerPair::FkMessageHandlerPair(FkID what, FkMessageHandler handler)
        : FkObject(), what(what), handler(handler) {

}

FkMessageHandlerPair::FkMessageHandlerPair(const FkMessageHandlerPair &o)
        : FkObject(), what(o.what), handler(o.handler) {

}

FkMessageHandlerPair::~FkMessageHandlerPair() {
    what = 0;
    handler = nullptr;
}

bool FkMessageHandlerPair::handle(FkEngine *target, AlMessage *msg) {
    return (target->*handler)(msg);
}

const FkID FkEngine::MSG_FUTURE = FK_KID('F', 'K', 'E', 0x01);

FkEngine::FkEngine(std::string name) : FkObject(), name(std::move(name)), state(kState::IDL) {
}

FkEngine::~FkEngine() {
    std::lock_guard<std::mutex> guard(mtx);
    FkAssert(kState::IDL == state);
}

FkResult FkEngine::create() {
    auto ret = _changeState(kState::IDL, kState::CREATED);
    if (FK_OK != ret) {
        return ret;
    }
    mThread = AlHandlerThread::create(name);
    mHandler = new AlHandler(mThread->getLooper(), [this](AlMessage *msg) {
        if (msg && msg->obj && typeid(AlRunnable) == typeid(*msg->obj)) {
            AlRunnable *run = dynamic_cast<AlRunnable *>(msg->obj);
            (*run)(msg);
        }
        this->_dispatch(msg);
    });
    return FK_OK;
}

FkResult FkEngine::destroy() {
    mThread->quitSafely();
    delete mThread;
    mThread = nullptr;
    delete mHandler;
    mHandler = nullptr;
    auto ret = _changeState(kState::STOPPED, kState::IDL);
    if (FK_OK != ret) {
        return ret;
    }
    return FK_OK;
}

FkResult FkEngine::start() {
    std::lock_guard<std::mutex> guard(mtx);
    if (kState::CREATED != state && kState::STOPPED != state) {
        FkLogW(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    this->state = kState::STARTED;
    return FK_OK;
}

FkResult FkEngine::stop() {
    auto ret = _changeState(kState::STARTED, kState::STOPPED);
    if (FK_OK != ret) {
        return ret;
    }
    return FK_OK;
}

FkResult FkEngine::_changeState(kState src, kState dst) {
    std::lock_guard<std::mutex> guard(mtx);
    if (src != state) {
        FkLogW(FK_DEF_TAG, "Invalid state");
        return FK_INVALID_STATE;
    }
    this->state = dst;
    return FK_OK;
}

FkResult FkEngine::_registerMessage(FkID what, FkMessageHandler handler) {
    mMsgMap.emplace(std::make_pair(what, FkMessageHandlerPair(what, handler)));
    return FK_OK;
}

void FkEngine::_dispatch(AlMessage *msg) {
    std::lock_guard<std::mutex> guard(mtx);
    if (kState::IDL == state) {
        FkLogW(FK_DEF_TAG, "Invalid state");
        return;
    }
    auto itr = mMsgMap.find(msg->what);
    if (mMsgMap.end() != itr) {
        itr->second.handle(this, msg);
    }
}

FkResult FkEngine::sendMessage(AlMessage *message) {
    mHandler->sendMessage(message);
    return FK_OK;
}
