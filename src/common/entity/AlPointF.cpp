/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPointF.h"

AlPointF::AlPointF(float x, float y) : AlVec2(x, y) {

}

AlPointF::AlPointF(const AlVec2 &o) : AlVec2(o) {

}

AlPointF::~AlPointF() {

}
