/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkUpdateTexWithBmpPrt.h"

FkUpdateTexWithBmpPrt::FkUpdateTexWithBmpPrt()
        : FkProtocol(), size(0, 0) {
    FK_MARK_SUPER
}

FkUpdateTexWithBmpPrt::FkUpdateTexWithBmpPrt(const FkUpdateTexWithBmpPrt &o)
        : FkProtocol(o),
          id(o.id),
          size(o.size) {
    FK_MARK_SUPER
}

FkUpdateTexWithBmpPrt::~FkUpdateTexWithBmpPrt() {
    pixels = nullptr;
    size.set(0, 0);
}