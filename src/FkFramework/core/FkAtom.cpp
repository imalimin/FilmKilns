/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAtom.h"

FkAtom::FkAtom() : FkQuark() {

}

FkAtom::~FkAtom() {

}

FkResult FkAtom::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkAtom::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkAtom::onStart() {
    return FkQuark::onStart();
}

FkResult FkAtom::onStop() {
    return FkQuark::onStop();
}