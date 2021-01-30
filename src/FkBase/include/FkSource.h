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
#include <unordered_map>

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
};

template<class T, class D>
FK_ABS_CLASS FkSourceAllocator FK_EXTEND FkObject {
public:
    FkSourceAllocator() : FkObject() {
        FK_MARK_SUPER
    }

    FkSourceAllocator(const FkSourceAllocator &o) = delete;

    virtual ~FkSourceAllocator() {
    }

    void release() {
        std::lock_guard<std::mutex> guard(mtx);
        auto itr0 = sMap.begin();
        while (sMap.end() != itr0) {
            itr0->second->destroy();
            delete itr0->second;
            itr0 = sMap.erase(itr0);
        }
        auto itr1 = sRecycler.begin();
        while (sRecycler.end() != itr1) {
            itr1->second->destroy();
            delete itr1->second;
            itr1 = sRecycler.erase(itr1);
        }
    }

    std::shared_ptr<T> alloc(D &desc) {
        std::lock_guard<std::mutex> guard(mtx);
        auto o = delegateAlloc(desc);
        if (nullptr == o) {
            return nullptr;
        }
        ++current;
        o->id = current;
        sMap.insert(std::make_pair(o->id, o.get()));
        return o;
    }

    virtual std::shared_ptr<T> delegateAlloc(D &desc) = 0;

    virtual size_t size() {
        std::lock_guard<std::mutex> guard(mtx);
        size_t size = 0;
        auto itr0 = sMap.begin();
        while (sMap.end() != itr0) {
            size += itr0->second->size();
            ++itr0;
        }
        return size;
    }

    virtual size_t capacity() {
        std::lock_guard<std::mutex> guard(mtx);
        size_t size = 0;
        auto itr0 = sMap.begin();
        while (sMap.end() != itr0) {
            size += itr0->second->size();
            ++itr0;
        }

        auto itr1 = sRecycler.begin();
        while (sRecycler.end() != itr1) {
            size += itr1->second->size();
            ++itr1;
        }
        return size;
    }

protected:
    void recycle(T *o) {
        std::lock_guard<std::mutex> guard(mtx);
        auto itr = sMap.find(o->id);
        if (sMap.end() != itr) {
            sRecycler.insert(std::make_pair(o->id, o));
            sMap.erase(itr);
        }
    }

private:
    std::mutex mtx;
    FkID current = FK_ID_NONE;
    std::unordered_map<FkID, T *> sMap;
    std::unordered_map<FkID, T *> sRecycler;
};


#endif //FK_BASE_FKSOURCE_H
