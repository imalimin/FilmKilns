/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMolecule.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicMVPAtom.h"
#include "FkGraphicSourceAtom.h"
#include "FkGraphicRenderAtom.h"
#include "FkGraphicScreenAtom.h"

FkGraphicMolecule::FkGraphicMolecule() : FkMolecule() {
    client = std::make_shared<FkLocalClient>();
    mMVPAtom = std::make_shared<FkGraphicMVPAtom>();
    mSrcAtom = std::make_shared<FkGraphicSourceAtom>();
    mRenderAtom = std::make_shared<FkGraphicRenderAtom>();
    mScreenAtom = std::make_shared<FkGraphicScreenAtom>();
}

FkGraphicMolecule::~FkGraphicMolecule() {

}

void FkGraphicMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
}

FkResult FkGraphicMolecule::onCreate() {
    auto ret = FkMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnCreatePrt>(mMVPAtom, mSrcAtom, mRenderAtom, mScreenAtom);
    if (FK_OK != ret) {
        return ret;
    }
    mLayerSession = FkSession::with(std::make_shared<FkGraphicLayerPrt>());
    ret = mLayerSession->connectTo(mMVPAtom);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mSrcAtom);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mRenderAtom);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mScreenAtom);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSession->open();
}

FkResult FkGraphicMolecule::onDestroy() {
    auto ret = FkMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnDestroyPrt>(mMVPAtom, mSrcAtom, mRenderAtom, mScreenAtom);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSession->close();
}

FkResult FkGraphicMolecule::onStart() {
    auto ret = FkMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnStartPrt>(mMVPAtom, mSrcAtom, mRenderAtom, mScreenAtom);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onStop() {
    auto ret = FkMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnStopPrt>(mMVPAtom, mSrcAtom, mRenderAtom, mScreenAtom);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return client->send(mLayerSession, p);
}