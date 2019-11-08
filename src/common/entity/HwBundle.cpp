/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwBundle.h"

const int32_t HwBundle::VALUE_NONE = INT32_MIN;

HwBundle::HwBundle() : Object() {

}

HwBundle::~HwBundle() {
    int32Map.clear();
    int64Map.clear();
    strMap.clear();
}

void HwBundle::putInt32(string key, int32_t value) {
    int32Map.insert(pair<string, int32_t>(key, value));
}

void HwBundle::putInt64(string key, int64_t value) {
    int64Map.insert(pair<string, int32_t>(key, value));
}

void HwBundle::putString(string key, string value) {
    strMap.insert(pair<string, string>(key, value));
}

void HwBundle::putObject(string key, Object *value) {
    objMap.insert(pair<string, Object *>(key, value));
}

const int32_t &HwBundle::getInt32(string key) {
    if (int32Map.end() == int32Map.find(key)) {
        return VALUE_NONE;
    }
    return int32Map[key];
}

const int64_t &HwBundle::getInt64(string key) {
    if (int64Map.end() == int64Map.find(key)) {
        return VALUE_NONE;
    }
    return int64Map[key];
}

const string &HwBundle::getString(string key) {
    if (strMap.end() == strMap.find(key)) {
        return "";
    }
    return strMap[key];
}

Object *HwBundle::getObject(string key) {
    if (objMap.end() == objMap.find(key)) {
        return nullptr;
    }
    return objMap[key];
}