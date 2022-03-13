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
#include <any>

class FkLooper;

class FkHandler;

FK_CLASS FkMessage FK_EXTEND FkSource {
public:
    FkID what = 0;
    int32_t arg1 = 0;
    int64_t arg2 = 0;
    std::string arg3;
    std::shared_ptr<FkObject> sp;
    std::any any;
    int32_t flags = FLAG_NORMAL;
private:
    friend FkLooper;
    friend FkHandler;
    FkHandler *target = nullptr;
    std::shared_ptr<std::promise<std::any>> promise;

public:
    FkMessage();

    FkMessage(FkID what);

    FkMessage(FkID what, std::shared_ptr<FkObject> sp);

    FkMessage(FkID what, std::shared_ptr<FkObject> sp, int32_t flags);

    FkMessage(const FkMessage &o) = delete;

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    void reset();

    void withPromise();

    bool hasPromise();

    void setPromiseResult(std::any any);

    template<typename T>
    FkResult getPromiseResult(T &o) {
        if (hasPromise()) {
            auto _any = promise->get_future().get();
            if (_any.has_value()) {
                o = std::any_cast<T>(_any);
                return FK_OK;
            }
        }
        return FK_FAIL;
    }


public:
    virtual ~FkMessage();

public:
    const static int32_t FLAG_NORMAL;
    const static int32_t FLAG_UNIQUE;
    const static int32_t FLAG_FIRST_ALWAYS;
    const static int32_t FLAG_CLEAR;

public:
    static std::shared_ptr<FkMessage> obtain(std::any any, int32_t what = 0);

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
