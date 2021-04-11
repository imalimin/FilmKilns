/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKENGINE_H
#define FK_FRAMEWORK_FKENGINE_H

#include "FkObject.h"
#include "FkSimpleMolecule.h"
#include "AlHandlerThread.h"
#include "AlHandler.h"
#include <future>
#include <map>
#include <mutex>

#define FK_REG_MSG(id, func) \
_registerMessage(id, reinterpret_cast<FkMessageHandler>(&func)); \

class FkEngine;

typedef bool (FkEngine::*FkMessageHandler)(AlMessage *);

FK_CLASS FkMessageHandlerPair FK_EXTEND FkObject {
public:
    FkMessageHandlerPair(FkID what, FkMessageHandler handler);

    FkMessageHandlerPair(const FkMessageHandlerPair &o);

    virtual ~FkMessageHandlerPair();

    bool handle(FkEngine *target, AlMessage *msg);

protected:
    FkID what = 0;
    FkMessageHandler handler;
};

FK_ABS_CLASS FkEngine FK_EXTEND FkObject {
public:
    FkEngine(std::string name);

    FkEngine(const FkEngine &o) = delete;

    virtual ~FkEngine();

    virtual FkResult create();

    virtual FkResult destroy();

    virtual FkResult start();

    virtual FkResult stop();

    FkResult sendMessage(AlMessage *message);

//    template<class T>
//    std::future<T> sendFuture(std::shared_ptr<T> protocol) {
//        auto prom = std::make_shared<std::promise<T>>();
//        auto *msg = AlMessage::obtain(MSG_FUTURE);
//        msg->sp = std::move(prom);
//        mHandler->sendMessage(msg);
//        return prom->get_future();
//    }

private:
    FkResult _changeState(kState src, kState dst);

    void _dispatch(AlMessage *msg);

    FkResult _registerMessage(FkID what, FkMessageHandler handler);

private:
    static const FkID FK_MSG_CREATE;
    static const FkID FK_MSG_DESTROY;
    static const FkID FK_MSG_START;
    static const FkID FK_MSG_STOP;
    std::string name;
    AlHandlerThread *mThread = nullptr;
    AlHandler *mHandler = nullptr;
    std::map<FkID, FkMessageHandlerPair> mMsgMap;
    std::mutex mtx;
    kState state = kState::IDL;
};


#endif //FK_FRAMEWORK_FKENGINE_H
