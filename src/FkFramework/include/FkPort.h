/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKPORT_H
#define FK_FRAMEWORK_FKPORT_H

#include "FkObject.h"
#include "FkProtocol.h"

class FkQuark;
class FkPortDesc;

FK_SUPER_CLASS(FkPort, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkPort)

public:
    friend FkPortDesc;
    typedef int (FkQuark::*PortFunc)(std::shared_ptr<FkProtocol>);
    static const uint32_t NONE;

public:
    FkPort(uint32_t port, FkProtocol::Desc desc, FkPort::PortFunc func);

    FkPort(const FkPort &o) = delete;

    virtual ~FkPort();

    FkResult chat(FkQuark *target, std::shared_ptr<FkProtocol> p) const;

public:
    uint32_t port = 0;
    FkProtocol::Desc desc = {0, 0};
    FkPort::PortFunc func = nullptr;
};


#endif //FK_FRAMEWORK_FKPORT_H
