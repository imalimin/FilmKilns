/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVec2.h"
#include "AlMath.h"

void FkVec2::swap(FkVec2 *vec1, FkVec2 *vec2) {
    float tmp = vec1->x;
    vec1->x = vec2->x;
    vec2->x = tmp;

    tmp = vec1->y;
    vec1->y = vec2->y;
    vec2->y = tmp;
}

FkVec2::FkVec2(float x, float y) : FkObject(), x(x), y(y) {

}

FkVec2::FkVec2(const FkVec2 &o) : FkObject(), x(o.x), y(o.y) {
}

FkVec2::~FkVec2() {
    x = 0.0f;
    y = 0.0f;
}

void FkVec2::set(FkVec2 vec) {
    x = vec.x;
    y = vec.y;
}

bool FkVec2::isZero() {
    return 0 == x && 0 == y;
}

FkVec2 &FkVec2::operator+(const float value) {
    static FkVec2 *vec2;
    if (vec2 == nullptr) {
        vec2 = new FkVec2();
    }
    vec2->x = x + value;
    vec2->y = y + value;
    return *vec2;
}

FkVec2 &FkVec2::operator+(const FkVec2 &vec) {
    static FkVec2 *vec2;
    if (vec2 == nullptr) {
        vec2 = new FkVec2();
    }
    vec2->x = x + vec.x;
    vec2->y = y + vec.x;
    return *vec2;
}

FkVec2 &FkVec2::operator-(const FkVec2 &vec) {
    static FkVec2 *vec2;
    if (vec2 == nullptr) {
        vec2 = new FkVec2();
    }
    vec2->x = x - vec.x;
    vec2->y = y - vec.x;
    return *vec2;
}

FkVec2 &FkVec2::operator/(const float value) {
    static FkVec2 *vec2;
    if (vec2 == nullptr) {
        vec2 = new FkVec2();
    }
    vec2->x = x / value;
    vec2->y = y / value;
    return *vec2;
}

bool FkVec2::operator<(const FkVec2 &vec) {
    return x < vec.x && y < vec.y;
}

bool FkVec2::operator>(const FkVec2 &vec) {
    return x > vec.x && y > vec.y;
}

bool FkVec2::operator==(const FkVec2 &vec) {
    return x == vec.x && y == vec.y;
}

bool FkVec2::isNan() {
    return isnanf(x) || isnanf(y) ;
}