//
// Created by mingyi.li on 2021-01-02.
//

#include "FkDivideQuark.h"
#include "FkCalculatePrt.h"

FK_IMPL_CLASS_TYPE(FkDivideQuark, FkQuark)

FkDivideQuark::FkDivideQuark() : FkQuark() {

}

FkDivideQuark::~FkDivideQuark() {

}

void FkDivideQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkCalculatePrt, FkDivideQuark::_onDivide);

}

FkResult FkDivideQuark::_onDivide(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculatePrt>(p);
    cp->number /= 2;
    return FK_OK;
}