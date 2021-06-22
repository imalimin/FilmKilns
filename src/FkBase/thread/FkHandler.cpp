/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkHandler.h"
#include "FkDefinition.h"

FkHandler::FkHandler(FkHandler::Callback callback) : FkObject(), callback(std::move(callback)) {
    FK_MARK_SUPER
    this->looper = FkLooper::myLooper();
    FkAssert(nullptr != looper, );
}

FkHandler::FkHandler(FkLooper *looper, FkHandler::Callback callback)
        : FkObject(), callback(std::move(callback)) {
    this->looper = looper;
    assert(nullptr != looper);
}

FkHandler::~FkHandler() {
    this->looper = nullptr;
    this->callback = nullptr;
}

void FkHandler::dispatchMessage(std::shared_ptr<FkMessage> msg) {
    if (callback) {
        callback(std::move(msg));
    }
}

void FkHandler::sendMessage(std::shared_ptr<FkMessage> msg) {
    _enqueueMessage(std::move(msg));
}

void FkHandler::_enqueueMessage(std::shared_ptr<FkMessage> msg) {
    if (this->looper) {
        msg->target = this;
        this->looper->sendMessage(std::move(msg));
    }
}