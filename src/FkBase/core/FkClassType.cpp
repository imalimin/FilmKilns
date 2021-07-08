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

#ifdef __FK_OPTIMIZE_CLASS_TYPE__

void FkClassType::super(const size_t &hashCode) {
    extends.emplace_back(hashCode);
}

#else

void FkClassType::super(const char *name) {
    std::string key(name);
    extends.emplace_back(key);
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
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
        str.append(FkString::valueOf<size_t>(*itr));
#else
        str.append(*itr);
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
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
    return extends.back();
#else
    return hashValue(extends.back());
#endif
}

std::string FkClassType::getName() const {
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
    return FkString::valueOf<size_t>(*(--extends.end()));
#else
    return *(--extends.end());
#endif
}
