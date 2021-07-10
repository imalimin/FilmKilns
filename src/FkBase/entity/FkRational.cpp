/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRational.h"
#include "AlMath.h"

FkRational::FkRational() : FkObject(), num(0), den(1) {

}

FkRational::FkRational(int32_t num, int32_t den) : FkObject(), num(num), den(den) {

}

FkRational::FkRational(const FkRational &o) : FkObject(), num(o.num), den(o.den) {

}

FkRational::~FkRational() {
    num = 0;
    den = 1;
}

FkRational &FkRational::operator+(FkRational r) {
    static FkRational *r1;
    if (r1 == nullptr) {
        r1 = new FkRational();
    }
    int32_t gcd = AlMath::gcd(this->den, r.den);
    r1->num = this->num * r.den / gcd + r.num * this->den / gcd;
    r1->den = this->den * r.den / gcd;
    return *r1;
}

FkRational &FkRational::operator=(const FkRational &o) {
    this->num = o.num;
    this->den = o.den;
    return *this;
}

float FkRational::toFloat() {
    if (0 == den) {
        return 0.0f;
    }
    return num / (float) den;
}

double FkRational::toDouble() {
    if (0 == den) {
        return 0.0;
    }
    return num / (double) den;
}

FkRational &FkRational::operator+=(const FkRational &value) {
    num += value.num;
    den += value.den;
    return *this;
}
