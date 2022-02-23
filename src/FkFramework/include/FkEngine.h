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
#include "FkHandlerThread.h"
#include "FkHandler.h"
#include <future>
#include <map>
#include <mutex>

#define FK_REG_MSG(id, func) \
registerMessage(id, reinterpret_cast<FkMessageHandler>(&func)) \


class FkEngine;

typedef FkResult (FkEngine::*FkMessageHandler)(std::shared_ptr<FkMessage>);

FK_CLASS FkMessageHandlerPair FK_EXTEND FkObject {
public:
    FkMessageHandlerPair(FkID what, FkMessageHandler handler);

    FkMessageHandlerPair(const FkMessageHandlerPair &o);

    virtual ~FkMessageHandlerPair();

    bool handle(FkEngine *target, std::shared_ptr<FkMessage> msg);

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

//    template<class T>
//    std::future<T> sendFuture(std::shared_ptr<T> protocol) {
//        auto prom = std::make_shared<std::promise<T>>();
//        auto *msg = AlMessage::obtain(MSG_FUTURE);
//        msg->sp = prom;
//        mHandler->sendMessage(msg);
//        return prom->get_future();
//    }
protected:
    virtual FkResult onCreate();

    virtual FkResult onDestroy();

    virtual FkResult onStart();

    virtual FkResult onStop();

    FkResult sendMessage(std::shared_ptr<FkMessage> &msg, bool ignoreState = false);

    FkResult registerMessage(FkID what, FkMessageHandler handler);

private:
    virtual FkResult _onCreate(std::shared_ptr<FkMessage> msg);

    virtual FkResult _onDestroy(std::shared_ptr<FkMessage> msg);

    virtual FkResult _onStart(std::shared_ptr<FkMessage> msg);

    virtual FkResult _onStop(std::shared_ptr<FkMessage> msg);

    FkResult _changeState(kState &_state, uint32_t src, kState dst);

    void _dispatch(std::shared_ptr<FkMessage> &msg);

private:
    static const FkID FK_MSG_CREATE;
    static const FkID FK_MSG_DESTROY;
    static const FkID FK_MSG_START;
    static const FkID FK_MSG_STOP;
    std::string name;
    std::shared_ptr<FkHandlerThread> mThread = nullptr;
    FkHandler *mHandler = nullptr;
    std::map<FkID, FkMessageHandlerPair> mMsgMap;
    std::recursive_mutex mtx;
    std::mutex msgMtx;
    kState outsideState = kState::IDL;
    kState internalState = kState::IDL;
};


#endif //FK_FRAMEWORK_FKENGINE_H
