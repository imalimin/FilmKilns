/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FKANDROID_FKINSTANCEHOLDER_H
#define FKANDROID_FKINSTANCEHOLDER_H

#include "FkObject.h"
#include <mutex>
#include <vector>
#include <any>

FK_CLASS FkInstanceHolder FK_EXTEND FkObject {
public:
    static FkInstanceHolder &getInstance();

    FkInstanceHolder(const FkInstanceHolder &o) = delete;

    ~FkInstanceHolder();

    long put(std::any instance);

    void release(long index);

    template<typename T>
    T find(const long index) {
        std::lock_guard<std::mutex> guard(mtx);
        return std::any_cast<T>(vec[index - 1]);
    }

private:
    FkInstanceHolder();

private:
    static std::unique_ptr<FkInstanceHolder> instance;
    std::mutex mtx;
    std::vector<std::any> vec;
};


#endif //FKANDROID_FKINSTANCEHOLDER_H
