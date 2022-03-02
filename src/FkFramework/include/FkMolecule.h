/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKMOLECULE_H
#define FK_FRAMEWORK_FKMOLECULE_H

#include "FkAtom.h"

FK_ABS_CLASS FkMolecule FK_EXTEND FkAtom {
public:
    FkMolecule();

    FkMolecule(const FkMolecule &o) = delete;

    virtual ~FkMolecule();

protected:
    virtual std::shared_ptr<FkQuarkContext> shareContextToSubQuark() = 0;

    virtual FkResult dispatch(std::shared_ptr<FkProtocol> p) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

};


#endif //FK_FRAMEWORK_FKMOLECULE_H
