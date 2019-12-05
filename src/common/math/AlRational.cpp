/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRational.h"
#include "AlMath.h"

AlRational::AlRational() : Object(), num(0), den(1) {

}

AlRational::AlRational(int32_t num, int32_t den) : Object(), num(num), den(den) {

}

AlRational::AlRational(const AlRational &o) : Object(), num(o.num), den(o.den) {

}

AlRational::~AlRational() {
    num = 0;
    den = 1;
}

AlRational &AlRational::operator+(AlRational r) {
    int32_t gcd = AlMath::gcd(this->den, r.den);
    this->den = this->den * r.den / gcd;
    this->num = this->num * r.den / gcd + r.num * this->den / gcd;
    return *this;
}

float AlRational::toFloat() {
    return num / (float) den;
}
