/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlIdentityCreator.h"

AlIdentityCreator::AlIdentityCreator() : Object(), cur(NONE_ID) {
}

AlIdentityCreator::AlIdentityCreator(const AlIdentityCreator &o) : Object(), cur(NONE_ID) {

}

AlIdentityCreator::~AlIdentityCreator() {
    this->cur = NONE_ID;
}

int32_t AlIdentityCreator::generate() {
    std::lock_guard<std::mutex> guard(mtx);
    ++this->cur;
    return this->cur;
}
