/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicSourceAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicContextQuark.h"
#include "FkGraphicTexQuark.h"
#include "FkGraphicFBOQuark.h"
#include "FkGraphicProgramQuark.h"

FkGraphicSourceAtom::FkGraphicSourceAtom() : FkAtom() {
    client = std::make_shared<FkLocalClient>();
    mCtxQuark = std::make_shared<FkGraphicContextQuark>();
    mTexQuark = std::make_shared<FkGraphicTexQuark>();
    mFBOQuark = std::make_shared<FkGraphicFBOQuark>();
    mProgramQuark = std::make_shared<FkGraphicProgramQuark>();
}

FkGraphicSourceAtom::~FkGraphicSourceAtom() {

}

void FkGraphicSourceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicSourceAtom::_onDrawLayer);
}

FkResult FkGraphicSourceAtom::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnCreatePrt>(mCtxQuark, mTexQuark, mFBOQuark, mProgramQuark);
    if (FK_OK != ret) {
        return ret;
    }
    mLayerSession = FkSession::with(std::make_shared<FkGraphicLayerPrt>());
    ret = mLayerSession->connectTo(mCtxQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mTexQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mFBOQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = mLayerSession->connectTo(mProgramQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSession->open();
}

FkResult FkGraphicSourceAtom::onDestroy() {
    auto ret = FkAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnDestroyPrt>(mCtxQuark, mTexQuark, mFBOQuark, mProgramQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return mLayerSession->close();
}

FkResult FkGraphicSourceAtom::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnStartPrt>(mCtxQuark, mTexQuark, mFBOQuark, mProgramQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicSourceAtom::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnStopPrt>(mCtxQuark, mTexQuark, mFBOQuark, mProgramQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicSourceAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return client->send(mLayerSession, p);
}
