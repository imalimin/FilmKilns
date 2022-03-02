/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSimpleMolecule.h"

FkSimpleMolecule::FkSimpleMolecule() : FkSimpleAtom() {
    FK_MARK_SUPER

}

FkSimpleMolecule::~FkSimpleMolecule() {

}

FkResult FkSimpleMolecule::dispatch(std::shared_ptr<FkProtocol> p) {
    if (FK_INSTANCE_OF(p, FkOnCreatePrt)) {
        auto proto = std::dynamic_pointer_cast<FkOnCreatePrt>(p);
        if (proto) {
            proto->context = shareContextToSubQuark();
        }
    }
    return FkSimpleAtom::dispatch(p);
}

FkResult FkSimpleMolecule::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleMolecule::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleMolecule::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkSimpleMolecule::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}