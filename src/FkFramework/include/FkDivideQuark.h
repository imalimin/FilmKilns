//
// Created by mingyi.li on 2021-01-02.
//

#ifndef FK_FRAMEWORK_FKDIVIDEQUARK_H
#define FK_FRAMEWORK_FKDIVIDEQUARK_H

#include "FkQuark.h"
#include "FkProtocol.h"

FK_CLASS FkDivideQuark FK_EXTEND FkQuark {
public:
    FkDivideQuark();

    virtual ~FkDivideQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

private:
    FkDivideQuark(const FkDivideQuark &o) : FkQuark() {};

private:
    FkResult _onDivide(std::shared_ptr<FkProtocol> p);

};


#endif //FK_FRAMEWORK_FKDIVIDEQUARK_H
