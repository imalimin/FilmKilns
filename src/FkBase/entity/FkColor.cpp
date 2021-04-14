/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColor.h"

FkColor::FkColor() {
    FK_MARK_SUPER
}

FkColor::FkColor(const FkColor &o)
        : FkObject(), format(o.format), red(o.red), greed(o.greed), blue(o.blue), alpha(o.alpha) {
    FK_MARK_SUPER
}

FkColor::~FkColor() {

}
