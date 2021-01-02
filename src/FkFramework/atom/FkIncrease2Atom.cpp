/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkIncrease2Atom.h"
#include "FkCalculateProt.h"

FkIncrease2Atom::FkIncrease2Atom() : FkAtom() {

}

FkIncrease2Atom::~FkIncrease2Atom() {

}

void FkIncrease2Atom::describeProtocols(std::shared_ptr<FkProtDesc> desc) {
    desc->add(std::static_pointer_cast<FkProtocol>(std::make_shared<FkCalculateProt>()),
              reinterpret_cast<FkQuark::ProtHandler>(&FkIncrease2Atom::_onIncrease2));
}

FkResult FkIncrease2Atom::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    mIncreaseQuark = std::make_shared<FkIncreaseQuark>();
    if (FK_OK != mIncreaseQuark->onCreate()) {
        FkLogI(FK_DEF_TAG, "Create increase atom failed.");
    }
    session = FkSession::with<FkCalculateProt>(std::make_shared<FkCalculateProt>());
    if (FK_OK != FK_CONNECT_TO(session, mIncreaseQuark)) {
        return FK_FAIL;
    }
    session->open();
    return ret;
}

FkResult FkIncrease2Atom::onDestroy() {
    auto ret = FkAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    if (FK_OK != mIncreaseQuark->onDestroy()) {
        FkLogI(FK_DEF_TAG, "Destroy increase atom failed.");
    }
    ret = session->close();
    return ret;
}

FkResult FkIncrease2Atom::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    if (FK_OK != mIncreaseQuark->onStart()) {
        FkLogI(FK_DEF_TAG, "Start increase atom failed.");
    }
    return ret;
}

FkResult FkIncrease2Atom::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    if (FK_OK != mIncreaseQuark->onStop()) {
        FkLogI(FK_DEF_TAG, "Stop increase atom failed.");
    }
    return ret;
}

FkResult FkIncrease2Atom::_onIncrease2(std::shared_ptr<FkProtocol> p) {
    executor.send(session, p);
    return executor.send(session, p);
}
