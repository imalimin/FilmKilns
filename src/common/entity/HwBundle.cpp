/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwBundle.h"

HwBundle::HwBundle() : Object() {

}

HwBundle::~HwBundle() {
    int32Map.clear();
    int64Map.clear();
    stringMap.clear();
}

void HwBundle::putInt32(string key, int32_t value) {
    int32Map.insert(pair<string, int32_t>(key, value));
}

void HwBundle::putInt64(string key, int64_t value) {
    int64Map.insert(pair<string, int32_t>(key, value));
}

void HwBundle::putString(string key, string value) {
    stringMap.insert(pair<string, string>(key, value));
}

int32_t HwBundle::getInt32(string key) {
    return int32Map[key];
}

int64_t HwBundle::getInt64(string key) {
    return int64Map[key];
}

string HwBundle::getString(string key) {
    return stringMap[key];
}