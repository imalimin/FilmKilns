/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAtom.h"

FK_IMPL_CLASS_TYPE(FkAtom, FkQuark)

FkAtom::FkAtom() : FkQuark() {

}

FkAtom::~FkAtom() {

}

FkResult FkAtom::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkAtom::onDestroy() {
    auto ret = FkQuark::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkAtom::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkAtom::onStop() {
    auto ret = FkQuark::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}