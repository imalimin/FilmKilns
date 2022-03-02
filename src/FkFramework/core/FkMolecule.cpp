/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMolecule.h"

FkMolecule::FkMolecule() : FkAtom() {

}

FkMolecule::~FkMolecule() {

}

FkResult FkMolecule::dispatch(std::shared_ptr<FkProtocol> p) {
    if (FK_INSTANCE_OF(p, FkOnCreatePrt)) {
        auto proto = std::dynamic_pointer_cast<FkOnCreatePrt>(p);
        if (proto) {
            proto->context = shareContextToSubQuark();
        }
    }
    return FkAtom::dispatch(p);
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
