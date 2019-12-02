/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVec4.h"
#include "Logcat.h"

#define TAG "AlVec4"

AlVec4::AlVec4(float x, float y, float z, float w) : Object() {

}

AlVec4::AlVec4(const AlVec4 &o) : Object(), x(o.x), y(o.y), z(o.z), w(o.w) {

}

AlVec4::~AlVec4() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

void AlVec4::set(int32_t i, float val) {
    switch (i) {
        case 0: {
            x = val;
            break;
        }
        case 1: {
            y = val;
            break;
        }
        case 2: {
            z = val;
            break;
        }
        case 3: {
            w = val;
            break;
        }
        default:
            break;
    }
}

void AlVec4::multiply(AlMatrix mat, bool pre = false) {
    AlVec4 vec = *this;
    for (int i = 0; i < 4; i++) {
        set(i, mat.get(i, 0) * vec.x +
               mat.get(i, 1) * vec.y +
               mat.get(i, 2) * vec.z +
               mat.get(i, 3) * vec.w);
    }

}

void AlVec4::dump() {
    Logcat::i(TAG, "[%f, %f, %f, %f]", x, y, z, w);
}
