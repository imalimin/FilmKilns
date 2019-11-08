/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <Object.h>
#include <cassert>
#include "AlHandler.h"

AlHandler::AlHandler(Callback callback) : Object(), callback(callback) {
    this->looper = AlLooper::myLooper();
    assert(nullptr != looper);
}

AlHandler::AlHandler(AlLooper *looper, Callback callback) : Object(), callback(callback) {
    this->looper = looper;
    assert(nullptr != looper);
}

AlHandler::~AlHandler() {
    this->looper = nullptr;
    this->callback = nullptr;
}

void AlHandler::dispatchMessage(AlMessage *msg) {
    if (callback) {
        callback(msg);
    }
}

void AlHandler::sendMessage(AlMessage *msg) {
    _enqueueMessage(msg);
}

void AlHandler::_enqueueMessage(AlMessage *msg) {
    if (this->looper) {
        msg->target = this;
        this->looper->sendMessage(msg);
    }
}