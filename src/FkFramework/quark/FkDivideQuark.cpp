//
// Created by mingyi.li on 2021-01-02.
//

#include "FkDivideQuark.h"
#include "FkCalculateProt.h"

FkDivideQuark::FkDivideQuark() : FkQuark() {

}

FkDivideQuark::~FkDivideQuark() {

}

void FkDivideQuark::describeProtocols(std::shared_ptr<FkProtDesc> desc) {
    desc->add(std::static_pointer_cast<FkProtocol>(std::make_shared<FkCalculateProt>()),
              reinterpret_cast<FkQuark::ProtHandler>(&FkDivideQuark::_onDivide));

}

FkResult FkDivideQuark::_onDivide(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculateProt>(p);
    cp->number /= 2;
    return FK_OK;
}