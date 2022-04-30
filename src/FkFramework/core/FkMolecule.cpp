/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMolecule.h"

FK_IMPL_CLASS_TYPE(FkMolecule, FkAtom)

FkMolecule::FkMolecule() : FkAtom() {

}

FkMolecule::~FkMolecule() {

}

FkResult FkMolecule::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkMolecule::onDestroy() {
    auto ret = FkAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkMolecule::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkMolecule::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}
