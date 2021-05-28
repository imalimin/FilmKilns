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
    FK_MARK_SUPER
}

FkMessageHandlerPair::FkMessageHandlerPair(const FkMessageHandlerPair &o)
        : FkObject(), what(o.what), handler(o.handler) {

}

FkMessageHandlerPair::~FkMessageHandlerPair() {
    what = 0;
    handler = nullptr;
}

bool FkMessageHandlerPair::handle(FkEngine *target, std::shared_ptr<FkMessage> msg) {
    return (target->*handler)(msg);
}

const FkID FkEngine::FK_MSG_CREATE = FK_KID('F', 'K', 'E', 0x01);
const FkID FkEngine::FK_MSG_DESTROY = FK_KID('F', 'K', 'E', 0x02);
const FkID FkEngine::FK_MSG_START = FK_KID('F', 'K', 'E', 0x03);
const FkID FkEngine::FK_MSG_STOP = FK_KID('F', 'K', 'E', 0x04);

FkEngine::FkEngine(std::string name) : FkObject(), name(std::move(name)), state(kState::IDL) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_CREATE, FkEngine::_onCreate);
    FK_REG_MSG(FK_MSG_DESTROY, FkEngine::_onDestroy);
    FK_REG_MSG(FK_MSG_START, FkEngine::_onStart);
    FK_REG_MSG(FK_MSG_STOP, FkEngine::_onStop);
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
    mThread = FkHandlerThread::create(name);
    mHandler = new FkHandler(mThread->getLooper(), [this](std::shared_ptr<FkMessage> msg) {
        this->_dispatch(std::move(msg));
    });
    sendMessage(FkMessage::obtain(FK_MSG_CREATE));
    return FK_OK;
}

FkResult FkEngine::destroy() {
    sendMessage(FkMessage::obtain(FK_MSG_DESTROY));
    mThread->quitSafely();
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
    sendMessage(FkMessage::obtain(FK_MSG_START));
    return FK_OK;
}

FkResult FkEngine::stop() {
    auto ret = _changeState(kState::STARTED, kState::STOPPED);
    if (FK_OK != ret) {
        return ret;
    }
    sendMessage(FkMessage::obtain(FK_MSG_STOP));
    return FK_OK;
}

FkResult FkEngine::onCreate() {
    return FK_OK;
}

FkResult FkEngine::onDestroy() {
    return FK_OK;
}

FkResult FkEngine::onStart() {
    return FK_OK;
}

FkResult FkEngine::onStop() {
    return FK_OK;
}

FkResult FkEngine::_onCreate(std::shared_ptr<FkMessage> msg) {
    return onCreate();
}

FkResult FkEngine::_onDestroy(std::shared_ptr<FkMessage> msg) {
    return onDestroy();
}

FkResult FkEngine::_onStart(std::shared_ptr<FkMessage> msg) {
    return onStart();
}

FkResult FkEngine::_onStop(std::shared_ptr<FkMessage> msg) {
    return onStop();
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

FkResult FkEngine::registerMessage(FkID what, FkMessageHandler handler) {
    mMsgMap.emplace(std::make_pair(what, FkMessageHandlerPair(what, handler)));
    return FK_OK;
}

void FkEngine::_dispatch(std::shared_ptr<FkMessage> msg) {
    std::lock_guard<std::mutex> guard(mtx);
    if (kState::IDL == state) {
        FkLogW(FK_DEF_TAG, "Invalid state");
        return;
    }
    auto itr = mMsgMap.find(msg->what);
    if (mMsgMap.end() != itr) {
        itr->second.handle(this, std::move(msg));
    }
}

FkResult FkEngine::sendMessage(std::shared_ptr<FkMessage> msg) {
    mHandler->sendMessage(std::move(msg));
    return FK_OK;
}
