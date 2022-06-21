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
#include <unordered_map>
#include <map>
#include <list>
#include <mutex>

FK_SUPER_CLASS(FkSource, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkSource)

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

FK_SUPER_TEMPLATE_CLASS(FkSourceAllocator, FkObject, class T, class D) {
FK_DEF_CLASS_TYPE_FUNC(FkSourceAllocator)

public:
    FkSourceAllocator(int capacity) : FkObject(), _capacity(capacity) {

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
        return o;
    }

    T *findFromRecycler(D &desc) {
        auto itr = sRecycler.begin();
        while (sRecycler.end() != itr) {
            auto o = *itr;
            if (delegateEquals(desc, o)) {
                sRecycler.erase(itr);
                return o;
            }
            ++itr;
        }
        return nullptr;
    }

    virtual T *delegateAlloc(D &desc) = 0;

    virtual bool delegateEquals(D &desc, T *) = 0;

    /// Return using bytes.
    virtual size_t size() {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        return _calcUsingCapacity();
    }

    /// Same with size().
    virtual size_t usingSize() {
        return size();
    }

    /// Return cache bytes.
    virtual size_t cacheSize() {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        return _calcCacheCapacity();
    }

    /// Return capacity bytes.
    virtual size_t capacity() {
        return _capacity;
    }

    virtual void clean() {

    }

protected:
    virtual void recycle(T *o) {
        std::lock_guard<std::recursive_mutex> guard(mtx);
        auto itr = usingMap.find(o->id);
        if (usingMap.end() != itr) {
            sRecycler.push_front(o);
            usingMap.erase(itr);
        }
        _dropCacheWhenBiggerThanCapacityLimit();
    }

private:
    void _dropCacheWhenBiggerThanCapacityLimit() {
        auto itr = sRecycler.end();
        --itr;
        size_t size = _calcUsingCapacity() + _calcCacheCapacity();
//        FkLogI(FK_DEF_TAG, "Drop cache begin: %d", size);
        while (size > _capacity && itr != sRecycler.begin()) {
            auto o = *itr;
            size -= o->size();
            o->destroy();
            delete o;
            sRecycler.erase(itr--);
        }
//        FkLogI(FK_DEF_TAG, "Drop cache finish: %d", size);
    }

    size_t _calcUsingCapacity() {
        size_t size = 0;
        auto itr0 = usingMap.begin();
        while (usingMap.end() != itr0) {
            size += itr0->second->size();
            ++itr0;
        }
        return size;
    }

    size_t _calcCacheCapacity() {
        size_t size = 0;
        auto itr1 = sRecycler.begin();
        while (sRecycler.end() != itr1) {
            size += (*itr1)->size();
            ++itr1;
        }
        return size;
    }

private:
    std::recursive_mutex mtx;
    FkID current = FK_ID_NONE;
    std::unordered_map<FkID, T *> usingMap;
    std::list<T *> sRecycler;
    int _capacity = 0;
};


#endif //FK_BASE_FKSOURCE_H
