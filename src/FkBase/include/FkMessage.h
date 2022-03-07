/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKMESSAGE_H
#define FK_BASE_FKMESSAGE_H

#include "FkSource.h"
#include <queue>
#include <future>

class FkLooper;

class FkHandler;

FK_CLASS FkMessage FK_EXTEND FkSource {
public:
    FkID what = 0;
    int32_t arg1 = 0;
    int64_t arg2 = 0;
    std::string arg3;
    std::shared_ptr<FkObject> sp;
    std::shared_ptr<std::promise<std::shared_ptr<FkObject>>> promise;
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

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

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

};

FK_CLASS FkMessageAllocator FK_EXTEND FkSourceAllocator<FkMessage, int32_t> {
public:
    static FkMessageAllocator *getInstance();

public:
    FkMessageAllocator();

    FkMessageAllocator(const FkMessageAllocator &o) = delete;

    ~FkMessageAllocator();

    virtual FkMessage *delegateAlloc(int32_t &desc) override;

    virtual bool delegateEquals(int32_t &desc, FkMessage *value) override;

    virtual void recycle(FkMessage *o) override;

private:
    static FkMessageAllocator *instance;
};


#endif //FK_BASE_FKMESSAGE_H
