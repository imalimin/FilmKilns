/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicUpdateTexPrt.h"

FkGraphicUpdateTexPrt::FkGraphicUpdateTexPrt()
        : FkProtocol(), size(0, 0) {
    FK_MARK_SUPER
}

FkGraphicUpdateTexPrt::FkGraphicUpdateTexPrt(const FkGraphicUpdateTexPrt &o) : FkProtocol(o),
                                                                               id(o.id),
                                                                               size(o.size) {
    FK_MARK_SUPER
}

FkGraphicUpdateTexPrt::~FkGraphicUpdateTexPrt() {
    size.set(0, 0);
}