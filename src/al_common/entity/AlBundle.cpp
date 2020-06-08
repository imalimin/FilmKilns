/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBundle.h"
#include "AlString.h"

#define PUT_PRI(_pri) \
if (map.end() != map.find(key)) { \
  return false; \
} \
_put(key, new _pri(val)); \

#define GET_PRI(_pri) \
auto itr = map.find(key); \
if (map.end() != itr && itr->second) { \
  _pri *val = dynamic_cast<_pri *>(itr->second); \
  if (val) { \
    return val->value(); \
  } \
} \


AlBundle::AlBundle() : Object() {

}

AlBundle::AlBundle(const AlBundle &o) : Object() {
    auto itr = map.begin();
    while (map.end() != itr) {
        if (AL_INSTANCE_OF(itr->second, AlInteger *)) {
            put(itr->first, dynamic_cast<AlInteger *>(itr->second)->value());
        } else if (AL_INSTANCE_OF(itr->second, AlLong *)) {
            put(itr->first, dynamic_cast<AlLong *>(itr->second)->value());
        } else if (AL_INSTANCE_OF(itr->second, AlFloat *)) {
            put(itr->first, dynamic_cast<AlFloat *>(itr->second)->value());
        } else if (AL_INSTANCE_OF(itr->second, AlDouble *)) {
            put(itr->first, dynamic_cast<AlDouble *>(itr->second)->value());
        } else if (AL_INSTANCE_OF(itr->second, AlByte *)) {
            put(itr->first, dynamic_cast<AlByte *>(itr->second)->value());
        } else if (AL_INSTANCE_OF(itr->second, AlChar *)) {
            put(itr->first, dynamic_cast<AlChar *>(itr->second)->value());
        } else if (AL_INSTANCE_OF(itr->second, AlString *)) {
            put(itr->first, dynamic_cast<AlString *>(itr->second)->str());
        }
        ++itr;
    }
}

AlBundle::~AlBundle() {
    auto itr = map.begin();
    while (map.end() != itr) {
        delete itr->second;
        ++itr;
    }
    map.clear();
}

void AlBundle::_put(std::string &key, Object *val) {
    map.insert(make_pair(key, val));
}

bool AlBundle::put(std::string key, int32_t val) {
    PUT_PRI(AlInteger)
    return true;
}

bool AlBundle::put(std::string key, int64_t val) {
    PUT_PRI(AlLong)
    return true;
}

bool AlBundle::put(std::string key, float val) {
    PUT_PRI(AlFloat)
    return true;
}

bool AlBundle::put(std::string key, double val) {
    PUT_PRI(AlDouble)
    return true;
}

bool AlBundle::put(std::string key, uint8_t val) {
    PUT_PRI(AlByte)
    return true;
}

bool AlBundle::put(std::string key, char val) {
    PUT_PRI(AlChar)
    return true;
}

bool AlBundle::put(std::string key, std::string val) {
    if (map.end() != map.find(key)) {
        return false;
    }
    _put(key, new AlString(val));
    return true;
}

int32_t AlBundle::get(std::string key, int32_t def) {
    auto itr = map.find(key);
    if (map.end() != itr && itr->second) {
        AlInteger *val = dynamic_cast<AlInteger *>(itr->second);
        if (val) {
            return val->value();
        }
    }
    return def;
}

int64_t AlBundle::get(std::string key, int64_t def) {
    GET_PRI(AlLong)
    return def;
}

float AlBundle::get(std::string key, float def) {
    GET_PRI(AlFloat)
    return def;
}

double AlBundle::get(std::string key, double def) {
    GET_PRI(AlDouble)
    return def;
}

uint8_t AlBundle::get(std::string key, uint8_t def) {
    GET_PRI(AlByte)
    return def;
}

char AlBundle::get(std::string key, char def) {
    GET_PRI(AlChar)
    return def;
}

std::string AlBundle::get(std::string key, std::string def) {
    auto itr = map.find(key);
    if (map.end() != itr && itr->second) {
        AlString *val = dynamic_cast<AlString *>(itr->second);
        if (val) {
            return val->str();
        }
    }
    return def;
}

void AlBundle::remove(std::string key) {
    auto itr = map.find(key);
    if (map.end() != itr) {
        delete itr->second;
        map.erase(itr);
    }
}

bool AlBundle::contains(std::string key) {
    return map.end() != map.find(key);
}
