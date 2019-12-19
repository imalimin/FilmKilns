//
// Created by mingyi.li on 2018/12/25.
//

#include "Unit.h"
#include "log.h"

Event::Event(int what, EventFunc handler) {
    this->what = what;
    this->handler = handler;
}

Event::~Event() {
    LOGE("~Event");
    this->what = 0;
    this->handler = nullptr;
}

bool Event::dispatch(Unit *unit, AlMessage *msg) {
    return (unit->*handler)(msg);
}

Unit::Unit(string alias) : alias(alias) {
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&Unit::onCreate));
    registerEvent(EVENT_COMMON_RELEASE, reinterpret_cast<EventFunc>(&Unit::onDestroy));
}

Unit::~Unit() {
    LOGI("~Unit(%s)", alias.c_str());
    if (eventMap.empty()) return;
    for (auto itr = eventMap.rbegin(); itr != eventMap.rend(); itr++) {
//        delete itr->second;
    }
    eventMap.clear();
}

bool Unit::registerEvent(int what, EventFunc handler) {
    eventMap.insert(pair<int, Event *>(what, new Event(what, handler)));
    return true;
}

void Unit::setController(UnitPipeline *pipeline) {
    this->pipeline = pipeline;
}

void Unit::postEvent(AlMessage *msg) {
    if (pipeline) {
        pipeline->postEvent(msg);
    } else {
        LOGE("%s`s pipeline is null", alias.c_str());
    }
}

bool Unit::dispatch(AlMessage *msg) {
    auto itr = eventMap.find(msg->what);
    if (eventMap.end() != itr) {
        return itr->second->dispatch(this, msg);
    }
    return false;
}

void Unit::setModelProvider(HwModelProvider *provider) {
    this->provider = provider;
}

HwModelProvider *Unit::getModelProvider() {
    return this->provider;
}

const int32_t &Unit::getInt32(string key) {
    return getModelProvider()->getInt32(alias + "_" + key);
}

const int64_t &Unit::getInt64(string key) {
    return getModelProvider()->getInt64(alias + "_" + key);
}

const string &Unit::getString(string key) {
    return getModelProvider()->getString(alias + "_" + key);
}

Object *Unit::getObject(string key) {
    return getModelProvider()->getObject(alias + "_" + key);
}
