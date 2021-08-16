/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPortDesc.h"
#include "FkOnCreatePrt.h"
#include "FkOnDestroyPrt.h"
#include "FkOnStartPrt.h"
#include "FkOnStopPrt.h"

FkPortDesc::FkPortDesc() : FkObject() {

}

FkPortDesc::~FkPortDesc() {
    ports.clear();
}

FkResult FkPortDesc::add(uint32_t port, std::shared_ptr<FkProtocol> p, FkPort::PortFunc func) {
    if (p && func) {
        if (FkPort::NONE == port) {
            std::lock_guard<std::mutex> guard(mtx);
            if (ports.empty()) {
                port = 1;
            } else {
                for (auto &it : ports) {
                    port = std::max(it.second->port, port);
                }
            }
        }
        auto type = p->getType();
        auto itr0 = ports.find(type);
        FkAssert(ports.end() == itr0, FK_FAIL);
        ports.insert(std::make_pair(type, std::make_shared<FkPort>(port, p, func)));
        return FK_OK;
    }
    return FK_FAIL;
}

std::shared_ptr<FkPort> FkPortDesc::find(FkProtocol::Type type) {
    auto itr = ports.find(type);
    if (ports.end() == itr) {
        return nullptr;
    }
    return itr->second;
}

FkResult FkPortDesc::query(std::list<std::shared_ptr<FkProtocol>> &protocols) {
    if (ports.empty()) {
        return FK_EMPTY_DATA;
    }
    for (auto &itr : ports) {
        protocols.push_back(itr.second->protocol);
    }
    return FK_OK;
}

void FkPortDesc::clear() {
    std::lock_guard<std::mutex> guard(mtx);
    auto itr = ports.begin();
    while (itr != ports.end()) {
        if (itr->first != FkClassType::type<FkOnCreatePrt>().hashCode()
            && itr->first != FkClassType::type<FkOnDestroyPrt>().hashCode()
            && itr->first != FkClassType::type<FkOnStartPrt>().hashCode()
            && itr->first != FkClassType::type<FkOnStopPrt>().hashCode()) {
            itr = ports.erase(itr);
        } else {
            ++itr;
        }
    }
}
