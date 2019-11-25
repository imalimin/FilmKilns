/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlIdentityCreator.h"

AlIdentityCreator::AlIdentityCreator() : Object(), cur(0) {
}

AlIdentityCreator::AlIdentityCreator(const AlIdentityCreator &o) : Object(), cur(0) {

}

AlIdentityCreator::~AlIdentityCreator() {
    this->cur = 0;
}

int32_t AlIdentityCreator::generate() {
    std::lock_guard<std::mutex> guard(mtx);
    ++this->cur;
    return this->cur;
}
