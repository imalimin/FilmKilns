/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-27 17:09:52
*/

#include "FkMath.h"

FkMath::~FkMath() {

}

float FkMath::distance(FkIntVec2 &p0, FkIntVec2 &p1) {
    return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}

double FkMath::distance(FkDoubleVec2 &p0, FkDoubleVec2 &p1) {
    return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}
