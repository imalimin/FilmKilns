/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMessage.h"

const int32_t FkMessage::FLAG_NORMAL = 0x01;
const int32_t FkMessage::FLAG_UNIQUE = 0x02;
const int32_t FkMessage::FLAG_FIRST_ALWAYS = 0x04;
const int32_t FkMessage::FLAG_CLEAR = 0x08;

std::shared_ptr<FkMessage> FkMessage::obtain(FkID what) {
    return obtain(what, nullptr, FLAG_NORMAL);
}

std::shared_ptr<FkMessage> FkMessage::obtain(FkID what, std::shared_ptr<FkObject> sp) {
    return obtain(what, std::move(sp), FLAG_NORMAL);
}

std::shared_ptr<FkMessage> FkMessage::obtain(FkID what, int32_t flags) {
    return obtain(what, nullptr, flags);
}

std::shared_ptr<FkMessage> FkMessage::obtain(FkID what,
                                             std::shared_ptr<FkObject> sp,
                                             int32_t flags) {
    auto msg = FkMessageManager::getInstance()->popOne();
    if (msg) {
        msg->what = what;
        msg->arg1 = 0;
        msg->arg2 = 0;
        msg->arg3 = "";
        msg->sp = std::move(sp);
        msg->flags = flags;
        return msg;
    }
    return std::make_shared<FkMessage>(what, sp, flags);
}

void FkMessage::recycle(std::shared_ptr<FkMessage> msg) {
    FkMessageManager::getInstance()->recycle(msg);
}

FkMessage::FkMessage()
        : FkMessage(FK_ID_NONE) {

}

FkMessage::FkMessage(FkID what)
        : FkMessage(what, nullptr) {

}

FkMessage::FkMessage(FkID what, std::shared_ptr<FkObject> sp)
        : FkMessage(what, std::move(sp), FLAG_NORMAL) {
}

FkMessage::FkMessage(FkID what, std::shared_ptr<FkObject> sp, int32_t flags)
        : FkObject(), what(what), arg1(0), arg2(0), arg3(""), sp(std::move(sp)), flags(flags) {
}

FkMessage::~FkMessage() {
}

FkMessageManager *FkMessageManager::instance = new FkMessageManager();

FkMessageManager *FkMessageManager::getInstance() {
    return instance;
}

std::shared_ptr<FkMessage> FkMessageManager::popOne() {
    std::lock_guard<std::mutex> guard(poolMtx);
    if (!pool.empty()) {
        auto msg = pool.front();
        pool.pop();
        return msg;
    }
    return nullptr;
}

void FkMessageManager::recycle(std::shared_ptr<FkMessage> msg) {
    if (nullptr == msg) {
        return;
    }
    std::lock_guard<std::mutex> guard(poolMtx);
    msg->what = FK_ID_NONE;
    msg->arg1 = 0;
    msg->arg2 = 0;
    msg->arg3 = "";
    msg->sp = nullptr;
    msg->flags = FkMessage::FLAG_NORMAL;
    pool.push(msg);
}