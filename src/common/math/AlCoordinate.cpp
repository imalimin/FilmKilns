/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCoordinate.h"

AlCoordinate &AlCoordinate::create() {
    static AlCoordinate *coord;
    if (coord == nullptr) {
        coord = new AlCoordinate();
    }
    return *coord;
}

AlCoordinate::AlCoordinate(const AlCoordinate &o)
        : Object(), mat(o.mat),
          transX(o.transX),
          transY(o.transY) {

}

AlCoordinate::~AlCoordinate() {
    transX = 0;
    transY = 0;
}

void AlCoordinate::translate(int32_t &x, int32_t &y) {
    AlVec4 src(x + transX, y + transY);
    AlVec4 dest = src * mat;
    x = static_cast<int32_t>(dest.x);
    y = static_cast<int32_t>(dest.y);
}

void AlCoordinate::setScale(float scaleX, float scaleY) {
    mat.setScale(scaleX, scaleY);
}

void AlCoordinate::setRotation(float alpha) {
    mat.setRotation(alpha);
}

void AlCoordinate::seTranslate(int32_t x, int32_t y) {
    transX = x;
    transY = y;
//    mat.setTranslate(x, y);
}
