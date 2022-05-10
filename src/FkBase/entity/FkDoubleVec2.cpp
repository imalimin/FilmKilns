/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 21:15:50
*/

#include "FkDoubleVec2.h"
#include <cmath>

FK_IMPL_CLASS_TYPE(FkDoubleVec2, FkVec2)

FkDoubleVec2::FkDoubleVec2(double x, double y) : FkVec2<double>(x, y) {

}

FkDoubleVec2::FkDoubleVec2(const FkDoubleVec2 &o) : FkVec2<double>(o) {

}

FkDoubleVec2::~FkDoubleVec2() {

}

FkDoubleVec2 FkDoubleVec2::operator/(double len) const {
    return FkDoubleVec2(x / len, y / len);
}

FkDoubleVec2 FkDoubleVec2::operator*(double len) const {
    return FkDoubleVec2(x * len, y * len);
}

FkDoubleVec2 FkDoubleVec2::operator-(const FkDoubleVec2 &other) const {
    return FkDoubleVec2(x - other.x, y - other.y);
}

FkDoubleVec2 FkDoubleVec2::operator+(const FkDoubleVec2 &other) const {
    return FkDoubleVec2(x + other.x, y + other.y);
}

FkDoubleVec2 FkDoubleVec2::normalize() {
    double len = std::sqrt(x * x + y * y);
    return *this / len;
}
