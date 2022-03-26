/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkEngine.h"

FkMsgHandle::FkMsgHandle(FkMsgHandle::Func func)
        : FkObject(), func(func) {
    FK_MARK_SUPER
}

FkMsgHandle::FkMsgHandle(const FkMsgHandle &o) : FkObject(o), func(o.func) {
    FK_MARK_SUPER
}

FkMsgHandle::~FkMsgHandle() {

}

FkResult FkMsgHandle::operator()(FkEngine *ptr, std::shared_ptr<FkMessage> &msg) {
    return (ptr->*func)(msg);
}

FkEngine::FkEngine(std::string name) : FkObject(), name(name),
                                       internalState(kState::IDL),
                                       outsideState(kState::IDL) {
    FK_MARK_SUPER
}

FkEngine::~FkEngine() {
    std::lock_guard<std::recursive_mutex> guard(mtx);
    FkAssert(kState::IDL == outsideState, );
}

FkResult FkEngine::create() {
    auto ret = _changeState(outsideState, (uint32_t) kState::IDL, kState::CREATED);
    if (FK_OK != ret) {
        return ret;
    }
    mThread = FkHandlerThread::create(name);
    mHandler = new FkHandler(mThread->getLooper(), [this](std::shared_ptr<FkMessage> msg) {
        this->_dispatch(msg);
    });
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkEngine::_onCreate));
    sendMessage(msg, true);
    return FK_OK;
}

FkResult FkEngine::destroy() {
    auto ret = _changeState(outsideState, (uint32_t) kState::STOPPED, kState::IDL);
    if (FK_OK != ret) {
        return ret;
    }
    if (nullptr == mThread) {
        return FK_INVALID_STATE;
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkEngine::_onDestroy));
    sendMessage(msg, true);
    mThread->quitSafely();
    mThread = nullptr;
    delete mHandler;
    mHandler = nullptr;
    return FK_OK;
}

FkResult FkEngine::start() {
    auto from = (uint32_t) kState::CREATED | (uint32_t) kState::STOPPED;
    auto ret = _changeState(outsideState, from, kState::RUNNING);
    if (FK_OK != ret) {
        return ret;
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkEngine::_onStart));
    sendMessage(msg, true);
    return FK_OK;
}

FkResult FkEngine::stop() {
    auto ret = _changeState(outsideState, (uint32_t) kState::RUNNING, kState::STOPPED);
    if (FK_OK != ret) {
        return ret;
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkEngine::_onStop));
    sendMessage(msg, true);
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
    auto ret = _changeState(internalState, (uint32_t) kState::IDL, kState::CREATING);
    if (FK_OK != ret) {
        return ret;
    }
    ret = onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    ret = _changeState(internalState, (uint32_t) kState::CREATING, kState::CREATED);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkEngine::_onDestroy(std::shared_ptr<FkMessage> msg) {
    auto ret = _changeState(internalState, (uint32_t) kState::STOPPED, kState::DESTROYING);
    if (FK_OK != ret) {
        return ret;
    }
    ret = onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    ret = _changeState(internalState, (uint32_t) kState::DESTROYING, kState::IDL);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkEngine::_onStart(std::shared_ptr<FkMessage> msg) {
    auto from = (uint32_t) kState::CREATED | (uint32_t) kState::STOPPED;
    auto ret = _changeState(internalState, from, kState::STARTING);
    if (FK_OK != ret) {
        return ret;
    }
    ret = onStart();
    if (FK_OK != ret) {
        return ret;
    }
    ret = _changeState(internalState, (uint32_t) kState::STARTING, kState::RUNNING);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkEngine::_onStop(std::shared_ptr<FkMessage> msg) {
    auto ret = _changeState(internalState, (uint32_t) kState::RUNNING, kState::STOPPING);
    if (FK_OK != ret) {
        return ret;
    }
    ret = onStop();
    if (FK_OK != ret) {
        return ret;
    }
    ret = _changeState(internalState, (uint32_t) kState::STOPPING, kState::STOPPED);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkEngine::_changeState(kState &_state, uint32_t src, kState dst) {
    std::lock_guard<std::recursive_mutex> guard(mtx);
    if (src & ((uint32_t) _state)) {
        _state = dst;
        return FK_OK;
    }
    FkLogW(FK_DEF_TAG, "Invalid state");
    return FK_INVALID_STATE;
}

void FkEngine::_dispatch(std::shared_ptr<FkMessage> &msg) {
    {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        if (outsideState == kState::IDL &&
            (internalState == kState::DESTROYING || internalState == kState::IDL)) {
            FkLogW(FK_DEF_TAG, "Invalid state. Engine state is DESTROYING/IDL.");
            return;
        }
    }
    std::lock_guard<std::mutex> guard(msgMtx);
    if (msg->any.has_value()) {
        auto func = std::any_cast<FkMsgHandle>(msg->any);
        func(this, msg);
    }
}

FkResult FkEngine::sendMessage(std::shared_ptr<FkMessage> &msg, bool ignoreState) {
    if (!ignoreState) {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        if (outsideState != kState::RUNNING) {
            return FK_INVALID_STATE;
        }
    }
    mHandler->sendMessage(msg);
    return FK_OK;
}
