/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkClassType.h"
#include "FkString.h"

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

#ifdef __FK_DEBUG__

void FkClassType::super(const char *name) {
    std::string key(name);
    extends.emplace_back(key);
}

#else
void FkClassType::super(const size_t &hashCode) {
    extends.emplace_back(hashCode);
}
#endif

bool operator==(const FkClassType &o0, const FkClassType &o1) {
    auto target = o1.extends.back();
    auto itr = std::find(o0.extends.begin(), o0.extends.end(), target);
    return o0.extends.end() != itr;
}

bool operator!=(const FkClassType &o0, const FkClassType &o1) {
    return !(o0 == o1);
}

std::string FkClassType::toString() const {
    std::string str;
    for (auto itr = extends.begin(); itr != extends.end(); ++itr) {
#ifdef __FK_DEBUG__
        str.append(*itr);
#else
        str.append(FkString::valueOf<size_t>(*itr));
#endif
        if ((--extends.end()) != itr) {
            str.append(FK_POINTER);
        }
    }
    return str;
}

size_t FkClassType::hashCode() const {
    if (extends.empty()) {
        return 0;
    }
#ifdef __FK_DEBUG__
    return hashValue(extends.back());
#else
    return extends.back();
#endif
}

std::string FkClassType::getName() const {
#ifdef __FK_DEBUG__
    return *(--extends.end());
#else
    return FkString::valueOf<size_t>(*(--extends.end()));
#endif
}
