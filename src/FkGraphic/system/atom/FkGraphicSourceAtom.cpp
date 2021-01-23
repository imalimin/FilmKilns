/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicSourceAtom.h"

FkGraphicSourceAtom::FkGraphicSourceAtom() : FkAtom() {
    client = std::make_shared<FkLocalClient>();

}

FkGraphicSourceAtom::~FkGraphicSourceAtom() {

}

FkResult FkGraphicSourceAtom::onCreate() {
    return FkAtom::onCreate();
}

FkResult FkGraphicSourceAtom::onDestroy() {
    return FkAtom::onDestroy();
}

FkResult FkGraphicSourceAtom::onStart() {
    return FkAtom::onStart();
}

FkResult FkGraphicSourceAtom::onStop() {
    return FkAtom::onStop();
}
