/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSOURCE_H
#define FK_BASE_FKSOURCE_H

#include "FkObject.h"
#include "FkDefinition.h"
#include "FkTimeUtils.h"
#include "AlString.h"
#include <unordered_map>
#include <map>

FK_ABS_CLASS FkSource FK_EXTEND FkObject {
public:
    FkSource();

    FkSource(const FkSource &o);

    virtual ~FkSource();

    virtual FkResult create() = 0;

    virtual void destroy() = 0;

    virtual size_t size() = 0;

public:
    FkID id = FK_ID_NONE;
    int64_t cTime = 0;
    int64_t eTime = 0;
    int useCnt = 0;
};

template<class T, class D>
FK_ABS_CLASS FkSourceAllocator FK_EXTEND FkObject {
public:
    FkSourceAllocator(int capacity) : FkObject(), _capacity(capacity) {
        FK_MARK_SUPER
    }

    FkSourceAllocator(const FkSourceAllocator &o) = delete;

    virtual ~FkSourceAllocator() {
    }

    void release() {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        auto itr0 = usingMap.begin();
        while (usingMap.end() != itr0) {
            itr0->second->destroy();
            delete itr0->second;
            itr0 = usingMap.erase(itr0);
        }
        auto itr1 = sRecycler.begin();
        while (sRecycler.end() != itr1) {
            (*itr1)->destroy();
            delete (*itr1);
            itr1 = sRecycler.erase(itr1);
        }
    }

    std::shared_ptr<T> alloc(D &desc) {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        T *ptr = findFromRecycler(desc);
        if (nullptr == ptr) {
            ptr = delegateAlloc(desc);
        }
        if (nullptr == ptr) {
            FkLogE(FK_DEF_TAG, "Alloc source failed.");
            return nullptr;
        }
        ++(ptr->useCnt);
        ptr->eTime = FkTimeUtils::getCurrentTimeUS();
        std::shared_ptr<T> o(ptr, [this](T *o) {
            this->recycle(o);
        });
        if (nullptr == o) {
            return nullptr;
        }
        ++current;
        o->id = current;
        usingMap.insert(std::make_pair(o->id, o.get()));
        usingCapacity += o->size();
        return o;
    }

    T *findFromRecycler(D &desc) {
        auto itr = sRecycler.begin();
        while (sRecycler.end() != itr) {
            auto o = *itr;
            if (delegateEquals(desc, o)) {
                sRecycler.erase(itr);
                cacheCapacity -= o->size();
                return o;
            }
            ++itr;
        }
        return nullptr;
    }

    virtual T *delegateAlloc(D &desc) = 0;

    virtual bool delegateEquals(D &desc, T *) = 0;

    /// Return used bytes.
    virtual size_t size() {
        return usingCapacity;
    }

    /// Return used and cache bytes.
    virtual size_t capacity() {
        return cacheCapacity + size();
    }

    virtual void clean() {

    }

protected:
    void recycle(T *o) {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        auto itr = usingMap.find(o->id);
        if (usingMap.end() != itr) {
            sRecycler.push_front(o);
            cacheCapacity += o->size();
            usingMap.erase(itr);
            usingCapacity -= o->size();
        }
        _dropCacheWhenBiggerThanCapacityLimit();
    }

    void _dropCacheWhenBiggerThanCapacityLimit() {
        FkLogI(FK_DEF_TAG, "Drop cache begin: %d", capacity());
        auto itr = sRecycler.end();
        --itr;
        while (capacity() > _capacity && itr != sRecycler.begin()) {
            auto o = *itr;
            o->destroy();
            delete o;
            itr = sRecycler.erase(itr--);
            cacheCapacity -= o->size();
        }
        FkLogI(FK_DEF_TAG, "Drop cache finish: %d", capacity());
    }

private:
    std::recursive_mutex mtx;
    FkID current = FK_ID_NONE;
    std::unordered_map<FkID, T *> usingMap;
    int64_t usingCapacity = 0;
    std::list<T *> sRecycler;
    int64_t cacheCapacity = 0;
    int _capacity = 0;
};


#endif //FK_BASE_FKSOURCE_H
