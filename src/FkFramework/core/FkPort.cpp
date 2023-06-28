/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPort.h"
#include <utility>

FK_IMPL_CLASS_TYPE(FkPort, FkObject)

const uint32_t FkPort::NONE = 0;

FkPort::FkPort(uint32_t port, FkProtocol::Desc desc, FkPort::PortFunc func)
        : FkObject(), port(port), desc(desc), func(func) {

}

FkPort::~FkPort() {
    port = 0;
    desc = {0, 0};
    func = nullptr;
}

FkResult FkPort::chat(FkQuark *target, std::shared_ptr<FkProtocol> p) const {
    return (target->*func)(p);
}
