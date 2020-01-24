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

AlCoordinate::AlCoordinate(const AlCoordinate &o) {

}

AlCoordinate::~AlCoordinate() {

}

void AlCoordinate::translate(AlVec2 &vec2, AlCoordinate &coordinate) {
    AlVec4 src(vec2);
    AlVec4 dest = src * mat;
    vec2.x = dest.x;
    vec2.y = dest.y;
}

void AlCoordinate::setScale(float scaleX, float scaleY) {
    mat.setScale(scaleX, scaleY);
}

void AlCoordinate::setRotation(float alpha) {
    mat.setRotation(alpha);
}

void AlCoordinate::setPosition(float x, float y) {
    mat.setTranslate(x, y);
}
