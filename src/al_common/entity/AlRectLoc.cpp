/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRectLoc.h"

AlRectLoc AlRectLoc::value(int32_t val) {
    return AlRectLoc(val);
}

AlRectLoc AlRectLoc::CENTER() {
    return AlRectLoc(0);
}

AlRectLoc AlRectLoc::LT() {
    return AlRectLoc(LEFT() | TOP());
}

AlRectLoc AlRectLoc::RT() {
    return AlRectLoc(RIGHT() | TOP());
}

AlRectLoc AlRectLoc::RB() {
    return AlRectLoc(RIGHT() | BOTTOM());
}

AlRectLoc AlRectLoc::LB() {
    return AlRectLoc(LEFT() | BOTTOM());
}

AlRectLoc AlRectLoc::LEFT() {
    return AlRectLoc(1);
}

AlRectLoc AlRectLoc::TOP() {
    return AlRectLoc(2);
}

AlRectLoc AlRectLoc::RIGHT() {
    return AlRectLoc(4);
}

AlRectLoc AlRectLoc::BOTTOM() {
    return AlRectLoc(8);
}

AlRectLoc::AlRectLoc(int32_t loc) : Object(), loc(loc) {

}

AlRectLoc::AlRectLoc(const AlRectLoc &o) : Object(), loc(o.loc) {

}

AlRectLoc::~AlRectLoc() {

}

bool AlRectLoc::operator==(const AlRectLoc &o) {
    return loc == o.loc;
}

int32_t AlRectLoc::int32() {
    return loc;
}

AlRectLoc AlRectLoc::operator|(const AlRectLoc &o) {
    return AlRectLoc(loc | o.loc);
}

AlRectLoc AlRectLoc::operator&(const AlRectLoc &o) {
    return AlRectLoc(loc & o.loc);
}
