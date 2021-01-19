/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPortDesc.h"

FkPortDesc::FkPortDesc() : FkObject() {

}

FkPortDesc::~FkPortDesc() {
    ports.clear();
}

void FkPortDesc::add(uint32_t port, std::shared_ptr<FkProtocol> p, FkPort::PortFunc func) {
    if (p && func) {
        auto type = p->getType();
        auto itr0 = ports.find(type);
        FkAssert(ports.end() == itr0);
        ports.insert(std::make_pair(type, std::make_shared<FkPort>(port, p, func)));
    }
}

FkPort::PortFunc FkPortDesc::find(FkProtocol *p) {
    if (nullptr == p) {
        return nullptr;
    }
    auto itr = ports.find(p->getType());
    if (ports.end() == itr) {
        return nullptr;
    }
    return itr->second->func;
}
