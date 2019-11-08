/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlMessage.h"
#include "ObjectBox.h"

AlMessage *AlMessage::obtain() {
    return new AlMessage();
}

AlMessage *AlMessage::obtain(int32_t what) {
    return new AlMessage(what);
}

AlMessage *AlMessage::obtain(int32_t what, Object *obj) {
    return new AlMessage(what, obj);
}

AlMessage *AlMessage::obtain(int32_t what, Object *obj, int16_t queueMode) {
    return new AlMessage(what, obj, queueMode);
}

AlMessage::AlMessage()
        : AlMessage(what) {

}

AlMessage::AlMessage(int32_t what)
        : AlMessage(what, nullptr) {

}

AlMessage::AlMessage(int32_t what,
                     Object *obj)
        : AlMessage(what, obj, QUEUE_MODE_NORMAL) {
}

AlMessage::AlMessage(int32_t what,
                     Object *obj,
                     int16_t queueMode)
        : Object() {
    this->what = what;
    this->obj = obj;
    this->arg1 = 0;
    this->arg2 = 0;
    this->desc = "Undef";
    this->queueMode = queueMode;
}

AlMessage::~AlMessage() {
    if (obj) {
        delete obj;
        obj = nullptr;
    }
}

void *AlMessage::tyrUnBox() {
    ObjectBox *ob = dynamic_cast<ObjectBox *>(obj);
    return ob->ptr;
}