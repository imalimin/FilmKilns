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

FK_CLASS FkPort FK_EXTEND FkObject {
public:
    typedef int (FkQuark::*PortFunc)(std::shared_ptr<FkProtocol>);
    static const uint32_t NONE;

public:
    FkPort(uint32_t port, std::shared_ptr<FkProtocol> protocol, FkPort::PortFunc func);

    FkPort(const FkPort &o) = delete;

    virtual ~FkPort();

    FkResult communicate(FkQuark *target, std::shared_ptr<FkProtocol> protocol) const;

public:
    uint32_t port = 0;
    std::shared_ptr<FkProtocol> protocol = nullptr;
    FkPort::PortFunc func = nullptr;
};


#endif //FK_FRAMEWORK_FKPORT_H
