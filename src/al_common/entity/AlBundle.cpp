/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBundle.h"
#include "AlString.h"
#include "AlLogcat.h"

#define PUT_PRI(_pri) \
auto itr = map.find(key); \
if (map.end() != itr) { \
  delete *(itr->second.release()); \
  map.erase(itr); \
} \
auto ptr = std::make_unique<Object *>(new _pri(val)); \
map.insert(make_pair(key, std::move(ptr))); \

#define GET_PRI(_pri) \
auto itr = map.find(key); \
if (map.end() != itr && itr->second) { \
  _pri *val = dynamic_cast<_pri *>(*itr->second); \
  if (val) { \
    return val->value(); \
  } \
} \

#define TAG "AlBundle"

AlBundle::AlBundle() : Object() {
}

AlBundle::AlBundle(const AlBundle &o) : Object() {
    auto itr = o.map.begin();
    while (o.map.end() != itr) {
        Object *obj = *itr->second;
        if (AL_INSTANCE_OF(obj, AlInteger *)) {
            put(itr->first, dynamic_cast<AlInteger *>(obj)->value());
        } else if (AL_INSTANCE_OF(obj, AlLong *)) {
            put(itr->first, dynamic_cast<AlLong *>(obj)->value());
        } else if (AL_INSTANCE_OF(obj, AlFloat *)) {
            put(itr->first, dynamic_cast<AlFloat *>(obj)->value());
        } else if (AL_INSTANCE_OF(obj, AlDouble *)) {
            put(itr->first, dynamic_cast<AlDouble *>(obj)->value());
        } else if (AL_INSTANCE_OF(obj, AlByte *)) {
            put(itr->first, dynamic_cast<AlByte *>(obj)->value());
        } else if (AL_INSTANCE_OF(obj, AlChar *)) {
            put(itr->first, dynamic_cast<AlChar *>(obj)->value());
        } else if (AL_INSTANCE_OF(obj, AlString *)) {
            put(itr->first, dynamic_cast<AlString *>(obj)->str());
        }
        ++itr;
    }
}

AlBundle::~AlBundle() {
    AlLogI(TAG, "%s", toString().c_str());
    auto itr = map.begin();
    while (map.end() != itr) {
        delete *(itr->second.release());
        ++itr;
    }
    map.clear();
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
    PUT_PRI(AlString)
    return true;
}

int32_t AlBundle::get(std::string key, int32_t def) {
    GET_PRI(AlInteger)
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
        AlString *val = dynamic_cast<AlString *>(*itr->second);
        if (val) {
            return val->str();
        }
    }
    return def;
}

void AlBundle::remove(std::string key) {
    auto itr = map.find(key);
    if (map.end() != itr) {
        delete *(itr->second.release());
        map.erase(itr);
    }
}

bool AlBundle::contains(std::string key) {
    return map.end() != map.find(key);
}

std::string AlBundle::toString() {
    std::string sb;
    sb.append("{");
    auto itr = map.begin();
    while (map.end() != itr) {
        sb.append("\"");
        sb.append(itr->first);
        sb.append("\":");
        sb.append((*itr->second)->toString());
        sb.append(", ");
        ++itr;
    }
    sb.append("}");
    return sb;
}
