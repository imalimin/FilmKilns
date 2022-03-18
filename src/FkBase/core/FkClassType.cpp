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
    for (auto &it : o.hierarchy) {
        hierarchy.emplace_back(it);
    }
}

FkClassType::~FkClassType() {

}

#ifdef __FK_OPTIMIZE_CLASS_TYPE__

void FkClassType::super(const size_t hashCode) {
    hierarchy.emplace_back(hashCode);
}

#else

void FkClassType::super(const char *name) {
    std::string key(name);
    hierarchy.emplace_back(key);
}

#endif

bool operator==(const FkClassType &o0, const FkClassType &o1) {
    auto target = o1.hierarchy.back();
    auto itr = std::find(o0.hierarchy.begin(), o0.hierarchy.end(), target);
    auto ret = o0.hierarchy.end() != itr;
    return ret;
}

bool operator!=(const FkClassType &o0, const FkClassType &o1) {
    return !(o0 == o1);
}

std::string FkClassType::toString() const {
    std::string str;
    for (auto itr = hierarchy.begin(); itr != hierarchy.end(); ++itr) {
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
        str.append(FkString::valueOf<size_t>(*itr));
#else
        str.append(*itr);
#endif
        if ((--hierarchy.end()) != itr) {
            str.append(FK_POINTER);
        }
    }
    return str;
}

size_t FkClassType::hashCode() const {
    if (hierarchy.empty()) {
        return 0;
    }
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
    return hierarchy.back();
#else
    return hashValue(hierarchy.back());
#endif
}

std::string FkClassType::getName() const {
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
    return FkString::valueOf<size_t>(*(--hierarchy.end()));
#else
    return *(--hierarchy.end());
#endif
}
