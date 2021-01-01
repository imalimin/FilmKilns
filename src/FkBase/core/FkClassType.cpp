/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkClassType.h"

#define FK_POINTER ":<-"

FkClassType::FkClassType() {

}

FkClassType::FkClassType(const FkClassType &o) {
    for (auto &it : o.extends) {
        extends.emplace_back(it);
    }
}

FkClassType::~FkClassType() {

}

void FkClassType::super(const char *name) {
    std::string key(name);
    extends.emplace_back(key);
}

bool operator==(const FkClassType &o0, const FkClassType &o1) {
    std::string target = o1.extends.back();
    auto itr = std::find(o0.extends.begin(), o0.extends.end(), target);
    return o0.extends.end() != itr;
}

std::string FkClassType::toString() {
    std::string str;
    for (auto itr = extends.begin(); itr != extends.end(); ++itr) {
        str.append(*itr);
        if ((--extends.end()) != itr) {
            str.append(FK_POINTER);
        }
    }
    return str;
}
