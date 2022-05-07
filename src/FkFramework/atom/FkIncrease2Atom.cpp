/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkIncrease2Atom.h"
#include "FkCalculatePrt.h"
#include "FkLocalClient.h"

FK_IMPL_CLASS_TYPE(FkIncrease2Atom, FkAtom)

FkIncrease2Atom::FkIncrease2Atom() : FkAtom() {
    client = std::make_shared<FkLocalClient>();
    mIncreaseQuark = std::make_shared<FkIncreaseQuark>();
    mDivideQuark = std::make_shared<FkDivideQuark>();
}

FkIncrease2Atom::~FkIncrease2Atom() {

}

void FkIncrease2Atom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkCalculatePrt, FkIncrease2Atom::_onIncrease2);
}

FkResult FkIncrease2Atom::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    ret = client->quickSend<FkOnCreatePrt>(mIncreaseQuark, mDivideQuark);
    if (FK_OK != ret) {
        return ret;
    }
    session = FkSession::with({FkCalculatePrt_Class::type.getId(), FkCalculatePrt_Class::type.getName()});
    ret = session->connectTo(mIncreaseQuark);
    if (FK_OK != ret) {
        return ret;
    }
    ret = session->connectTo(mDivideQuark);
    if (FK_OK != ret) {
        return ret;
    }
    return session->open();
}

FkResult FkIncrease2Atom::onDestroy() {
    auto ret = FkAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    session->close();
    return client->quickSend<FkOnDestroyPrt>(mIncreaseQuark, mDivideQuark);
}

FkResult FkIncrease2Atom::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return client->quickSend<FkOnStartPrt>(mIncreaseQuark, mDivideQuark);
}

FkResult FkIncrease2Atom::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return client->quickSend<FkOnStopPrt>(mIncreaseQuark, mDivideQuark);
}

FkResult FkIncrease2Atom::_onIncrease2(std::shared_ptr<FkProtocol> p) {
    return client->send(session, p);
}
