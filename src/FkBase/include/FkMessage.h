/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKMESSAGE_H
#define FK_BASE_FKMESSAGE_H

#include "FkObject.h"
#include <queue>
#include <future>

class FkLooper;

class FkHandler;

class FkMessageManager;

FK_CLASS FkMessage FK_EXTEND FkObject {
public:
    FkID what = 0;
    int32_t arg1 = 0;
    int64_t arg2 = 0;
    std::string arg3;
    std::shared_ptr<FkObject> sp;
    std::promise<Object> prom;
    int32_t flags = FLAG_NORMAL;
private:
    friend FkLooper;
    friend FkHandler;
    FkHandler *target = nullptr;

public:
    FkMessage();

    FkMessage(FkID what);

    FkMessage(FkID what, std::shared_ptr<FkObject> sp);

    FkMessage(FkID what, std::shared_ptr<FkObject> sp, int32_t flags);

    FkMessage(const FkMessage &o) = delete;

public:
    virtual ~FkMessage();

public:
    const static int32_t FLAG_NORMAL;
    const static int32_t FLAG_UNIQUE;
    const static int32_t FLAG_FIRST_ALWAYS;
    const static int32_t FLAG_CLEAR;

public:
    static std::shared_ptr<FkMessage> obtain(int32_t what);

    static std::shared_ptr<FkMessage> obtain(int32_t what, std::shared_ptr<FkObject> sp);

    static std::shared_ptr<FkMessage> obtain(int32_t what, int32_t flags);

    static std::shared_ptr<FkMessage> obtain(int32_t what,
                                        std::shared_ptr<FkObject> sp,
                                        int32_t queueMode);

    static void recycle(std::shared_ptr<FkMessage> msg);

};

FK_CLASS FkMessageManager FK_EXTEND FkObject {
private:
    friend FkMessage;

    static FkMessageManager *getInstance();

    FkMessageManager() : FkObject() {}

    FkMessageManager(FkMessageManager &o) = delete;

public:
    void recycle(std::shared_ptr<FkMessage> msg);

    std::shared_ptr<FkMessage> popOne();

private:
    static FkMessageManager *instance;
    std::queue<std::shared_ptr<FkMessage>> pool;
    std::mutex poolMtx;

};


#endif //FK_BASE_FKMESSAGE_H
