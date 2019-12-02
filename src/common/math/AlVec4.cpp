/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVec4.h"
#include "Logcat.h"

#define TAG "AlVec4"

AlVec4::AlVec4(float x, float y, float z, float w) : Object(), x(x), y(y), z(z), w(w) {

}

AlVec4::AlVec4(const AlVec4 &o) : Object(), x(o.x), y(o.y), z(o.z), w(o.w) {

}

AlVec4::~AlVec4() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
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

AlVec4 &AlVec4::multiply(AlMatrix mat, bool pre) {
    AlVec4 vec = *this;
    for (int i = 0; i < 4; i++) {
        if (pre) {
            set(i, mat.get(0, i) * vec.x +
                   mat.get(1, i) * vec.y +
                   mat.get(2, i) * vec.z +
                   mat.get(3, i) * vec.w);
        } else {
            set(i, mat.get(i, 0) * vec.x +
                   mat.get(i, 1) * vec.y +
                   mat.get(i, 2) * vec.z +
                   mat.get(i, 3) * vec.w);
        }
    }
    return *this;
}

AlVec2 AlVec4::xy() {
    return AlVec2(x, y);
}

void AlVec4::dump() {
    Logcat::i(TAG, "[%f, %f, %f, %f]", x, y, z, w);
}
