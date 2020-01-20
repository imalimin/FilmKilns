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

void AlCoordinate::translate(AlVec2 vec2, AlCoordinate &coordinate) {

}
