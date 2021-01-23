/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPAtom.h"
#include "FkGraphicLayerPrt.h"

FkGraphicMVPAtom::FkGraphicMVPAtom() : FkAtom() {
    client = std::make_shared<FkLocalClient>();
    mTransQuark = std::make_shared<FkGraphicTransQuark>();
    mScaleQuark = std::make_shared<FkGraphicScaleQuark>();
    mRotateQuark = std::make_shared<FkGraphicRotateQuark>();
}

FkGraphicMVPAtom::~FkGraphicMVPAtom() {

}

void FkGraphicMVPAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMVPAtom::_onDrawLayer);
}

FkResult FkGraphicMVPAtom::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnCreatePrt>(mTransQuark, mScaleQuark, mRotateQuark);
    if (FK_OK != ret) {
        return ret;
    }
    mLayerSesion = FkSession::with(std::make_shared<FkGraphicLayerPrt>());
    ret = mLayerSesion->connectTo(mTransQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSesion->connectTo(mScaleQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSesion->connectTo(mRotateQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSesion->open();
}

FkResult FkGraphicMVPAtom::onDestroy() {
    return FkAtom::onDestroy();
}

FkResult FkGraphicMVPAtom::onStart() {
    return FkAtom::onStart();
}

FkResult FkGraphicMVPAtom::onStop() {
    return FkAtom::onStop();
}

FkResult FkGraphicMVPAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return client->send(mLayerSesion, p);
}