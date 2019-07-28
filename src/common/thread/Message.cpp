/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Message.h"
#include "../include/ObjectBox.h"

Message::Message(int32_t what, function<void(Message *msg)> runnable)
        : Message(what, nullptr, QUEUE_MODE_NORMAL, runnable) {
}

Message::Message(int32_t what, Object *obj, function<void(Message *msg)> runnable)
        : Message(what, obj, QUEUE_MODE_NORMAL, runnable) {
}

Message::Message(int32_t what, Object *obj, int16_t queueMode, function<void(Message *msg)> runnable)
        : Object() {
    this->what = what;
    this->obj = obj;
    this->runnable = runnable;
    this->arg1 = 0;
    this->arg2 = 0;
    this->desc = "Undef";
    this->queueMode = queueMode;
}

Message::~Message() {
    this->runnable = nullptr;
    if (obj) {
        delete obj;
        obj = nullptr;
    }
}

void *Message::tyrUnBox() {
    ObjectBox *ob = dynamic_cast<ObjectBox *>(obj);
    return ob->ptr;
}