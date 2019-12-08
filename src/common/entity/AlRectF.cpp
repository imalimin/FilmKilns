/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRectF.h"

AlRectF::AlRectF()
        : Object(),
          left(0.0f),
          top(0.0f),
          right(0.0f),
          bottom(0.0f) {

}

AlRectF::AlRectF(float left, float top, float right, float bottom)
        : Object(),
          left(left),
          top(top),
          right(right),
          bottom(bottom) {

}

AlRectF::AlRectF(const AlRectF &o)
        : Object(),
          left(o.left),
          top(o.top),
          right(o.right),
          bottom(o.bottom) {

}

AlRectF::~AlRectF() {
    set(0.0f, 0.0f, 0.0f, 0.0f);
}

float AlRectF::getWidth() {
    float result = right - left;
    if (result < 0) {
        result = -result;
    }
    return result;
}

float AlRectF::getHeight() {
    float result = top - bottom;
    if (result < 0) {
        result = -result;
    }
    return result;
}

void AlRectF::set(float left, float top, float right, float bottom) {
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}