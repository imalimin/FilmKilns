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

FK_IMPL_CLASS_TYPE(FkPortDesc, FkObject)

FkPortDesc::FkPortDesc() : FkObject() {

}

FkPortDesc::~FkPortDesc() {
    ports.clear();
}

FkResult FkPortDesc::add(uint32_t port, FkProtocol::Desc desc, FkPort::PortFunc func) {
    if (desc.type != 0 && func) {
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
        auto itr = ports.find(desc.type);
        if (ports.end() != itr) {
            ports.erase(itr);
        }
//        FkAssert(ports.end() == itr0, FK_FAIL);
        ports.insert(std::make_pair(desc.type, std::make_shared<FkPort>(port, desc, func)));
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

FkResult FkPortDesc::query(std::list<FkProtocol::Desc> &protocols) {
    if (ports.empty()) {
        return FK_EMPTY_DATA;
    }
    for (auto &itr : ports) {
        protocols.push_back(itr.second->desc);
    }
    return FK_OK;
}

void FkPortDesc::clear() {
    std::lock_guard<std::mutex> guard(mtx);
    auto itr = ports.begin();
    while (itr != ports.end()) {
        if (itr->first != FK_GET_CLASS_TYPE(FkOnCreatePrt).getId()
            && itr->first != FK_GET_CLASS_TYPE(FkOnDestroyPrt).getId()
            && itr->first != FK_GET_CLASS_TYPE(FkOnStartPrt).getId()
            && itr->first != FK_GET_CLASS_TYPE(FkOnStopPrt).getId()) {
            itr = ports.erase(itr);
        } else {
            ++itr;
        }
    }
}
