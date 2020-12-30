/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkQuark.h"

FkQuark::FkQuark() : FkObject() {
    prot = std::make_shared<FkProtDesc>(this);
}

FkQuark::~FkQuark() {

}

void FkQuark::onCreate() {
    describeProtocols(prot);
}

void FkQuark::onDestroy() {

}

void FkQuark::onStart() {

}

void FkQuark::onStop() {

}