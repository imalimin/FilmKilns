/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMessage.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "FkMessage"

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
    int32_t desc = 0;
    auto msg = FkMessageAllocator::getInstance()->alloc(desc);
    if (msg) {
        msg->destroy();
        msg->what = what;
        msg->sp = std::move(sp);
        msg->flags = flags;
        return msg;
    }
    return std::make_shared<FkMessage>(what, sp, flags);
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
        : FkSource(), what(what), arg1(0), arg2(0), arg3(""), sp(std::move(sp)),
          flags(flags), promise(nullptr), target(nullptr) {
}

FkMessage::~FkMessage() {
}

FkResult FkMessage::create() {
    return FK_OK;
}

void FkMessage::destroy() {
    what = FK_ID_NONE;
    arg1 = 0;
    arg2 = 0;
    arg3 = "";
    sp = nullptr;
    flags = FkMessage::FLAG_NORMAL;
    promise = nullptr;
    target = nullptr;
}

size_t FkMessage::size() {
    return 1;
}

FkMessageAllocator *FkMessageAllocator::instance = new FkMessageAllocator();

FkMessageAllocator *FkMessageAllocator::getInstance() {
    return instance;
}

FkMessageAllocator::FkMessageAllocator() : FkSourceAllocator<FkMessage, int32_t>() {
    FK_MARK_SUPER
}

FkMessageAllocator::~FkMessageAllocator() {

}

FkMessage *FkMessageAllocator::delegateAlloc(int32_t &desc) {
    auto o = new FkMessage(desc);
    if (FK_OK != o->create()) {
        delete o;
        return nullptr;
    }
    return o;
}

bool FkMessageAllocator::delegateEquals(int32_t &desc, FkMessage *value) {
    return true;
}
