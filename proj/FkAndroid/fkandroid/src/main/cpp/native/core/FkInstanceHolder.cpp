/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkInstanceHolder.h"

FK_IMPL_CLASS_TYPE(FkInstanceHolder, FkObject)

std::unique_ptr<FkInstanceHolder>
        FkInstanceHolder::instance = std::unique_ptr<FkInstanceHolder>(new FkInstanceHolder());

FkInstanceHolder &FkInstanceHolder::getInstance() {
    return *instance;
}

FkInstanceHolder::FkInstanceHolder() : FkObject() {
}

FkInstanceHolder::~FkInstanceHolder() {
}

long FkInstanceHolder::put(std::any instance) {
    std::lock_guard<std::mutex> guard(mtx);
    vec.emplace_back(instance);
    return vec.size();
}

void FkInstanceHolder::release(long index) {
    std::lock_guard<std::mutex> guard(mtx);
    vec[index - 1] = nullptr;
}
