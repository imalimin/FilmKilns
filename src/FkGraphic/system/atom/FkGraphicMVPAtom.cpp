/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicTransQuark.h"
#include "FkGraphicScaleQuark.h"
#include "FkGraphicRotateQuark.h"

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
    mLayerSession = FkSession::with(std::make_shared<FkGraphicLayerPrt>());
    ret = mLayerSession->connectTo(mTransQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mScaleQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mRotateQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSession->open();
}

FkResult FkGraphicMVPAtom::onDestroy() {
    auto ret = FkAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnDestroyPrt>(mTransQuark, mScaleQuark, mRotateQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSession->close();
}

FkResult FkGraphicMVPAtom::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnStartPrt>(mTransQuark, mScaleQuark, mRotateQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnStopPrt>(mTransQuark, mScaleQuark, mRotateQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return client->send(mLayerSession, p);
}