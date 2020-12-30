/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkProtDesc.h"

FkProtDesc::FkProtDesc(FkQuark *target) : FkObject(), target(target) {

}

FkProtDesc::FkProtDesc(const FkProtDesc &o) : FkObject(), target(o.target) {
    for (auto &it : o.prots) {
        prots.insert(std::make_pair(it.first, it.second));
    }
    for (auto &it : o.funcs) {
        funcs.insert(std::make_pair(it.first, it.second));
    }
}

FkProtDesc::~FkProtDesc() {
    target = nullptr;
    prots.clear();
    funcs.clear();
}

void FkProtDesc::add(std::shared_ptr<FkProtocol> p, FkProtDesc::ProtHandler h) {
    if (p && h) {
        auto type = p->getType();
        auto itr0 = prots.find(type);
        if (prots.end() != itr0) {
            prots.erase(itr0);
        }
        auto itr1 = funcs.find(type);
        if (funcs.end() != itr1) {
            funcs.erase(itr1);
        }
        prots.insert(std::make_pair(type, p));
        funcs.insert(std::make_pair(type, h));
    }
}
