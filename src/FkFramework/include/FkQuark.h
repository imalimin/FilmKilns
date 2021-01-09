/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKQUARK_H
#define FK_FRAMEWORK_FKQUARK_H

#include "FkObject.h"
#include "FkProtDesc.h"
#include <mutex>

class FkSession;

FK_ABS_CLASS FkQuark FK_EXTEND FkObject {
    friend FkSession;
public:
    AFK_ENUM kState : int {
        IDL = -1,
        CREATED,
        STARTED,
        STOPPED,
    };

    typedef int (FkQuark::*ProtHandler)(std::shared_ptr<FkProtocol>);

public:
    FkQuark();

    virtual ~FkQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkProtDesc> desc) = 0;

    virtual FkResult onCreate();

    virtual FkResult onDestroy();

    virtual FkResult onStart();

    virtual FkResult onStop();

    virtual FkResult dispatch(std::shared_ptr<FkProtocol> p);

private:
    FkQuark(const FkQuark &o) : FkObject() {};

    FkResult _onCreate(std::shared_ptr<FkProtocol> p);

    FkResult _onDestroy(std::shared_ptr<FkProtocol> p);

    FkResult _onStart(std::shared_ptr<FkProtocol> p);

    FkResult _onStop(std::shared_ptr<FkProtocol> p);

    FkResult accept(const std::shared_ptr<FkProtocol> p);

    FkResult _changeState(kState src, kState dst);

private:
    std::shared_ptr<FkProtDesc> prot = nullptr;
    std::mutex mtx;
    kState state = kState::IDL;
};


#endif //FK_FRAMEWORK_FKQUARK_H
