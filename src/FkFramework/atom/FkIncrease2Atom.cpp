/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkIncrease2Atom.h"
#include "FkCalculateProt.h"

FkIncrease2Atom::FkIncrease2Atom() : FkAtom() {
    mIncreaseQuark = std::make_shared<FkIncreaseQuark>();
    mDivideQuark = std::make_shared<FkDivideQuark>();
}

FkIncrease2Atom::~FkIncrease2Atom() {

}

void FkIncrease2Atom::describeProtocols(std::shared_ptr<FkProtDesc> desc) {
    FK_PROT_DESC_ADD(desc, FkCalculateProt, FkIncrease2Atom::_onIncrease2);
}

FkResult FkIncrease2Atom::onCreate() {
    auto ret = FkAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    ret = executor.quickSend<FkOnCreateProt>({mIncreaseQuark, mDivideQuark});
    if (FK_OK != ret) {
        return ret;
    }
    session = FkSession::with(std::make_shared<FkCalculateProt>());
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
    return executor.quickSend<FkOnDestroyProt>({mIncreaseQuark, mDivideQuark});
}

FkResult FkIncrease2Atom::onStart() {
    auto ret = FkAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return executor.quickSend<FkOnStartProt>({mIncreaseQuark, mDivideQuark});
}

FkResult FkIncrease2Atom::onStop() {
    auto ret = FkAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return executor.quickSend<FkOnStopProt>({mIncreaseQuark, mDivideQuark});
}

FkResult FkIncrease2Atom::_onIncrease2(std::shared_ptr<FkProtocol> p) {
    return executor.send(session, p);
}
