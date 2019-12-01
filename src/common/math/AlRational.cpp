/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRational.h"

AlRational::AlRational(int32_t num, int32_t den) : Object(), num(num), den(den) {

}

AlRational::AlRational(const AlRational &o) : Object(), num(o.num), den(o.den) {

}

AlRational::~AlRational() {
    num = 0;
    num = 1;
}
