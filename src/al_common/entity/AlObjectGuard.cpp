/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlObjectGuard.h"

AlObjectGuard::AlObjectGuard(Object **obj) : Object(), obj(obj) {

}

AlObjectGuard::~AlObjectGuard() {
    delete *obj;
    *obj = nullptr;
}
