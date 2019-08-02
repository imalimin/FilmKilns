/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/BinaryUtils.h"

bool BinaryUtils::match(const uint8_t *p, initializer_list<int8_t> args) {
    for (int i = 0; i < args.size(); ++i) {
        if (p[i] != args.begin()[i]) {
            return false;
        }
    }
    return true;
}