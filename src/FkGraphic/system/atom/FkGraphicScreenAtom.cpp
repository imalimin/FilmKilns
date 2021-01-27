/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicScreenAtom.h"
#include "FkGraphicLayerPrt.h"

FkGraphicScreenAtom::FkGraphicScreenAtom() : FkAtom() {
    client = std::make_shared<FkLocalClient>();

}

FkGraphicScreenAtom::~FkGraphicScreenAtom() {

}

void FkGraphicScreenAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicScreenAtom::_onDrawLayer);

}

FkResult FkGraphicScreenAtom::onCreate() {
    return FkAtom::onCreate();
}

FkResult FkGraphicScreenAtom::onDestroy() {
    return FkAtom::onDestroy();
}

FkResult FkGraphicScreenAtom::onStart() {
    return FkAtom::onStart();
}

FkResult FkGraphicScreenAtom::onStop() {
    return FkAtom::onStop();
}

FkResult FkGraphicScreenAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
