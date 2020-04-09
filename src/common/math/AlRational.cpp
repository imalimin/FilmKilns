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
    static AlRational *r1;
    if (r1 == nullptr) {
        r1 = new AlRational();
    }
    int32_t gcd = AlMath::gcd(this->den, r.den);
    r1->num = this->num * r.den / gcd + r.num * this->den / gcd;
    r1->den = this->den * r.den / gcd;
    return *r1;
}

AlRational &AlRational::operator=(const AlRational &o) {
    this->num = o.num;
    this->den = o.den;
    return *this;
}

float AlRational::toFloat() {
    if (0 == den) {
        return 0.0f;
    }
    return num / (float) den;
}

double AlRational::toDouble() {
    if (0 == den) {
        return 0.0;
    }
    return num / (double) den;
}
