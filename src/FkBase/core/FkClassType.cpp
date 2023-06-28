/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkClassType.h"

FkClassType::FkClassType(const char *_name, size_t _id)
        : name(_name), id(_id) {
}

FkClassType::FkClassType(const char *_name, size_t _id, const FkClassType &o)
        : name(_name), id(_id), next(&o) {
}

FkClassType::~FkClassType() {}

const char *FkClassType::getName() const {
    return name;
}

bool FkClassType::is(const FkClassType &o) const {
    if (this == &o) {
        return true;
    }
    auto cur = const_cast<FkClassType *>(this);
    do {
        if (cur->id == o.id) {
            return true;
        }
    } while ((cur = const_cast<FkClassType *>(cur->next)) != nullptr);
    return false;
}

size_t FkClassType::getId() const {
    return id;
}

bool operator==(const FkClassType &o0, const FkClassType &o1) {
    return o0.is(o1);
}

bool operator!=(const FkClassType &o0, const FkClassType &o1) {
    return !(o0 == o1);
}
