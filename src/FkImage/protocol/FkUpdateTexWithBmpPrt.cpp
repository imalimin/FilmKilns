/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkUpdateTexWithBmpPrt.h"

FkUpdateTexWithBmpPrt::FkUpdateTexWithBmpPrt()
        : FkProtocol() {
    FK_MARK_SUPER
}

FkUpdateTexWithBmpPrt::FkUpdateTexWithBmpPrt(const FkUpdateTexWithBmpPrt &o)
        : FkProtocol(o),
          id(o.id) {
    FK_MARK_SUPER
    bmp = o.bmp;
}

FkUpdateTexWithBmpPrt::~FkUpdateTexWithBmpPrt() {
}