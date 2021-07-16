/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkWinModelQuark.h"

FkWinModelQuark::FkWinModelQuark() : FkQuark(), winSize(0, 0) {
    FK_MARK_SUPER
}

FkWinModelQuark::~FkWinModelQuark() {

}

void FkWinModelQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}

FkResult FkWinModelQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkWinModelQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkWinModelQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkWinModelQuark::onStop() {
    return FkQuark::onStop();
}