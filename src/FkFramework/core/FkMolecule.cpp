/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMolecule.h"

FkMolecule::FkMolecule() : FkQuark() {

}

FkMolecule::~FkMolecule() {

}

FkResult FkMolecule::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkMolecule::onDestroy() {
    auto ret = FkQuark::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkMolecule::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkMolecule::onStop() {
    auto ret = FkQuark::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}
