/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 23:50:58
*/

#include "FkEmptyQuark.h"

FkEmptyQuark::FkEmptyQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkEmptyQuark::~FkEmptyQuark() {

}

void FkEmptyQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}