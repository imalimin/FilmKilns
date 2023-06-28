/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKQUARK_H
#define FK_FRAMEWORK_FKQUARK_H

#include "FkObject.h"
#include "FkPortDesc.h"
#include "FkFrameworkDefine.h"
#include "FkEngineContext.h"
#include <mutex>

class FkSession;
class FkConnectChain;

FK_SUPER_CLASS(FkQuark, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkQuark)

public:
    friend FkSession;
    friend FkConnectChain;

public:
    FkQuark();

    FkQuark(const FkQuark &o) = delete;

    virtual ~FkQuark();

    virtual std::shared_ptr<FkEngineContext> getContext();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) = 0;

    virtual FkResult onCreate();

    virtual FkResult onDestroy();

    virtual FkResult onStart();

    virtual FkResult onStop();

    virtual FkResult dispatch(std::shared_ptr<FkProtocol> p);

    virtual FkResult queryProtocols(std::list<FkProtocol::Desc> &protocols);

    FkResult accept(const FkProtocol::Type protoType);

    FkResult addPort(uint32_t port, FkProtocol::Desc protoDesc, FkPort::PortFunc func);

private:
    FkResult _onCreate(std::shared_ptr<FkProtocol> p);

    FkResult _onDestroy(std::shared_ptr<FkProtocol> p);

    FkResult _onStart(std::shared_ptr<FkProtocol> p);

    FkResult _onStop(std::shared_ptr<FkProtocol> p);

    FkResult accept(const std::shared_ptr<FkProtocol> p);

    FkResult _changeState(uint32_t src, kState dst);

private:
    std::shared_ptr<FkEngineContext> context = nullptr;
    std::shared_ptr<FkPortDesc> desc = nullptr;
    std::mutex mtx;
    kState state = kState::IDL;
};


#endif //FK_FRAMEWORK_FKQUARK_H
