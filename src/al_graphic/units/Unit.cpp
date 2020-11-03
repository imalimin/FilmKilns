/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "Unit.h"
#include "StringUtils.h"
#include "log.h"

#define TAG "Unit"

Event::Event(int what, EventFunc handler) {
    this->what = what;
    this->handler = handler;
}

Event::~Event() {
    this->what = 0;
    this->handler = nullptr;
}

bool Event::handle(Unit *unit, AlMessage *msg) {
    return (unit->*handler)(msg);
}

std::string Unit::kidRestore(int32_t kid) {
    std::string str;
    char *s = new char[4];
    s[0] = (char) (kid >> 24);
    s[1] = (char) (kid >> 16);
    s[2] = (char) (kid >> 8);
    s[3] = 0;
    int d = (char) (kid);
    str.append(s);
    str.append(StringUtils::valueOf(d));
    return str;
}

Unit::Unit(string alias, AlUnitSetting setting) : Object(), alias(alias), setting(setting) {
    created = false;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&Unit::onCreate));
    registerEvent(EVENT_COMMON_RELEASE, reinterpret_cast<EventFunc>(&Unit::onDestroy));
}

Unit::~Unit() {
    AlLogI(TAG, "%s", alias.c_str());
    if (eventMap.empty()) return;
    for (auto itr = eventMap.rbegin(); itr != eventMap.rend(); itr++) {
        delete itr->second;
    }
    eventMap.clear();
}

bool Unit::onMsgReceived(AlMessage *msg) {
    return false;
}

bool Unit::registerEvent(int what, EventFunc handler) {
    eventMap.insert(pair<int, Event *>(what, new Event(what, handler)));
    return true;
}

void Unit::setController(AlAbsPoster *poster) {
    this->poster = poster;
}

void Unit::postEvent(AlMessage *msg) {
    postMessage(msg);
}

void Unit::postMessage(AlMessage *msg) {
    if (poster) {
        poster->postMessage(msg);
    } else {
        AlLogE(TAG, "failed. skip message %p", msg);
    }
}

bool Unit::dispatch(AlMessage *msg) {
    if (created && EVENT_COMMON_PREPARE == msg->what) {
        return false;
    }
    if (!created) {
        if (EVENT_COMMON_PREPARE != msg->what) {
            return false;
        }
        created = true;
    }
    onMsgReceived(msg);
    auto itr = eventMap.find(msg->what);
    if (eventMap.end() != itr) {
        return itr->second->handle(this, msg);
    }
    return false;
}

void Unit::setModelProvider(HwModelProvider *provider) {
    this->provider = provider;
}

HwModelProvider *Unit::getModelProvider() {
    return this->provider;
}

const int32_t Unit::getInt32(string key) {
    return getModelProvider()->getInt32(alias + "_" + key);
}

const int64_t Unit::getInt64(string key) {
    return getModelProvider()->getInt64(alias + "_" + key);
}

const string Unit::getString(string key) {
    return getModelProvider()->getString(alias + "_" + key);
}

Object *Unit::getObject(string key) {
    return getModelProvider()->getObject(alias + "_" + key);
}

Unit::AlUnitSetting::AlUnitSetting(bool hosted)
        : Object(),
          hosted(hosted) {

}

Unit::AlUnitSetting::AlUnitSetting(const Unit::AlUnitSetting &o)
        : Object(),
          hosted(o.hosted) {

}

Unit::AlUnitSetting::~AlUnitSetting() {

}
