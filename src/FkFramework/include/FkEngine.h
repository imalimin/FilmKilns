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
#include "FkEngineSettings.h"
#include "FkEngineMonitor.h"
#include <future>
#include <map>
#include <mutex>

class FkEngine;

FK_SUPER_CLASS(FkMsgHandle, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkMsgHandle)

public:
    typedef FkResult (FkEngine::*Func)(std::shared_ptr<FkMessage>);

public:
    FkMsgHandle(FkMsgHandle::Func func);

    FkMsgHandle(const FkMsgHandle &o);

    virtual ~FkMsgHandle();

    virtual FkResult operator()(FkEngine *ptr, std::shared_ptr<FkMessage> &msg);

private:
    FkMsgHandle::Func func;
};

#define FK_REG_MSG(id, func) \
registerMessage(id, reinterpret_cast<FkMsgHandle::Func>(&func))
                             \
#define FK_WRAP_FUNC(func) \
FkMsgHandle(reinterpret_cast<FkMsgHandle::Func>(&func))

FK_SUPER_CLASS(FkEngine, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkEngine)

public:
    FkEngine(std::string name);

    FkEngine(const FkEngine &o) = delete;

    virtual ~FkEngine();

    virtual void setSettings(std::shared_ptr<FkEngineSettings> _settings);

    virtual FkResult create();

    virtual FkResult destroy();

    virtual FkResult start();

    virtual FkResult stop();

    virtual void setMonitor(std::shared_ptr<FkAbsEngineMonitor> &_monitor);

    std::shared_ptr<FkEngineContext> getContext();

protected:
    virtual FkResult onCreate();

    virtual FkResult onDestroy();

    virtual FkResult onStart();

    virtual FkResult onStop();

    FkResult sendMessage(std::shared_ptr<FkMessage> &msg, bool ignoreState = false);

private:
    virtual FkResult _onCreate(std::shared_ptr<FkMessage> msg);

    virtual FkResult _onDestroy(std::shared_ptr<FkMessage> msg);

    virtual FkResult _onStart(std::shared_ptr<FkMessage> msg);

    virtual FkResult _onStop(std::shared_ptr<FkMessage> msg);

    FkResult _changeState(kState &_state, uint32_t src, kState dst);

    FkResult _dispatch(std::shared_ptr<FkMessage> &msg);

    bool _isEnableEngineThread();

private:
    std::string name;
    std::shared_ptr<FkHandlerThread> mThread = nullptr;
    FkHandler *mHandler = nullptr;
    std::recursive_mutex mtx;
    std::mutex msgMtx;
    kState outsideState = kState::IDL;
    kState internalState = kState::IDL;
    std::shared_ptr<FkEngineContext> context = nullptr;
};


#endif //FK_FRAMEWORK_FKENGINE_H
