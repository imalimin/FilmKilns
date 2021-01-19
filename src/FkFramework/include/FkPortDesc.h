/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKPORTDESC_H
#define FK_FRAMEWORK_FKPORTDESC_H

#include "FkPort.h"
#include <unordered_map>

#define FK_PORT_DESC_ADD(desc, port, PRT, method) \
    desc->add(port, std::static_pointer_cast<FkProtocol>(std::make_shared<PRT>()), \
        reinterpret_cast<FkPort::PortFunc>(&method)) \


#define FK_PORT_DESC_SIMPLE_ADD(desc, PRT, method) \
    desc->add(0, std::static_pointer_cast<FkProtocol>(std::make_shared<PRT>()), \
        reinterpret_cast<FkPort::PortFunc>(&method)) \

FK_CLASS FkPortDesc FK_EXTEND FkObject {
public:
    FkPortDesc();

    FkPortDesc(const FkPortDesc &o) = delete;

    virtual ~FkPortDesc();

    void add(uint32_t port, std::shared_ptr<FkProtocol> p, FkPort::PortFunc func);

    FkPort::PortFunc find(FkProtocol *p);

private:
    std::unordered_map<FkProtocol::Type, std::shared_ptr<FkPort>> ports;
};


#endif //FK_FRAMEWORK_FKPORTDESC_H
