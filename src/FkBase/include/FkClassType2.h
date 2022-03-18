/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-18 23:10:14
*/

#ifndef FK_BASE_FKCLASSTYPE2_H
#define FK_BASE_FKCLASSTYPE2_H

#include "FkClassType.h"

template<typename T>
class FkClassType2 : public FkClassType {
public:
#if defined(__FK_OPTIMIZE_CLASS_TYPE__)

    FkClassType2(const std::list<size_t> &h) {
#else

        FkClassType2(const std::list<string> &h) {
#endif
        for (auto &it:h) {
            hierarchy.emplace_back(it);
        }
        put(typeid(T));
    }

    virtual ~FkClassType2() {
    }

public:
    FkClassType2() {
        put(typeid(T));
    }
};

#endif //FK_BASE_FKCLASSTYPE2_H