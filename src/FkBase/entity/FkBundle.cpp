/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkBundle.h"
#include "FkString.h"
#include "FkPrimitiveType.h"

#define PUT_PRI(_pri) \
std::lock_guard<std::mutex> guard(mtx); \
auto itr = map.find(key); \
if (map.end() != itr) { \
  map.erase(itr); \
} \
auto ptr = std::make_shared<_pri>(val); \
map.insert(make_pair(key, std::move(ptr))); \

#define GET_PRI(_pri) \
std::lock_guard<std::mutex> guard(mtx); \
auto itr = map.find(key); \
if (map.end() != itr && itr->second) { \
  auto val = std::dynamic_pointer_cast<_pri>(itr->second);  \
  if (val) { \
    return val->value(); \
  } \
} \

#define TAG "FkBundle"

FK_IMPL_CLASS_TYPE(FkBundle, FkObject)

FkBundle::FkBundle() : FkObject() {
}

FkBundle::FkBundle(const FkBundle &o) : FkObject() {
    auto itr = o.map.begin();
    while (o.map.end() != itr) {
        auto obj = itr->second;
        if (FK_INSTANCE_OF(obj, FkInteger)) {
            put(itr->first, std::dynamic_pointer_cast<FkInteger>(obj)->value());
        } else if (FK_INSTANCE_OF(obj, FkLong)) {
            put(itr->first, std::dynamic_pointer_cast<FkLong>(obj)->value());
        } else if (FK_INSTANCE_OF(obj, FkFloat)) {
            put(itr->first, std::dynamic_pointer_cast<FkFloat>(obj)->value());
        } else if (FK_INSTANCE_OF(obj, FkDouble)) {
            put(itr->first, std::dynamic_pointer_cast<FkDouble>(obj)->value());
        } else if (FK_INSTANCE_OF(obj, FkByte)) {
            put(itr->first, std::dynamic_pointer_cast<FkByte>(obj)->value());
        } else if (FK_INSTANCE_OF(obj, FkChar)) {
            put(itr->first, std::dynamic_pointer_cast<FkChar>(obj)->value());
        } else if (FK_INSTANCE_OF(obj, FkString)) {
            put(itr->first, std::dynamic_pointer_cast<FkString>(obj)->toString());
        }
        ++itr;
    }
    auto itr1 = o.anyMap.begin();
    while (o.anyMap.end() != itr1) {
        put(itr->first, itr->second);
        ++itr1;
    }
}

FkBundle::~FkBundle() {
    map.clear();
    anyMap.clear();
}

bool FkBundle::put(std::string key, int32_t val) {
    PUT_PRI(FkInteger)
    return true;
}

bool FkBundle::put(std::string key, int64_t val) {
    PUT_PRI(FkLong)
    return true;
}

bool FkBundle::put(std::string key, float val) {
    PUT_PRI(FkFloat)
    return true;
}

bool FkBundle::put(std::string key, double val) {
    PUT_PRI(FkDouble)
    return true;
}

bool FkBundle::put(std::string key, uint8_t val) {
    PUT_PRI(FkByte)
    return true;
}

bool FkBundle::put(std::string key, char val) {
    PUT_PRI(FkChar)
    return true;
}

bool FkBundle::put(std::string key, std::string val) {
    PUT_PRI(FkString)
    return true;
}

bool FkBundle::put(std::string key, std::any val) {
    std::lock_guard<std::mutex> guard(mtx);
    auto itr = anyMap.find(key);
    if (anyMap.end() != itr) {
        anyMap.erase(itr);
    }
    anyMap.insert(make_pair(key, std::move(val)));
    return true;
}

int32_t FkBundle::get(std::string key, int32_t def) {
    GET_PRI(FkInteger)
    return def;
}

int64_t FkBundle::get(std::string key, int64_t def) {
    GET_PRI(FkLong)
    return def;
}

float FkBundle::get(std::string key, float def) {
    GET_PRI(FkFloat)
    return def;
}

double FkBundle::get(std::string key, double def) {
    GET_PRI(FkDouble)
    return def;
}

uint8_t FkBundle::get(std::string key, uint8_t def) {
    GET_PRI(FkByte)
    return def;
}

char FkBundle::get(std::string key, char def) {
    GET_PRI(FkChar)
    return def;
}

std::string FkBundle::get(std::string key, std::string def) {
    std::lock_guard<std::mutex> guard(mtx);
    auto itr = map.find(key);
    if (map.end() != itr && itr->second) {
        auto val = std::dynamic_pointer_cast<FkString>(itr->second); ;
        if (val) {
            return val->str();
        }
    }
    return def;
}

std::any FkBundle::get(std::string key) {
    std::lock_guard<std::mutex> guard(mtx);
    auto itr = anyMap.find(key);
    if (anyMap.end() != itr) {
        return itr->second;
    }
    return nullptr;
}

void FkBundle::remove(std::string key) {
    auto itr = map.find(key);
    if (map.end() != itr) {
        map.erase(itr);
    }
}

bool FkBundle::contains(std::string key) {
    return map.end() != map.find(key);
}

std::string FkBundle::toString() {
    std::string sb;
    sb.append("{");
    auto itr = map.begin();
    while (map.end() != itr) {
        sb.append("\"");
        sb.append(itr->first);
        sb.append("\":");
        sb.append((itr->second.get())->toString());
        sb.append(", ");
        ++itr;
    }
    sb.append("}");
    return sb;
}
