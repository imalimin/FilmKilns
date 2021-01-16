/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKPROTDESC_H
#define FK_FRAMEWORK_FKPROTDESC_H

#include "FkObject.h"
#include "FkProtocol.h"
#include <unordered_map>

#define FK_PROT_DESC_ADD(desc, prot, method) \
    desc->add(std::static_pointer_cast<FkProtocol>(std::make_shared<prot>()), \
        reinterpret_cast<FkQuark::ProtHandler>(&method)) \

class FkQuark;

FK_CLASS FkProtDesc FK_EXTEND FkObject {
private:
    typedef int (FkQuark::*ProtHandler)(std::shared_ptr<FkProtocol>);
public:
    FkProtDesc();

    FkProtDesc(const FkProtDesc &o);

    virtual ~FkProtDesc();

    void add(std::shared_ptr<FkProtocol> p, ProtHandler h);

    ProtHandler find(FkProtocol *p);

private:
    std::unordered_map<FkProtocol::Type, std::shared_ptr<FkProtocol>> prots;
    std::unordered_map<FkProtocol::Type, ProtHandler> funcs;
};


#endif //FK_FRAMEWORK_FKPROTDESC_H
